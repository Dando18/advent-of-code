#include <cstdio>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <sstream>
#include <map>


struct Range {
    uint32_t lower, upper;

    bool in(uint32_t val) const { return (val >= lower) && (val <= upper); }
};

struct Field {
    std::string name;
    Range range1, range2;

    bool isValid(uint32_t val) const { return range1.in(val) || range2.in(val); }
};

typedef std::vector<uint32_t> Ticket;


Range parseRange(std::string const& s) {
    auto mid = s.find('-');
    uint32_t lower = std::stoul(s.substr(0,mid));
    uint32_t upper = std::stoul(s.substr(mid+1));
    return {lower, upper};
}

Field parseField(std::string const& s) {    
    auto name = s.substr(0, s.find(':'));
    
    auto firstRangeStart = s.find_first_of(' ', s.find(':'))+1;
    auto secondRangeStart = s.find_last_of(' ')+1;

    std::string range1Str = s.substr(firstRangeStart, s.find(' ', firstRangeStart)-firstRangeStart);
    auto range1 = parseRange(range1Str);

    std::string range2Str = s.substr(secondRangeStart);
    auto range2 = parseRange(range2Str);

    return {name, range1, range2};
}

Ticket parseTicket(std::string const& s) {
    std::istringstream ss(s);
    Ticket t;
    std::string valStr;

    while (std::getline(ss, valStr, ',')) {
        t.push_back( std::stoul(valStr) );
    }
    return t;
}

int main() {

    std::ifstream inFile("./inputs/dec16.txt");
    std::string line;

    std::vector<Field> fields;
    std::vector<Ticket> tickets;
    Ticket myTicket;

    /* read in fields */
    while (std::getline(inFile, line)) {
        if (line == "") break;
        fields.push_back( parseField(line) );
    }

    /* read in my_ticket */
    std::getline(inFile, line);
    std::getline(inFile, line);
    myTicket = parseTicket(line);
    std::getline(inFile, line);

    /* read in nearby tickets */
    std::getline(inFile, line);
    while (std::getline(inFile, line)) {
        tickets.push_back( parseTicket(line) );
    }

    /* find ticket error rate */
    uint32_t ticketErrorRate = 0;
    for (auto const& ticket : tickets) {
        for (auto const& entry : ticket) {
            bool hasValidField = false;
            for (auto const& field : fields) {
                if (field.isValid(entry)) {
                    hasValidField = true;
                }
            }
            if (!hasValidField) {
                ticketErrorRate += entry;
            }
        }
    }
    printf("ticket error rate = %u\n", ticketErrorRate);


    /* erase erroneous tickets */
    for (auto it = std::begin(tickets); it != std::end(tickets);) {
        bool shouldErase = false;

        for (auto const& entry : *it) {
            bool hasValidField = false;
            for (auto const& field : fields) {
                if (field.isValid(entry)) hasValidField = true;
            }
            if (!hasValidField) {
                shouldErase = true;
                break;
            }
        }

        if (shouldErase) {
            it = tickets.erase(it);
        } else {
            it++;
        }
    }

    /* find the correct index for each field */
    const size_t TICKET_SIZE = tickets.front().size();
    std::map<std::string, std::vector<size_t>> possibleFieldIndices;
    for (auto const& field : fields) {
        /* find an index for which field would work */
        for (size_t i = 0; i < TICKET_SIZE; i++) {
            bool isIndexValidForAllTickets = true;
            for (auto const& ticket : tickets) {
                if (!field.isValid(ticket.at(i))) {
                    isIndexValidForAllTickets = false;                    
                    break;
                }
            }

            /* this was valid so add it to map and break to look for next field */
            if (isIndexValidForAllTickets) {
                possibleFieldIndices[field.name].push_back(i);
            }
        }
    }

    /* for each possible ticket index */
    std::vector<size_t> usedIndices;
    std::map<std::string, size_t> fieldIndices;
    for (size_t i  = 0; i < TICKET_SIZE; i++) {
        for (auto & [name, indices] : possibleFieldIndices) {

            for (auto const& f : usedIndices) {
                indices.erase(std::remove(std::begin(indices), std::end(indices), f), std::end(indices));
            }

            if (indices.size() == 1) {
                /* must assign this field to index i */
                fieldIndices.insert({name, indices.at(0)});
                usedIndices.push_back(indices.at(0));
                indices.pop_back();

                break;
            }
        }
    }
    

    /* prod of all the 'departure ...' values in my_ticket */
    uint64_t prod = 1;
    for (auto const& [name, idx] : fieldIndices) {
        if (name.starts_with("departure")) {
            prod *= myTicket.at(idx);
        }
    }
    printf("product of departure fields on my ticket = %lu\n", prod);
}