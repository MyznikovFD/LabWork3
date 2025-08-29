/**
 * @file cyclic_list.h
 * @brief STL-style cyclic list container implementation
 */

#pragma once

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <stdexcept>

/**
 * @brief Cyclic doubly-linked list container
 */
template <typename T>
class CyclicList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;

        Node(const T& value = T{}, Node* n = nullptr, Node* p = nullptr)
            : data(value), next(n), prev(p) {}
    };

    Node* sentinel;
    size_t size_;

public:
    class iterator;        // Functionality of the classes will
    class const_iterator;  //be used earler than it described

    /**
     * @brief Default constructor
     */
    CyclicList() : size_(0) {
        sentinel = new Node();
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    }

    /**
     * @brief Initializer list constructor
     * @param init List of values to initialize the container
     */
    CyclicList(std::initializer_list<T> init) : CyclicList() {
        for (const auto& item : init) {
            push_back(item);
        }
    }

    /**
     * @brief Copy constructor
     * @param other List to copy from
     */
    CyclicList(const CyclicList& other) : CyclicList() {
        for (const auto& item : other) {
            push_back(item);
        }
    }

    /**
     * @brief Move constructor
     * @param other List to move from
     */
    CyclicList(CyclicList&& other) noexcept 
        : sentinel(other.sentinel), size_(other.size_) {
        other.sentinel = nullptr;
        other.size_ = 0;
    }

    /**
     * @brief Destructor
     */
    ~CyclicList() {
        clear();
        delete sentinel;
    }

    /**
     * @brief Copy assignment operator
     * @param other List to copy from
     * @return Reference to this list
     */
    CyclicList& operator=(const CyclicList& other) {
    if (this != &other) {
        CyclicList temp(other);
        swap(temp);
    }
    return *this;
}

    /**
     * @brief Move assignment operator
     * @param other List to move from
     * @return Reference to this list
     */
    CyclicList& operator=(CyclicList&& other) noexcept {
        if (this != &other) {
            clear();
            delete sentinel;
            
            sentinel = other.sentinel;
            size_ = other.size_;
            
            other.sentinel = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    /**
     * @brief Swap contents with another list
     * @param other List to swap with
     */
    void swap(CyclicList& other) noexcept {
        std::swap(sentinel, other.sentinel);
        std::swap(size_, other.size_);
    }

    /**
     * @brief Check if the list is empty
     * @return true if empty, false otherwise
     */
    bool empty() const noexcept { return size_ == 0; }

    /**
     * @brief Get the number of elements
     * @return Number of elements in the list
     */
    size_t size() const noexcept { return size_; }

    /**
     * @brief Add element to the end of the list
     * @param value Value to add
     */
    void push_back(const T& value) {
        insert(end(), value);
    }

    /**
     * @brief Add element to the beginning of the list
     * @param value Value to add
     */
    void push_front(const T& value) {
        insert(begin(), value);
    }

    /**
     * @brief Remove the last element
     * @throw std::out_of_range if the list is empty
     */
    void pop_back() {
        erase(--end());
    }

    /**
     * @brief Remove the first element
     * @throw std::out_of_range if the list is empty
     */
    void pop_front() {
        erase(begin());
    }

    /**
     * @brief Insert element before a position
     * @param pos Iterator before which to insert the element
     * @param value Value to insert
     * @return Iterator to the inserted element
     */
    iterator insert(iterator pos, const T& value) {
        Node* newNode = new Node(value, pos.node, pos.node->prev);
        pos.node->prev->next = newNode;
        pos.node->prev = newNode;
        ++size_;
        return iterator(newNode);
    }

    /**
     * @brief Remove element at a position
     * @param pos Iterator to the element to remove
     * @return Iterator to the element following the removed one
     * @throw std::out_of_range if the list is empty
     */
    iterator erase(iterator pos) {
        if (empty()) throw std::out_of_range("List is empty");
        Node* nextNode = pos.node->next;
        pos.node->prev->next = nextNode;
        nextNode->prev = pos.node->prev;
        delete pos.node;
        --size_;
        return iterator(nextNode);
    }

    /**
     * @brief Remove all elements from the list
     */
    void clear() noexcept {
        while (!empty()) {
            pop_front();
        }
    }

    /**
     * @brief Access the first element
     * @return Reference to the first element
     */
    T& front() { return sentinel->next->data; }

    /**
     * @brief Access the first element (const)
     * @return Const reference to the first element
     */
    const T& front() const { return sentinel->next->data; }

    /**
     * @brief Access the last element
     * @return Reference to the last element
     */
    T& back() { return sentinel->prev->data; }

    /**
     * @brief Access the last element (const)
     * @return Const reference to the last element
     */
    const T& back() const { return sentinel->prev->data; }

    /**
     * @brief Get iterator to the beginning
     * @return Iterator to the first element
     */
    iterator begin() noexcept { return iterator(sentinel->next); }

    /**
     * @brief Get const iterator to the beginning
     * @return Const iterator to the first element
     */
    const_iterator begin() const noexcept { return const_iterator(sentinel->next); }

    /**
     * @brief Get const iterator to the beginning
     * @return Const iterator to the first element
     */
    const_iterator cbegin() const noexcept { return begin(); }

    /**
     * @brief Get iterator to the end
     * @return Iterator to the element following the last element
     */
    iterator end() noexcept { return iterator(sentinel); }

    /**
     * @brief Get const iterator to the end
     * @return Const iterator to the element following the last element
     */
    const_iterator end() const noexcept { return const_iterator(sentinel); }

    /**
     * @brief Get const iterator to the end
     * @return Const iterator to the element following the last element
     */
    const_iterator cend() const noexcept { return end(); }

    /**
     * @brief Bidirectional iterator for CyclicList
     */
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(Node* n = nullptr) : node(n) {}

        reference operator*() const { return node->data; }
        pointer operator->() const { return &node->data; }

        iterator& operator++() {
            node = node->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            node = node->next;
            return tmp;
        }

        iterator& operator--() {
            node = node->prev;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            node = node->prev;
            return tmp;
        }

        bool operator==(const iterator& other) const { return node == other.node; }
        bool operator!=(const iterator& other) const { return node != other.node; }

    private:
        Node* node;
        friend class CyclicList;
    };

    /**
     * @brief Const bidirectional iterator for CyclicList
     */
    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator(Node* n = nullptr) : node(n) {}
        const_iterator(const iterator& it) : node(it.node) {}

        reference operator*() const { return node->data; }
        pointer operator->() const { return &node->data; }

        const_iterator& operator++() {
            node = node->next;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            node = node->next;
            return tmp;
        }

        const_iterator& operator--() {
            node = node->prev;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            node = node->prev;
            return tmp;
        }

        bool operator==(const const_iterator& other) const { return node == other.node; }
        bool operator!=(const const_iterator& other) const { return node != other.node; }

    private:
        Node* node;
        friend class CyclicList;
    };
};

/**
 * @brief Swap contents of two CyclicList containers
 * @tparam T Type of elements in the lists
 * @param lhs First list
 * @param rhs Second list
 */
template <typename T>
void swap(CyclicList<T>& lhs, CyclicList<T>& rhs) noexcept {
    lhs.swap(rhs);
}
