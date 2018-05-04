#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

struct Relation
{
      bool in_use{false};
      int first_node;
      int second_node;
      int first_prev_id;
      int first_next_id;
      int second_prev_id;
      int second_next_id;
      int next_property;
      bool first_chain_marker;

      int getFirstNode()
      {
            return first_node;
      }

      int getSecondNode()
      {
            return second_node;
      }

      bool hasNext(int node_id)
      {
            if (node_id == first_node)  return first_next_id != -1;
            if (node_id == second_node) return second_next_id != -1;

            return -1;
      }

      int getNext(int node_id)
      {
            if (node_id == first_node) return first_next_id;
            if (node_id == second_node) return second_next_id;

            return -1;
      }

      void setFirstNextRel(int next_relID)
      {
            first_next_id = next_relID;
      }

      void setFirstPrevRel(int prev_relID)
      {
            first_prev_id = prev_relID;
      }

      void setSecondNextRel(int next_relID)
      {
            second_next_id = next_relID;
      }

      void setSecondPrevRel(int prev_relID)
      {
            second_prev_id = prev_relID;
      }
};

#endif /* RELATIONSHIP_H */
