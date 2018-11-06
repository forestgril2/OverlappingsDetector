#include "OverlappingDetector.h"
#include <set>
#include <map>
#include <vector>

using namespace std;
using namespace IntegerSurfaceAlgorithms;

OverlappingDetector::OverlappingDetector(const RectIdMap& m)
{
    for (const auto& mapping : m)
    {
        m_rects[mapping.first] = mapping.second;
    }
}

Overlappings OverlappingDetector::overlappings()
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
            {//this is the same rect or already contains this overlapping
                continue;
            }
            
            auto ovlapRect = overlappingRectToIds.first;
            
            IntPoint newOvlapStart;
            IntPoint newOvlapEnd;
            
            auto resolveOverlappingRectStartsInOther = [&](const IntRect& rect, const IntRect& other, int coord) {
                auto otherStart = other[0][coord];
                auto otherEnd = other[1][coord];
                auto rectPoint = rect[0][coord];
                
                if (otherStart <= rectPoint && rectPoint <= otherEnd)
                {//rect coord starts in other
                    newOvlapStart[coord] = rect[0][coord];
                    
                    rectPoint = rect[1][coord];
                    if (otherStart <= rectPoint && rectPoint <= otherEnd)
                    {//rect coord ends in other
                        newOvlapEnd[coord] = rect[1][coord];
                    }
                    else
                    {//overlapping ends on other end
                        newOvlapEnd[coord] = other[1][coord];
                    }
                    return true;
                }
                return false;
            };
            
            
            bool xIsOverlapping = resolveOverlappingRectStartsInOther(ovlapRect, freeRect, 0);
            if (!xIsOverlapping )
            {
                xIsOverlapping = resolveOverlappingRectStartsInOther(freeRect, ovlapRect, 0);
            }
            
            if (xIsOverlapping)
            {//don't check y, if x is not overlapping
                bool yIsOverlapping = resolveOverlappingRectStartsInOther(ovlapRect, freeRect, 1);
                if (!yIsOverlapping )
                {
                    yIsOverlapping = resolveOverlappingRectStartsInOther(freeRect, ovlapRect, 1);
                }
                
                if (yIsOverlapping)
                {//this free rect and current overlapping rect have common overlapping rect
                    ovlapRect = IntRect({newOvlapStart, newOvlapEnd});
                    overlappingRectsToIdsSetMap[ovlapRect].insert(freeId);
                    for (const auto& id : overlappingIdSet)
                    {
                        overlappingRectsToIdsSetMap[ovlapRect].insert(id);
                    }
                }
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

