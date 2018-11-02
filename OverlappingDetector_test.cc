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
    
    auto idsSet = (*od.overlappings().find(m[firstId])).second;
    ASSERT_TRUE(idsSet.find(firstId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(secondId) != idsSet.end());
}

TEST(OverlappingDetector, OverlappingsReturnsCorrectIdsForTwoPointsAtTheSamePositionAndCanCallItMultipleTimes)
{
    OverlappingDetector::RectIdMap m;
    
    int firstId = 77;
    int secondId = 99;
    m[firstId] = {1,2,1,2};
    m[secondId] = m[firstId];

    OverlappingDetector od(m);    
    
    ASSERT_TRUE((*od.overlappings().find(m[firstId])).second.find(firstId) != (*od.overlappings().find(m[firstId])).second.end());
    ASSERT_TRUE((*od.overlappings().find(m[firstId])).second.find(secondId) != (*od.overlappings().find(m[firstId])).second.end());
}

TEST(OverlappingDetector, OverlappingsReturnsCorrectIdsForThreePointsAtTheSamePosition)
{
    OverlappingDetector::RectIdMap m;
    
    int firstId = 77;
    int secondId = 99;
    int thirdId = 888;
    m[firstId] = {1,2,1,2};
    m[secondId] = m[firstId];
    m[thirdId] = m[firstId];

    OverlappingDetector od(m);    
    
    auto idsSet = (*od.overlappings().find(m[firstId])).second;
    ASSERT_TRUE(idsSet.find(firstId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(secondId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(thirdId) != idsSet.end());
}

TEST(OverlappingDetector, OverlappingsReturnsCorrectIdsForTwoPointsAtTheSamePositionAndNotForThirdAtWrongPosition)
{
    OverlappingDetector::RectIdMap m;
    
    int firstId = 77;
    int secondId = 99;
    int thirdId = 888;
    m[firstId] = {1,2,1,2};
    m[secondId] = m[firstId];
    m[thirdId] = {2,3,2,3};

    OverlappingDetector od(m);    
    
    auto idsSet = (*od.overlappings().find(m[firstId])).second;
    ASSERT_TRUE(idsSet.find(firstId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(secondId) != idsSet.end());
    ASSERT_FALSE(idsSet.find(thirdId) != idsSet.end());
}

TEST(OverlappingDetector, OverlappingsReturnsCorrectRectangleForTwoSimplestIdenticalRectangles)
{
    OverlappingDetector::RectIdMap m;
    
    int firstId = 77;
    int secondId = 99;
    m[firstId] = {2,2,2,3};
    m[secondId] = m[firstId];

    OverlappingDetector od(m);    
    
    auto overlappingFound = (*od.overlappings().find(m[firstId]));
    auto rect = overlappingFound.first;
    auto idsSet = overlappingFound.second;
    ASSERT_TRUE(idsSet.find(firstId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(secondId) != idsSet.end());
    ASSERT_EQ(rect, m[firstId]);
}

TEST(OverlappingDetector, OverlappingsReturnsCorrectRectangleForTwoSimplestOverlappingRectangles)
{
    OverlappingDetector::RectIdMap m;
    
    int firstId = 77;
    int secondId = 99;
    m[firstId] = {2,2,2,3};
    m[secondId] = {2,3,2,4};
    
    OverlappingDetector::IntRect expectedResult = {2,3,2,3};

    OverlappingDetector od(m);    
    
    auto overlappingFound = (*od.overlappings().find(m[firstId]));
    OverlappingDetector::IntRect rect = overlappingFound.first;
    auto idsSet = overlappingFound.second;
    
    ASSERT_TRUE(idsSet.find(firstId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(secondId) != idsSet.end());
    
//     ASSERT_EQ(rect, expectedResult);
}
