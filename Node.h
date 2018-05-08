#ifndef NODE_H
#define NODE_H

#pragma pack(push, 1)
class Node
{
      private:
            bool m_used{true};
            int m_relation{-1};
            int m_property{-1};
            int m_label{-1};
      public:
            Node() = default;
            Node(int label) : m_label(label) {}

      bool hasProperty(){
            return m_property != -1;
      }

      bool hasRelation(){
            return m_relation != -1;
      }

      int getRelation(){
            return m_relation;
      }

      int getProperty(){
            return m_property;
      }

      int getLabel(){
            return m_label;
      }

      Node &setRelation(int relation){
            m_relation = relation;
            return *this;
      }

      Node &setLabel(int label){
            m_label = label;
            return *this;
      }
      Node &setProperty(int property){
            m_property = property;
            return *this;
      }

      Node &clear(){
            m_used = false;
            return *this;
      }
};
#pragma pack(pop)

#endif /* NODE_H */
