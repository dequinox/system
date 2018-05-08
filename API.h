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

class Database {

      private:

            std::string m_db;

            IO::Store<Node> * nodes;
            IO::Store<Label> * labels;
            IO::Store<Property> * properties;
            IO::Store<Relation> * relations;

      public:
            Database(const Database &other) = delete;
            Database() = delete;

            Database(std::string db) : m_db(db) {
                  nodes = new IO::Store<Node>(m_db + ".nodes.store", m_db + ".node.ids");
                  labels = new IO::Store<Label>(m_db + ".labels.store", m_db + ".label.ids");
                  relations = new IO::Store<Relation>(m_db + ".relations.store", m_db + ".relation.ids");
                  properties = new IO::Store<Property>(m_db + ".properties.store", m_db + ".property.ids");
            }

            int create(std::string label_name) {
                  Node new_node(add(Label(label_name), labels));

                  return add(new_node, nodes);
            }

            template<typename T>
            int add(T entity, IO::Store<T> *store) {
                  int id = store->getNewID();

                  store->set(id, entity);
                  return id;
            }

            template<typename T>
            void remove(int id, IO::Store<T> *store) {
                  T entity = store->get(id);
                  entity.clear();
                  store->set(id, entity);
            }

            int createRelation(int fnode, int snode, std::string label_name)
            {
                  Relation new_relation(fnode, snode, add(Label(label_name), labels));

                  int id = add(new_relation, relations);

                  linkRelation(fnode, id);
                  linkRelation(snode, id);

                  return id;
            }

            void linkRelation(int node_id, int rel_id)
            {
                  Node node = nodes->get(node_id);

                  relations->set(rel_id, relations->get(rel_id).setNext(node_id, node.getRelation()));
                  if (node.hasRelation()){

                        relations->set(node.getRelation(), relations->get(node.getRelation()).setPrevious(node_id, rel_id));
                  }
                  nodes->set(node_id, nodes->get(node_id).setRelation(rel_id));
            }

            void unlinkRelation(int node_id, int rel)
            {
                  Relation relation = relations->get(rel);
                  int next = relation.getNext(node_id);
                  int previous = relation.getPrevious(node_id);

                  if (relation.hasPrevious(node_id))
                        relations->set(previous, relations->get(previous).setNext(node_id, next));
                  else
                        nodes->set(node_id, nodes->get(node_id).setRelation(next));

                  if (relation.hasNext(node_id))
                        relations->set(next, relations->get(next).setPrevious(node_id, previous));

            }

            int createProperty(int node, std::string key, std::string value)
            {
                  Property new_property(key, value, nodes->get(node).getProperty());
                  int id = add(new_property, properties);
                  nodes->set(node, nodes->get(node).setProperty(id));

                  return id;
            }

            template<typename T>
            void removeEntity(IO::Store<T> *store, int id){
                  store->set(id, store->get(id).clear());
                  store->remove(id);
            }

            void deleteRelation(int relation){
                  Relation drelation = relations->get(relation);

                  unlinkRelation(drelation.getFirstNode(), relation);
                  unlinkRelation(drelation.getSecondNode(), relation);

                  removeEntity(relations, relation);
            }

            void deleteProperty(int property){
                  removeEntity(properties, property);
            }

            template<typename T>
            void traverse(IO::Store<T> *store, void (Database::*operation)(int)) {
                  for (auto it = store->begin(); it != store->end(); it++)
                  {
                        (this->*operation)(*it);
                  }

            }

            void deleteNode(int node){
                  Node dnode = nodes->get(node);

                  traverse(properties, &Database::deleteProperty);
                  traverse(relations,  &Database::deleteRelation);

                  removeEntity(labels, dnode.getLabel());
                  removeEntity(nodes, node);
            }

            std::vector < <vector <int> > > bfs (std::string labels, int size)
            {
                  std::vector<vector<int> > result(vector<int>(size));
                  std::queue<int> depth;
                  std::queue<int> nid;
                  Node node;
                  Relation relation;
                  int d;
                  int id;

                  for (auto it = nodes->begin(); it != nodes->end(); it++){
                        if (labels->get(nodes->get(*it).getLabel()).getValue() == labels[0]){
                              result[0].push_back(*it);
                              nid.push_back(*it);
                              snodes.push_back(nodes->get(*it));
                              depth.push_back(0);
                        }
                  }

                  while (!ids.empty()){
                        d = depth.top();
                        id = nid.top();

                        depth.pop();
                        nid.pop();

                        node = nodes->get(id);

                        if (node.hasRelation()){
                              relation = relations->get(node.getRelation());

                              while (true){
                                    if (labels->get(relation.getLabel()).getValue() == labels[d + 1]){
                                          if (labels->get(relation.getOtherNode(id)).getValue() == labels[d + 2]){
                                                nid.push_back(relation.getOtherNode(id));
                                                depth.push_back(d + 2);
                                                result[d + 1].push_back(node.getRelation());
                                                result[d + 2].push_back(relation.getOtherNode(id));
                                          }
                                    }

                                    if (!relation.hasNext(id)) break;
                                    relation = relations->get(relation.getNext(id));
                              }
                        }
                  }

                  return result;
            }



            void print() {
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

            ~Database() {
                  delete nodes;
                  delete labels;
                  delete properties;
                  delete relations;
            }

};

#endif /* API_H */
