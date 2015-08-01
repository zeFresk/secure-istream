#include <iostream>
#include "siostream.h"

sistream scin(std::cout, "Erreur de saisie, retapez.");

int main()
{
    std::cout << "Entrez un nom :" << std::endl;
    std::string nom;
    scin >> nom;
    std::cout << nom << std::endl;
    return 0;
}

