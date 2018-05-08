#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "API.h"
#include "IO.h"
#include <string.h>

enum Token {
  tok_end = -1,

  // conditions
  tok_match = -2,
  tok_where = -3,

  // commands
  tok_set = -4,
  tok_return = -5,

  tok_unknown = -6

};


class Processor
{
      private:
            std::string m_keys[15];
            std::string m_labels[15];
            std::string m_statement;
            std::string condition;
            int size{0};
            Database &m_db;
            int CurTok;
            int position{0};
            int len;


      public:
            Processor() = delete;
            Processor(Database &db) : m_db(db) {}

            /* ------- Parser -------- */

            int getNextToken() { return CurTok = gettok(); }
            int gettok() {

              while (position < len && isspace(m_statement[position])){
                position++;
              }

              if (position == len) return tok_end;

              if (isalpha(m_statement[position])) {
                std::string identifier = "";
                while (position < len && isalpha(m_statement[position]))
                  identifier += m_statement[position++];

                if (identifier == "MATCH")
                  return tok_match;
                if (identifier == "WHERE")
                  return tok_where;
                  if (identifier == "SET")
                        return tok_set;
                  if (identifier == "RETURN")
                        return tok_return;
                return tok_unknown;
              }

              if (m_statement[position] == ';')
                return tok_end;

              return tok_unknown;

            }

            bool ParsePattern(){
                  while (position < len && m_statement[position] == ' ')
                        position++;

                  while (position < len && m_statement[position] != ' '){
                        if (m_statement[position] == '['){
                              std::string key;
                              std::string value;
                              bool flag = false;
                              position++;
                              while (position < len && m_statement[position] != ']'){
                                    if (m_statement[position] == ':') {
                                          flag = true;
                                          position++;
                                          continue;
                                    }
                                    if (flag) value += m_statement[position];
                                    else key += m_statement[position];
                                    position++;
                              }
                              if (position == len || m_statement[position] != ']') return false;
                              m_keys[size] = key;
                              m_labels[size] = value;
                              size++;
                              position++;
                              continue;
                        }
                        else if (m_statement[position] == '-'){
                              position++;
                              if (m_statement[position] == '['){
                                    std::string key;
                                    std::string value;
                                    bool flag = false;
                                    position++;
                                    while (position < len && m_statement[position] != ']'){
                                          if (m_statement[position] == ':') {
                                                flag = true;
                                                position++;
                                                continue;
                                          }
                                          if (flag) value += m_statement[position];
                                          else key += m_statement[position];
                                          position++;
                                    }
                                    if (position == len || m_statement[position] != ']') return false;
                                    m_keys[size] = key;
                                    m_labels[size] = value;
                                    position++;
                                    size++;
                                    if (m_statement[position] == '-') position++;
                                    else return false;
                                    continue;
                              } else {
                                    return false;
                              }
                        }
                        else return false;
                  }

                  for (int i = 0; i < size; i++)
                  {
                        std::cout << m_keys[i] << " " << m_labels[i] << std::endl;
                  }
                  return true;

            }

            bool ParseCondition(){
                  condition = "";
                  while (position < len && m_statement[position] == ' '){
                        position++;
                  }

                  while (position < len && m_statement[position] != ' '){
                        if (m_statement[position] == ';') return false;
                        else condition += m_statement[position++];
                  }

                  std::cout << condition << std::endl;

                  return true;
            }

            bool ParseSet(){
                  std::string set;
                  condition = "";
                  while (position < len && m_statement[position] == ' '){
                        position++;
                  }

                  while (position < len && m_statement[position] != ' '){
                        if (m_statement[position] == ';') return false;
                        else set += m_statement[position++];
                  }

                  std::cout << set << std::endl;

                  return true;
            }

            void readSpaces(){
                  while (position < len && m_statement[position] == ' '){
                        position++;
                  }
            }

            bool ParseReturn(){
                  std::string r = "";

                  while (position < len && m_statement[position] != ';'){
                        readSpaces();
                        if (isalpha(m_statement[position])){
                              while (position < len && isalpha(m_statement[position])){
                                    r += m_statement[position];
                                    position++;
                              }
                              std::cout << r << std::endl;
                              r = "";
                        } else {
                              return false;
                        }
                        readSpaces();
                        if (position < len && m_statement[position] == ','){
                              position++;
                              continue;
                        } else if (m_statement[position] == ';'){
                              continue;
                        } else {
                              return false;
                        }
                  }

                  if (position == len) return false;
                  return true;
            }

            void HandlePattern(){
                  if (!ParsePattern()){
                        std::cout << "Syntax error : pattern definition at position "
                                  << position << std::endl;
                  }


            }

            void HandleCondition(){
                  if (!ParseCondition()){
                        std::cout << "Syntax error : WHERE condition at position "
                                  << position << std::endl;
                  }
            }

            void HandleSet(){
                  if (!ParseSet()){
                        std::cout << "Syntax error : SET condition at position "
                                  << position << std::endl;
                  }
            }

            void HandleReturn(){
                  if (!ParseReturn()){
                        std::cout << "Syntax error : RETURN condition at position "
                                  << position << std::endl;
                  }
            }

            void search(int depth){
                  if (depth == size) {
                        return;
                  }


            }

            void execute(std::string statement){
                  size = 0;
                  len = statement.size();
                  m_statement = statement;

                  while (true){
                        getNextToken();
                        switch(CurTok){
                              case tok_end:
                                    return;
                              case tok_match:
                                    HandlePattern();
                                    break;
                              case tok_where:
                                    HandleCondition();
                                    break;
                              case tok_set:
                                    HandleSet();
                                    break;
                              case tok_return:
                                    HandleReturn();
                                    return;
                              default:
                                    std::cout << "syntax error at" << position << std::endl;
                                    return;
                        }
                  }
            }
};

#endif /* PROCESSOR_H */
