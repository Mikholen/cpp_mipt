#include <cassert>
#include <cstddef>
#include <new>
#include <print>
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

class ListAllocator : public Allocator, private boost::noncopyable {

public:

    struct Node {

        Node* next = nullptr;
    };

    ListAllocator(std::size_t size, std::size_t step) : m_size(size), m_step(step) {

        assert(m_size % m_step == 0 && m_step >= sizeof(Node));
        make_list();
        m_begin = m_head;
    }

    ~ListAllocator() {

        for (auto list : m_lists) {

            operator delete(list, m_size, std::align_val_t{s_alignment});
        }
    }

    void* allocate(std::size_t, std::size_t = s_alignment) override {

        if (!m_head) {

            if (m_offset == std::size(m_lists)) {

                make_list();
            }
            else {

                m_head = get<Node>(m_lists[++m_offset - 1]);
            }
        }

        auto node = m_head;

        if (!node->next) {

            auto next = get<std::byte>(node) + m_step;
            if (next != get<std::byte>(m_lists[m_offset - 1]) + m_size) {

                m_head = get<Node>(next);
                m_head->next = nullptr;
            }
            else {

                m_head = m_head->next;
            }
        }
        else {

            m_head = m_head->next;
        }

        return node;
    }

    void deallocate(void* ptr) override {

        auto node = get<Node>(ptr);
        node->next = m_head;
        m_head = node;
    }

    void show() const {

        std::print(
            "ListAllocator::show : m_size = {} m_step = {} m_begin = {} m_head = {} m_offset = {}\n",
            m_size, m_step, m_begin, static_cast<void*>(m_head), m_offset
        );
    }

private:

    void make_list() {

        m_head = get<Node>(operator new(m_size, std::align_val_t{s_alignment}));
        m_head->next = nullptr;
        ++m_offset;
        m_lists.push_back(m_head);
    }

    std::size_t           m_size   = 0;
    std::size_t           m_step   = 0;
    std::size_t           m_offset = 0;
    void*                 m_begin  = nullptr;
    Node*                 m_head   = nullptr;
    std::vector<void*>    m_lists;

    static constexpr auto s_alignment = alignof(std::max_align_t);
};

void test_v1(benchmark::State& state) {

    constexpr auto kb = 1uz << 10;
    constexpr auto mb = 1uz << 20;

    std::vector<void*> vector(kb, nullptr);

    for (auto _ : state) {

        for (auto i = 0uz; i < kb; ++i) {

            vector[i] = operator new(mb);
        }

        for (auto i = 0uz; i < kb; i += 2) {

            operator delete(vector[i], mb);
        }

        for (auto i = 0uz; i < kb; i += 2) {

            vector[i] = operator new(mb);
        }

        for (auto i = 0uz; i < kb; ++i) {

            operator delete(vector[i], mb);
        }

        benchmark::DoNotOptimize(vector);
    }
}

void test_v2(benchmark::State& state) {

    constexpr auto kb = 1uz << 10;
    constexpr auto mb = 1uz << 20;
    constexpr auto gb = 1uz << 30;

    std::vector<void*> vector(kb, nullptr);

    for (auto _ : state) {

        ListAllocator allocator(gb, mb);

        for (auto i = 0uz; i < kb; ++i) {

            vector[i] = allocator.allocate(0);
        }

        for (auto i = 0uz; i < kb; i += 2) {

            allocator.deallocate(vector[i]);
        }

        for (auto i = 0uz; i < kb; i += 2) {

            vector[i] = allocator.allocate(0);
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

    ListAllocator list(32, 8);
    Allocator* alloc = &list;

    list.show();
    alloc->allocate(0);
    list.show();
    auto x = alloc->allocate(0);
    list.show();
    auto y = alloc->allocate(0);
    list.show();
    alloc->allocate(0);
    list.show();
    alloc->allocate(0);
    list.show();

    alloc->deallocate(x);
    list.show();
    alloc->deallocate(y);
    list.show();

    auto z = alloc->allocate(0);
    list.show();

    assert(z == y);

    benchmark::RunSpecifiedBenchmarks();
}
