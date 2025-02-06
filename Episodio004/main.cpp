#include <iostream>

#include "riff.h"

int main(int argc, char* argv[])
{
     if (argc != 2)
     {
          std::cout << "Falta proporcionar el nombre del archivo." << std::endl;
     }

     RIFF riff(argv[1]);

     return 0;
}
