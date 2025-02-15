#include "riff.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

RIFF::RIFF(std::string_view filename):
     _filename{std::string{filename}},
     _chunks{}
{
     std::fstream stream(filename.data());

     stream.seekg(0, stream.end);
     int file_length = stream.tellg();
     stream.seekg(0, stream.beg);

     std::vector<char> buffer(file_length, '\0');
     stream.read(&buffer[0], file_length);

     Chunk::Parse_All(&buffer[0], &buffer[file_length], _chunks);
}

char const* RIFF::Chunk::Parse_All(char const* begin, char const* end, std::list<Chunk>& chunks)
{
     char const* pointer = begin;

     while (pointer < end)
     {
          Chunk& chunk = chunks.emplace_back();
          pointer = chunk.From(pointer);
     }

     return pointer;
}

RIFF::Chunk::Chunk():
     _id{},
     _chunk_type{},
     _data{}
{}

char const* RIFF::Chunk::From(char const* pointer)
{
     _id = *reinterpret_cast<Type const*>(pointer);
     pointer+= sizeof(_id);

     uint32_t const* chunk_size = reinterpret_cast<uint32_t const*>(pointer);
     pointer+= sizeof(*chunk_size);

     _data.assign(pointer, pointer + *chunk_size);
     pointer+= _data.size();

     _chunk_type = *reinterpret_cast<Type*>(&_data[0]);

     if ((_id == "RIFF") || (_id == "LIST"))
     {
          char const* start = &_data[sizeof(Type)];
          Type const* type = reinterpret_cast<Type const*>(&_data[0]);

          std::cout << _id << " " << std::hex << *chunk_size << std::dec << " " << *type << std::endl;

          Parse_All(start, &_data[_data.size()], _chunks);
     } else {
          std::cout << _id << " " << std::hex << *chunk_size << std::dec << std::endl;
     }

     return pointer;
}

RIFF::Chunk::Type::Type(std::string_view string_rep):
     Value{0}
{
     if (string_rep.length() < sizeof(Value))
          throw string_rep.length();

     Value = string_rep[0] | (string_rep[1] << 8) | (string_rep[2] << 16) | (string_rep[3] << 24);
}

bool RIFF::Chunk::Type::operator==(std::string_view other) const
{
     if (other.length() < sizeof(Value))
          return false;

     return *reinterpret_cast<uint32_t const*>(other.data()) == Value;
}

bool RIFF::Chunk::Type::operator!=(std::string_view other) const
{
     return !operator==(other);
}
