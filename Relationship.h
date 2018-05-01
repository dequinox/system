#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

struct Relationship
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
};

#endif /* RELATIONSHIP_H */
