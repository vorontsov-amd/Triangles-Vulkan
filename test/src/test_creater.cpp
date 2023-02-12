#include <fstream>
#include <random>

#if 0
int main(int argc, char const *argv[])
{
    std::ofstream out("test.txt");

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> bigRand(-1000, 1000);
    std::uniform_int_distribution<> smallRand(-50, 50);

    out << "10000 \n";

    for (int i = 0; i < 10000; i++)
    {
        auto coord_1 = bigRand(gen);
        auto coord_2 = bigRand(gen);
        auto coord_3 = bigRand(gen);

        out << coord_1 << " "
            << coord_2 << " "
            << coord_3 << "\t";

        out << coord_1 + smallRand(gen) << " "
            << coord_2 + smallRand(gen) << " "
            << coord_3 + smallRand(gen) << "\t";

        out << coord_1 + smallRand(gen) << " "
            << coord_2 + smallRand(gen) << " "
            << coord_3 + smallRand(gen) << "\n";
    }

    return 0;
}
#endif
