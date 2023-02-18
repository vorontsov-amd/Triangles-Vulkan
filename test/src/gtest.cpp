#include <gtest/gtest.h>
#include <fstream>
#include "intersect.hpp"

#ifndef PROJECT_DIR_PATH
  #define PROJECT_DIR_PATH ""
#endif

void TestBody (const std::string& path, int testNumber) {

    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Problem with open file " << testNumber <<  "\n";
        std::cerr << path << '\n';
        std::cerr << "test " << testNumber << " FAILURE \n";
        file.close();
        return;
    }

    GeomObj::Triangle first;
    GeomObj::Triangle second;
    bool result;

    file >> first >> second >> result;
    
    auto res = GeomObj::IntersectTriangles(first, second);
    EXPECT_EQ(res, result);
    
    if (res == result) {
        std::cout << "test " << testNumber << " PASSED \n";
    }
    else {
        std::cout << "test " << testNumber << " FAILED \n";
    }

    file.close();  
}


void TestBody2(const std::string& path, int testNumber) {

    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Problem with open file " << testNumber <<  "\n";
        std::cerr << path << '\n';
        std::cerr << "test FAILURE\n";
        file.close();
        return;
    }

    for (int i = 0; i < testNumber; ++i) {
        GeomObj::Triangle first;
        GeomObj::Triangle second;
        bool result;

        file >> first >> second >> result;
        
        auto res = GeomObj::IntersectTriangles(first, second);
        EXPECT_EQ(res, result);
        
        if (res == result) {
            std::cout << "test " << i << " PASSED \n";
        }
        else {
            std::cout << "test " << i << " FAILED \n";
        }
    }
    file.close();  
}




TEST(TestTriangles, TriangleXTriangle) {       
    const int NUMBER_OF_TEST = 8;
    
    for (int i = 1; i <= NUMBER_OF_TEST; ++i) {
        std::string path = static_cast<std::string> (PROJECT_DIR_PATH)
            + "/test/trXtr/test" + std::to_string(i) + ".txt";

        ::TestBody(path, i);
    }
}

TEST(TestTriangles, TriangleXSegment) {       
    const int NUMBER_OF_TEST = 10;
    
    for (int i = 1; i <= NUMBER_OF_TEST; ++i) {
        std::string path = static_cast<std::string> (PROJECT_DIR_PATH)
            + "/test/trXseg/test" + std::to_string(i) + ".txt";

        ::TestBody(path, i);
    }
}


TEST(TestTriangles, SegmentXSegment) {       
    const int NUMBER_OF_TEST = 8;
    
    for (int i = 1; i <= NUMBER_OF_TEST; ++i) {
        std::string path = static_cast<std::string> (PROJECT_DIR_PATH)
            + "/test/segXseg/test" + std::to_string(i) + ".txt";

        ::TestBody(path, i);
    }
}



TEST(TestTriangles, TriangleXPoint) {       
    const int NUMBER_OF_TEST = 3;
    std::string path = static_cast<std::string> (PROJECT_DIR_PATH) + "/test/trXpnt.txt";
    ::TestBody2(path, NUMBER_OF_TEST);
}


TEST(TestTriangles, PointXPoint) {       
    const int NUMBER_OF_TEST = 3;
    std::string path = static_cast<std::string> (PROJECT_DIR_PATH) + "/test/pntXpnt.txt";
    ::TestBody2(path, NUMBER_OF_TEST);
}


TEST(TestTriangles, SegmentXPoint) {       
    const int NUMBER_OF_TEST = 4;
    std::string path = static_cast<std::string> (PROJECT_DIR_PATH) + "/test/segXpnt.txt";
    ::TestBody2(path, NUMBER_OF_TEST);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
