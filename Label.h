#ifndef LABEL_H
#define LABEL_H

#pragma pack(push, 1)
class Label
{
      private:
            bool m_used{true};
            char m_value[20]{};
      public:
            Label() = default;
            Label(const std::string value)
            {
                  strcpy(m_value, value.c_str());
            }

            Label &clear(){
                  m_used = false;
                  return *this;
            }

            char * getValue(){
                  return m_value;
            }
};
#pragma pack(pop)

#endif /* LABEL_H */
