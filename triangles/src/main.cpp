#include <iostream>
#include "octree.hpp"
#include <fstream>
#include "intersect.hpp"
//=====================================================================================================

int TestIntersectionFunction ();

//=====================================================================================================
//#define DEBUG

int main()
{
    
#ifdef DEBUG
    if (TestIntersectionFunction ())
        std::cout << "Problems with file" << std::endl;
#else
    auto res = GetTriangles ();

    std::cout << "count  intersect: " << res << std::endl;
#endif
    
    return 0;
}

//=====================================================================================================

int TestIntersectionFunction () {

    const int numTests = 26;
    const char* nameTests[numTests] = {"../tests/trXtr/test1.txt", "../tests/trXtr/test2.txt", "../tests/trXtr/test3.txt", "../tests/trXtr/test4.txt", 
                                       "../tests/trXtr/test5.txt", "../tests/trXtr/test6.txt", "../tests/trXtr/test7.txt", "../tests/trXtr/test8.txt", 
                                       "../tests/trXseg/test1.txt", "../tests/trXseg/test2.txt", "../tests/trXseg/test3.txt", "../tests/trXseg/test4.txt", 
                                       "../tests/trXseg/test5.txt", "../tests/trXseg/test6.txt", "../tests/trXseg/test7.txt", "../tests/trXseg/test8.txt",
                                       "../tests/trXseg/test9.txt", "../tests/trXseg/test10.txt", "../tests/segXseg/test1.txt", "../tests/segXseg/test2.txt", 
                                       "../tests/segXseg/test3.txt", "../tests/segXseg/test4.txt", "../tests/segXseg/test5.txt", "../tests/segXseg/test6.txt", 
                                       "../tests/segXseg/test7.txt", "../tests/segXseg/test8.txt"};

    for (int i = 0; i < numTests; ++i) {

        std::ifstream ftest;

        ftest.open(nameTests[i]);
            if (!ftest.is_open()) {

                std::cerr << "File open error:";
                return -1;
            }

        GeomObj::Triangle tr1 {};
        GeomObj::Triangle tr2 {};

        ftest >> tr1 >> tr2;
        //assert (std::cin.good());

        bool result = GeomObj::IntersectTriangles(tr1, tr2);

        bool trueResult;
        ftest >> trueResult;

        if (result == trueResult)
            std::cout << "TEST " << i + 1 << " PASSED!" << std::endl;
        else    
            std::cout << "TEST " << i + 1 << " WRONG!" << std::endl;

        ftest.close();
    }

    

    return 0;   

}

