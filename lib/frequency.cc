#include "lib/frequency.h"

#include <cctype>
#include <cmath>

std::unordered_map<char, double> frequency::characterFrequencies = {
    {'a', 0.082},
    {'b', 0.015},
    {'c', 0.028},
    {'d', 0.043},
    {'e', 0.127},
    {'f', 0.022},
    {'g', 0.020},
    {'h', 0.061},
    {'i', 0.070},
    {'j', 0.0015},
    {'k', 0.0077},
    {'l', 0.040},
    {'m', 0.024},
    {'n', 0.067},
    {'o', 0.075},
    {'p', 0.019},
    {'q', 0.00095},
    {'r', 0.060},
    {'s', 0.063},
    {'t', 0.091},
    {'u', 0.028},
    {'v', 0.0098},
    {'w', 0.024},
    {'x', 0.0015},
    {'y', 0.020},
    {'z', 0.00074}
};

frequency::FrequencyMap::FrequencyMap(const std::string &s) {
    for(const auto &c : s) {
        ++m_map[std::tolower(c)];
    }
    for(auto &[k, v] : m_map) {
        v /= s.size();
    }
}

frequency::FrequencyMap::FrequencyMap() {
    FrequencyMap("");
}

double frequency::FrequencyMap::distance() {
    double totalDistance = 0.0;
    for(const auto &[k, v] : frequency::characterFrequencies) {
        double m_v = m_map[k];
        totalDistance += std::abs(m_v - v);
    }
    return totalDistance;
}