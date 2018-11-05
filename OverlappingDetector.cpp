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
    Overlappings overlappingRectsToIdsSetMap;
    
    for (const auto& idToRectPair : m_rects)
    {//write down mappings for rects with all region ids which mutually contain them
        auto freeRect = idToRectPair.second;
        auto freeId = idToRectPair.first;
        
        if (overlappingRectsToIdsSetMap.empty()) 
        {
            overlappingRectsToIdsSetMap[freeRect] = {freeId};
            continue;
        }
        
        for (const auto& overlappingRectToIds : overlappingRectsToIdsSetMap)
        {
            auto overlappingIdSet = overlappingRectToIds.second;
            
            if (overlappingIdSet.find(freeId) != overlappingIdSet.end())
            {//it is the same rect
                continue;
            }
            
            auto overlappingRect = overlappingRectToIds.first;
            
            auto A = overlappingRect;
            auto B = freeRect;
            
            bool Ax0WithinB = (x0(B) >= x0(A) && x0(A) >= x1(B));
            bool Ax1WithinB = (x0(B) >= x1(A) && x1(A) >= x1(B));
            
            bool Bx0WithinA = (x0(A) >= x0(B) && x0(B) >= x0(A));
            bool Bx1WithinA = (x0(A) >= x1(B) && x1(B) >= x0(A));
            
            bool Ay0WithinB = (y0(B) >= y0(A) && y0(A) >= y1(B));
            bool Ay1WithinB = (y0(B) >= y1(A) && y1(A) >= y1(B));
            
            bool By0WithinA = (y0(A) >= y0(B) && y0(B) >= y0(A));
            bool By1WithinA = (y0(A) >= y1(B) && y1(B) >= y0(A));
            
            
            if(Ax0WithinB || Ax1WithinB || Bx0WithinA || Bx1WithinA || 
               Ay0WithinB || Ay1WithinB || By0WithinA || By1WithinA)
            {//this free rect and current overlapping rect have common overlapping rect
                overlappingRectsToIdsSetMap[overlappingRect].insert(freeId);
            }
        }
    }
    
    for (const auto& r : overlappingRectsToIdsSetMap)
    {//remove single entries - rects, for which only one id exists
        if (r.second.size() < 2)
        {
            overlappingRectsToIdsSetMap.erase(r.first);
        }
    }
    return overlappingRectsToIdsSetMap;
}

