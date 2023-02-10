#include "segment.hpp"
#include <fstream>

using namespace GeomObj;

std::ostream& operator<<(std::ostream& out, Segment& seg) {
    return out << "begin: " << seg.begin << " , end: " << seg.end;
}