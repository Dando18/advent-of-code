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
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

// tpl includes
#include "gif.h"

namespace util {

std::vector<std::string> readLines(std::string const &fname) {
    std::ifstream infile(fname);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(infile, line)) {
        lines.push_back(line);
    }

    return lines;
}

std::vector<std::string> split(std::string const &s, char const delim) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string part;

    while (std::getline(ss, part, delim)) {
        parts.push_back(part);
    }
    return parts;
}

std::vector<std::string> split(std::string const &s, std::string const &delim) {
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
    return set.size() != static_cast<size_t>(std::distance(first, last));
}

template <typename DType_>
bool isBetween(DType_ val, DType_ min, DType_ max) {
    return (val >= min) && (val <= max);
}

template <typename DType_>
DType_ clamp(DType_ val, DType_ min, DType_ max) {
    return (val < min) ? min : (val > max) ? max : val;
}


class Timer {
  public:
    void start() noexcept { start_ = std::chrono::high_resolution_clock::now(); }

    void stop() noexcept { end_ = std::chrono::high_resolution_clock::now(); }

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

    bool operator==(Point2D const &p) const noexcept {
        if constexpr (std::is_floating_point_v<DType>) {
            return (std::abs(this->x - p.x) <= std::numeric_limits<DType>::epsilon()) &&
                   (std::abs(this->y - p.y) <= std::numeric_limits<DType>::epsilon());
        } else {
            return (this->x == p.x) && (this->y == p.y);
        }
    }

    template <typename U>
    friend std::ostream &operator<<(std::ostream &oss, Point2D<U> const &p);
};

template <typename DType_>
std::ostream &operator<<(std::ostream &oss, Point2D<DType_> const &p) {
    oss << '(' << p.x << ',' << p.y << ')';
    return oss;
}

class Gif {
  public:
    typedef std::array<uint8_t, 4> Color;

    Gif(std::string const &fname, uint32_t width, uint32_t height, int delay)
        : width_(width), height_(height), delay_(delay), writer_(), data_(width * height * 4, 0) {

        GifBegin(&writer_, fname.c_str(), width_, height_, delay_);
    }

    ~Gif() { GifEnd(&writer_); }

    uint32_t width() const noexcept { return width_; }
    uint32_t height() const noexcept { return height_; }

    void setPixel(uint32_t i, uint32_t j, Color const &color) {
        const uint32_t index = 4 * (i * width_ + j);
        for (auto const offset : {0, 1, 2, 3})
            data_.at(index + offset) = color.at(offset);
    }

    void fillBlack() noexcept { std::fill(std::begin(data_), std::end(data_), 0); }
    void fillWhite() noexcept { std::fill(std::begin(data_), std::end(data_), 255); }
    void fill(Color const &color) {
        for (uint32_t i = 0; i < height_; i += 1)
            for (uint32_t j = 0; j < width_; j += 1)
                setPixel(i, j, color);
    }

    template <typename Container, typename Func>
    void map(Container vals, Func func) {
        if (vals.size() != width_ * height_) {
            std::cerr << "Gif::map -- not enough pixels\n";
            return;
        }
        for (uint32_t i = 0; i < height_; i += 1)
            for (uint32_t j = 0; j < width_; j += 1)
                setPixel(i, j, func(vals.at(i * width_ + j)));
    }

    template <typename Grid, typename Func>
    void mapGrid(Grid vals, Func func) {
        if (vals.size() != height_ || vals.front().size() != width_) {
            std::cerr << "Gif::mapGrid -- wrong grid dimensions\n";
            return;
        }
        for (uint32_t i = 0; i < height_; i += 1)
            for (uint32_t j = 0; j < width_; j += 1)
                setPixel(i, j, func(vals.at(i).at(j)));
    }

    void dumpFrame() noexcept { GifWriteFrame(&writer_, data_.data(), width_, height_, delay_); }

  private:
    uint32_t width_, height_;
    int delay_;
    GifWriter writer_;
    std::vector<uint8_t> data_;
};

} // end namespace util


namespace std {
template <typename T>
struct hash<::util::Point2D<T>> {
    std::size_t operator()(const ::util::Point2D<T> &pos) const noexcept {
        return std::hash<int64_t>()(pos.x) * 31 + std::hash<int64_t>()(pos.y);
    }
};
} // namespace std