#include <iostream>
#include "API.h"
#include "Processor.h"
#include "Exception.h"
#include "Parser.h"

using namespace std;

int main()
{
      string query;

      /*while(getline(cin, query) && query != "quit")
      {
            try
            {
                  Processor::process(query);
                  cout << "::: Process query [DONE]" << endl;
            }
            catch (GraphException &e)
            {
                  cout << e.what() << endl;
            }
      }*/

      Database my_db("dequinox");
      string name;
      cin >> query >> name;

      if (query == "get")
      {
            my_db.print(name);
      }
      else
      {
            my_db.create(name);
      }

      return 0;
}
