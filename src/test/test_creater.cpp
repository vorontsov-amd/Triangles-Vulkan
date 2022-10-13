#include <fstream>
#include <random>


int main(int argc, char const *argv[])
{
    std::ofstream out("test.txt");

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> rand(-200, 200);



    out << "1000000 \n";

    for (int i = 0; i < 1000000; i++)
    {
        out << rand(gen) << " "
            << rand(gen) << " "
            << rand(gen) << "\t";

        out << rand(gen) << " "
            << rand(gen) << " "
            << rand(gen) << "\t";

        out << rand(gen) << " "
            << rand(gen) << " "
            << rand(gen) << "\n";
    }

    return 0;
}

