#include <iostream>

class Entity {

private:

    
    friend class Attorney_1;
    friend class Attorney_2;

    static void test_v1 () {

        std::cout << "Entity::test_v1" << '\n';
    }

    static void test_v2 () {

        std::cout << "Entity::test_v2" << '\n';
    }
};

class Attorney_1 {

private:

    friend class Tester_v1;

    static void test_v1() {

        std::cout << "Attorney_1::test_v1" << '\n';

        Entity::test_v1();
    }

    static void test_v2() {

        std::cout << "Attorney_1::test_v2" << '\n';
    }
};

class Attorney_2 {

private:

    friend class Tester_v2;

    static void test_v1() {

        std::cout << "Attorney_2::test_v1" << '\n';
    }

    static void test_v2() {

        std::cout << "Attorney_2::test_v2" << '\n';

        Entity::test_v2();
    }
};

class Tester_v1 {

public:

    static void test () {

        Attorney_1::test_v1 ();
    }
};

class Tester_v2 {

public:

    static void test () {

        Attorney_2::test_v2 ();
    }
};

int main () {

    [[maybe_unused]] Entity entity;
    return 0;
}