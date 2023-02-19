#include <gtest/gtest.h>
#include <fstream>
#include "intersect.hpp"

#ifndef PROJECT_DIR_PATH
  #define PROJECT_DIR_PATH ""
#endif

void TestBody(const std::string& path, int testNumber) {

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
        bool expected_result;

        file >> first >> second >> expected_result;
        
        auto recived_result = GeomObj::IntersectTriangles(first, second);
        EXPECT_EQ(recived_result, expected_result);
        
        if (recived_result != expected_result) {
            std::cout << "test " << i << " FAILED \n";
        }
    }
    file.close();  
}




TEST(TestTriangles, TriangleXPoint) {       
    const int NUMBER_OF_TEST = 4;
    std::string path = static_cast<std::string> (PROJECT_DIR_PATH) + "/test/trXpnt.txt";
    ::TestBody(path, NUMBER_OF_TEST);
}



TEST(TestTriangles, PointXPoint) {       
    const int NUMBER_OF_TEST = 3;
    std::string path = static_cast<std::string> (PROJECT_DIR_PATH) + "/test/pntXpnt.txt";
    ::TestBody(path, NUMBER_OF_TEST);
}


TEST(TestTriangles, SegmentXPoint) {       
    const int NUMBER_OF_TEST = 6;
    std::string path = static_cast<std::string> (PROJECT_DIR_PATH) + "/test/segXpnt.txt";
    ::TestBody(path, NUMBER_OF_TEST);
}



TEST(TestTriangles, SegmentXSegment) {       
    const int NUMBER_OF_TEST = 17;
    std::string path = static_cast<std::string> (PROJECT_DIR_PATH) + "/test/segXseg.txt";
    ::TestBody(path, NUMBER_OF_TEST);
}


TEST(TestTriangles, TriangleXSegment) {       
    const int NUMBER_OF_TEST = 19;
    std::string path = static_cast<std::string> (PROJECT_DIR_PATH) + "/test/trXseg.txt";
    ::TestBody(path, NUMBER_OF_TEST);
}

TEST(TestTriangles, TriangleXTriangle2d) {       
    const int NUMBER_OF_TEST = 9;
    std::string path = static_cast<std::string> (PROJECT_DIR_PATH) + "/test/trXtr2d.txt";
    ::TestBody(path, NUMBER_OF_TEST);
}

TEST(TestTriangles, TriangleXTriangle3d) {       
    const int NUMBER_OF_TEST = 15;
    std::string path = static_cast<std::string> (PROJECT_DIR_PATH) + "/test/trXtr3d.txt";
    ::TestBody(path, NUMBER_OF_TEST);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
