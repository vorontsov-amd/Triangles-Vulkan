#include "triangle.hpp"
#include <unordered_set>
#include <vector>


int main()
{
    int num_of_triangles = 0;
    
    std::cin >> num_of_triangles;

    std::vector<Geomitric::Triangle> array(num_of_triangles);
    
    for (int i = 0; i < num_of_triangles; ++i)
    {
        std::cin >> array[i];
    }
    
    std::unordered_set<int> intersect_index;

    for (int i = 0; i < num_of_triangles; ++i)
    {
        if (intersect_index.contains(i)) continue;

        for (int j = i; j < num_of_triangles; ++j)
        {
            if (intersect_index.contains(j)) continue;
            
            if (Geomitric::TrianglesIntersect(array[i], array[j]))
            {
                intersect_index.insert({i, j});
            }
        }

        std::cout << intersect_index.size() << std::endl;

    }
}
