#ifndef LinkedList_H
#define LinkedList_H

#include <iostream>
#include <stdexcept>

template <typename T> class Node;
template <typename T> class LinkedList;

// Singly linked list node that contains template determined content
template<typename T>
class Node 
{
    friend class LinkedList<T>;

    public:         

        Node(T value, Node<T> *next = nullptr)
        {
            this->value = value;
            this->next = next;
        };
        
        T value;
        Node<T>* next;
        
};

//  Singly linked list
template <typename T>
class LinkedList
{

    friend class Node<T>;

    public:

        LinkedList();
        LinkedList(std::initializer_list<T> initL);
        ~LinkedList();

        void push_back(T value);
        void push_back(LinkedList<T>& ll);
        void push_front(T value);
        void pop_back();
        void pop_front();
        void insert(const size_t& index, T value);
        T at(const size_t& index);
        void remove(const size_t& index);
        size_t get_size();
        void clear();
        void set(const size_t& index, T _value);
        bool isEmpty();

        
        template <typename TT>
        friend std::ostream& operator<<(std::ostream& ostr, const LinkedList<TT>& ll);
        template <typename TT>
        friend bool operator==(const LinkedList<TT>& ll1, const LinkedList<TT>& ll2);
    private:

        size_t size;
        Node<T> *head;

};

template <typename T>
LinkedList<T>::LinkedList()
{
    size = 0;
    head = nullptr;
}

template <typename T>
LinkedList<T>::LinkedList(std::initializer_list<T> initL)
{
    size = 0;
    head = nullptr;
    for (auto value : initL) {
        push_back(value);
    }
}

template <typename T>
LinkedList<T>::~LinkedList()
{
    Node<T>* current = head;
    while(current != nullptr) {
        Node<T>* temp = current->next;
        delete current;
        current = temp;
    }
}

//  Adds new element with given value at the end of linked list after its
//  current last element, increasing list size by one.
//      value   -   value of the new element
template <typename T>
void LinkedList<T>::push_back(T value)
{
    if (head == nullptr) {
        head = new Node<T>(value);
    } else {
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new Node<T>(value);
    }
    ++size;
}

//  Appends one list to another, increasing the size of current list by the
//  size of another one.
//      ll  -   list to append
template <typename T> 
void LinkedList<T>::push_back(LinkedList<T>& ll)
{
    if (ll.head == nullptr) {
        return;
    } else {
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        Node<T>* pushedCurrent = ll.head;
        while (pushedCurrent != nullptr) {
            current->next = new Node<T>(pushedCurrent->value);
            pushedCurrent = pushedCurrent->next;
            current = current->next;
            ++size;
        }
    }
}

//  Adds new element with given value at front of linked list right before
//  its first element, increasing list size by one.
//      value   -   value of the new element
template <typename T>
void LinkedList<T>::push_front(T value)
{
    Node<T>* newHead = new Node<T>(value);
    newHead->next = head;
    head = newHead;
    ++size;
}

//  Removes the last element in the list, reducing the list size by one.
//  The removed element is destroyed.
template <typename T>
void LinkedList<T>::pop_back()
{
    if (head == nullptr) {
        throw std::out_of_range("Tried to pop empty list");
    } else if (head->next == nullptr) {
        delete head;
        head = nullptr;
        --size;
    } else {
        Node<T>* current = head;
        while (current->next->next != nullptr) {
            current = current->next;
        }
        delete current->next;
        current->next = nullptr;
        --size;
    }
}

//  Removes the first element in the linked list, reducing its size by one.
//  The removed element is destoyed.
template <typename T>
void LinkedList<T>::pop_front()
{
    if (head == nullptr) {
        throw std::out_of_range("Tried to pop empty list");
    } else if (head->next == nullptr) {
        delete head;
        head = nullptr;
        --size;
    } else {
        Node<T>* temp = head->next;
        delete head;
        head = temp;
        --size;
    }
}

//  Inserts a new element with given value at the specified position, increasing
//  list size by one.
//      index   -   position in the list where the new element is inserted
//      value   -   value of the new element
template <typename T>
void LinkedList<T>::insert(const size_t& index, T value)
{
    if (((size != 0) && (index > size - 1)) ||
        ((size == 0) && (index > size ))) {
        throw std::out_of_range("Index does not exist");
    } else if (index == 0) {
        Node<T>* insertion = new Node<T>(value);
        insertion->next = head;
        head = insertion;
        ++size;
    } else {
        Node<T>* current = head;
        size_t count = 0;
        while (count != index - 1) {
            current = current->next;
            ++count;
        }
        Node<T>* insertion = new Node<T>(value);
        insertion->next = current->next;
        current->next = insertion;
        ++size;
    }
}

//  Returns value of the element at given position in the list.
//      index   -   index of element to access
template <typename T>
T LinkedList<T>::at(const size_t& index)
{
    if (((size != 0) && (index > size - 1)) || (size == 0)) {
        throw std::out_of_range("Index does not exist");
    } else {
        Node<T>* current = head;
        size_t count = 0;
        while (count != index) {
            current = current->next;
            ++count;
        }
        return current->value;
    }
}

//  Removes element at given index from the list, decresing its size by one.
//      index   -   index of element to remove
template <typename T>
void LinkedList<T>::remove(const size_t& index)
{
    if (head == nullptr) {
        throw std::out_of_range("Tried to remove from empty list");
    } else if (((size != 0) && (index > size - 1)) ||
               ((size == 0) && (index >= size ))) {
        throw std::out_of_range("Index does not exist");
    } else if (index == 0) {
        Node<T>* temp = head->next;
        delete head;
        head = temp;
        --size;
    } else {
        Node<T>* current = head;
        size_t count = 0;
        while (count != index - 1) {
            current = current->next;
            ++count;
        }
        Node<T>* temp = current->next->next;
        delete current->next;
        current->next = temp;
        --size;
    }
}

//  Returns the number of elements in the list.
template <typename T>
size_t LinkedList<T>::get_size()
{
    return size;
}

//  Removes all elements from the list (which are destroyed), leaving the list
//  with a size of zero.
template <typename T>
void LinkedList<T>::clear()
{
    this->~LinkedList();
    head = nullptr; // makes list remain
    size = 0;
}

//  Assigns new value to an element at given index.
//      index   -   index of element to assign
//      _value  -   new value
template <typename T>
void LinkedList<T>::set(const size_t& index, T _value)
{
    if (((size != 0) && (index > size - 1)) ||
         (size == 0)) {
        throw std::out_of_range("Index does not exist");
    } else {
        Node<T>* current = head;
        size_t count = 0;
        while (count != index) {
            current = current->next;
            ++count;
        }
        current->value = _value;
    }
}

//  Returns true if the list is empty and false if not;
template <typename T>
bool LinkedList<T>::isEmpty()
{
    if (head == nullptr) {
        return true;
    } else {
        return false;
    }
}

//  Generates a sequence of list element values, formatted as follows:
//  [ x x x x x x x ]
template <typename TT>
std::ostream& operator<<(std::ostream& ostr, const LinkedList<TT>& ll)
{
    ostr << "[ ";
    Node<TT>* current = ll.head;
    while (current != nullptr) {
        ostr << current->value << " ";
        current = current->next;
    }
    ostr << "]";
    return ostr;
}

template <typename TT>
bool operator==(const LinkedList<TT>& ll1, const LinkedList<TT>& ll2)
{
    
    Node<TT>* current = ll1.head;
    Node<TT>* lhs = ll2.head;
    while (current != nullptr) {
        if (current->value != lhs->value) return false;
        current = current->next;
        lhs = lhs->next;
    }
    return true;
}
#endif



