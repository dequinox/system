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
  tok_create = -5,
  tok_return = -6,
  tok_link = -7,

  tok_unknown = -8

};


class Processor
{
      private:
            std::string m_keys[15];
            std::string m_labels[15];
            std::string m_pkeys[15];
            std::string m_pvalues[15];
            std::string m_statement;
            std::string returns[15];
            std::vector< std::vector < int > > ids;
            std::string newLabel;
            int size{0};
            std::pair<std::string, std::string> m_properties[15];
            std::pair<std::string, std::string> m_new_properties[15];
            int npsize{0};
            int psize{0};
            int m_positions[15];
            Database &m_db;
            int CurTok;
            int position{0};
            int len;
            int rsize;
            bool match{false};
            bool link{false};


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
                  if (identifier == "CREATE")
                        return tok_create;
                  if (identifier == "LINK")
                        return tok_link;
                return tok_unknown;
              }

              if (m_statement[position] == ';')
                return tok_end;

              return tok_unknown;

            }

            bool ParsePattern(){
                  size = 0;
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
                  return true;

            }

            bool ParseCondition(){
                  psize = 0;
                  readSpaces();
                  if (position < len && m_statement[position] == '(') {
                        position++;
                        std::string key;
                        std::string pkey;
                        std::string value;
                        while (position < len && m_statement[position] != ')'){
                              readSpaces();
                              key = pkey = value = "";
                              while (position < len && m_statement[position] != '.'){
                                    key += m_statement[position];
                                    position++;
                              }
                              if (position++ == len) return false;
                              while (position < len && m_statement[position] != '='){
                                    pkey += m_statement[position];
                                    position++;
                              }
                              if (position++ == len) return false;
                              while (position < len && m_statement[position] != ',' && m_statement[position] != ')'){
                                    value += m_statement[position];
                                    position++;
                              }
                              for (int i = 0; i < size; i++){
                                    if (m_keys[i] == key) {
                                          m_positions[psize] = i;
                                          m_properties[psize] = make_pair(pkey, value);
                                          psize++;
                                    }
                              }
                              if (position == len) return false;
                              if (m_statement[position] == ',') position++;
                        }
                        if (position == len) return false;
                        position++;
                  } else {
                        return false;
                  }

                  return true;
            }

            bool ParseSet(){
                  std::string set;
                  /*while (position < len && m_statement[position] == ' '){
                        position++;
                  }

                  while (position < len && m_statement[position] != ' '){
                        if (m_statement[position] == ';') return false;
                        else set += m_statement[position++];
                  }

                  std::cout << set << std::endl;
*/
                  return true;
            }

            void readSpaces(){
                  while (position < len && m_statement[position] == ' '){
                        position++;
                  }
            }

            void print(){

                  for (int i = 0; i < rsize; i++){
                        for (int j = 0; j < size; j++){
                              if (returns[i] == m_keys[j]){
                                    if (j % 2 == 0){
                                          m_db.printNodes(ids[j]);
                                    } else {
                                          m_db.printRelations(ids[j]);
                                    }
                              }
                        }
                  }
            }

            bool ParseLink(){
                  size = 0;
                  newLabel = "";
                  std::string key, value;

                  readSpaces();
                  if (position < len && m_statement[position] == '['){
                        position++;
                        key = value = "";
                        while (position < len && m_statement[position] != ':'){
                              key += m_statement[position++];
                        }
                        if (position == len) return false;
                        position++;
                        while (position < len && m_statement[position] != ']'){
                              value += m_statement[position++];
                        }
                        if (position == len) return false;
                        position++;
                        m_keys[size] = key;
                        m_labels[size] = value;
                        size++;
                  } else {
                        return false;
                  }
                  if (position == len) return false;
                  if (m_statement[position] == '-'){
                        npsize = 0;
                        position++;
                        if (position < len && m_statement[position] == '['){
                              position++;
                              while (position < len && isalpha(m_statement[position])){
                                    newLabel += m_statement[position++];
                              }

                              readSpaces();
                              if (position < len && m_statement[position] == '{'){
                                    position++;
                                    while (position < len && m_statement[position] != '}'){
                                          readSpaces();
                                          key = value = "";
                                          if (isalpha(m_statement[position])){
                                                while (position < len && m_statement[position] != ':') {
                                                      key += m_statement[position];
                                                      position++;
                                                }
                                                if (position == len) return false;
                                                position++;
                                                while (position < len && m_statement[position] != ','
                                                      && m_statement[position] != '}') {
                                                      value += m_statement[position];
                                                      position++;
                                                }
                                                m_new_properties[npsize++] = make_pair(key, value);
                                          } else {
                                                return false;
                                          }
                                          readSpaces();
                                          if (position < len && m_statement[position] == ','){
                                                position++;
                                                continue;
                                          } else if (m_statement[position] == '}'){
                                                continue;
                                          } else {
                                                return false;
                                          }
                                    }
                              } else if (m_statement[position] != ']') {
                                    return false;
                              }
                              if (position == len) return false;
                              if (m_statement[position] == '}') position++;
                        } else {
                              return false;
                        }
                        if (!(position < len && m_statement[position] == ']')) return false;
                        position++;
                        if (!(position < len && m_statement[position] == '-')) return false;
                        position++;
                  } else {
                        return false;
                  }
                  if (position < len && m_statement[position] == '['){
                        position++;
                        key = value = "";
                        while (position < len && m_statement[position] != ':'){
                              key += m_statement[position++];
                        }
                        if (position == len) return false;
                        position++;
                        while (position < len && m_statement[position] != ']'){
                              value += m_statement[position++];
                        }
                        if (position == len) return false;
                        position++;
                        m_keys[size] = key;
                        m_labels[size] = value;
                        size++;
                  } else {
                        return false;
                  }
                  if (position == len) return false;
                  return true;
            }

            bool ParseCreate(){
                  size = 0;
                  npsize = 0;
                  newLabel = "";
                  std::string key, value;
                  readSpaces();

                  if (position < len && m_statement[position] == '['){
                        position++;
                        while (position < len && isalpha(m_statement[position])){
                              newLabel += m_statement[position++];
                        }

                        readSpaces();

                        if (position < len && m_statement[position] == '{'){
                              position++;
                              while (position < len && m_statement[position] != '}'){
                                    readSpaces();
                                    key = value = "";
                                    if (isalpha(m_statement[position])){
                                          while (position < len && m_statement[position] != ':') {
                                                key += m_statement[position];
                                                position++;
                                          }
                                          if (position == len) return false;
                                          position++;
                                          while (position < len && m_statement[position] != ','
                                                && m_statement[position] != '}') {
                                                value += m_statement[position];
                                                position++;
                                          }

                                          m_new_properties[npsize++] = make_pair(key, value);
                                    } else {
                                          return false;
                                    }
                                    readSpaces();
                                    if (position < len && m_statement[position] == ','){
                                          position++;
                                          continue;
                                    } else if (m_statement[position] == '}'){
                                          continue;
                                    } else {
                                          return false;
                                    }
                              }
                        } else {
                              return false;
                        }
                        if (position == len) return false;
                        if (m_statement[position] == '}') position++;
                  } else {
                        return false;
                  }
                  if (position < len && m_statement[position] == ']')
                        return true;
                  return false;
            }

            bool ParseReturn(){
                  std::string r = "";
                  rsize = 0;

                  while (position < len && m_statement[position] != ';'){
                        readSpaces();
                        if (isalpha(m_statement[position])){
                              while (position < len && isalpha(m_statement[position])){
                                    r += m_statement[position];
                                    position++;
                              }
                              returns[rsize++] = r;
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

                  match = true;
            }

            void HandleCondition(){
                  if (!ParseCondition()){
                        std::cout << "Syntax error : WHERE condition at position "
                                  << position << std::endl;
                  }
            }

            void HandleCreate(){
                  if (!ParseCreate()){
                        std::cout << "Syntax error : CREATE at position "
                                  << position << std::endl;
                  }

                  int node_id = m_db.create(newLabel);
                  for (int i = 0; i < npsize; i++){
                        m_db.createProperty(node_id, m_new_properties[i].first, m_new_properties[i].second);
                  }
                  std::cout << "CREATE [SUCCESSFUL]\n";
            }

            void HandleLink(){
                  if (!ParseLink()){
                        std::cout << "Syntax error : LINK at position "
                                  << position << std::endl;
                  }
                  link = true;
            }

            void HandleSet(){
                  if (!ParseCondition()){
                        std::cout << "Syntax error : SET condition at position "
                                  << position << std::endl;
                  }

                  for (int i = 0; i < psize; i++){
                        m_db.set(ids[m_positions[i]], m_properties[i]);
                  }
            }

            void HandleReturn(){
                  if (!ParseReturn()){
                        std::cout << "Syntax error : RETURN condition at position "
                                  << position << std::endl;
                  }
                  if (match){
                        ids = m_db.bfs(m_labels, size, m_positions, m_properties, psize);
                        print();
                  }
            }

            void HandleFinal(){
                  if (link){
                        m_db.createRelations(m_labels, size, m_positions, m_properties, npsize, newLabel);
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
                                    HandleFinal();
                                    return;
                              case tok_create:
                                    HandleCreate();
                                    return;
                              case tok_link:
                                    HandleLink();
                                    break;
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
