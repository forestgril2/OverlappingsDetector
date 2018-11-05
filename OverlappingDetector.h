#pragma once


#include <array>
#include <map>
#include <list>
#include <set>

namespace IntegerSurfaceAlgorithms
{

typedef std::array<int, 2> IntPoint;
typedef std::array<IntPoint, 2> IntRect;
typedef std::map<int, IntRect> RectIdMap;
typedef std::map<IntRect, std::set<int>> Overlappings;

// IntPoint start(const IntRect& r) {return r[0];}
// IntPoint end(const IntRect& r) {return r[1];}
// int x(const IntPoint& p) {return p[0];} 
// int y(const IntPoint& p) {return p[1];} 

class OverlappingDetector
{
public:    
    OverlappingDetector() {};
    OverlappingDetector(const RectIdMap& m);
    
    Overlappings overlappings();

private:
    RectIdMap m_rects;
};

}
