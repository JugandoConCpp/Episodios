#include "riff.h"

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

void Dump(char const* begin, char const* end)
{
     int o = 0;

     std::cout << std::hex;

     for (char const* p = begin; p < end; ++p, ++o)
     {
          if ((((uint64_t)o) % 128) == 0)
               std::cout << std::endl;

          if ((((uint64_t)o) % 16) == 0)
               std::cout << std::endl << std::setw(4) << std::setfill('0') << o << "  ";
          else
          if ((((uint64_t)o) % 8) == 0)
               std::cout << "-";
          else
               std::cout << " ";

          std::cout << std::setw(2) << std::setfill('0') << int(*p & 0xff);
     }

     std::cout << std::dec << std::endl;
}

RIFF::RIFF(std::string_view filename):
     _filename{std::string{filename}},
     _chunks{}
{
     std::ifstream stream(filename.data(), std::ios_base::in | std::ios_base::binary);

     stream.seekg(0, stream.end);
     std::streamsize remaining_bytes = stream.tellg();
     std::cout << "remaining_bytes: " << remaining_bytes << std::endl;
     stream.seekg(0, stream.beg);

     std::vector<char> buffer(remaining_bytes, '\0');

     for (std::streamsize offset = 0; remaining_bytes;)
     {
          stream.read(&buffer[0], remaining_bytes);

          std::streamsize read_bytes = stream.gcount();
          std::cout << "read " << read_bytes << " bytes" << std::endl;

          offset+= read_bytes;
          remaining_bytes-= read_bytes;

          if (read_bytes == 0)
               break;
     }

     Chunk::Parse_All(&buffer[0], &buffer[buffer.size()], _chunks);
}

char const* RIFF::Chunk::Parse_All(char const* begin, char const* end, std::list<Chunk>& chunks)
{
     char const* pointer = begin;
     bool trace = false;

//     std::cout << "[" << (void const*) begin << " .. " << (void const*) end << "]" << std::endl;
     while (pointer < end)
     {
          char const* initial_pointer = pointer;

          if (trace)
          {
//               std::cout << "initial_pointer: " << (void const*) initial_pointer << std::endl;
          }

          Chunk& chunk = chunks.emplace_back();
          pointer = chunk.From(pointer, trace);

          trace = (chunk.Is_List() && (chunk.Chunk_Type() == "rec ") && (chunk.Size() == 0x37f8));
     }

     return pointer;
}

RIFF::Chunk::Chunk():
     _id{},
     _chunk_type{},
     _data{}
{}

char const* RIFF::Chunk::From(char const* pointer, bool trace)
{
     _id = *reinterpret_cast<Type const*>(pointer);
     if (trace) std::cout << (void const*) pointer << " id: " << _id << std::endl;
     pointer+= sizeof(_id);

     uint32_t const* chunk_size = reinterpret_cast<uint32_t const*>(pointer);
     if (trace) std::cout << (void const*) pointer << " chunk_size: " << *chunk_size << std::endl;
     pointer+= sizeof(*chunk_size);

     if (*chunk_size > 0)
     {
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
     }

     uint64_t pointer_as_int = reinterpret_cast<uint64_t>(pointer);

     uint64_t modulo = pointer_as_int % 4;
     pointer_as_int+= (4 - modulo) % 4;
     pointer = reinterpret_cast<char const*>(pointer_as_int);

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
