#include <vector>
#include <cassert>

template <typename C, typename ... Ts> void push_back (C &container, Ts ... xs) {

    (container.push_back (xs), ...);
}

int main () {

    std::vector<int> vec_1 = {1, 2, 3};

    push_back (vec_1, 5, 3, 8);

    assert (std::size (vec_1) == 6);

    push_back (vec_1, 100 ,84);

    assert (std::size (vec_1) == 8);

    return 0;
}