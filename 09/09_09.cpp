#include <cassert>
#include <cstddef>
#include <memory>
#include <new>
#include <random>
#include <utility>
#include <vector>
#include <iostream>

#include <boost/noncopyable.hpp>

#include <benchmark/benchmark.h>

enum class SearchPolicy {

    FirstFit,
    BestFit
};

class Allocator : private boost::noncopyable {

public :

    explicit Allocator(std::size_t size, SearchPolicy policy = SearchPolicy::FirstFit)
        : m_size(size)
        , m_policy(policy) {

        assert(m_size >= sizeof(Node) + 1);

        m_begin = operator new(m_size, std::align_val_t(s_alignment));

        m_head = get_node(m_begin);

        m_head->size = m_size - sizeof(Header);

        m_head->next = nullptr;
    }

   ~Allocator() {

        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    auto allocate(std::size_t size) -> void * {

        void * end = get_byte(m_begin) + sizeof(Header) + size;
        void * next = end;

        auto free = 2 * alignof(Header);

        if (next = std::align(alignof(Header), sizeof(Header), next, free); next) {

            auto padding = get_byte(next) - get_byte(end);

            auto [current, previous] = find_block(size + padding);

            if (current) {

                if (current->size >= size + padding + sizeof(Node) + 1) {

                    auto step = sizeof(Header) + size + padding;

                    auto node = get_node(get_byte(current) + step);

                    node->size = current->size - step;

                    node->next = current->next;

                    current->next = node;
                }
                else {

                    padding += current->size - size - padding;
                }

                if (!previous) {

                    m_head = current->next;
                }
                else {

                    previous->next = current->next;
                }

                auto header = get_header(current);

                header->size = size + padding;

                return get_byte(current) + sizeof(Header);
            }
        }

        return nullptr;
    }

    void deallocate(void * x) {

        auto node = get_node(get_byte(x) - sizeof(Header));

        Node * previous = nullptr;
        Node * current = m_head;

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

        std::cout << "Chain_Allocator::show : m_size = " << m_size
          << " m_begin = " << m_begin
          << " m_head = " << static_cast<void*>(m_head) << " ";

        if (m_head->next) {
            std::cout << "m_head->next = " << static_cast<void*>(m_head->next) << "\n";
        } else {
            std::cout << "\n";
        }
    }

private :

    struct Node {

        std::size_t size = 0;

        Node * next = nullptr;
    };

    struct alignas(std::max_align_t) Header {

        std::size_t size = 0;
    };

    auto get_byte(void * x) const -> std::byte * {

        return static_cast < std::byte * > (x);
    }

    auto get_node(void * x) const -> Node * {

        return static_cast < Node * > (x);
    }

    auto get_header(void * x) const -> Header * {

        return static_cast < Header * > (x);
    }

    auto find_first(std::size_t size) const -> std::pair < Node *, Node * > {

        Node * current = m_head;
        Node * previous = nullptr;

        while (current && size > current->size) {

            previous = current;
            current  = current->next;
        }

        return std::make_pair(current, previous);
    }

    auto find_best(std::size_t size) const -> std::pair < Node *, Node * > {

        Node * best = nullptr;
        Node * best_previous = nullptr;
        Node * current = m_head;
        Node * previous = nullptr;

        while (current) {

            if (current->size >= size) {

                if (!best || current->size < best->size) {

                    best = current;
                    best_previous = previous;
                }
            }

            previous = current;
            current  = current->next;
        }

        return std::make_pair(best, best_previous);
    }

    auto find_block(std::size_t size) const -> std::pair < Node *, Node * > {

        if (m_policy == SearchPolicy::FirstFit) {

            return find_first(size);
        }
        else {

            return find_best(size);
        }
    }

    void merge(Node * previous, Node * node) const {

        if (node->next && get_byte(node) + sizeof(Header) + node->size == get_byte(node->next)) {

            node->size += sizeof(Header) + node->next->size;

            node->next = node->next->next;
        }

        if (previous && get_byte(previous) + sizeof(Header) + previous->size == get_byte(node)) {

            previous->size += sizeof(Header) + node->size;

            previous->next = node->next;
        }
    }

    std::size_t m_size = 0;

    void * m_begin = nullptr;

    Node * m_head  = nullptr;

    SearchPolicy m_policy = SearchPolicy::FirstFit;

    static inline auto s_alignment = alignof(std::max_align_t);
};

void test_v1(benchmark::State & state) {

    auto kb = 1uz << 10;
    auto mb = 1uz << 20;

    std::uniform_int_distribution < std::size_t > distribution(1, 16);

    std::default_random_engine engine;

    std::vector < std::pair < void *, std::size_t > > vector(kb);

    for (auto element : state) {

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

void test_first_fit(benchmark::State & state) {

    auto kb = 1uz << 10;
    auto mb = 1uz << 20;
    auto gb = 1uz << 30;

    std::uniform_int_distribution < std::size_t > distribution(1, 16);

    std::default_random_engine engine;

    std::vector < void * > vector(kb, nullptr);

    for (auto element : state) {

        Allocator allocator(16 * gb, SearchPolicy::FirstFit);

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

void test_best_fit(benchmark::State & state) {

    auto kb = 1uz << 10;
    auto mb = 1uz << 20;
    auto gb = 1uz << 30;

    std::uniform_int_distribution < std::size_t > distribution(1, 16);

    std::default_random_engine engine;

    std::vector < void * > vector(kb, nullptr);

    for (auto element : state) {

        Allocator allocator(16 * gb, SearchPolicy::BestFit);

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

BENCHMARK(test_first_fit);

BENCHMARK(test_best_fit);

int main() {

    std::cout << "=== Testing First Fit Allocator ===\n";

    Allocator allocator_first(1 << 10, SearchPolicy::FirstFit);

    allocator_first.show();
    allocator_first.allocate(16);

    allocator_first.show();
    auto x = allocator_first.allocate(16);

    allocator_first.show();
    auto y = allocator_first.allocate(16);

    allocator_first.show();
    allocator_first.allocate(16);

    allocator_first.show();
    allocator_first.deallocate(y);

    allocator_first.show();
    allocator_first.deallocate(x);

    allocator_first.show();
    auto z = allocator_first.allocate(32);

    allocator_first.show();

    assert(z == x);

    std::cout << "\n=== Testing Best Fit Allocator ===\n";

    Allocator allocator_best(1 << 10, SearchPolicy::BestFit);

    allocator_best.show();
    allocator_best.allocate(16);

    allocator_best.show();
    auto a = allocator_best.allocate(16);

    allocator_best.show();
    auto b = allocator_best.allocate(16);

    allocator_best.show();
    allocator_best.allocate(16);

    allocator_best.show();
    allocator_best.deallocate(b);

    allocator_best.show();
    allocator_best.deallocate(a);

    allocator_best.show();
    auto c = allocator_best.allocate(32);

    allocator_best.show();

    assert(c == a);

    std::cout << "\n=== Running Benchmarks ===\n";

    benchmark::RunSpecifiedBenchmarks();

    benchmark::Shutdown();
}
