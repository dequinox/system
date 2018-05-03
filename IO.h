#ifndef IO_H
#define IO_H

#include <stack>
#include <fstream>
#include <cstdlib>
#include <iostream>

namespace IO
{

      template<typename T>
      class File
      {
            private:
                  std::fstream file;
                  std::string filename;
                  unsigned long pos;
            public:
                  File(std::string filename) : filename(filename)
                  {
                        file.open(filename, std::ios::app | std::ios::binary);
                        file.close();
                        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
                  }

                  unsigned long getSize()
                  {
                        file.seekg(0, std::ios::end);
                        return file.tellg();
                  }

                  void write(unsigned long id, T new_data)
                  {
                        file.seekp(id);
                        file.write(reinterpret_cast<char *>(&new_data), sizeof(T));
                  }

                  ~File()
                  {
                        file.close();
                  }

      };

      std::stack<unsigned long> GetData(std::string filename)
      {
            std::stack<unsigned long> result;
            std::ifstream file(filename);

            unsigned long data;
            while (file.read(reinterpret_cast<char *>(&data), sizeof(unsigned long)))
            {
                  result.push(data);
            }

            return result;
      }

      template<typename T>
      void print(std::string filename)
      {
            T data;
            std::ifstream file(filename);

            while (file.read(reinterpret_cast<char *>(&data), sizeof(T)))
            {
                  std::cout << data.in_use << " " << data.label_id << std::endl;
            }
      }
}

#endif /* IO_H */
