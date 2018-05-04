#ifndef PROPERTY_H
#define PROPERTY_H

struct Property
{
      bool in_use{false};
      char key[15];
      char value[15];
      int next_property_id{-1};

      bool hasNext()
      {
            return next_property_id != -1;
      }

      int getNext()
      {
            return next_property_id;
      }

      void setNext(int next_id)
      {
            next_property_id = next_id;
      }
};

#endif /* PROPERTY_H */
