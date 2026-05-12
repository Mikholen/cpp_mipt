#include <algorithm>
#include <cassert>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

using namespace std::literals;

int main()
{
    auto text = R"(Contact us at support@google.com or admin@example.org. For billing: sales@shop.net.)"s;

    auto begin = std::cbegin(text);
    auto end = std::cend(text);

    std::regex pattern(R"(([a-z0-9_\.-]+)@([a-z0-9_\.-]+\.[a-z]{2,}))", std::regex_constants::icase);

    std::vector<std::string> results;

    auto lambda = [&results](auto const& match) { results.push_back(match.str()); };

    std::ranges::for_each
    (
        std::sregex_token_iterator(begin, end, pattern, { 0, 2 }),
        std::sregex_token_iterator(),
        lambda
    );

    std::vector<std::string> expected = {
        "support@google.com", "google.com",
        "admin@example.org", "example.org",
        "sales@shop.net", "shop.net"
    };

    assert(results == expected);

    return 0;
}
// Score is 9/10


