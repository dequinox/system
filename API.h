#ifndef API_H
#define API_H

#include "IO.h"
#include "Node.h"
#include <stack>
#include <queue>
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

            bool propertyExists(int id, std::pair<std::string, std::string> keyValue){
                  Property property = properties->get(id);
                  while (true){
                        if (property.getKey() == keyValue.first && property.getValue() == keyValue.second)
                              return true;
                        if (property.hasNext())
                              property = properties->get(property.getNext());
                        else break;
                  }
                  return false;
            }

            std::vector<std::vector<int> > bfs(std::string label[], int size,
                  int positions[], std::pair<std::string, std::string> keyValues[], int psize)
            {
                  std::vector<std::vector<int> > result(size);
                  std::queue<int> depth;
                  std::queue<int> nid;
                  Node node;
                  Relation relation;
                  int d;
                  int id;

                  for (auto it = nodes->begin(); it != nodes->end(); it++){
                        if (labels->get(nodes->get(*it).getLabel()).getValue() == label[0]){
                              bool flag = true;
                              for (int i = 0; i < psize; i++){
                                    if (positions[i] == 0){
                                          if (nodes->get(*it).hasProperty() && !propertyExists(nodes->get(*it).getProperty(), keyValues[i]))
                                                flag = false;
                                    }
                              }
                              if (flag){
                                    int rid = *it;
                                    result[0].push_back(rid);
                                    nid.push(*it);
                                    depth.push(0);
                              }
                        }
                  }

                  while (!nid.empty()){
                        d = depth.front();
                        id = nid.front();

                        depth.pop();
                        nid.pop();

                        if (d + 1 == size) continue;
                        node = nodes->get(id);

                        if (node.hasRelation()){
                              relation = relations->get(node.getRelation());

                              while (true){
                                    if (labels->get(relation.getLabel()).getValue() == label[d + 1]){
                                          if (labels->get(relation.getOtherNode(id)).getValue() == label[d + 2]){
                                                bool flag = true;
                                                for (int i = 0; i < psize; i++){
                                                      if (positions[i] == d + 1){
                                                            if (!propertyExists(relation.getProperty(), keyValues[i]))
                                                                  flag = false;
                                                      }
                                                      if (positions[i] == d + 2){
                                                            if (!propertyExists(node.getProperty(), keyValues[i]))
                                                                  flag = false;
                                                      }
                                                }
                                                if (flag){
                                                      nid.push(relation.getOtherNode(id));
                                                      depth.push(d + 2);
                                                      result[d + 1].push_back(node.getRelation());
                                                      result[d + 2].push_back(relation.getOtherNode(id));
                                                }
                                          }
                                    }

                                    if (!relation.hasNext(id)) break;
                                    relation = relations->get(relation.getNext(id));
                              }
                        }
                  }

                  return result;
            }

            void createRelations(std::string label[], int size, int positions[],
                  std::pair<std::string,std::string> keyValues[], int psize, std::string newLabel){
                  Node fnode;
                  Node snode;
                  for (auto f = nodes->begin(); f != nodes->end(); f++)
                  {
                        for (auto s = nodes->begin(); s != nodes->end(); s++){
                              if (*f != *s){
                                    fnode = nodes->get(*f);
                                    snode = nodes->get(*s);
                                    if (labels->get(fnode.getLabel()).getValue() == label[0]){
                                          if (labels->get(snode.getLabel()).getValue() == label[1]){
                                                bool flag = true;
                                                for (int i = 0; i < psize; i++){
                                                      if (positions[i] == 0){
                                                            if (!propertyExists(fnode.getProperty(), keyValues[i]))
                                                                  flag = false;
                                                      }
                                                      if (positions[i] == 1){
                                                            if (!propertyExists(snode.getProperty(), keyValues[i]))
                                                                  flag = false;
                                                      }
                                                }
                                                if (flag){
                                                      createRelation(*f, *s, newLabel);
                                                }
                                          }
                                    }
                              }
                        }
                  }
            }

            void set(std::vector<int> &ids, std::pair<std::string, std::string> keyValue){
                  for (auto id : ids){
                        if (nodes->get(id).hasProperty())
                              modify(nodes->get(id).getProperty(), keyValue);
                  }
            }

            void modify(int id, std::pair<std::string, std::string> keyValue){
                  int next = id;
                  Property property = properties->get(next);

                  while (true){
                        if (property.getKey() == keyValue.first)
                              properties->set(next, property.setValue(keyValue.second));
                        if (property.hasNext()){
                              next = property.getNext();
                              property = properties->get(next);
                        }
                        else break;
                  }
            }

            void printPropertiesList(int id){

                  Property property = properties->get(id);

                  while (true){
                        std::cout << property.getKey() << ": " << property.getValue();
                        std::cout << std::endl;

                        if (property.hasNext())
                              property = properties->get(property.getNext());
                        else break;
                  }
            }


            void printNodes(std::vector<int> &node_ids){
                  for (int i = 0; i < node_ids.size(); i++){
                        Node node = nodes->get(node_ids[i]);

                        std::cout << std::endl;
                        std::cout << "ID: " << node_ids[i] / sizeof(Node) << std::endl;
                        std::cout << "Label: " << labels->get(node.getLabel()).getValue() << std::endl;
                        if (node.hasProperty())
                              printPropertiesList(node.getProperty());
                        std::cout << std::endl;
                  }
            }

            void printRelations(std::vector<int> &rel_ids){
                  for (int i = 0; i < rel_ids.size(); i++){
                        Relation relation = relations->get(rel_ids[i]);

                        std::cout << std::endl;
                        std::cout << "ID: " << rel_ids[i] / sizeof(Relation) << std::endl;
                        std::cout << "TYPE: " << labels->get(relation.getLabel()).getValue() << std::endl;
                        std::cout << "FNODE: " << relation.getFirstNode() << std::endl;
                        std::cout << "SNODE: " << relation.getSecondNode() << std::endl;
                        if (relation.hasProperty())
                              printPropertiesList(relation.getProperty());
                        std::cout << std::endl;
                  }
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
