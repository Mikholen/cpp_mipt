#include <cmath>
#include <cctype>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

class Stream
{
public:

    using token_t = std::variant<char, double, std::string>;

    Stream(std::string const& string) : m_stream(string + ';') {}

    auto empty()
    {
        return m_stream.peek() == ';';
    }

    auto get()
    {
        if (m_has_token)
        {
            m_has_token = false;
            return m_token;
        }

        char x = '\0';

        m_stream >> x;

        switch (x)
        {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '!':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case ';':
            {
                return token_t(x);
            }

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '.':
            {
                m_stream.unget();

                double y = 0;

                m_stream >> y;

                return token_t(y);
            }

            default:
            {
                std::string string(1, x);

                while (m_stream.get(x) && (std::isalpha(x) || std::isdigit(x)))
                {
                    string += x;
                }

                m_stream.unget();

                return token_t(string);
            }
        }
    }

    void put(token_t const& token)
    {
        m_token = token;
        m_has_token = true;
    }

private:

    std::stringstream m_stream;

    token_t m_token;

    bool m_has_token = false;
};

class Calculator
{
public:

    auto evaluate(std::string const& string) -> double
    {
        Stream stream(string);
        return statement(stream);
    }

private:

    auto factorial(double x) const -> double
    {
        double result = 1;

        for (int i = 1; i <= static_cast<int>(x); ++i)
        {
            result *= i;
        }

        return result;
    }

    auto statement(Stream& stream) -> double
    {
        auto token = stream.get();

        if (std::holds_alternative<std::string>(token))
        {
            if (std::get<std::string>(token) == "set")
            {
                return declaration(stream);
            }
        }

        stream.put(token);

        return expression(stream);
    }

    auto declaration(Stream& stream) -> double
    {
        auto string = std::get<std::string>(stream.get());

        m_variables[string] = expression(stream);

        return m_variables[string];
    }

    auto expression(Stream& stream) const -> double
    {
        auto x = term(stream);

        auto token = stream.get();

        while (true)
        {
            switch (std::get<char>(token))
            {
                case '+':
                {
                    x += term(stream);
                    break;
                }

                case '-':
                {
                    x -= term(stream);
                    break;
                }

                default:
                {
                    stream.put(token);
                    return x;
                }
            }

            token = stream.get();
        }
    }

    auto term(Stream& stream) const -> double
    {
        auto x = power(stream);

        auto token = stream.get();

        while (true)
        {
            switch (std::get<char>(token))
            {
                case '*':
                {
                    x *= power(stream);
                    break;
                }

                case '/':
                {
                    x /= power(stream);
                    break;
                }

                case '%':
                {
                    x = std::fmod(x, power(stream));
                    break;
                }

                default:
                {
                    stream.put(token);
                    return x;
                }
            }

            token = stream.get();
        }
    }

    auto power(Stream& stream) const -> double
    {
        auto x = postfix(stream);

        auto token = stream.get();

        if (std::holds_alternative<char>(token) &&
            std::get<char>(token) == '^')
        {
            x = std::pow(x, power(stream));
        }
        else
        {
            stream.put(token);
        }

        return x;
    }

    auto postfix(Stream& stream) const -> double
    {
        auto x = primary(stream);

        auto token = stream.get();

        while (std::holds_alternative<char>(token) &&
               std::get<char>(token) == '!')
        {
            x = factorial(x);
            token = stream.get();
        }

        stream.put(token);

        return x;
    }

    auto primary(Stream& stream) const -> double
    {
        auto token = stream.get();

        if (std::holds_alternative<char>(token))
        {
            switch (std::get<char>(token))
            {
                case '(':
                case '[':
                case '{':
                {
                    char left = std::get<char>(token);

                    auto x = expression(stream);

                    auto right = std::get<char>(stream.get());

                    if ((left == '(' && right != ')') ||
                        (left == '[' && right != ']') ||
                        (left == '{' && right != '}'))
                    {
                        throw std::runtime_error("bracket error");
                    }

                    return x;
                }

                case '+':
                {
                    return primary(stream);
                }

                case '-':
                {
                    return -primary(stream);
                }
            }
        }

        if (std::holds_alternative<double>(token))
        {
            return std::get<double>(token);
        }

        return m_variables.at(std::get<std::string>(token));
    }

    std::unordered_map<std::string, double> m_variables;
};

int main()
{
    std::fstream file("tests.txt", std::ios::in);

    if (!file)
    {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    Calculator calculator;

    std::string line;

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (line.empty())
        {
            continue;
        }

        try
        {
            std::cout << line
                    << " = "
                    << calculator.evaluate(line)
                    << '\n';
        }
        catch (std::exception const& exception)
        {
            std::cout << line
                    << " : error -> "
                    << exception.what()
                    << '\n';
        }
    }

    return 0;
}