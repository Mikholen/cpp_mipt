#include <algorithm>
#include <cassert>
#include <cctype>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

bool is_blank(std::string const& line)
{
    return std::ranges::all_of
    (
        line,
        [](unsigned char c)
        {
            return std::isspace(c);
        }
    );
}

int main()
{
    std::stringstream stream_1(R"(First line

    
Second line
     
Third line

    Fourth line
)");

    std::stringstream stream_2;

    std::vector<std::string> lines;

    std::ranges::copy
    (
        std::istream_iterator<std::string>(stream_1),
        std::istream_iterator<std::string>(),
        std::back_inserter(lines)
    );

    lines.erase
    (
        std::remove_if
        (
            lines.begin(),
            lines.end(),
            [](std::string const& line)
            {
                return line.empty() || is_blank(line);
            }
        ),
        lines.end()
    );

    std::ranges::copy
    (
        lines,
        std::ostream_iterator<std::string>(stream_2, "\n")
    );

    assert
    (
        stream_2.str() ==
        "First\n"
        "line\n"
        "Second\n"
        "line\n"
        "Third\n"
        "line\n"
        "Fourth\n"
        "line\n"
    );
}

// Score is 9/10
