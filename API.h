#ifndef API_H
#define API_H

#include "IO.h"
#include "Node.h"
#include <stack>
#include <vector>
#include <iostream>
#include <cstring>
#include "Label.h"
#include "Restore.h"
#include "Property.h"
#include "Relation.h"

class Database
{
      private:

            std::string m_db;

            IO::Store<Node> * nodes;
            IO::Store<Label> * labels;
            IO::Store<Property> * properties;
            IO::Store<Relation> * relations;

      public:
            Database(const Database &other) = delete;
            Database() = delete;

            Database(std::string db) : m_db(db)
            {
                  nodes = new IO::Store<Node>(m_db + ".nodes.store", m_db + ".node.ids");
                  labels = new IO::Store<Label>(m_db + ".labels.store", m_db + "label.ids");
                  relations = new IO::Store<Relation>(m_db + ".relations.store", m_db + "relation.ids");
                  properties = new IO::Store<Property>(m_db + ".properties.store", m_db + "property.ids");
            }

            int create(std::string label)
            {
                  Label new_label{true};
                  strcpy(new_label.value, label.c_str());

                  int label_id = add(new_label, labels);

                  Node new_node{true, -1, -1, label_id};
                  return add(new_node, nodes);
            }

            template<typename T>
            int add(T data, IO::Store<T> *store)
            {
                  unsigned long data_id = store->getNewID();
                  store->write(data_id, data);

                  return data_id;
            }

            template<typename T>
            void delete(int data_id, IO::Store<T> *store)
            {
                  T data = store->read(data_id);
                  data.setInUse(false);
                  store->write(data);
            }

            void addRelation(Relation new_relation, std::string label)
            {
                  unsigned long fnodeID = new_relation.getFirstNode();
                  unsigned long snodeID = new_relation.getSecondNode();

                  Node fnode = nodes->read(fnodeID);
                  Node snode = nodes->read(snodeID);

                  unsigned long relID = add(new_relation, relations);

                  if (fnode.hasRelation())
                  {
                        unsigned long last_rel_id = getLastRelID(fnodeID, fnode.getRelationID());
                        Relation last_relation = relations->read(last_rel_id);

                        if (last_relation.getFirstNode() == fnodeID)
                              last_relation.setFirstNextRel(relID);
                        else
                              last_relation.setSecondNextRel(relID);

                        relations->write(last_rel_id, last_relation);
                        new_relation.setFirstPrevRel(last_rel_id);
                  }
                  else
                  {
                        fnode.setNextRel(relID);
                        nodes->write(fnodeID, fnode);
                  }

                  if (snode.hasRelation())
                  {
                        unsigned long last_rel_id = getLastRelID(snodeID, snode.getRelationID());
                        Relation last_relation = relations->read(last_rel_id);

                        if (last_relation.getFirstNode() == snodeID)
                              last_relation.setFirstNextRel(relID);
                        else
                              last_relation.setSecondNextRel(relID);

                        relations->write(last_rel_id, last_relation);
                        new_relation.setSecondPrevRel(last_rel_id);
                  }
                  else
                  {
                        snode.setNextRel(relID);
                        nodes->write(snodeID, snode);
                  }


                  Label new_label{true};
                  strcpy(new_label.value, label.c_str());
                  int label_id = add(new_label, labels);

                  new_relation.setSecondPrevRel(relID);
                  new_relation.setLabel(label_id);
                  relations->write(relID, new_relation);
            }

            void print()
            {
                  std::cout << "Properties---------\n";
                  properties->printProperties();
                        std::cout << "Labels---------\n";
                  labels->printLabels();
                        std::cout << "Nodes---------\n";
                  nodes->printNodes();
                        std::cout << "Relations---------\n";
                  relations->printRelations();
                        std::cout << "---------\n";
            }


            void addProperties(int node_id, std::vector<Property> new_properties)
            {
                  Node node = nodes->read(node_id);

                  int last_prop_id;
                  int next_prop_id;

                  if (!new_properties.empty())
                  {
                        Property last_prop;

                        int next_id = -1;
                        int id = -1;
                        for (auto property : new_properties)
                        {
                              property.setNext(next_id);
                              id = add(property, properties);
                        }

                        if (node.hasProperty())
                        {
                              last_prop_id = getLastPropID(node.property_id);
                              Property last_prop = properties->read(last_prop_id);
                              last_prop.setNext(id);
                              properties->write(last_prop_id, last_prop);
                        }
                        else
                        {
                              node.setNextProperty(id);
                              nodes->write(node_id, node);
                        }
                  }
            }

            int getLastPropID(int start_id) const
            {
                  int next = start_id;
                  Property property = properties->read(next);

                  while (property.hasNext())
                  {
                        next = property.getNext();
                        std::cout << next << std::endl;
                        property = properties->read(next);
                  }

                  return next;
            }

            int getLastRelID(int node_id, int rel_start_id) const
            {
                  int next = rel_start_id;
                  Relation relation = relations->read(next);

                  while (relation.hasNext(node_id))
                  {
                        next = relation.getNext(node_id);
                        relation = relations->read(next);
                  }

                  return next;
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
