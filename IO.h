#ifndef IO_H
#define IO_H

#include <fstream>
#include <cstdlib>
#include <iostream>

namespace IO
{

      template<typename T>
      void write(unsigned long id, T data, std::string file_name)
      {
            std::fstream file(file_name, std::ios::in | std::ios::out | std::ios::binary);

            file.seekp(id);
            file.write(reinterpret_cast<char *>(&data), sizeof(T));
            file.close();
      }

      unsigned long getFileSize(std::string file_name)
      {
            std::ifstream file(file_name, std::ios::binary | std::ios::ate);

            if (!file)
                  return 0;

            return file.tellg();
      }

      template<typename T>
      T pop(std::string file_name)
      {
            T data;

            std::fstream file(file_name, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
            unsigned long file_size = file.tellg();

            file.seekg(file_size - sizeof(T));
            file.read(reinterpret_cast<char *>(&data), sizeof(T));

            file.seekp(file_size - sizeof(T));
            file.write(NULL, 0);
            file.close();

            return data;
      }
}

#endif /* IO_H */
