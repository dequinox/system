#ifndef PROPERTY_H
#define PROPERTY_H

struct Property
{
      bool in_use{false};
      char key[15];
      char value[15];
      int next_property_id;
};

#endif /* PROPERTY_H */
