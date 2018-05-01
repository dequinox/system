#ifndef LABEL_H
#define LABEL_H

#pragma pack(push, 1)
struct Label
{
      bool in_use{false};
      char value[100]{};
};
#pragma pack(pop)

#endif /* LABEL_H */
