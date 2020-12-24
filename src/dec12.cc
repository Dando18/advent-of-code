#include <cstdio>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

struct Nav {
    char type_;
    int value_;
};

struct Point {
    int x_; int y_;
};

void moveForward(Point &p, char curDirection, int value) {
    switch (curDirection) {
        case 'N': p.y_ += value; break;
        case 'S': p.y_ -= value; break;
        case 'E': p.x_ += value; break;
        case 'W': p.x_ -= value; break;
    }
}

/* PSA -- there are definitely better ways to do this; this is quick and hacky */
char rotate(char curDirection, char turnDirection, int deg) {
    deg = deg % 360;

    // only handle right turn
    if (turnDirection == 'L')
        deg = 360 - deg;

    switch (deg){
        case 0: return curDirection;
        case 90:
            if (curDirection == 'E') return 'S';
            else if (curDirection == 'S') return 'W';
            else if (curDirection == 'W') return 'N';
            else return 'E';
        case 180:
            if (curDirection == 'E') return 'W';
            else if (curDirection == 'S') return 'N';
            else if (curDirection == 'W') return 'E';
            else return 'S';
        case 270:
            if (curDirection == 'E') return 'N';
            else if (curDirection == 'S') return 'E';
            else if (curDirection == 'W') return 'S';
            else return 'W';
        default: fprintf(stderr, "Invalid degree %d\n", deg); return '_';
    }
}

void moveTowardsWaypoint(Point &p, Point const& waypoint, int amt) {
    Point dirVec {waypoint.x_ , waypoint.y_};

    p.x_ += amt * dirVec.x_;
    p.y_ += amt * dirVec.y_;
}

void rotateWaypoint(Point &waypoint, char dir, int deg) {
    deg = deg % 360;
    if (dir == 'R') deg = 360 - deg;

    int cosDeg = 1;
    int sinDeg = 0;
    switch (deg){
        case 0: cosDeg = 1; sinDeg = 0; break;
        case 90: cosDeg = 0; sinDeg = 1; break;
        case 180: cosDeg = -1; sinDeg = 0; break;
        case 270: cosDeg = 0; sinDeg = -1; break;
        default: fprintf(stderr, "Invalid degree %d\n", deg); break;
    }

    Point adj {0,0};
    adj.x_ = waypoint.x_*cosDeg - waypoint.y_*sinDeg;
    adj.y_ = waypoint.x_*sinDeg + waypoint.y_*cosDeg;
    waypoint.x_ = adj.x_;
    waypoint.y_ = adj.y_;
}

int main() {

    std::ifstream inFile("./inputs/dec12.txt");
    std::string line;
    std::vector<Nav> values;

    while (std::getline(inFile, line)) {
        Nav n;
        n.type_ = line.at(0);
        n.value_ = std::stoi(line.substr(1));
        values.push_back(n);
    }

    /* part 1 */
    Point pos {0, 0};
    char curDirection = 'E';
    for (auto const& v : values) {

        switch (v.type_) {
            case 'N': pos.y_ += v.value_; break;
            case 'S': pos.y_ -= v.value_; break;
            case 'E': pos.x_ += v.value_; break;
            case 'W': pos.x_ -= v.value_; break;
            case 'F': moveForward(pos, curDirection, v.value_); break;
            case 'R':
            case 'L': curDirection = rotate(curDirection, v.type_, v.value_); break;
        }

    }
    int manhattenDistanceTravelled = std::abs(pos.x_) + std::abs(pos.y_);
    printf("manhatten distance travelled part 1 = %d\n", manhattenDistanceTravelled);


    /* part 2 */
    pos.x_ = 0; pos.y_ = 0;
    Point waypoint {10, 1};
    for (auto const& v : values) {
        switch (v.type_) {
            case 'N': waypoint.y_ += v.value_; break;
            case 'S': waypoint.y_ -= v.value_; break;
            case 'E': waypoint.x_ += v.value_; break;
            case 'W': waypoint.x_ -= v.value_; break;
            case 'F': moveTowardsWaypoint(pos, waypoint, v.value_); break;
            case 'R':
            case 'L': rotateWaypoint(waypoint, v.type_, v.value_); break;
        }
    }
    manhattenDistanceTravelled = std::abs(pos.x_) + std::abs(pos.y_);
    printf("manhatten distance travelled part 2 = %d\n", manhattenDistanceTravelled);
}