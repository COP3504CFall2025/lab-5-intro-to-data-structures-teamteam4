#pragma once

#include <cstddef>
#include <stdexcept>
#include "Interfaces.hpp"
#include <utility>

template <typename T>
class ABDQ : public DequeInterface<T> {
private:
    T* data_;                 // underlying dynamic array
    std::size_t capacity_;    // total allocated capacity
    std::size_t size_;        // number of stored elements
    std::size_t front_;       // index of front element
    std::size_t back_;        // index after the last element (circular)

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
        front_ = other.front_;
        back_ = other.back_;
        data_ = new T[capacity_];
        for (size_t i = 0 ; i < other.size_; i++){
            data_[i] = other.data_[i];
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
        front_ = other.front_;
        back_ = other.back_;
        data_ = new T[capacity_];
        for (size_t i = 0 ; i < other.size_; i++){
            data_[i] = other.data_[i];
        }
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
    }

    ~ABDQ() override{
        delete[] data_;
    }

    // Insertion
    void pushFront(const T& item) override{
        front_--;
        if (front_ == 0){
            front_ = capacity_;
        }
        data_[front_] = item;
        size++;
    }
    void pushBack(const T& item) override{
        data_[back_] = item;
        back_++;
        if (back_ > capacity_){
            back_ = 0;
        }
        size_--;
    }

    void ensureCapacity(){
        if (size_ == capacity_){
            capacity_ *= 2;
            T* tempArray = new T[capacity_];
            for (size_t i = 0; i < size_; i++){
                tempArray[i] = data_[i];
            }
            delete[] data_;
            data_ = tempArray;
        }
    }

    void shrinkIfNeeded(){
        if (size <= capacity_/2){
            capacity_ /= 2;
            T* tempArray = new T[capacity_];
            for (size_t i = 0; i < size_; i++){
                tempArray[i] = data_[i];
            }
            delete[] data_;
            data_ = tempArray;
        }
    }

    // Deletion
    T popFront() override{
        T temp = data_(front_);
        front_++;
        size_--;
        return temp;
    }

    T popBack() override{
        T temp = data_(back_);
        back_--;
        size_--;
        return temp;
    }

    
    // Access
    const T& front() const override{
        return data_->front_;
    }

    const T& back() const override{
        return data_->back_;
    }

    // Getters
    std::size_t getSize() const noexcept override{
        return size_;
    }

    void PrintForward() const{
        for (size_t i = 0; i < curr_size_; i++){
            std::cout << array_[i] << std::endl;
        }
    }

    void PrintReverse() const{
        for (int i = curr_size_ - 1; i >= 0; i--){
            std::cout << array_[i] << std::endl;
        }
    }
};
