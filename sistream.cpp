#include "sistream.h"

//globals definitions
sistream scin(std::cin);
wsistream swcin(std::wcin);

//static definitions
std::ostream& output_object<char>::object = std::cout;
std::wostream& output_object<wchar_t>::object = std::wcout;
