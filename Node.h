#ifndef NODE_H
#define NODE_H

#pragma pack(push, 1)
struct Node
{
      bool in_use{false};
      int relationship_id{0};
      int property_id{0};
      int label_id{0};
};
#pragma pack(pop)

#endif /* NODE_H */
