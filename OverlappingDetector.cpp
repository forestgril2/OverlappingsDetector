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
            {//it is the same rect
                continue;
            }
            
            auto overlappingRect = overlappingRectToIds.first;
            
            auto A = overlappingRect;
            auto B = freeRect;
            
            auto RectPointCoordWithinOtherRect = [this](const IntRect& A, const IntRect& B, int rectPointIndex, int coordIndex) {
                return (B[0][coordIndex] >= A[rectPointIndex][coordIndex] && A[rectPointIndex][coordIndex] >= B[1][coordIndex]);
            };
            
            if (RectPointCoordWithinOtherRect(A, B, 0, 0) ||
                RectPointCoordWithinOtherRect(A, B, 1, 0) ||
                RectPointCoordWithinOtherRect(A, B, 0, 1) ||
                RectPointCoordWithinOtherRect(A, B, 1, 1) ||
                RectPointCoordWithinOtherRect(B, A, 0, 0) ||
                RectPointCoordWithinOtherRect(B, A, 1, 0) ||
                RectPointCoordWithinOtherRect(B, A, 0, 1) ||
                RectPointCoordWithinOtherRect(B, A, 1, 1))
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

