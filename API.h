#ifndef API_H
#define API_H

#include "IO.h"
#include "Node.h"
#include <stack>
#include <iostream>
#include <cstring>
#include "Label.h"
#include "Restore.h"
#include "Property.h"
#include "Relation.h"

class Database
{
      private:
            const std::string node_ids_file{".node.ids"};
            const std::string label_ids_file{".label.ids"};
            const std::string property_ids_file{".property.ids"};
            const std::string relation_ids_file{".relation.store"};

            std::string m_db;

            IO::File<Node> * nodes;
            IO::File<Label> * labels;
            IO::File<Property> * properties;
            IO::File<Relation> * relations;

            std::stack<unsigned long> nodeIDs;
            std::stack<unsigned long> labelIDs;
            std::stack<unsigned long> relationIDs;
            std::stack<unsigned long> propertyIDs;



      public:
            Database(const Database &other) = delete;
            Database() = delete;

            Database(std::string db) : m_db(db)
            {
                  nodeIDs = IO::GetData(node_ids_file);
                  labelIDs = IO::GetData(label_ids_file);
                  relationIDs = IO::GetData(relation_ids_file);
                  propertyIDs = IO::GetData(property_ids_file);

                  nodes = new IO::File<Node>(m_db + ".nodes.store");
                  labels = new IO::File<Label>(m_db + ".labels.store");
                  relations = new IO::File<Relation>(m_db + ".relation.store");
                  properties = new IO::File<Property>(m_db + ".properties.store");
            }

            void create(std::string label)
            {
                  int node_id = nodes->getSize();
                  int label_id = labels->getSize();

                  if (!nodeIDs.empty())
                  {
                        node_id = nodeIDs.top();
                        nodeIDs.pop();
                  }

                  if (!labelIDs.empty())
                  {
                        label_id = labelIDs.top();
                        labelIDs.pop();
                  }

                  Node new_node{true, 0, 0, label_id};

                  Label new_label{true};
                  strcpy(new_label.value, label.c_str());

                  nodes->write(node_id, new_node);
                  labels->write(label_id, new_label);
            }

            void print(std::string filename)
            {
                  IO::print<Node>(filename);
            }

            void newProperty(int id, auto list)
            {
                  // TODO
            }

            void truncate()
            {
                  // TODO
            }

            ~Database()
            {
                  delete nodes;
                  delete labels;
                  delete properties;
                  delete relations;
            }

};

#endif /* API_H */
