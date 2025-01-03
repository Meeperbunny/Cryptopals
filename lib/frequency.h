#ifndef FREQUENCY
#define FREQUENCY

#include <unordered_map>

namespace frequency {
    extern std::unordered_map<char, double> characterFrequencies;
    
    class FrequencyMap {
    private:
        std::unordered_map<char, double> m_map;
    public:
        FrequencyMap();
        FrequencyMap(const std::string&);
        double distance();
    };

} /* frequency */

#endif /* FREQUENCY */