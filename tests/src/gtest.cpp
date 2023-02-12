#include <gtest/gtest.h>
#include <fstream>
#include "intersect.hpp"

#ifndef PROJECT_DIR_PATH
  #define PROJECT_DIR_PATH ""
#endif

std::string concat(std::string str) {
    return PROJECT_DIR_PATH + str;
}

TEST(TriangleXTriangle, TestNumber0) {   
    std::ifstream file(concat("/test/trXtr/test1.txt"));
    
    GeomObj::Triangle first;
    GeomObj::Triangle second;
    bool result;

    file >> first >> second >> result;
    
    EXPECT_EQ(GeomObj::IntersectTriangles(first, second), result);
    
    file.close();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}