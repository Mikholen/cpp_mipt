#include <memory>

class Entity {

public:

    virtual ~Entity() = default;
};

class Client : public Entity {};

class Server : public Entity {};

class Factory {

public:

    virtual ~Factory() = default;

    virtual auto make_entity() const -> std::unique_ptr<Entity> = 0;
};

class Factory_Client : public Factory {

public:

    auto make_entity() const -> std::unique_ptr<Entity> override {

        return std::make_unique<Client>();
    }
};

class Factory_Server : public Factory {

public:

    auto make_entity() const -> std::unique_ptr<Entity> override {

        return std::make_unique<Server>();
    }
};

int main() {

    auto factory = std::make_unique<Factory_Client>();
    [[maybe_unused]] auto entity = factory->make_entity();
}
