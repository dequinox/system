#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "API.h"

enum Token {
  tok_end = -1,

  // conditions
  tok_match = -2,
  tok_where = -3,

  // commands
  tok_set = -3,
  tok_return = -4,

  tok_unknown = -5

};


class Processor
{
      private:
            std::string m_keys[15];
            std::string m_labels[15];
            std::string m_statement;
            int size{0};
            Database &m_db;
            static int CurTok;
            int position{0};
            int len;


      public:
            Processor() = delete;
            Processor(Database &db) : m_db(db) {}

            /* ------- Parser -------- */

            static int getNextToken() { return CurTok = gettok(); }
            static int gettok() {

              while (position < len && isspace(m_statement[position])){
                position++;
              }

              if (position == size) return tok_end;

              if (isalpha(m_statement[position])) {
                string identifier = "";
                while (position < len && isalpha(m_statement[position]))
                  identifier += m_statement[position++];

                if (identifier == "MATCH")
                  return tok_match;
                if (identifier == "WHERE")
                  return tok_where;
                return tok_unknown;
              }

              if (LastChar == ";")
                return tok_end;

              return unknown;

            }

            bool ParsePattern(){

            }

            bool ParseCondition(){

            }

            void HandlePattern(){
                  if (!ParsePattern()){
                        std::cout << "Syntax error : pattern definition at position "
                                  << position << std::endl;
                  }
                  // run serach
            }

            void HandleCondition(){
                  if (!ParseCondition()){
                        std::cout << "Syntax error : condition definition at position"
                                  << position << std::endl;
                  }
            }

            void HandleSet(){

            }

            void HandleReturn(){

            }

            void execute(std::string statement){
                  size = 0;
                  len = statement.size();
                  m_statement = statement;

                  while (true){
                        getNextToken();
                        switch(curTok){
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
                                    break;
                        }
                  }
            }
}

#endif /* PROCESSOR_H */
