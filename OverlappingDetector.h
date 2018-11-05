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
    
    int x0(const IntRect& r) {return r[0];}
    int x1(const IntRect& r) {return r[2];}
    int y0(const IntRect& r) {return r[1];}
    int y1(const IntRect& r) {return r[3];}
    
    RectIdMap m_rects;
};

