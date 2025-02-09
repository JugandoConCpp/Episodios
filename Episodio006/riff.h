#ifndef RIFF_H__INCLUDED
#define RIFF_H__INCLUDED

#include <cstdint>
#include <list>
#include <string>
#include <string_view>
#include <vector>

class RIFF
{
     public:
          class Chunk;
          class List_Chunk;

     private:
          std::string const _filename;

          std::list<Chunk> _chunks;

     public:
          RIFF(std::string_view filename);
};

class RIFF::Chunk
{
     private:
          struct Type
          {
               uint32_t Value;

               Type() = default;

               explicit Type(uint32_t value): Value{value} {}
               explicit operator uint32_t () const {return Value;}

               bool operator==(std::string_view other) const;
               bool operator!=(std::string_view other) const;
          };

          Type _id;
          Type _chunk_type;
          std::vector<char> _data;

          friend std::ostream& operator<<(std::ostream& stream, Type type);

     public:
          Chunk();
          char* From(char* pointer);

          Type ID() const { return _id; }
          Type Chunk_Type() const { return _chunk_type; }
};

class RIFF::List_Chunk: public RIFF::Chunk
{
     private:
          std::vector<Chunk> _chunks;
};

inline std::ostream& operator<<(std::ostream& stream, RIFF::Chunk::Type type);

#endif  // RIFF_H__INCLUDED
