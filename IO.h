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
      class Store {

            private:
                  std::fstream file;
                  std::string m_id_file_store;
                  std::stack<unsigned long> id_store;

            public:
                  class iterator;
            public:
                  Store(std::string filename, std::string ids_filename) {
                        file.open(filename, std::ios::app | std::ios::binary);
                        file.close();

                        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);

                        std::ifstream id_store_file(ids_filename, std::ios::binary);

                        m_id_file_store = ids_filename;
                        int id;
                        while (id_store_file.read(reinterpret_cast<char *>(&id), sizeof(int))) {
                              id_store.push(id);
                        }
                        id_store_file.close();

                  }

                  int getNewID() {
                        int id = size();

                        if (!id_store.empty()) {
                              id = id_store.top();
                              id_store.pop();
                        }

                        return id;
                  }

                  int size() {
                        file.seekg(0, std::ios::end);
                        return file.tellg();
                  }

                  T get(int id) {
                        T data;
                        file.seekg(id);
                        file.read(reinterpret_cast<char *>(&data), sizeof(T));

                        return data;
                  }

                  void set(int id, T new_data) {
                        file.seekp(id);
                        file.write(reinterpret_cast<char *>(&new_data), sizeof(T));
                  }

                  void remove(int id){
                        id_store.push(id);
                  }

                  iterator begin() {
                        return iterator(0, *this);
                  }

                  iterator end() {
                        return iterator(size(), *this);
                  }

                  ~Store() {
                        file.close();

                        std::ofstream id_file_store(m_id_file_store, std::ios::binary);
                        while (!id_store.empty()){
                              int id = id_store.top();
                              id_store.pop();
                              id_file_store.write(reinterpret_cast<char *>(&id), sizeof(int));
                        }
                        id_file_store.close();
                  }

                  /* -------------------------------------------------- */
                  void printLabels() {
                        file.seekg(0);
                        T data;
                        while (file.read(reinterpret_cast<char *>(&data), sizeof(T))) {
                              std::cout<< (int)file.tellg() - sizeof(T) << " " << data.getValue() << std::endl;
                        }
                  }
                  void printNodes() {
                        file.seekg(0);
                        T data;
                        while (file.read(reinterpret_cast<char *>(&data), sizeof(T))) {
                              std::cout << (int)file.tellg() - sizeof(T) << " "
                                        << data.getRelation() << " " << data.getProperty() << " "
                                        << data.getLabel() << std::endl;
                        }
                  }
                  void printProperties() {
                        file.seekg(0);
                        T data;
                        while (file.read(reinterpret_cast<char *>(&data), sizeof(T))) {
                              std::cout << (int)file.tellg() - sizeof(T) << " "
                                        << data.getKey() << " " << data.getValue() << " "
                                        << data.getNext() << std::endl;
                        }
                  }
                  void printRelations() {
                        file.seekg(0);
                        T data;
                        while (file.read(reinterpret_cast<char *>(&data), sizeof(T))) {
                              std::cout << (int)file.tellg() - sizeof(T) << " " << data.getFirstNode() << " "
                                        << data.getSecondNode() << " "
                                        << data.getNext(data.getFirstNode()) << " "
                                        << data.getNext(data.getSecondNode())<< " "
                                        << data.getPrevious(data.getFirstNode()) << " "
                                        << data.getPrevious(data.getSecondNode()) << std::endl;
                        }
                  }

      };

      template<typename T>
      class Store<T>::iterator {

            private:
                  int m_pos;
                  Store &m_store;
            public:
                  iterator(int pos, Store &aStore) : m_pos(pos), m_store(aStore) {}

                  const iterator operator++(int) {
                        Store<T>::iterator temp(*this);
                        m_pos += sizeof(T);
                        return temp;
                  }

                  const iterator &operator++() {
                        m_pos += sizeof(T);
                        return *this;
                  }

                  /*T &operator*() {
                        return m_store.read(m_pos);
                  }*/
                  int operator*() {
                        return m_pos;
                  }

                  bool operator!=(const iterator &other) const {
                        return m_pos != other.m_pos;
                  }
      };
}

#endif /* IO_H */
