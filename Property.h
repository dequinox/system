#ifndef PROPERTY_H
#define PROPERTY_H

class Property
{
      private:
            bool m_used{true};
            char m_key[20];
            char m_value[100];
            int m_property{-1};
      public:
            Property() = default;
            Property(const std::string key, const std::string value, const int property){
                  strcpy(m_key, key.c_str());
                  strcpy(m_value, value.c_str());
                  m_property = property;
            }

            bool hasNext(){
                  return m_property != -1;
            }

            int getNext(){
                  return m_property;
            }

            Property &setValue(std::string value){
                  strcpy(m_value, value.c_str());
                  return *this;
            }

            Property &setNext(int property){
                  m_property = property;
                  return *this;
            }

            Property &clear(){
                  m_used = false;
                  return *this;
            }

            char * getKey(){
                  return m_key;
            }

            char * getValue(){
                  return m_value;
            }
};

#endif /* PROPERTY_H */
