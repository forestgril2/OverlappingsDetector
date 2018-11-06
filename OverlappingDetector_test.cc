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
using namespace IntegerSurfaceAlgorithms;

class OverlappingDetectorTest : public ::testing::Test
{
protected:
    OverlappingDetector od;
    RectIdMap m;
    IntPoint point = {1,2}; //some default point
    IntPoint otherPoint;
    int firstId = 77;
    int secondId = 99;
    
    OverlappingDetectorTest() 
    {
        otherPoint = {point[0] +1, point[1] +1};
    }
    
    void setRect(int id, IntPoint a, IntPoint b)
    {
        m[id] = IntRect({a, b});
        od = OverlappingDetector(m);
    }
};

TEST(OverlappingDetector, Exists) 
{
    OverlappingDetector od;
}

TEST(OverlappingDetector, AcceptsMapOf4IntegerArrayToIntId)
{
    RectIdMap m;
    OverlappingDetector od(m);
}

TEST(OverlappingDetector, HasOverlappingsFunction)
{
    OverlappingDetector od;
    od.overlappings();
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsEmptyMapWhenNotInitialized)
{
    ASSERT_EQ(od.overlappings().size(), 0);
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsEmptyMapWhenInitializedWithEmptyMap)
{
    OverlappingDetector od(m);    
    
    ASSERT_EQ(od.overlappings().size(), 0);
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsEmptyMapForOneElementMap)
{
    setRect(0, point, point);    
    
    ASSERT_EQ(od.overlappings().size(), 0);
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsEmptyMapForTwoPointsAtDifferentPositions)
{
    setRect(0, point, point);    
    setRect(1, otherPoint, otherPoint);
    
    ASSERT_EQ(od.overlappings().size(), 0);
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsOneElementMapForTwoPointsAtTheSamePosition)
{
    setRect(0, point, point);    
    setRect(1, point, point);   
    
    ASSERT_EQ(od.overlappings().size(), 1);
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsCorrectOverlappingPositionForTwoPointsAtTheSamePosition)
{
    setRect(0, point, point);    
    setRect(1, point, point);   
    
    ASSERT_EQ((*od.overlappings().begin()).first, m[0]);
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsCorrectIdsForTwoPointsAtTheSamePosition)
{
    setRect(firstId, point, point);    
    setRect(secondId, point, point);   
    
    auto idsSet = (*od.overlappings().find(m[firstId])).second;
    ASSERT_TRUE(idsSet.find(firstId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(secondId) != idsSet.end());
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsCorrectIdsForTwoPointsAtTheSamePositionAndCanCallItMultipleTimes)
{
    setRect(firstId, point, point);    
    setRect(secondId, point, point);     
    
    ASSERT_TRUE((*od.overlappings().find(m[firstId])).second.find(firstId) != (*od.overlappings().find(m[firstId])).second.end());
    ASSERT_TRUE((*od.overlappings().find(m[firstId])).second.find(secondId) != (*od.overlappings().find(m[firstId])).second.end());
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsCorrectIdsForThreePointsAtTheSamePosition)
{
    int thirdId = 888;
    
    setRect(firstId, point, point);    
    setRect(secondId, point, point);
    setRect(thirdId, point, point);
    
    auto idsSet = (*od.overlappings().find(m[firstId])).second;
    ASSERT_TRUE(idsSet.find(firstId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(secondId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(thirdId) != idsSet.end());
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsCorrectIdsForTwoPointsAtTheSamePositionAndNotForThirdAtWrongPosition)
{
    int thirdId = 888;
    
    setRect(firstId, point, point);    
    setRect(secondId, point, point);
    setRect(thirdId, otherPoint, otherPoint);   
    
    auto idsSet = (*od.overlappings().find(m[firstId])).second;
    ASSERT_TRUE(idsSet.find(firstId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(secondId) != idsSet.end());
    ASSERT_FALSE(idsSet.find(thirdId) != idsSet.end());
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsCorrectRectangleForTwoSimplestIdenticalRectangles)
{
    otherPoint = {point[0], point[1] +1};
    
    setRect(firstId, point, otherPoint);    
    setRect(secondId, point, otherPoint);    
    
    auto overlappingFound = (*od.overlappings().find(m[firstId]));
    auto rect = overlappingFound.first;
    auto idsSet = overlappingFound.second;
    ASSERT_TRUE(idsSet.find(firstId) != idsSet.end());
    ASSERT_TRUE(idsSet.find(secondId) != idsSet.end());
    ASSERT_EQ(rect, m[firstId]);
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsOneElementMapForTwoSimplestNonIdenticalOverlappingRectangles)
{
    otherPoint = {point[0], point[1] +1};
    IntPoint nextPoint = {otherPoint[0], otherPoint[1] +1};
    
    setRect(firstId, point, otherPoint);    
    setRect(secondId, otherPoint, nextPoint); 
    
    ASSERT_EQ(od.overlappings().size(), 1);

}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsCorrectRectangleForTwoSimplestNonIdenticalOverlappingRectangles)
{
    otherPoint = {point[0], point[1] +1};
    IntPoint nextPoint = {otherPoint[0], otherPoint[1] +1};
    
    IntRect expectedResult = {otherPoint, otherPoint};
    
    setRect(firstId, point, otherPoint);    
    setRect(secondId, otherPoint, nextPoint); 
    
    auto overlappings = od.overlappings();
    ASSERT_TRUE(overlappings.find(expectedResult) != overlappings.end());
}

TEST_F(OverlappingDetectorTest, OverlappingsReturnsCorrectIdsTwoSimplestNonIdenticalOverlappingRectangles)
{
    otherPoint = {point[0], point[1] +1};
    IntPoint nextPoint = {otherPoint[0], otherPoint[1] +1};
    
    IntRect expectedResult = {otherPoint, otherPoint};
    
    setRect(firstId, point, otherPoint);    
    setRect(secondId, otherPoint, nextPoint); 
    
    auto overlappingFoundsId = (*(od.overlappings().find(expectedResult))).second;
    ASSERT_TRUE(overlappingFoundsId.find(firstId) != overlappingFoundsId.end());
    ASSERT_TRUE(overlappingFoundsId.find(secondId) != overlappingFoundsId.end());
}

// TEST_F(OverlappingDetectorTest, OverlappingCorrectForNegativePointOrdering)
// {
// }
