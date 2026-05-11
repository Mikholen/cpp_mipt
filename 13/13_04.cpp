#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

int main()
{
    std::filesystem::create_directories("1/2/3");

    std::fstream("1/file.txt", std::ios::out);
    std::fstream("1/data.bin", std::ios::out);
    std::fstream("1/test.txt", std::ios::out);
    std::fstream("1/2/image.png", std::ios::out);
    std::fstream("1/2/3/output.data", std::ios::out);

    std::vector<std::string> tests =
    {
        R"(.*\.txt)",
        R"(.*\.data)",
        R"(.*\.png)"
    };

    for (auto const& pattern : tests)
    {
        std::cout << "regex: " << pattern << '\n';

        std::regex regex(pattern);

        for (auto const& entry :
             std::filesystem::recursive_directory_iterator("1"))
        {
            std::string name = entry.path().filename().string();

            if (std::regex_match(name, regex))
            {
                std::cout << entry.path() << '\n';
            }
        }

        std::cout << '\n';
    }

    std::filesystem::remove_all("1");
}