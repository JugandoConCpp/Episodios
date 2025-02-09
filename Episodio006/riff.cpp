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
          Chunk& chunk = _chunks.emplace_back();
          pointer = chunk.From(pointer);

          if (chunk.ID() != "RIFF")
               throw chunk.ID();
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
     pointer+= _data.size();

     _chunk_type = *reinterpret_cast<Type*>(&_data[0]);

     std::cout << "Nuevo chunk " << _id << " tipo " << _chunk_type
               << " de " << *chunk_size << " bytes."
               << std::endl;

     if (_data.size() > 8)
     {
          char* possible_list_type_start = &_data[sizeof(Type)];
          Type const* type = reinterpret_cast<Type*>(possible_list_type_start);

          if (*type == "LIST")
          {
               List_Chunk list;
               list.From(possible_list_type_start);
          }
     }

     return pointer;
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

inline std::ostream& operator<<(std::ostream& stream, RIFF::Chunk::Type type)
{
     char* value = reinterpret_cast<char*>(&type.Value);
     return stream << value[0] << value[1] << value[2] << value[3];
     return stream;
}
