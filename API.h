#ifndef API_H
#define API_H

#include "IO.h"
#include "Node.h"
#include <iostream>
#include <cstring>
#include "Label.h"
#include "Restore.h"
#include "Property.h"
#include "Relationship.h"

const std::string node_file = "nodestore";
const std::string node_restore = "node.restore";

const std::string label_file = "labelstore";
const std::string label_restore = "label.restore";

const std::string property_file = "propertystore";
const std::string property_restore = "property.restore";

namespace API
{

      int getRestoreId(std::string filename)
      {
            Restore new_restore = IO::pop<Restore>(filename);
            return new_restore.restore_id;
      }

      void show(std::string filename)
      {
            std::ifstream file(filename);

            if (filename == "nodestore")
            {
                  Node data;
                  while (file.read(reinterpret_cast<char *>(&data), sizeof(Node)))
                  {
                        std::cout << data.in_use << " " << data.label_id << std::endl;
                  }
            }

            else
            {
                  Label data;
                  while (file.read(reinterpret_cast<char *>(&data), sizeof(Label)))
                  {
                        std::cout << data.in_use << " " << data.value << std::endl;
                  }
            }

            file.close();
      }

      void create(std::string label, std::initializer_list<std::pair<std::string, std::string> > list)
      {
            int node_id = getRestoreId(node_restore);
            int label_id = getRestoreId(label_restore);
            int property_id = getRestoreId(property_restore);

            if (node_id == 0)  node_id = IO::getFileSize(node_file);
            if (label_id == 0) label_id = IO::getFileSize(label_file);
            if (property_id == 0) property_id = IO::getFileSize(property_id);

            Node new_node{true, 0, property_id, label_id};

            Label new_label;
            new_label.in_use = true;
            strcpy(new_label.value, label.c_str());

            IO::write(node_id, new_node, node_file);
            IO::write(label_id, new_label, label_file);
            IO::write()
      }
}

#endif /* API_H */
