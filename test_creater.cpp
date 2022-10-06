#include <fstream>

int main(int argc, char const *argv[])
{
    std::ofstream out("test.txt");

    out << "10000 \n";

    for (int i = 0; i < 10000; i++)
    {
        out << (rand() % 10) << " "
            << (rand() % 10) << " "
            << (rand() % 10) << "\t";

        out << (rand() % 10) << " "
            << (rand() % 10) << " "
            << (rand() % 10) << "\t";

        out << (rand() % 10) << " "
            << (rand() % 10) << " "
            << (rand() % 10) << "\n";
    }

    return 0;
}

