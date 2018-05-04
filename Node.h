#ifndef NODE_H
#define NODE_H

#pragma pack(push, 1)
struct Node
{
      bool in_use{false};
      int relationship_id{-1};
      int property_id{-1};
      int label_id{-1};

      bool hasProperty()
      {
            return property_id != -1;
      }

      bool hasRelation()
      {
            return relationship_id != -1;
      }

      int getRelationID()
      {
            return relationship_id;
      }

      void setNextRel(int rel_id)
      {
            relationship_id = rel_id;
      }

      void setNextLabel(int next_id)
      {
            label_id = next_id;
      }
      void setNextProperty(int next_id)
      {
            property_id = next_id;
      }
};
#pragma pack(pop)

#endif /* NODE_H */
