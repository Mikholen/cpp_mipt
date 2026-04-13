#include <memory>
#include <vector>

class Entity {

public:

    virtual ~Entity() = default;

    virtual auto copy() const -> std::unique_ptr<Entity> = 0;
};

class Client : public Entity {

public:

    auto copy() const -> std::unique_ptr<Entity> override {

        return std::make_unique<Client>(*this);
    }
};

class Server : public Entity {

public:

    auto copy() const -> std::unique_ptr<Entity> override {

        return std::make_unique<Server>(*this);
    }
};

class Prototype {

public:

    Prototype() {

        m_entities.push_back(std::make_unique<Client>());

        m_entities.push_back(std::make_unique<Server>());
    }

    auto make_client() -> std::unique_ptr<Entity> {

        return m_entities.at(0)->copy();
    }

    auto make_server() -> std::unique_ptr<Entity> {

        return m_entities.at(1)->copy();
    }

private:

    std::vector<std::unique_ptr<Entity>> m_entities;
};

int main()
{
    [[maybe_unused]] auto client = Prototype().make_client();
}

