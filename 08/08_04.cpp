#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cctype>

int main() {
    std::string target = "methinksitislikeaweasel";
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> letter_dist(0, 25);
    std::uniform_real_distribution<double> mutation_dist(0.0, 1.0);
    
    std::string current(23, 'a');
    for (char& c : current) {
        c = 'a' + letter_dist(engine);
    }
    
    int generation = 0;
    
    while (true) {
        std::cout << "Generation " << generation++ << ": " << current << std::endl;
        
        auto score = [&target](const std::string& s) {
            int matches = 0;
            for (size_t i = 0; i < target.size(); ++i) {
                if (s[i] == target[i]) ++matches;
            }
            return target.size() - matches;
        };
        
        if (score(current) == 0) {
            std::cout << "Success at generation " << generation << std::endl;
            break;
        }
        
        std::vector<std::pair<std::string, int>> candidates;
        
        for (int i = 0; i < 100; ++i) {
            std::string mutant = current;
            for (char& c : mutant) {
                if (mutation_dist(engine) < 0.05) {
                    c = 'a' + letter_dist(engine);
                }
            }
            candidates.emplace_back(mutant, score(mutant));
        }
        
        auto best = std::min_element(candidates.begin(), candidates.end(),
            [](const auto& a, const auto& b) {
                return a.second < b.second;
            });
        
        current = best->first;
    }
    
    return 0;
}
