#ifndef RIFF_H__INCLUDED
#define RIFF_H__INCLUDED

#include <cstdint>
#include <list>
#include <string>
#include <string_view>
#include <vector>

class RIFF
{
     private:
          class Chunk;
          class List_Chunk;

          typedef uint32_t Type;

          std::string const _filename;

          std::list<Chunk> _chunks;

     public:
          RIFF(std::string_view filename);
};

class RIFF::Chunk
{
     private:
          Type _id;
          Type _chunk_type;
          std::vector<char> _data;

     public:
          Chunk();
          char* From(char* pointer);

          Type ID() const { return _id; }
          Type Chunk_Type() const { return _chunk_type; }
};

#endif  // RIFF_H__INCLUDED
