#include <iostream>

#include "riff.h"

int main(int argc, char* argv[])
{
     if (argc != 2)
     {
          std::cout << "Falta proporcionar el nombre del archivo." << std::endl;
     }

     try {
          RIFF riff(argv[1]);
     } catch (RIFF::Chunk::Type type) {
          std::cout << "Exception!  " << type << std::endl;
     } catch (size_t size) {
          std::cout << "Size exception!  " << size << std::endl;
     }

     return 0;
}
