#include <iostream>
#include "API.h"
#include <vector>
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
      cin >> query;



      if (query == "get")
      {
            my_db.print();
      }
      else if (query == "new")
      {
            cin >> name;
            my_db.create(name);
      }
      else if (query == "prop")
      {
            cin >> name;
            Property p;
            p.in_use = 1;
            int id;
            strcpy(p.key, name.c_str());
            string value;
            cin >> value;
            cin >> id;
            strcpy(p.value, value.c_str());
            vector<Property> v;
            v.push_back(p);
            my_db.addProperties(id * sizeof(Node), v);
      }
      else
      {
            int fnode;
            int snode;
            cin >> name;
            cin >> fnode >> snode;
      }

      return 0;
}
