#pragma once

#include <cstddef>
#include <stdexcept>
#include "Interfaces.hpp"
#include <utility>
#include <iostream>

template <typename T>
class ABDQ : public DequeInterface<T> {
private:
    std::size_t capacity_;    // total allocated capacity
    std::size_t size_;        // number of stored elements
    std::size_t front_;       // index of front element
    std::size_t back_;        // index after the last element (circular)
    T* data_;                 // underlying dynamic array

    static constexpr std::size_t SCALE_FACTOR = 2;

public:
    // Big 5
    
    ABDQ() : capacity_(4), size_(0), front_(0), back_(0), data_(new T[capacity_]) {}

    explicit ABDQ(std::size_t capacity){
        capacity_ = capacity;
        size_ = 0;
        front_ = 0;
        back_ = 0;
        data_ = new T[capacity_];
    }

    ABDQ(const ABDQ& other){ //copy constructor
        capacity_ = other.capacity_;
        size_ = other.size_;
        front_ = 0;
        back_ = size_;
        data_ = new T[capacity_];
        for (std::size_t i = 0 ; i < other.size_; i++){
            data_[i] = other.data_[(other.front_ + i) % other.capacity_];
        }
    }

    ABDQ(ABDQ&& other) noexcept{ //move constructor
        capacity_ = other.capacity_;
        size_ = other.size_;
        front_ = other.front_;
        back_ = other.back_;
        data_ = other.data_;

        other.capacity_ = 0;
        other.size_ = 0;
        other.front_ = 0;
        other.back_ = 0;
        other.data_ = nullptr;
        
    }

    ABDQ& operator=(const ABDQ& other){ //copy assignment
		if (this == &other){
			return *this;
		}

        delete[] data_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        front_ = 0;
        back_ = size_;
        data_ = new T[capacity_];
        for (std::size_t i = 0 ; i < other.size_; i++){
            data_[i] = other.data_[(other.front_ + i) % other.capacity_];
        }
        return *this;
    }

    ABDQ& operator=(ABDQ&& other) noexcept{ //move assignment
		if (this == &other){
			return *this;
		}

        delete[] data_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        front_ = other.front_;
        back_ = other.back_;
        data_ = other.data_;

        other.capacity_ = 0;
        other.size_ = 0;
        other.front_ = 0;
        other.back_ = 0;
        other.data_ = nullptr;
        return *this;
    }

    ~ABDQ() {
        delete[] data_;
    }

    // Insertion
    void pushFront(const T& item) override{
        ensureCapacity();
        front_ = (front_ - 1 + capacity_) % capacity_;
        data_[front_] = item;
        size_++;
    }
    void pushBack(const T& item) override{
        ensureCapacity();
        data_[back_] = item;
        back_ = (back_ + 1) % capacity_;
        size_++;
    }

    void ensureCapacity(){
        if (size_ == capacity_){
            std::size_t oldCapacity_ = capacity_;
            capacity_ = (capacity_ == 0) ? 1 : capacity_ *= SCALE_FACTOR;
            T* tempArray = new T[capacity_];
            for (std::size_t i = 0 ; i < size_; i++){
                tempArray[i] = data_[(front_ + i) % oldCapacity_];
            }
            delete[] data_;
            data_ = tempArray;
            front_ = 0;
            back_ = size_;
        }
    }

    void shrinkIfNeeded(){
        if (size_ <= capacity_/4){
            std::size_t oldCapacity = capacity_;
            capacity_ /= 2;
            T* tempArray = new T[capacity_];
            for (std::size_t i = 0 ; i < size_; i++){
                tempArray[i] = data_[(front_ + i) % oldCapacity];
            }
            delete[] data_;
            data_ = tempArray;
            front_ = 0;
            back_ = size_;
        }
    }

    // Deletion
    T popFront() override{
        if (size_ == 0) {
            throw std::runtime_error("Cannot pop an empty deque");
        }
        T temp = data_[front_];
        front_ = (front_ + 1) % capacity_;
        size_--;
        shrinkIfNeeded();
        return temp;
    }

    T popBack() override{
        if (size_ == 0) {
            throw std::runtime_error("Cannot pop an empty deque");
        }
        back_ = (back_ - 1 + capacity_) % capacity_;
        T temp = data_[back_];
        size_--;
        shrinkIfNeeded();
        return temp;
    }

    
    // Access
    const T& front() const override{
        if (size_ == 0) {
            throw std::runtime_error("Cannot access an empty deque");
        }
        return data_[front_];
    }

    const T& back() const override{
        if (size_ == 0) {
            throw std::runtime_error("Cannot access an empty deque");
        }
        return data_[(back_ - 1 + capacity_) % capacity_];
    }

    // Getters
    std::size_t getSize() const noexcept override{
        return size_;
    }

    void PrintForward() const{
        for (std::size_t i = 0; i < size_; i++){
            std::cout << data_[(front_ + i) % capacity_] << std::endl;
        }
    }

    void PrintReverse() const{
        for (int i = 0; i < size_; i++){
            std::cout << data_[(back_ - 1 - i + capacity_) % capacity_] << std::endl;
        }
    }
};
