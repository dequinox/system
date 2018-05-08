#include <iostream>
#include "API.h"
#include <vector>
#include "Parser.h"
#include "Processor.h"

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
/*      getline(cin, query);
      Processor p(my_db);
      p.execute(query);

*/
      cin >> query;
      if (query == "get"){
            my_db.print();
      }
      else if (query == "new"){
            cin >> name;
            my_db.create(name);
      }
      else if (query == "prop"){
            cin >> name;
            string value;
            cin >> value;
            int id;
            cin >> id;
            id *= sizeof(Node);
            my_db.createProperty(id, name, value);

      }
      else if (query == "rel"){
            int fid, sid;
            cin >> name >> fid >> sid;
            fid *= sizeof(Node);
            sid *= sizeof(Node);
            my_db.createRelation(fid, sid, name);
      }
      else if (query == "drel"){
            int id;
            cin >> id;
            id *= sizeof(Relation);
            my_db.deleteRelation(id);
      }
      else if (query == "dnode"){
            int id;
            cin >> id;
            id *= sizeof(Node);
            my_db.deleteNode(id);
      }

      return 0;
}
