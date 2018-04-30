/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "gtest/gtest.h"

#include "utility/Point3.hpp"

namespace mc {

TEST(TestPoint, BraceConstructor) {
    Point3i p(1, 2, 3);
    ASSERT_EQ(1, p[0]);
    ASSERT_EQ(2, p[1]);
    ASSERT_EQ(3, p[2]);
}

TEST(TestPoint, InitializerListConstructor) {
    Point3i p { 1, 2, 3 };
    ASSERT_EQ(1, p[0]);
    ASSERT_EQ(2, p[1]);
    ASSERT_EQ(3, p[2]);
}

TEST(TestPoint, SingleValueConstructor) {
    Point3i p(1);
    ASSERT_EQ(1, p[0]);
    ASSERT_EQ(1, p[1]);
    ASSERT_EQ(1, p[2]);
}

TEST(TestPoint, CopyConstructor) {
    Point3i p(1, 2, 3);
    Point3i p2(p);
    ASSERT_EQ(p[0], p2[0]);
    ASSERT_EQ(p[1], p2[1]);
    ASSERT_EQ(p[2], p2[2]);
}

TEST(TestPoint, CopyAssignment) {
    Point3i p(1, 2, 3);
    Point3i p2 = p;
    ASSERT_EQ(p[0], p2[0]);
    ASSERT_EQ(p[1], p2[1]);
    ASSERT_EQ(p[2], p2[2]);
}

TEST(TestPoint, AssignAddition) {
    Point3i p(1, 2, 3);
    Point3i p2(4, 5, 6);
    p2 += p;
    ASSERT_EQ(5, p2[0]);
    ASSERT_EQ(7, p2[1]);
    ASSERT_EQ(9, p2[2]);
}

TEST(TestPoint, AssignSubtraction) {
    Point3i p(30, 20, 10);
    Point3i p2(10, 20, 30);
    p2 -= p;
    ASSERT_EQ(-20, p2[0]);
    ASSERT_EQ(0, p2[1]);
    ASSERT_EQ(20, p2[2]);
}

TEST(TestPoint, AssignMultiplication) {
    Point3i p(1, 2, 3);
    Point3i p2(4, 5, 6);
    p2 *= p;
    ASSERT_EQ(4, p2[0]);
    ASSERT_EQ(10, p2[1]);
    ASSERT_EQ(18, p2[2]);
}

TEST(TestPoint, ManipulationByIndex) {
    Point3i p(1, 2, 3);
    p[0] = 4;
    p[1] = 5;
    p[2] = 6;
    ASSERT_EQ(4, p[0]);
    ASSERT_EQ(5, p[1]);
    ASSERT_EQ(6, p[2]);
}

TEST(TestPoint, Addition) {
    Point3i p(1, 2, 3);
    Point3i p2(4, 5, 6);
    Point3i result = p + p2;
    ASSERT_EQ(5, result[0]);
    ASSERT_EQ(7, result[1]);
    ASSERT_EQ(9, result[2]);
}

TEST(TestPoint, Subtraction) {
    Point3i p(1, 2, 3);
    Point3i p2(4, 5, 6);
    Point3i result = p - p2;
    ASSERT_EQ(-3, result[0]);
    ASSERT_EQ(-3, result[1]);
    ASSERT_EQ(-3, result[2]);
}

TEST(TestPoint, Multiplication) {
    Point3i p(1, 2, 3);
    Point3i p2(4, 5, 6);
    Point3i result = p * p2;
    ASSERT_EQ(4, result[0]);
    ASSERT_EQ(10, result[1]);
    ASSERT_EQ(18, result[2]);
}

TEST(TestPoint, Division) {
    Point3i p(10, 20, 30);
    Point3i p2(10);
    Point3i result = p / p2;
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
}

TEST(TestPoint, AdditionScalar) {
    Point3i p(1, 2, 3);
    Point3i result = p + 10;
    ASSERT_EQ(11, result[0]);
    ASSERT_EQ(12, result[1]);
    ASSERT_EQ(13, result[2]);
}

TEST(TestPoint, SubtractionScalar) {
    Point3i p(1, 2, 3);
    Point3i result = p - 1;
    ASSERT_EQ(0, result[0]);
    ASSERT_EQ(1, result[1]);
    ASSERT_EQ(2, result[2]);
}

TEST(TestPoint, MultiplicationScalar) {
    Point3i p(1, 2, 3);
    Point3i result = p * 10;
    ASSERT_EQ(10, result[0]);
    ASSERT_EQ(20, result[1]);
    ASSERT_EQ(30, result[2]);
}

TEST(TestPoint, DivisionScalar) {
    Point3i p(10, 20, 30);
    Point3i result = p / 5;
    ASSERT_EQ(2, result[0]);
    ASSERT_EQ(4, result[1]);
    ASSERT_EQ(6, result[2]);
}

TEST(TestPoint, AssignMultiplicationScalar) {
    Point3i p(1, 2, 3);
    p *= 10;
    ASSERT_EQ(10, p[0]);
    ASSERT_EQ(20, p[1]);
    ASSERT_EQ(30, p[2]);
}

TEST(TestPoint, AssignDivisionScalar) {
    Point3i p(10, 20, 30);
    p /= 5;
    ASSERT_EQ(2, p[0]);
    ASSERT_EQ(4, p[1]);
    ASSERT_EQ(6, p[2]);
}

TEST(TestPoint, LessFirstOnly) {
    Point3i p(3, 6, 6);
    Point3i p2(4, 5, 6);
    ASSERT_LT(p, p2);
}

TEST(TestPoint, LessFirstSecond) {
    Point3i p(3, 4, 6);
    Point3i p2(4, 5, 6);
    ASSERT_LT(p, p2);
}

TEST(TestPoint, LessAll) {
    Point3i p(3, 4, 5);
    Point3i p2(4, 5, 6);
    ASSERT_LT(p, p2);
}

TEST(TestPoint, BiggerFirstOnly) {
    Point3i p(5, 5, 6);
    Point3i p2(4, 5, 6);
    ASSERT_GT(p, p2);
}

TEST(TestPoint, BiggerFirstSecond) {
    Point3i p(5, 6, 6);
    Point3i p2(4, 5, 6);
    ASSERT_GT(p, p2);
}

TEST(TestPoint, BiggerAll) {
    Point3i p(5, 6, 7);
    Point3i p2(4, 5, 6);
    ASSERT_GT(p, p2);
}

TEST(TestPoint, LessEqualFirstOnly) {
    Point3i p(4, 6, 7);
    Point3i p2(4, 5, 6);
    ASSERT_LE(p, p2);
}

TEST(TestPoint, LessEqualFirstSecond) {
    Point3i p(4, 5, 7);
    Point3i p2(4, 5, 6);
    ASSERT_LE(p, p2);
}

TEST(TestPoint, LessEqualAll) {
    Point3i p(4, 5, 6);
    Point3i p2(4, 5, 6);
    ASSERT_LE(p, p2);
}

TEST(TestPoint, Equality) {
    Point3i p(4, 5, 6);
    Point3i p2(4, 5, 6);
    ASSERT_EQ(p, p2);
}

TEST(TestPoint, NonEqualityFirstOnly) {
    Point3i p(1, 2, 3);
    Point3i p2(1, 3, 2);
    ASSERT_NE(p, p2);
}

TEST(TestPoint, NonEqualityFirstSecondLastBigger) {
    Point3i p(1, 2, 3);
    Point3i p2(1, 2, 2);
    ASSERT_NE(p, p2);
}

TEST(TestPoint, NonEqualityFirstSecondLastSmaller) {
    Point3i p(1, 2, 2);
    Point3i p2(1, 2, 3);
    ASSERT_NE(p, p2);
}

TEST(TestPoint, InBoundariesAllAtLower) {
    Point3i min(0);
    Point3i max(100);
    Point3i p(0, 0, 0);
    ASSERT_TRUE(p.InBoundaries(min, max));
}

TEST(TestPoint, InBoundariesAllAtUpper) {
    Point3i min(0);
    Point3i max(100);
    Point3i p(100, 100, 100);
    ASSERT_TRUE(p.InBoundaries(min, max));
}

TEST(TestPoint, InBoundariesBelowLower) {
    Point3i min(0);
    Point3i max(100);
    Point3i p(-1, -1, -1);
    ASSERT_FALSE(p.InBoundaries(min, max));
}

TEST(TestPoint, InBoundariesAllAboveUpper) {
    Point3i min(0);
    Point3i max(100);
    Point3i p(101, 101, 101);
    ASSERT_FALSE(p.InBoundaries(min, max));
}

TEST(TestPoint, InBoundariesFirstBelowLower) {
    Point3i min(0);
    Point3i max(100);
    Point3i p(-1, 0, 0);
    ASSERT_FALSE(p.InBoundaries(min, max));
}

TEST(TestPoint, InBoundariesFirstSecondBelowLower) {
    Point3i min(0);
    Point3i max(100);
    Point3i p(-1, -1, 0);
    ASSERT_FALSE(p.InBoundaries(min, max));
}

TEST(TestPoint, InBoundariesFirstAboveUpper) {
    Point3i min(0);
    Point3i max(100);
    Point3i p(101, 100, 100);
    ASSERT_FALSE(p.InBoundaries(min, max));
}

TEST(TestPoint, InBoundariesFirstSecondAboveUpper) {
    Point3i min(0);
    Point3i max(100);
    Point3i p(101, 100, 100);
    ASSERT_FALSE(p.InBoundaries(min, max));
}

}   // namespace mc