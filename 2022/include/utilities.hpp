/**
 * @file utilities.hpp
 * @author Daniel Nichols
 * @brief Utility routines
 * @date December 2022
 * 
 */
// stl includes
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>

namespace util {


std::vector<std::string> readLines(std::string const& fname) {
    std::ifstream infile(fname);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(infile, line)) {
        lines.push_back(line);
    }

    return lines;
}

std::vector<std::string> split(std::string const& s, char const delim) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string part;

    while (std::getline(ss, part, delim)) {
        parts.push_back(part);
    }
    return parts;
}

template <typename Iterator>
bool hasDuplicates(Iterator first, Iterator last) {
    std::unordered_set<std::iter_value_t<Iterator>> set(first, last);
    return set.size() != static_cast<size_t>( std::distance(first, last) );
}

class Timer {
public:
    void start() noexcept {
        start_ = std::chrono::high_resolution_clock::now();
    }

    void stop() noexcept {
        end_ = std::chrono::high_resolution_clock::now();
    }

    int64_t getDurationSeconds() const noexcept {
        return std::chrono::duration_cast<std::chrono::seconds>(end_ - start_).count();
    }

    int64_t getDurationMilliseconds() const noexcept {
        return std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count();
    }

    int64_t getDurationMicroseconds() const noexcept {
        return std::chrono::duration_cast<std::chrono::microseconds>(end_ - start_).count();
    }

    int64_t getDurationNanoseconds() const noexcept {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end_ - start_).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start_{}, end_{};
};


} // end namespace util