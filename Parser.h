#ifndef PARSER_H
#define PARSER_H

#include <map>

class Data
{
      private:
            std::map<std::string, Data *> data;
      public:
            Data * operator[](std::string key)
            {
                  return data[key];
            }
};

#endif /* PARSER_H */
