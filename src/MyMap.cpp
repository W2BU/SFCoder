#ifndef MyMap_H
#define MyMap_H

#include <stdexcept>
#include <string>
#include <iostream>
#include <LinkedList.cpp>

template <typename Key, typename Value> class TreeNode;
template <typename Key, typename Value> class MyMap;


template <typename Key, typename Value>
class TreeNode
{
    friend class MyMap<Key, Value>;

    private:
        TreeNode(Key key, Value value, bool color = 0,
                 TreeNode<Key, Value>* right = nullptr,
                 TreeNode<Key, Value>* left = nullptr,
                 TreeNode<Key, Value>* parent = nullptr) 
        {
            this->key = key;
            this->value = value;
            this->left = left;
            this->right = right;
            this->parent = parent;
            this->color = color;
        }
        TreeNode(
            bool color = 0,
            TreeNode<Key, Value>* right = nullptr,
            TreeNode<Key, Value>* left = nullptr,
            TreeNode<Key, Value>* parent = nullptr)
        {
            this->left = left;
            this->right = right;
            this->parent = parent;
            this->color = color;
        };

        Key key;
        Value value;
        TreeNode<Key, Value>* right;
        TreeNode<Key, Value>* left;
        TreeNode<Key, Value>* parent;
        bool color; // 0 - black, 1 - red
};


template <typename Key, typename Value>
class MyMap
{

    friend class TreeNode<Key, Value>;

    public:

        MyMap();
        MyMap(std::initializer_list<std::pair<Key, Value>> initList);
        ~MyMap();
        
        void insert(Key key, Value value);
        void remove(Key key);
        Value find(Key key);
        bool has(Key key);
        void addToValue(Key key, int add);
        void clear();
        LinkedList<Key> get_keys();
        LinkedList<Value> get_values();
        void printKeys();
        void printValues();


    private:

        TreeNode<Key, Value>* root;
        TreeNode<Key, Value>* nil;

        void destroyRecursive(TreeNode<Key, Value>* node);
        void leftRotate(TreeNode<Key, Value>* x);
        void rightRotate(TreeNode<Key, Value>* x);
        void transplant(TreeNode<Key, Value>* child, TreeNode<Key, Value>* deletion);
        TreeNode<Key, Value>* minimum(TreeNode<Key, Value>* start);
        void get_keysHelper(const TreeNode<Key, Value>* node, LinkedList<Key>& result);
        void get_valuesHelper(const TreeNode<Key, Value>* node, LinkedList<Value>& result);
        void printKeysHelper(const std::string& prefix, const TreeNode<Key, Value>* node, bool isLeft);
        void printKeysHelper(const TreeNode<Key, Value>* node);
        void printValuesHelper(const std::string& prefix, const TreeNode<Key, Value>* node, bool isLeft);
        void printValuesHelper(const TreeNode<Key, Value>* node);
        void removeFix(TreeNode<Key, Value>* start);
        void insertFix(TreeNode<Key, Value>* insertion);

    
};

template <typename Key, typename Value>
MyMap<Key, Value>::MyMap()
{
    nil = new TreeNode<Key, Value>();
    root = nil;
}

template <typename Key, typename Value>
MyMap<Key, Value>::MyMap(std::initializer_list<std::pair<Key, Value>> initList)
{
    nil = new TreeNode<Key, Value>();
    root = nil;
    for (auto pair : initList) {
        insert(pair.first, pair.second);
    }
}

template <typename Key, typename Value>
MyMap<Key, Value>::~MyMap()
{
    destroyRecursive(this->root);
    delete nil;
}

//  Inserts a node with the given key and value in the tree.
//  If such node already exists throws invalid_argument exception.
template <typename Key, typename Value>
void MyMap<Key, Value>::insert(Key key, Value value)
{
    TreeNode<Key, Value>* insertion = new TreeNode<Key, Value>(key, value, 1, nil, nil, nullptr);

    TreeNode<Key, Value>* leaf = nullptr;
    TreeNode<Key, Value>* current = this->root;

     while (current != nil) {
      leaf = current;
      if (insertion->key < current->key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }

    insertion->parent = leaf;

    if (leaf == nullptr) {
        root = insertion;
    } else if (insertion->key < leaf->key) {
        leaf->left = insertion;
    } else if (insertion->key > leaf->key){
        leaf->right = insertion;
    } else {
        throw std::invalid_argument("No duplicates allowed");
    }

    if (insertion->parent == nullptr) {
      insertion->color = 0;
      return;
    }

    if (insertion->parent->parent == nullptr) {
      return;
    }

    insertFix(insertion);
}

template <typename Key, typename Value>
void MyMap<Key, Value>::insertFix(TreeNode<Key, Value>* insertion)
{
    TreeNode<Key, Value>* other;
    while (insertion->parent->color == 1) {
      if (insertion->parent == insertion->parent->parent->right) {
        other = insertion->parent->parent->left;
        if (other->color == 1) {
          other->color = 0;
          insertion->parent->color = 0;
          insertion->parent->parent->color = 1;
          insertion = insertion->parent->parent;    
        } else {
          if (insertion == insertion->parent->left) {
            insertion = insertion->parent;
            rightRotate(insertion);
          }
          insertion->parent->color = 0;
          insertion->parent->parent->color = 1;
          leftRotate(insertion->parent->parent);
        }
      } else {
        other = insertion->parent->parent->right;
        if (other->color == 1) {
          other->color = 0;
          insertion->parent->color = 0;
          insertion->parent->parent->color = 1;
          insertion = insertion->parent->parent;
        } else {
          if (insertion == insertion->parent->right) {
            insertion = insertion->parent;
            leftRotate(insertion);
          }
          insertion->parent->color = 0;
          insertion->parent->parent->color = 1;
          rightRotate(insertion->parent->parent);
        }
      }
      if (insertion == root) {
        break;
      }
    }
    root->color = 0;
}

//  Removes the node with the given key from the tree
template <typename Key, typename Value>
void MyMap<Key, Value>::remove(Key key)
{
    TreeNode<Key, Value>* current = this->root, *deletion = nil;
    TreeNode<Key, Value>* child;
    while (current != nil) {
        if (current->key == key) {
            deletion = current;
        }
        if (current->key <= key) {
            current = current->right;
        } else {
            current = current->left;
        }
    }

    
    bool originalColor = deletion->color;
    if (deletion == nil) {
        throw std::invalid_argument("Key not found");
    } else if (deletion->left == nil) {
        child = deletion->right;
        transplant(deletion, deletion->right);
    } else if (deletion->right == nil) {
        child = deletion->left;
        transplant(deletion, deletion->left);
    } else {
        TreeNode<Key, Value>* minOfRight= minimum(deletion->right);
        originalColor = minOfRight->color;
        child = minOfRight->right;
        if (deletion == child->parent) {
            child->parent = minOfRight;  
        } else {
            transplant(minOfRight, minOfRight->right);
            minOfRight->right = deletion->right;
            minOfRight->right->parent = child;
        }
        transplant(deletion, minOfRight);
        minOfRight->left = deletion->left;
        minOfRight->left->parent = minOfRight;
        minOfRight->color = deletion->color;
    }
    delete deletion;
    if (originalColor == 0/*black*/ ) {
        removeFix(child);
    }
}

// Transplants x with y
template <typename Key, typename Value>
void MyMap<Key, Value>::transplant(TreeNode<Key, Value>* x, TreeNode<Key, Value>* y)
{
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->parent = x->parent;
}

//  Finds the most left node(minimum) from the given position
template <typename Key, typename Value>
TreeNode<Key, Value>* MyMap<Key, Value>::minimum(TreeNode<Key, Value>* start)
{
    while (start->left != nil) {
      start = start->left;
    }
    return start;
}

template <typename Key, typename Value>
void MyMap<Key, Value>::removeFix(TreeNode<Key, Value>* start)
{
    TreeNode<Key, Value>* sibling;
    while (start != root && start->color == 0) {
      if (start == start->parent->left) {
        sibling = start->parent->right;
        if (sibling->color == 1) {
          sibling->color = 0;
          start->parent->color = 1;
          leftRotate(start->parent);
          sibling = start->parent->right;
        }

        if (sibling->left->color == 0 && sibling->right->color == 0) {
          sibling->color = 1;
          start = start->parent;
        } else {
          if (sibling->right->color == 0) {
            sibling->left->color = 0;
            sibling->color = 1;
            rightRotate(sibling);
            sibling = start->parent->right;
          }

          sibling->color = start->parent->color;
          start->parent->color = 0;
          sibling->right->color = 0;
          leftRotate(start->parent);
          start = root;
        }
      } else {
        sibling = start->parent->left;
        if (sibling->color == 1) {
          sibling->color = 0;
          start->parent->color = 1;
          rightRotate(start->parent);
          sibling = start->parent->left;
        }

        if (sibling->right->color == 0 && sibling->right->color == 0) {
          sibling->color = 1;
          start = start->parent;
        } else {
          if (sibling->left->color == 0) {
            sibling->right->color = 0;
            sibling->color = 1;
            leftRotate(sibling);
            sibling = start->parent->left;
          }

          sibling->color = start->parent->color;
          start->parent->color = 0;
          sibling->left->color = 0;
          rightRotate(start->parent);
          start = root;
        }
      }
    }
    start->color = 0;
}

//  Finds the node with the given key and returns its value otherwise throws
//  exception
template <typename Key, typename Value>
Value MyMap<Key, Value>::find(Key key)
{
    TreeNode<Key, Value>* current = this->root, *find = nil;
    while (current != nil) {
        if (current->key == key) find = current;
        if (current->key <= key) {
            current = current->right;
        } else {
            current = current->left;
        }
    }
    if (find == nil) {
        throw std::invalid_argument("Key not found");
    } else {
        return find->value;
    }
}

template <typename Key, typename Value>
bool MyMap<Key, Value>::has(Key key)
{
    TreeNode<Key, Value>* current = this->root, *find = nil;
    while (current != nil) {
        if (current->key == key) find = current;
        if (current->key <= key) {
            current = current->right;
        } else {
            current = current->left;
        }
    }
    if (find == nil) {
        return false;
    } else {
        return true;
    }
}

template <typename Key, typename Value>
void MyMap<Key, Value>::addToValue(Key key, int add)
{
    TreeNode<Key, Value>* current = this->root, *find = nil;
    while (current != nil) {
        if (current->key == key) find = current;
        if (current->key <= key) {
            current = current->right;
        } else {
            current = current->left;
        }
    }
    if (find == nil) {
        return;
    } else {
        find->value++;
    }
}


template <typename Key, typename Value>
void MyMap<Key, Value>::clear()
{
    destroyRecursive(this->root);
    root = nil;
}

// Returns the list of keys in post-order
template <typename Key, typename Value>
LinkedList<Key> MyMap<Key, Value>::get_keys()
{
    LinkedList<Key> result = {};
    get_keysHelper(this->root, result);
    return result;
}


template <typename Key, typename Value>
void MyMap<Key, Value>::get_keysHelper(const TreeNode<Key, Value>* node, LinkedList<Key>& result)
{
    if (node != nil) {
        get_keysHelper(node->left, result);
        get_keysHelper(node->right, result);
        result.push_back(node->key);
    }
}

// Returns the list of values in post-order
template <typename Key, typename Value>
LinkedList<Value> MyMap<Key, Value>::get_values()
{
    LinkedList<Value> result = {};
    get_valuesHelper(this->root, result);
    return result;
}

template <typename Key, typename Value>
void MyMap<Key, Value>::get_valuesHelper(const TreeNode<Key, Value>* node, LinkedList<Value>& result)
{
    if (node != nil) {
        get_valuesHelper(node->left, result);
        get_valuesHelper(node->right, result);
        result.push_back(node->value);
    }
}

template <typename Key, typename Value>
void MyMap<Key, Value>::printKeys()
{
    printKeysHelper(this->root);
}

template <typename Key, typename Value>
void MyMap<Key, Value>::printKeysHelper(const TreeNode<Key, Value>* node)
{
    printKeysHelper("", node, false);    
}

template <typename Key, typename Value>
void MyMap<Key, Value>::printKeysHelper(const std::string& prefix, const TreeNode<Key, Value>* node, bool isLeft)
{
    if (node != nil) {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->key << std::endl;

        // enter the next tree level - left and right branch
        printKeysHelper( prefix + (isLeft ? "│   " : "    "), node->right, true);
        printKeysHelper( prefix + (isLeft ? "│   " : "    "), node->left, false);
    }
}

template <typename Key, typename Value>
void MyMap<Key, Value>::printValues()
{
    printValuesHelper(this->root);
}

template <typename Key, typename Value>
void MyMap<Key, Value>::printValuesHelper(const TreeNode<Key, Value>* node)
{
    printValuesHelper("", node, false);    
}

template <typename Key, typename Value>
void MyMap<Key, Value>::printValuesHelper(const std::string& prefix, const TreeNode<Key, Value>* node, bool isLeft)
{
    if (node != nil) {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->value << std::endl;

        // enter the next tree level - left and right branch
        printValuesHelper( prefix + (isLeft ? "│   " : "    "), node->right, true);
        printValuesHelper( prefix + (isLeft ? "│   " : "    "), node->left, false);
    }
}

template <typename Key, typename Value>
void MyMap<Key, Value>::destroyRecursive(TreeNode<Key, Value>* node)
{
    if (node != nil) {
        destroyRecursive(node->left);
        destroyRecursive(node->right);
        delete node;
    }
}

template <typename Key, typename Value>
void MyMap<Key, Value>::leftRotate(TreeNode<Key, Value>* x)
{
    TreeNode<Key, Value>* y = x->right;
    x->right = y->left;
    if (y->left != nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

template <typename Key, typename Value>
void MyMap<Key, Value>::rightRotate(TreeNode<Key, Value>* x)
{
    TreeNode<Key, Value>* y = x->left;
    x->left = y->right;
    if (y->right != nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

#endif