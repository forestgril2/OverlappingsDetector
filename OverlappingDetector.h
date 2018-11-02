#pragma once


#include <array>
#include <map>
#include <list>
#include <set>

class OverlappingDetector
{
public:    
    typedef std::array<int, 4> IntRect;
    typedef std::map<int, IntRect> RectIdMap;
    typedef std::map<IntRect, std::set<int>> Overlappings;
    
    OverlappingDetector() {};
    OverlappingDetector(const RectIdMap& m);
    
    Overlappings overlappings();

private:
    RectIdMap m_rects;
};

