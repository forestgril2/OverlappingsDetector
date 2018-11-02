#include "OverlappingDetector.h"
#include <set>
#include <map>
#include <vector>

using namespace std;

OverlappingDetector::OverlappingDetector(const RectIdMap& m)
{
    for (const auto& mapping : m)
    {
        m_rects[mapping.first] = mapping.second;
    }
}

OverlappingDetector::Overlappings OverlappingDetector::overlappings()
{
    Overlappings overlaps;
    
    for (const auto& mapping : m_rects)
    {//write down all entries - rects with ids
        auto it = overlaps.find(mapping.second); 
        if (it == overlaps.end())
        {
            overlaps[mapping.second] = {mapping.first};
        }
        else
        {
            overlaps[mapping.second].insert(mapping.first);
        }
    }
    
    for (const auto& r : overlaps)
    {//remove single entries - rects, for which only one id exists
        if (r.second.size() < 2)
        {
            overlaps.erase(r.first);
        }
    }
    return overlaps;
}

