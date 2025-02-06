#include "riff.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

RIFF::RIFF(std::string_view filename):
     _filename{std::string{filename}}
{
     std::fstream stream(filename.data());

     stream.seekg(0, stream.end);
     int file_length = stream.tellg();
     stream.seekg(0, stream.beg);

     std::vector<char> buffer(file_length, '\0');
     stream.read(&buffer[0], file_length);

     for (char* pointer = &buffer[0]; pointer < &buffer[file_length];)
     {
          _chunks.emplace_back();
          pointer = _chunks.back().From(pointer);

          if (_chunks.back().ID() != 0x46464952)
               throw _chunks.back().ID();
     }
}

RIFF::Chunk::Chunk():
     _id{},
     _chunk_type{},
     _data{}
{}

char* RIFF::Chunk::From(char* pointer)
{
     _id = *reinterpret_cast<Type*>(pointer);
     pointer+= sizeof(_id);

     uint32_t* chunk_size = reinterpret_cast<uint32_t*>(pointer);
     pointer+= sizeof(*chunk_size);

     _data.assign(pointer, pointer + *chunk_size);
     pointer+= sizeof(*chunk_size);

     _chunk_type = *reinterpret_cast<Type*>(&_data[0]);

     std::cout << "Nuevo chunk " << std::hex << _id
               << " tipo " << _chunk_type << " de " << *chunk_size << " bytes."
               << std::endl;

     return pointer;
}
