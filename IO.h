#ifndef IO_H
#define IO_H

#include <stack>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <stack>

namespace IO
{

      template<typename T>
      class Store
      {
            private:
                  std::fstream file;
                  std::stack<unsigned long> id_store;
            public:
                  Store(std::string filename, std::string ids_filename)
                  {
                        file.open(filename, std::ios::app | std::ios::binary);
                        file.close();

                        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);

                        std::ifstream id_store_file(ids_filename, std::ios::binary);

                        unsigned long id;
                        while (id_store_file.read(reinterpret_cast<char *>(&id), sizeof(unsigned long)))
                        {
                              id_store.push(id);
                        }
                        id_store_file.close();

                  }

                  int getNewID()
                  {
                        int id = getSize();

                        if (!id_store.empty())
                        {
                              id = id_store.top();
                              id_store.pop();
                        }

                        return id;
                  }

                  int getSize()
                  {
                        file.seekg(0, std::ios::end);
                        return file.tellg();
                  }

                  T read(int id)
                  {
                        T data;
                        file.seekg(id);
                        file.read(reinterpret_cast<char *>(&data), sizeof(T));

                        return data;
                  }

                  void write(unsigned long id, T new_data)
                  {
                        file.seekp(id);
                        file.write(reinterpret_cast<char *>(&new_data), sizeof(T));
                  }

                  ~Store()
                  {
                        file.close();
                  }

                  /* -------------------------------------------------- */
                  void printLabels()
                  {
                        file.seekg(0);
                        T data;
                        while (file.read(reinterpret_cast<char *>(&data), sizeof(T)))
                        {
                              std::cout<< file.tellg() - sizeof(T) << " " << data.in_use << " " << data.value << std::endl;
                        }
                  }
                  void printNodes()
                  {
                        file.seekg(0);
                        T data;
                        while (file.read(reinterpret_cast<char *>(&data), sizeof(T)))
                        {
                              std::cout << file.tellg() - sizeof(T) << " " << data.in_use << " " << data.relationship_id << " " << data.property_id << " " << data.label_id << std::endl;
                        }
                  }
                  void printProperties()
                  {
                        file.seekg(0);
                        T data;
                        while (file.read(reinterpret_cast<char *>(&data), sizeof(T)))
                        {
                              std::cout << file.tellg() - sizeof(T) << " " << data.in_use << " " << data.key << " " << data.value << " " << data.next_property_id << std::endl;
                        }
                  }
                  void printRelations()
                  {
                        file.seekg(0);
                        T data;
                        while (file.read(reinterpret_cast<char *>(&data), sizeof(T)))
                        {
                              std::cout << file.tellg() - sizeof(T) << " " << data.in_use << " " << data.first_node << " " << data.second_node << " " <<
                                      data.first_next_id << " " << data.second_next_id << std::endl;
                        }
                  }

      };
}

#endif /* IO_H */
