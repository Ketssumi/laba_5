#ifndef QUEUE_H
#define QUEUE_H

#include <memory_resource>
#include <memory>
#include <iterator>
#include <stdexcept>

template<typename T>
class Queue {
private:
    struct Node {
        T value;
        Node* next;
        
        Node(const T& val) : value(val), next(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    size_t sz;
    std::pmr::polymorphic_allocator<Node> alloc;
    
    Node* allocate_node(const T& value) {
        Node* node = alloc.allocate(1);
        new (node) Node(value);
        return node;
    }
    
    void deallocate_node(Node* node) {
        if (node != nullptr) {
            node->~Node();
            alloc.deallocate(node, 1);
        }
    }
    
public:
    // Iterator
    class Iterator {
    private:
        Node* current;
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::forward_iterator_tag;
        
        Iterator(Node* node = nullptr) : current(node) {}
        
        reference operator*() const { return current->value; }
        pointer operator->() const { return &(current->value); }
        
        Iterator& operator++() {
            if (current != nullptr) current = current->next;
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };
    
    explicit Queue(std::pmr::memory_resource* resource = nullptr)
        : head(nullptr), tail(nullptr), sz(0), alloc(resource) {}
    
    ~Queue() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            deallocate_node(temp);
        }
    }
    
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
    
    void push(const T& value) {
        Node* new_node = allocate_node(value);
        if (tail != nullptr) {
            tail->next = new_node;
        } else {
            head = new_node;
        }
        tail = new_node;
        sz++;
    }
    
    T pop() {
        if (head == nullptr) {
            throw std::underflow_error("Queue is empty");
        }
        T value = head->value;
        Node* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        deallocate_node(temp);
        sz--;
        return value;
    }
    
    const T& front() const {
        if (head == nullptr) {
            throw std::underflow_error("Queue is empty");
        }
        return head->value;
    }
    
    bool empty() const {
        return head == nullptr;
    }
    
    size_t size() const {
        return sz;
    }
    
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

#endif
