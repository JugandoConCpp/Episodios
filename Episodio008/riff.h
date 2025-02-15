#ifndef RIFF_H__INCLUDED
#define RIFF_H__INCLUDED

#include <cstdint>
#include <iostream>
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
     public:
          struct Type
          {
               uint32_t Value;

               Type() = default;

               explicit Type(char const* string_rep): Type{std::string_view{string_rep}} {}
               explicit Type(std::string_view string_rep);
               explicit Type(uint32_t value): Value{value} {}
               explicit operator uint32_t () const {return Value;}

               bool operator==(std::string_view other) const;
               bool operator!=(std::string_view other) const;

               bool operator==(Type other) const {return *this == other;}
               bool operator!=(Type other) const {return *this != other;}
          };

     private:
          Type _id;
          Type _chunk_type;
          std::vector<char> _data;
          std::list<Chunk> _chunks;

          friend std::ostream& operator<<(std::ostream& stream, Type type);

     public:
          static char const* Parse_All(char const* begin, char const* end, std::list<Chunk>& chunks);

          Chunk();
          char const* From(char const* pointer);

          Type ID() const { return _id; }
          Type Chunk_Type() const { return _chunk_type; }

          bool Is_Riff() const { return (_id == "RIFF"); }
          bool Is_List() const { return (_id == "LIST"); }

          uint32_t Size() const { return uint32_t(_data.size()); }
};

class RIFF::List_Chunk: public RIFF::Chunk
{
     private:
          std::vector<Chunk> _chunks;
};

inline std::ostream& operator<<(std::ostream& stream, RIFF::Chunk::Type type)
{
     char* value = reinterpret_cast<char*>(&type.Value);
     return stream << value[0] << value[1] << value[2] << value[3];
     return stream;
}

#endif  // RIFF_H__INCLUDED
