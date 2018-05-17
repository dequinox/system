#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#pragma pack(push, 1)
class Relation
{
      private:
            bool m_used{true};
            int m_fnode{-1};
            int m_snode{-1};
            int m_fprevious{-1};
            int m_fnext{-1};
            int m_sprevious{-1};
            int m_snext{-1};
            int m_label{-1};
            int m_property{-1};
            bool m_chained{false};
      public:
            Relation() = default;
            Relation(int fnode, int snode, int label) : m_fnode(fnode), m_snode(snode), m_label(label) {}

            int getFirstNode(){
                  return m_fnode;
            }

            int getSecondNode(){
                  return m_snode;
            }

            int getOtherNode(int node){
                  if (node == m_fnode) return m_snode;
                  if (node == m_snode) return m_fnode;
                  return -1;
            }

            int getLabel(){
                  return m_label;
            }

            int getProperty(){
                  return m_property;
            }

            bool hasProperty(){
                  return m_property != -1;
            }

            bool hasNext(int node){
                  if (node == m_fnode) return m_fnext != -1;
                  if (node == m_snode) return m_snext != -1;
                  return false;
            }

            bool hasPrevious(int node){
                  if (node == m_fnode) return m_fprevious != -1;
                  if (node == m_snode) return m_sprevious != -1;
                  return false;
            }

            int getNext(int node){
                  if (node == m_fnode) return m_fnext;
                  if (node == m_snode) return m_snext;
                  return -1;
            }

            int getPrevious(int node){
                  if (node == m_fnode) return m_fprevious;
                  if (node == m_snode) return m_sprevious;
                  return -1;
            }

            Relation &setNext(int node, int relation){
                  if (node == m_fnode) m_fnext = relation;
                  if (node == m_snode) m_snext = relation;
                  return *this;
            }

            Relation &setPrevious(int node, int relation){
                  if (node == m_fnode) m_fprevious = relation;
                  if (node == m_snode) m_sprevious = relation;
                  return *this;
            }

            Relation &clear(){
                  m_used = false;
                  return *this;
            }
};
#pragma pack(pop)

#endif /* RELATIONSHIP_H */
