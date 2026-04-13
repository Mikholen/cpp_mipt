#include <cassert>
#include <cstddef>
#include <memory>
#include <new>
#include <print>
#include <random>
#include <utility>
#include <vector>

#include <boost/noncopyable.hpp>

#include <benchmark/benchmark.h>

class Allocator {

public:

    virtual ~Allocator() = default;

    virtual void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) = 0;
    virtual void  deallocate(void* ptr) = 0;

protected:

    template<typename T>
    T* get(void* ptr) const {

        return static_cast<T*>(ptr);
    }
};

class FreeListAllocator : public Allocator, private boost::noncopyable {

public:

    struct Node {

        std::size_t size = 0;
        Node*       next = nullptr;
    };

    struct alignas(std::max_align_t) Header {

        std::size_t size = 0;
    };

    FreeListAllocator(std::size_t size) : m_size(size) {

        assert(m_size >= sizeof(Node) + 1);

        m_begin = operator new(m_size, std::align_val_t{s_alignment});
        m_head = get<Node>(m_begin);
        m_head->size = m_size - sizeof(Header);
        m_head->next = nullptr;
    }

    ~FreeListAllocator() {

        operator delete(m_begin, m_size, std::align_val_t{s_alignment});
    }

    void* allocate(std::size_t size, std::size_t = s_alignment) override {

        void* end  = get<std::byte>(m_begin) + sizeof(Header) + size;
        void* next = end;
        auto  free = 2 * alignof(Header);

        if (next = std::align(alignof(Header), sizeof(Header), next, free); next) {

            auto padding = get<std::byte>(next) - get<std::byte>(end);

            if (auto [current, previous] = find(size + padding); current) {

                if (current->size >= size + padding + sizeof(Node) + 1) {
                    auto step = sizeof(Header) + size + padding;
                    auto node = get<Node>(get<std::byte>(current) + step);
                    node->size = current->size - step;
                    node->next = current->next;
                    current->next = node;
                }
                else {

                    padding += current->size - size - padding;
                }

                if (!previous) {

                    m_head = current->next;
                } else {

                    previous->next = current->next;
                }

                auto header = get<Header>(current);
                header->size = size + padding;

                return get<std::byte>(current) + sizeof(Header);
            }
        }
        return nullptr;
    }

    void deallocate(void* ptr) override {

        auto node = get<Node>(get<std::byte>(ptr) - sizeof(Header));

        Node* previous = nullptr;
        Node* current  = m_head;

        while (current) {

            if (node < current) {

                node->next = current;
                if (!previous) {

                    m_head = node;
                }
                else {

                    previous->next = node;
                }
                break;
            }

            previous = current;
            current  = current->next;
        }

        merge(previous, node);
    }

    void show() const {

        std::print(

            "FreeListAllocator::show : m_size = {} m_begin = {} m_head = {} ",
            m_size, m_begin, static_cast<void*>(m_head)
        );

        if (m_head->next) {

            std::print("m_head->next = {}\n", static_cast<void*>(m_head->next));
        }
        else {

            std::print("\n");
        }
    }

private:

    auto find(std::size_t size) const -> std::pair<Node*, Node*> {

        Node* current  = m_head;
        Node* previous = nullptr;

        while (current && size > current->size) {

            previous = current;
            current  = current->next;
        }

        return std::make_pair(current, previous);
    }

    void merge(Node* previous, Node* node) const {

        if (node->next && get<std::byte>(node) + sizeof(Header) + node->size == get<std::byte>(node->next)) {

            node->size += sizeof(Header) + node->next->size;
            node->next = node->next->next;
        }

        if (previous && get<std::byte>(previous) + sizeof(Header) + previous->size == get<std::byte>(node)) {

            previous->size += sizeof(Header) + node->size;
            previous->next = node->next;
        }
    }

    std::size_t m_size  = 0;
    void*       m_begin = nullptr;
    Node*       m_head  = nullptr;

    static constexpr auto s_alignment = alignof(std::max_align_t);
};

void test_v1(benchmark::State& state) {

    constexpr auto kb = 1uz << 10;
    constexpr auto mb = 1uz << 20;

    std::uniform_int_distribution<std::size_t> distribution(1, 16);
    std::default_random_engine                 engine;

    std::vector<std::pair<void*, std::size_t>> vector(kb);

    for (auto _ : state) {

        for (auto i = 0uz; i < kb; ++i) {

            vector[i].second = distribution(engine) * mb;
            vector[i].first  = operator new(vector[i].second);
        }

        for (auto i = 0uz; i < kb; i += 32) {

            operator delete(vector[i].first, vector[i].second);
        }

        for (auto i = 0uz; i < kb; i += 32) {

            vector[i].second = distribution(engine) * mb;
            vector[i].first  = operator new(vector[i].second);
        }

        for (auto i = 0uz; i < kb; ++i) {

            operator delete(vector[i].first, vector[i].second);
        }
        
        benchmark::DoNotOptimize(vector);
    }
}

void test_v2(benchmark::State& state) {

    constexpr auto kb = 1uz << 10;
    constexpr auto mb = 1uz << 20;
    constexpr auto gb = 1uz << 30;

    std::uniform_int_distribution<std::size_t> distribution(1, 16);
    std::default_random_engine                 engine;

    std::vector<void*> vector(kb, nullptr);

    for (auto _ : state) {

        FreeListAllocator allocator(16 * gb);

        for (auto i = 0uz; i < kb; ++i) {

            vector[i] = allocator.allocate(distribution(engine) * mb);
        }

        for (auto i = 0uz; i < kb; i += 32) {

            allocator.deallocate(vector[i]);
        }

        for (auto i = 0uz; i < kb; i += 32) {

            vector[i] = allocator.allocate(distribution(engine) * mb);
        }

        for (auto i = 0uz; i < kb; ++i) {

            allocator.deallocate(vector[i]);
        }

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(test_v1);
BENCHMARK(test_v2);

int main() {

    FreeListAllocator free_list(1 << 10);
    Allocator* alloc = &free_list;

    free_list.show();
    alloc->allocate(16);
    free_list.show();
    auto x = alloc->allocate(16);
    free_list.show();
    auto y = alloc->allocate(16);
    free_list.show();
    alloc->allocate(16);
    free_list.show();

    alloc->deallocate(y);
    free_list.show();
    alloc->deallocate(x);
    free_list.show();

    auto z = alloc->allocate(32);
    free_list.show();

    assert(z == x);

    benchmark::RunSpecifiedBenchmarks();
}
