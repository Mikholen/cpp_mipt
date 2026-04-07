#include <array>
#include <bit>
#include <cstddef>
#include <iostream>
#include <memory>
#include <new>
#include <type_traits>

class Entity {
    
public:

    Entity() {

        static_assert(sizeof(Implementation) <= StorageSize);
        static_assert(alignof(Implementation) <= alignof(std::max_align_t));

        new (m_storage.data()) Implementation(42);
    }

    Entity(Entity&& other) {

        new (m_storage.data()) Implementation(std::move(*other.get()));
    }

    ~Entity() {

        std::destroy_at(get());
    }

    Entity& operator=(Entity&& other) {

        if (this != &other) {

            std::destroy_at(get());
            new (m_storage.data()) Implementation(std::move(*other.get()));
        }
        return *this;
    }

    void test() const {

        get()->print();
    }

private:

    class Implementation {

    public:
        Implementation(int x) : m_x(x) {

            std::cout << "Implementation ctor\n";
        }

        ~Implementation() {

            std::cout << "Implementation dtor\n";
        }

        void print() const {

            std::cout << "value = " << m_x << "\n";
        }

    private:
        int m_x;
    };

    static constexpr std::size_t StorageSize = 16;

    alignas(std::max_align_t)
    std::array<std::byte, StorageSize> m_storage;

    Implementation* get() {

        return std::launder(
            std::bit_cast<Implementation*>(m_storage.data())
        );
    }

    const Implementation* get() const {

        return std::launder(
            std::bit_cast<const Implementation*>(m_storage.data())
        );
    }
};

int main() {

    Entity e;
    e.test();

    Entity e2 = std::move(e);
    e2.test();
}