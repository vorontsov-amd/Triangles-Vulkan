#include <fstream>
#include <random>


int main(int argc, char const *argv[])
{
    std::ofstream out("test.txt");

    std::random_device rd;
    std::mt19937 gen(rd());

    //std::uniform_int_distribution<> zone(0, 8);
    std::uniform_int_distribution<> rand(-1000, 1000);
    std::uniform_int_distribution<> randlol(-50, 50);

    out << "10000 \n";

    for (int i = 0; i < 10000; i++)
    {
        auto coord_1 = rand(gen);
        auto coord_2 = rand(gen);
        auto coord_3 = rand(gen);

        out << coord_1 << " "
            << coord_2 << " "
            << coord_3 << "\t";

        out << coord_1 + randlol(gen) << " "
            << coord_2 + randlol(gen) << " "
            << coord_3 + randlol(gen) << "\t";

        out << coord_1 + randlol(gen) << " "
            << coord_2 + randlol(gen) << " "
            << coord_3 + randlol(gen) << "\n";
    }

    return 0;
}

