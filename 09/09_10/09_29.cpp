#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
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

class StackAllocator : public Allocator, private boost::noncopyable {

public:

    StackAllocator(std::size_t size) : m_size(size) {

        m_begin = operator new(m_size, std::align_val_t{s_alignment});
    }

    ~StackAllocator() {

        operator delete(m_begin, m_size, std::align_val_t{s_alignment});
    }

    void* allocate(std::size_t size, std::size_t alignment = s_alignment) override {

        using header_t = std::uint8_t;

        void* begin = get<std::byte>(m_begin) + m_offset + sizeof(header_t);
        auto  free  = m_size - m_offset - sizeof(header_t);

        if (begin = std::align(alignment, size, begin, free); begin) {

            auto header = get<header_t>(get<std::byte>(begin) - sizeof(header_t));
            *header = static_cast<header_t>(
                std::distance(get<std::byte>(m_begin) + m_offset, get<std::byte>(begin))
            );
            m_offset = get<std::byte>(begin) - get<std::byte>(m_begin) + size;
            return begin;
        }
        else {

            return nullptr;
        }
    }

    void deallocate(void* ptr) override {

        using header_t = std::uint8_t;

        auto header = get<header_t>(get<std::byte>(ptr) - sizeof(header_t));
        m_offset = get<std::byte>(ptr) - get<std::byte>(m_begin) - *header;
    }

    void show() const {

        std::print(
            "StackAllocator::show : m_size = {} m_begin = {} m_offset = {:0>4}\n",
            m_size, m_begin, m_offset
        );
    }

private:

    std::size_t m_size   = 0;
    std::size_t m_offset = 0;
    void*       m_begin  = nullptr;

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

        for (auto i = 0uz; i < kb; ++i) {

            operator delete(vector[std::size(vector) - 1 - i], mb);
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

        StackAllocator allocator(2 * gb);

        for (auto i = 0uz; i < kb; ++i) {

            vector[i] = allocator.allocate(mb);
        }

        for (auto i = 0uz; i < kb; ++i) {

            allocator.deallocate(vector[std::size(vector) - 1 - i]);
        }

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(test_v1);
BENCHMARK(test_v2);

int main() {

    StackAllocator stack(1 << 10);
    Allocator* alloc = &stack;

    stack.show();
    alloc->allocate(1, 1);
    stack.show();
    alloc->allocate(2, 2);
    stack.show();
    auto x = alloc->allocate(4, 4);
    stack.show();
    auto y = alloc->allocate(8, 8);
    stack.show();

    alloc->deallocate(y);
    stack.show();
    alloc->deallocate(x);
    stack.show();

    auto z = alloc->allocate(8, 8);
    stack.show();

    assert(z == x);

    benchmark::RunSpecifiedBenchmarks();
}
