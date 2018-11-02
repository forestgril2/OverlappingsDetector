// Google Mock - a framework for writing C++ mock classes.
//
// This file tests code in OverlappingDetector.c

#include "gmock/gmock.h"

#include <string>
#include "gtest/gtest.h"
#include "gtest/internal/custom/gtest.h"
#include "OverlappingDetector.h"
#include <list>
#include <set>
#include <iostream>

using namespace std;


TEST(OverlappingDetector, Exists) 
{
    OverlappingDetector od;
}

TEST(OverlappingDetector, AcceptsMapOf4IntegerArrayToIntId)
{
    OverlappingDetector::RectIdMap m;
    OverlappingDetector od(m);
}

TEST(OverlappingDetector, HasOverlappingsFunction)
{
    OverlappingDetector od;
    od.overlappings();
}

TEST(OverlappingDetector, OverlappingsReturnsEmptyListWhenNotInitialized)
{
    OverlappingDetector od;        
    ASSERT_EQ(od.overlappings().size(), 0);
}

TEST(OverlappingDetector, OverlappingsReturnsEmptyListWhenInitializedWithEmptyMap)
{
    OverlappingDetector::RectIdMap m;
    OverlappingDetector od(m);    
    
    ASSERT_EQ(od.overlappings().size(), 0);
}

TEST(OverlappingDetector, OverlappingsReturnsEmptyListForOneElementMap)
{
    OverlappingDetector::RectIdMap m;
    m[0] = {0,0,0,0};
    OverlappingDetector od(m);    
    
    ASSERT_EQ(od.overlappings().size(), 0);
}

TEST(OverlappingDetector, OverlappingsReturnsEmptyListForForTwoPointsAtDifferentPositions)
{
    OverlappingDetector::RectIdMap m;
    m[0] = {1,2,1,2};
    m[1] = m[0];
    
    for (auto& elem : m[1])
    {//change this point
       elem++;
    }
    for (int i = 0; i < m[0].size(); i++)
    {
       ASSERT_NE(m[0][i], m[1][i]);
    }
    OverlappingDetector od(m);    
    
    ASSERT_EQ(od.overlappings().size(), 0);
}

TEST(OverlappingDetector, OverlappingsReturnsOneElementListForTwoPointsAtTheSamePosition)
{
    OverlappingDetector::RectIdMap m;
    m[0] = {1,2,1,2};
    m[1] = m[0];
    OverlappingDetector od(m);    
    
    ASSERT_EQ(od.overlappings().size(), 1);
}

TEST(OverlappingDetector, OverlappingsReturnsCorrectOverlappingPositionForTwoPointsAtTheSamePosition)
{
    OverlappingDetector::RectIdMap m;
    m[0] = {1,2,1,2};
    m[1] = m[0];
    OverlappingDetector od(m);
    
    ASSERT_EQ((*od.overlappings().begin()).first, m[0]);
}

TEST(OverlappingDetector, OverlappingsReturnsCorrectIdsForTwoPointsAtTheSamePosition)
{
    OverlappingDetector::RectIdMap m;
    
    int firstId = 77;
    int secondId = 99;
    m[firstId] = {1,2,1,2};
    m[secondId] = m[firstId];

    OverlappingDetector od(m);    
    
    OverlappingDetector::Overlappings overlappings_map = od.overlappings();
    auto overlappingIt = overlappings_map.find(m[firstId]);
    auto idsSet = (*overlappingIt).second;
    ASSERT_FALSE(idsSet.find(firstId) == idsSet.end());
    ASSERT_FALSE(idsSet.find(secondId) == idsSet.end());
}

// TEST(OverlappingDetector, OverlappingsReturnsAllCorrectPositionsForMultiplePointsAtTheSamePosition)
// {
//     OverlappingDetector::RectIdMap m;
//     m[0] = {1,2,1,2};
//     m[1] = m[0];
//     OverlappingDetector od(m);    
//     
//     ASSERT_EQ(od.overlappings().back().first, m[0]);
// }



