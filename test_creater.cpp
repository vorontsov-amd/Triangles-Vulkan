#include <fstream>

int main(int argc, char const *argv[])
{
    std::ofstream out("test.txt");

    out << "1000 \n";

    for (int i = 0; i < 1000; i++)
    {
        out << int(rand() % 1000) << " "
            << int(rand() % 1000) << " "
            << int(rand() % 1000) << "\t";

        out << int(rand() % 1000) << " "
            << int(rand() % 1000) << " "
            << int(rand() % 1000) << "\t";

        out << int(rand() % 1000) << " "
            << int(rand() % 1000) << " "
            << int(rand() % 1000) << "\n";
    }

    return 0;
}

