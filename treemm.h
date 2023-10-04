#include <set>
#include <iostream>
#include <vector>

#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

template <typename KeyType, typename ValueType>
class TreeMultimap {
  public:

    class Iterator { // to iterate through the binary search tree
      public:
        Iterator() {m_isValid = false;}
        Iterator(std::vector<ValueType>* values) {
          m_it = values->begin();
          m_end = values->end();
          m_isValid = true;
        }

        ValueType& get_value() const {return *m_it;}

        bool is_valid() const {return m_isValid;}

        void advance() {
          if (m_isValid) {
            m_it++;
            if (m_it == m_end)
              m_isValid = false;
          }
        }

      private:
        bool m_isValid;
        typename std::vector<ValueType>::iterator m_it;
        typename std::vector<ValueType>::iterator m_end;
    };

    TreeMultimap() {root = nullptr;}

    ~TreeMultimap() {destructorHelper(root);}

    void insert(const KeyType& key, const ValueType& value) {
       // if tree empty
       if (root == nullptr) {
        root = new Node(key, value);
        return;
       }

      // if key doesn't already exist on tree
      Node* ptr;
       for(ptr = root; ptr->m_key != key;) {
        if (key < ptr->m_key) {
          if (ptr->left != nullptr)
            ptr = ptr->left;
          else {
            ptr->left = new Node(key, value);
            return;
          }
        } else if (key > ptr->m_key) {
          if (ptr->right != nullptr)
            ptr = ptr->right;
          else {
            ptr->right = new Node(key, value);
            return;
          }
        }
       }

       // if key already exists on tree
      ptr->values.push_back(value);
    }

    Iterator find(const KeyType& key) const {
      Node* curr = root;
      // looks to see if key exists on tree
      while(curr && key != curr->m_key) {
        if (key < curr->m_key)
          curr = curr->left;
        else if (key > curr->m_key)
          curr = curr->right;
      }
      // if the key doesn't exist on the tree
      if (!curr) {
          Iterator it;
          return it;
      }
      Iterator it(&(curr->values));
      return it;
    }

  private:
    struct Node {
      Node(KeyType key, ValueType value) {
        m_key = key;
        values.push_back(value);
        left = nullptr;
        right = nullptr;
      }
      KeyType m_key;
      std::vector<ValueType> values;
      Node* left;
      Node* right;
    };
    Node* root;
    void destructorHelper(Node* curr) {
      if(!curr)
        return;
      destructorHelper(curr->left);
      destructorHelper(curr->right);
      delete curr;
    }
};

#endif // TREEMULTIMAP_INCLUDED
