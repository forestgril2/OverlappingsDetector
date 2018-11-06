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
            
            auto overlappingRect = overlappingRectToIds.first;
            
            auto A = overlappingRect;
            auto B = freeRect;
            
            auto RectPointCoordWithinOtherRect = [this](const IntRect& rect, const IntRect& other, int point, int coord) {
                
                auto otherStart = other[0][coord];
                auto otherEnd = other[1][coord];
                auto rectPoint = rect[point][coord];
                
                return otherStart <= rectPoint && rectPoint <= otherEnd;
            };
            
            IntPoint ovlapStart;
            IntPoint ovlapEnd;
            
            bool xIsOverlapping = false;
            if (RectPointCoordWithinOtherRect(A, B, 0, 0))
            {//A x starts in B
                xIsOverlapping  = true;
                ovlapStart[0] = A[0][0];
                if (RectPointCoordWithinOtherRect(A, B, 1, 0))
                {//A x ends in B
                    ovlapEnd[0] = A[1][0];
                }
                else
                {//overlapping ends on B end
                    ovlapEnd[0] = B[1][0];
                }
            }
            else if (RectPointCoordWithinOtherRect(B, A, 0, 0))
            {//B x starts in A
                xIsOverlapping = true;
                ovlapStart[0] = B[0][0];
                if (RectPointCoordWithinOtherRect(B, A, 1, 0))
                {//B x ends in A
                    ovlapEnd[0] = B[1][0];
                }
                else
                {//overlapping ends on A end
                    ovlapEnd[0] = A[1][0];
                }
            }
            
            if (xIsOverlapping)
            {//don't check y, if x is not overlapping
                bool yIsOverlapping = false;
                if (RectPointCoordWithinOtherRect(A, B, 0, 1))
                {//A y starts in B
                    yIsOverlapping = true;
                    ovlapStart[1] = A[0][1];
                    if (RectPointCoordWithinOtherRect(A, B, 1, 1))
                    {//A y ends in B
                        ovlapEnd[1] = A[1][1];
                    }
                    else
                    {//overlapping ends on B end
                        ovlapEnd[1] = B[1][1];
                    }
                }
                else if (RectPointCoordWithinOtherRect(B, A, 0, 1))
                {//B y stars in A
                    yIsOverlapping = true;
                    ovlapStart[1] = B[0][1];
                    if (RectPointCoordWithinOtherRect(B, A, 1, 1))
                    {//B y ends in A
                        ovlapEnd[1] = B[1][1];
                    }
                    else
                    {//overlapping ends on A end
                        ovlapEnd[1] = A[1][1];
                    }
                }
                
                if (yIsOverlapping)
                {//this free rect and current overlapping rect have common overlapping rect
                    overlappingRect = IntRect({ovlapStart, ovlapEnd});
                    overlappingRectsToIdsSetMap[overlappingRect].insert(freeId);
                    auto setSize = overlappingIdSet.size();
                    if (setSize > 0)
                    {
                        for (const auto& id : overlappingIdSet)
                        {
                            overlappingRectsToIdsSetMap[overlappingRect].insert(id);
                        }
                    }
                }
            }
        }
    }
    
    for (auto r : overlappingRectsToIdsSetMap)
    {//remove single entries - rects, for which only one id exists
        if (r.second.size() < 2)
        {
            overlappingRectsToIdsSetMap.erase(r.first);
        }
    }
    return overlappingRectsToIdsSetMap;
}

