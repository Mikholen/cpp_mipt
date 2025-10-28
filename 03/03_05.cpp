#include <iostream>

class Entity_v1 {

public: 

    virtual ~Entity_v1 () {

        std::cout << "Entity_v1::~Entity_v1" << '\n';
    };

    virtual void test () const {

        std::cout << "Entity_v1::test" << '\n';
    }

};

class Entity_v2 {

public: 

    virtual ~Entity_v2 () {

        std::cout << "Entity_v2::~Entity_v2" << '\n';
    };

    virtual void test () const {

        std::cout << "Entity_v2::test" << '\n';
    }

};

class Adapter_v1 : public Entity_v1 {

public: 

    virtual void test_v1 () const {

        std::cout << "Adapter_v1::test_v1" << '\n';
        Entity_v1::test ();
    }
};

class Adapter_v2 : public Entity_v2 {

public: 

    virtual void test_v2 () const {

        std::cout << "Adapter_v2::test_v2" << '\n';
        Entity_v2::test ();
    }
};

class Client : public Adapter_v1, public Adapter_v2 {

public:

    void test_v1 () const override final {

        std::cout << "Client::test_v1" << '\n';
        Adapter_v1::test_v1 ();
    }

    void test_v2 () const override final {

        std::cout << "Client::test_v1" << '\n';
        Adapter_v2::test_v2 ();
    }
};

int main () {

    Client client;

    // client.test_v1 ();
    client.test_v2 ();

    return 0;
}