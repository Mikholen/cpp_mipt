#include <iostream>
#include <cmath>
#include <cassert>
#include <sstream>

class List {

public:

    static const int poizon = 9417676;

    struct Node {

        int element         = poizon;
        Node *next_elem_ptr = nullptr;
    };

    bool empty () {

        return begin == nullptr;
    }

    void show () {

        Node *current = begin;

        while (current != nullptr) {

            std::cout << current->element << " ";
            current = current->next_elem_ptr;
        }
        
        std::cout << '\n';
    }

    void push_front (const int new_elem) {

        if (begin == nullptr) {

            begin = end =  new Node{new_elem, nullptr};
        }

        else {

            begin = new Node{new_elem, begin};
        }
    }

    void push_back (const int new_elem) {

        if (begin == nullptr) {

            begin = end = new Node{new_elem, nullptr};
        }

        else {

            end->next_elem_ptr = new Node{new_elem, nullptr};
            end = end->next_elem_ptr;
        }
    }

    int pop_front () {

        if (begin == nullptr) {

            return poizon;
        }

        else {

            Node *tmp = begin;
            int ret_val = tmp->element;
            begin = tmp->next_elem_ptr;
            delete tmp;
            
            return ret_val;
        }
    }

    int pop_back () {

        if (begin == nullptr) {

            return poizon;
        }

        else if (begin == end) {

            int ret_val = end->element;
            delete begin;

            begin = end = nullptr;

            return ret_val;
        }

        else {

            Node *current = begin;
            while (current->next_elem_ptr != end) {

                current = current->next_elem_ptr;
            }

            int ret_val = end->element;
            delete end;

            end = current;
            end->next_elem_ptr = nullptr;
            
            return ret_val;
        }
    }

    int get () {

        if (begin == nullptr) return poizon;

        Node* slow = begin;
        Node* fast = begin;

        while (fast != nullptr && fast->next_elem_ptr != nullptr) {

            slow = slow->next_elem_ptr;
            fast = fast->next_elem_ptr->next_elem_ptr;
        }

        return slow->element;
    }

    ~List () {

        Node *current = begin;

        while (current != nullptr) {

            Node *next = current->next_elem_ptr;
            delete current;
            current = next;
        }
    }

private:

    Node *begin = nullptr;
    Node *end   = nullptr;
};

void test_empty() {

    List list;
    assert(list.empty());

    list.push_front(10);
    assert(!list.empty());

    list.pop_front();
    assert(list.empty());
}

void test_push_front_and_pop_front() {

    List list;
    list.push_front(1);
    assert(list.get() == 1);

    list.push_front(2);
    list.push_front(8);
    assert(list.get() == 2);

    assert(list.pop_front() == 8);
    assert(list.pop_front() == 2);
    assert(list.pop_front() == 1);

    assert(list.empty());
}

void test_push_back_and_pop_back() {

    List list;
    list.push_back(1);
    assert(list.get() == 1);

    list.push_back(2);
    list.push_back(3);
    assert(list.get() == 2);

    assert(list.pop_back() == 3);
    assert(list.pop_back() == 2);
    assert(list.pop_back() == 1);

    assert(list.empty());
}

void test_integrated_behavior() {

    List list;
    list.push_back(1);
    list.push_back(2);
    list.push_front(0);

    assert(list.get() == 1);
    assert(list.pop_front() == 0);
    assert(list.pop_back() == 2);
    assert(list.get() == 1);

    list.pop_front();
    assert(list.empty());
}


void tests () {

    test_empty ();
    test_push_back_and_pop_back ();
    test_push_front_and_pop_front ();
    test_integrated_behavior ();
}

int main () {

    tests ();

    return 0;
}