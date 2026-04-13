#include <cstddef>
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

class LinearAllocator : public Allocator, private boost::noncopyable {

public:

    LinearAllocator(std::size_t size) : m_size(size) {

        m_begin = operator new(m_size, std::align_val_t{s_alignment});
    }

    ~LinearAllocator() {

        operator delete(m_begin, m_size, std::align_val_t{s_alignment});
    }

    void* allocate(std::size_t size, std::size_t alignment = s_alignment) override {

        void* begin = get<std::byte>(m_begin) + m_offset;

        auto free = m_size - m_offset;

        if (begin = std::align(alignment, size, begin, free); begin) {
            m_offset = m_size - free + size;
            return begin;
        } else {
            return nullptr;
        }
    }

    void deallocate(void*) override {
    }

    void show() const {

        std::print(
            "LinearAllocator::show : m_size = {} m_begin = {} m_offset = {:0>4}\n",
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

        LinearAllocator allocator(gb);

        for (auto i = 0uz; i < kb; ++i) {

            vector[i] = allocator.allocate(mb);
        }

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(test_v1);
BENCHMARK(test_v2);

int main() {

    LinearAllocator linear(1 << 10);
    Allocator* alloc = &linear;

    linear.show();
    alloc->allocate(1, 1);
    linear.show();
    alloc->allocate(2, 2);
    linear.show();
    alloc->allocate(4, 4);
    linear.show();
    alloc->allocate(8, 8);
    linear.show();

    benchmark::RunSpecifiedBenchmarks();
}
