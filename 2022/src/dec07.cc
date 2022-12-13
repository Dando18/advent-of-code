/**
 * @file dec07.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 7 https://adventofcode.com/2022/day/7
 *
 */
// stl includes
#include <iostream>
#include <limits>
#include <memory>
#include <stack>
#include <string>
#include <vector>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec07.txt";

struct File {
    std::string name;
    uint64_t size;
    bool isDir;
    File *parent;
    std::vector<File> children;
};

uint64_t getSize(File &f, bool setDirs = false) {
    if (!f.isDir) return f.size;

    uint64_t sum = 0;
    for (auto &child : f.children)
        sum += getSize(child, setDirs);
    if (setDirs) f.size = sum;
    return sum;
}

File parseInput(std::vector<std::string> const &lines) {

    File root{"/", 0, true, nullptr, {}};
    File *curFile;

    for (auto it = std::begin(lines); it != std::end(lines); it++) {
        auto const &line = *it;
        if (!line.starts_with('$')) {
            std::cerr << "Error\n";
            continue;
        }

        const auto lineParts = util::split(line, ' ');
        const auto cmd = lineParts.at(1);

        if (cmd == "cd") { /* cd command */
            const auto arg = lineParts.at(2);
            if (arg == "/") {
                curFile = &root;
            } else if (arg == "..") {
                curFile = curFile->parent;
            } else {
                /* arg must be one of the children */
                const auto fileIter = std::find_if(std::begin(curFile->children), std::end(curFile->children),
                                                   [&arg](File const &f) -> bool { return f.name == arg; });

                if (fileIter == std::end(curFile->children)) {
                    std::cerr << "Could not find '" << arg << "' in '" << curFile->name << "'\n";
                } else {
                    curFile = &(*fileIter);
                }
            }
        } else if (cmd == "ls") { /* ls command */

            it++;
            while (!it->starts_with('$') && !it->empty()) {
                auto const &lsLine = *it;
                auto const lsLineParts = util::split(lsLine, ' ');

                File newFile{lsLineParts.at(1), 0, false, curFile, {}};
                if (lsLine.starts_with("dir")) {
                    newFile.isDir = true;
                } else {
                    newFile.size = std::stoull(lsLineParts.at(0));
                }
                curFile->children.push_back(newFile);

                it++;
            }
            it--; /* move back one for for loop */
        }
    }

    /* set directory sizes */
    root.size = getSize(root, true);

    return root;
}

void printFileSystem(File const &file, int level = 0) {
    std::string indent(level, ' ');
    std::cout << indent << "- " << file.name << " ";
    if (file.isDir) {
        std::cout << "(dir)\n";
        for (auto const &child : file.children) {
            printFileSystem(child, level + 1);
        }
    } else {
        std::cout << "(file, size=" << file.size << ")\n";
    }
}

int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);
    File root = parseInput(lines);
    // printFileSystem(root);

    // part 1 -- sum of size of all dirs with size at most 100000
    constexpr uint64_t MAX = 100000;
    uint64_t sum = 0;
    std::stack<File *> toVisit;
    toVisit.push(&root);
    while (!toVisit.empty()) {
        auto curFile = toVisit.top();
        toVisit.pop();

        if (curFile->isDir) {
            if (curFile->size <= MAX) sum += curFile->size;

            for (auto &child : curFile->children)
                toVisit.push(&child);
        }
    }
    std::cout << sum << "\n";

    // part 2 -- 70000000 total disk space and we need 30000000. What's the
    //           smallest directory we can delete to get enough space.
    constexpr uint64_t TOTAL_SPACE = 70000000;
    constexpr uint64_t SPACE_NEEDED = 30000000;

    const uint64_t curAvailable = TOTAL_SPACE - root.size;
    const uint64_t stillNeeded = SPACE_NEEDED - curAvailable;

    uint64_t minViable = std::numeric_limits<uint64_t>::max();
    while (!toVisit.empty())
        toVisit.pop(); /* clear */
    toVisit.push(&root);
    while (!toVisit.empty()) {
        auto curFile = toVisit.top();
        toVisit.pop();

        if (curFile->isDir) {
            if (curFile->size >= stillNeeded) minViable = std::min(minViable, curFile->size);

            for (auto &child : curFile->children)
                toVisit.push(&child);
        }
    }
    std::cout << minViable << "\n";
}