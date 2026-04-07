#include <iterator>
#include <memory>


template <typename T>
class List {

private:

    struct Node {

        T x = T();

        std::shared_ptr<Node> next = nullptr;
        std::weak_ptr<Node>   prev;

        Node(T value) : x(value) {}
    };

public:

    class Iterator {

    public:

        using iterator_category = std::bidirectional_iterator_tag; 

        Iterator(std::shared_ptr<Node> node = nullptr) : m_node(node) {}


        auto operator++(int) {

            auto tmp = *this;
            m_node = m_node->next;
            return tmp;
        }

        auto& operator++() {

            m_node = m_node->next;
            return *this;
        }


        auto operator--(int) {

            auto tmp = *this;
            m_node = m_node->prev.lock();
            return tmp;
        }

        auto& operator--() {

            m_node = m_node->prev.lock();
            return *this;
        }


        auto& operator*() const { return m_node->x; }
        auto  operator->() const { return &m_node->x; }


        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {

            return lhs.m_node == rhs.m_node;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            
            return !(lhs == rhs);
        }

    private:

        std::shared_ptr<Node> m_node;
    };


    auto begin() const { return Iterator(m_head); }
    auto end()   const { return Iterator(nullptr); }


    void push_back(T value) {

        auto node = std::make_shared<Node>(value);

        if (!m_head) {

            m_head = node;
            m_tail = node;
            return;
        }

        node->prev = m_tail;
        m_tail->next = node;

        m_tail = node;
    }

private:

    std::shared_ptr<Node> m_head = nullptr;
    std::shared_ptr<Node> m_tail = nullptr; 
};


int main() {

    List<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    for (auto it = list.begin(); it != list.end(); ++it)
        (void)*it;

    for ([[maybe_unused]] auto x : list);

    auto it = list.begin();
    ++it; ++it; 

    --it; 
}