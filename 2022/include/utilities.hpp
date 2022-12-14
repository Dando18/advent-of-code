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

std::vector<std::string> split(std::string const& s, std::string const& delim) {
    std::vector<std::string> parts;

    auto start = 0U;
    auto end = s.find(delim);
    while (end != std::string::npos) {
        parts.push_back(s.substr(start, end - start));
        start = end + delim.length();
        end = s.find(delim, start);
    }
    parts.push_back(s.substr(start, end));

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




template <typename DType>
class Point2D {
public:
    DType x, y;

    bool operator==(Point2D const& p) const noexcept {
        if constexpr (std::is_floating_point_v<DType>) {
            return (std::abs(this->x - p.x) <= std::numeric_limits<DType>::epsilon())
                && (std::abs(this->y - p.y) <= std::numeric_limits<DType>::epsilon());
        } else {
            return (this->x == p.x) && (this->y == p.y);
        }
    }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& oss, Point2D<U> const& p);
};

template<typename DType_>
std::ostream& operator<<(std::ostream& oss, Point2D<DType_> const& p) {
    oss << '(' << p.x << ',' << p.y << ')';
    return oss;
}



} // end namespace util