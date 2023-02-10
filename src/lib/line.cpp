#include "line.hpp"
#include <fstream>

using namespace GeomObj;

std::ostream& operator<<(std::ostream& out, Line& line) {
    return out << "entry: " << line.entry << " , diretiction: " << line.direction;
}