/*
Author: Viacheslav Shapoval
Title: Templated List Class
Description: Implementation of a generic doubly linked list
             that supports deep copying, moving, insertion, removal,
             and O(min(index, n - index)) traversal.
Date Created: 9/18/2026
Date Last Modified: 9/20/2026
*/
#ifndef PROJ4_LINKEDLIST_H
#define PROJ4_LINKEDLIST_H

#include <cstddef>      // std::size_t type
#include <iosfwd>       // forward declaration of stream types
#include <stdexcept>    // std::out_of_range exception handling
#include <iostream>     // std::ostream, std::endl, console operations
using namespace std;

template <typename T>
class LinkedList {
public:
    LinkedList() noexcept;

    LinkedList(const LinkedList& other);
    LinkedList(LinkedList&& other) noexcept;

    ~LinkedList();

    LinkedList& operator=(const LinkedList& other);
    LinkedList& operator=(LinkedList&& other) noexcept;

    void swap(LinkedList& other) noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    T& at(std::size_t index);
    const T& at(std::size_t index) const;

    void pushFront(const T& value);
    void pushFront(T&& value);

    void pushBack(const T& value);
    void pushBack(T&& value);

    void popFront();
    void popBack();

    void insert(std::size_t index, const T& value);
    void insert(std::size_t index, T&& value);

    void erase(std::size_t index);
    void clear() noexcept;

    void print(std::ostream& output) const;

    // description: outputs the list elements to a stream
    // return: ostream&
    // precondition: the list is in a valid state
    // postcondition: elements are written to the stream separated by spaces
    friend ostream& operator<<(ostream& os, const LinkedList<T>&
        list) {
        const Node* traverseP = list.head;
        while (traverseP != nullptr) {
            os << traverseP->value << " ";
            traverseP = traverseP->next;
        }
        os << endl;
        return os;
    }
private:
    struct Node {
        T value;            // The stored data
        Node* previous;     // Pointer to the previous node in the list
        Node* next;         // Pointer to the next node in the list

        explicit Node(const T& value);
        explicit Node(T&& value);
    };

    Node* head;                 // Pointer to the first node in the list
    Node* tail;                 // Pointer to the last node in the list
    std::size_t elementCount;   // The number of elements currently stored

    Node* findNode(std::size_t index) const;
};

// description: Node copy constructor
// return: none
// precondition: value is a valid T object
// postcondition: Node is initialized with a copy of value and null pointers
template <typename T>
LinkedList<T>::Node::Node(const T& value) : value(value), previous(nullptr),
                                            next(nullptr) {}

// description: Node move constructor
// return: none
// precondition: value is a movable T object
// postcondition: Node is initialized by moving value and null pointers
template <typename T>
LinkedList<T>::Node::Node(T&& value) : value(std::move(value)),
                                    previous(nullptr), next(nullptr) {}

// description: helper function to find a node at a specific index
// return: Node*
// precondition: index is strictly less than elementCount
// postcondition: returns a pointer to the node at the requested index
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::findNode(size_t index) const {
    Node* current = nullptr;

    if (elementCount / 2 > index) {
        //from head
        size_t count = 0;
        current = head;
        while (count != index) {
            current = current->next;
            ++count;
        }
    }
    else if (elementCount / 2 <= index) {
        //from tail
        size_t count = elementCount - 1;
        current = tail;
        while (count != index) {
            current = current->previous;
            --count;
        }
    }

    return current;
}

// description: default constructor
// return: none
// precondition: none
// postcondition: list is initialized with zero elements and null pointers
template <typename T>
LinkedList<T>::LinkedList() noexcept : head(nullptr), tail(nullptr),
                            elementCount(0) {}

// description: deep copy constructor
// return: none
// precondition: other is a valid LinkedList
// postcondition: list is initialized as an independent deep copy of other
template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : head(nullptr),
                    tail(nullptr), elementCount(0) {
    Node* traverseP = other.head;
    while (traverseP != nullptr) {
        //push_back logic
        pushBack(traverseP->value);
        traverseP = traverseP->next;
    }
}

// description: move constructor
// return: none
// precondition: other is a valid LinkedList
// postcondition: list takes ownership of other's data; other is left empty
template <typename T>
LinkedList<T>::LinkedList(LinkedList&& other) noexcept : head(other.head),
                    tail(other.tail), elementCount(other.elementCount) {
    other.head = nullptr;
    other.tail = nullptr;
    other.elementCount = 0;
}

// description: destructor
// return: none
// precondition: none
// postcondition: all dynamically allocated memory is freed
template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

// description: deep copy assignment operator
// return: LinkedList<T>&
// precondition: other is a valid LinkedList
// postcondition: current list is replaced by a deep copy of other
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this != &other) {
        //using copy constructor copy other list
        LinkedList temp(other);
        //swap copy of the other list and current list
        swap(temp);
    }
    //temp goes out of scope and discarded
    //return the list
    return *this;
}

// description: move assignment operator
// return: LinkedList<T>&
// precondition: other is a valid LinkedList
// postcondition: current list takes ownership of other's data;
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept {
    if (this != &other) {
        //same logic as =operator but temporary list is skipped since we
        //don't need other list anymore, once std::move is called in main
        //other list will be destroyed
        swap(other);
    }
    return *this;
}

// description: swaps the contents of two lists
// return: void
// precondition: other is a valid LinkedList
// postcondition: head, tail, and elementCount are swapped with other
template <typename T>
void LinkedList<T>::swap(LinkedList& other) noexcept {
    std::swap(elementCount,other.elementCount);
    std::swap(head,other.head);
    std::swap(tail,other.tail);
}

// description: checks if the list is empty
// return: bool
// precondition: none
// postcondition: returns true if elementCount is 0, false otherwise
template <typename T>
bool LinkedList<T>::empty() const noexcept {
    return (elementCount == 0);
}

// description: retrieves the number of elements in the list
// return: std::size_t
// precondition: none
// postcondition: returns elementCount
template <typename T>
std::size_t LinkedList<T>::size() const noexcept {
    return elementCount;
}

// description: accesses the first element
// return: T&
// precondition: list is not empty
// postcondition: returns a reference to the head node's value
template <typename T>
T& LinkedList<T>::front() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return head->value;
}

// description: accesses the first element (const)
// return: const T&
// precondition: list is not empty
// postcondition: returns a const reference to the head node's value
template <typename T>
const T& LinkedList<T>::front() const {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return head->value;
}

// description: accesses the last element
// return: T&
// precondition: list is not empty
// postcondition: returns a reference to the tail node's value
template <typename T>
T& LinkedList<T>::back() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return tail->value;
}

// description: accesses the last element (const)
// return: const T&
// precondition: list is not empty
// postcondition: returns a const reference to the tail node's value
template <typename T>
const T& LinkedList<T>::back() const {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return tail->value;
}

// description: accesses an element at a specific index
// return: T&
// precondition: index is valid (less than elementCount)
// postcondition: returns a reference to the value at the requested index
// Time complexity  O(min(index, n - index))
template <typename T>
T& LinkedList<T>::at(std::size_t index) {
    if (empty() || index >= elementCount) {
        throw std::out_of_range("List is empty");
    }

    if (index == 0) {
        return head->value;
    }
    else if (index == elementCount - 1) {
        return tail->value;
    }

    Node* current = findNode(index);
    return current->value;
}

// description: accesses an element at a specific index (const)
// return: const T&
// precondition: index is valid (less than elementCount)
// postcondition: returns a const reference to the value at the requested index
template <typename T>
const T& LinkedList<T>::at(std::size_t index) const {
    if (empty() || index >= elementCount) {
        throw std::out_of_range("List is empty");
    }

    if (index == 0) {
        return head->value;
    }
    else if (index == elementCount - 1) {
        return tail->value;
    }

    Node* current = findNode(index);
    return current->value;
}

// description: inserts a copy of a value at the front of the list
// return: void
// precondition: value is a valid T object
// postcondition: value is inserted at the head, elementCount increases by 1
template <typename T>
void LinkedList<T>::pushFront(const T& value) {
    Node* newNode = new Node(value);
    //empty case
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    //populated list
    else {
        head->previous = newNode;
        newNode->next = head;
        head = newNode;
    }
    ++elementCount;
}

// description: moves a value into the front of the list
// return: void
// precondition: value is a movable T object
// postcondition: value is inserted at the head, elementCount increases by 1
template <typename T>
void LinkedList<T>::pushFront(T&& value) {
    Node* newNode = new Node(std::move(value));
    //empty case
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    //populated list
    else {
        head->previous = newNode;
        newNode->next = head;
        head = newNode;
    }
    ++elementCount;
}

// description: inserts a copy of a value at the back of the list
// return: void
// precondition: value is a valid T object
// postcondition: value is inserted at the tail, elementCount increases by 1
template <typename T>
void LinkedList<T>::pushBack(const T& value) {
    Node* newNode = new Node(value);
    //empty case
    if (tail == nullptr) {
        head = newNode;
        tail = newNode;
    }
    //populated list
    else {
        tail->next = newNode;
        newNode->previous = tail;
        tail = newNode;
    }
    ++elementCount;
}

// description: moves a value into the back of the list
// return: void
// precondition: value is a movable T object
// postcondition: value is inserted at the tail, elementCount increases by 1
template <typename T>
void LinkedList<T>::pushBack(T&& value) {
    Node* newNode = new Node(std::move(value));
    //empty case
    if (tail == nullptr) {
        head = newNode;
        tail = newNode;
    }
    //populated list
    else {
        tail->next = newNode;
        newNode->previous = tail;
        tail = newNode;
    }
    ++elementCount;
}

// description: removes the first element
// return: void
// precondition: list is not empty
// postcondition: head node is deleted, elementCount decreases by 1
template <typename T>
void LinkedList<T>::popFront() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    Node* current = head;
    if (head == tail) {
        head = tail = nullptr;
    }
    else {
        head = head->next;
        head->previous = nullptr;
    }
    delete current;
    --elementCount;
}

// description: removes the last element
// return: void
// precondition: list is not empty
// postcondition: tail node is deleted, elementCount decreases by 1
template <typename T>
void LinkedList<T>::popBack() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    Node* current = tail;
    if (head == tail) {
        head = tail = nullptr;
    }
    else {
        tail = tail->previous;
        tail->next = nullptr;
    }
    delete current;
    --elementCount;
}

// description: inserts a copy of a value at a specific index
// return: void
// precondition: index is valid (0 to elementCount)
// postcondition: value is inserted at the index, elementCount increases by 1
template <typename T>
void LinkedList<T>::insert(std::size_t index, const T& value) {
    if (index > elementCount) {
        throw std::out_of_range("Out of bounds insertion attempt");
    }
    if (index == 0) {
        pushFront(value);
    }
    else if (index == elementCount) {
        pushBack(value);
    }
    else {
        Node* newNode = new Node(value);
        Node* current = findNode(index);
        newNode->next = current;
        newNode->previous = current->previous;
        current->previous->next = newNode;
        current->previous = newNode;
        ++elementCount;
    }
}

// description: moves a value into a specific index
// return: void
// precondition: index is valid (0 to elementCount)
// postcondition: value is inserted at the index, elementCount increases by 1
template <typename T>
void LinkedList<T>::insert(std::size_t index, T&& value) {
    if (index > elementCount) {
        throw std::out_of_range("Out of bounds insertion attempt");
    }
    if (index == 0) {
        pushFront(std::move(value));
    }
    else if (index == elementCount) {
        pushBack(std::move(value));
    }
    else {
        Node* newNode = new Node(std::move(value));
        Node* current = findNode(index);
        newNode->next = current;
        newNode->previous = current->previous;
        current->previous->next = newNode;
        current->previous = newNode;
        ++elementCount;
    }
}

// description: removes an element at a specific index
// return: void
// precondition: index is valid (less than elementCount)
// postcondition: node at the index is deleted, elementCount decreases by 1
template <typename T>
void LinkedList<T>::erase(std::size_t index) {
    if (empty() || index >= elementCount) {
        throw std::out_of_range("List is empty");
    }
        if (index == 0) {
            popFront();
        }
        else if (index == elementCount - 1) {
            popBack();
        }
        else {
            Node* current = findNode(index);
            current->previous->next = current->next;
            current->next->previous = current->previous;
            delete current;
            --elementCount;
        }
}

// description: deletes all elements in the list
// return: void
// precondition: none
// postcondition: all nodes are deleted, elementCount is 0, pointers are null
template <typename T>
void LinkedList<T>::clear() noexcept {
    Node* current = head;
    while (current != nullptr) {
        Node* successor = current->next;
        delete current;
        current = successor;
    }
    head = tail = nullptr;
    elementCount = 0;
}

// description: prints the contents of the list to an output stream
// return: void
// precondition: T supports operator<<
// postcondition: list elements are written to the provided output stream
template <typename T>
void LinkedList<T>::print(std::ostream& output) const {
    //Using overloaded << operator with ostream
    output << *this;
}

#endif //PROJ4_LINKEDLIST_H
