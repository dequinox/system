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

      string name;
      cin >> query >> name;

      if (query == "get")
      {
            API::show(name);
      }
      else
      {
            pair <string, string> p;
            p = make_pair("x", "1");
            API::create(name, {p});
      }

      return 0;
}
