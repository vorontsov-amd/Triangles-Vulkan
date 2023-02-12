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
    
    EXPECT_EQ(GeomObj::IntersectTriangles(first, second), result);
    
    std::cout << "test " << testNumber << " PASSED \n";

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
