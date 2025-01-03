#ifndef FREQUENCY
#define FREQUENCY

#include "lib/bytestring.h"

#include <unordered_map>

namespace frequency {
    extern std::unordered_map<char, double> characterFrequencies;
    std::pair<std::string, double> singleCharXORDecrypt(Bytestring);

    class FrequencyMap {
    private:
        std::string m_s;
        std::unordered_map<char, double> m_map;
    public:
        FrequencyMap();
        FrequencyMap(const std::string&);
        double distance();
    };

} /* frequency */

#endif /* FREQUENCY */