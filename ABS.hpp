#pragma once

#include <cstddef>
#include <stdexcept>
#include "Interfaces.hpp"
#include <iostream>

// Technically bad, but size_t isn't likely to conflict with any client code.
using std::size_t;

template<typename T>
class ABS : public StackInterface<T> {
public:
    // Big 5 + Parameterized Constructor
    ABS() : capacity_(1), curr_size_(0), array_(new T[capacity_]) {}

    explicit ABS(const size_t capacity){
        capacity_ = capacity;
        curr_size_ = 0;
        array_ = new T[capacity_];
    }
    ABS(const ABS& other){ //copy constructor
        capacity_ = other.capacity_;
        curr_size_ = other.curr_size_;
        array_ = new T[other.capacity_];
        for (size_t i = 0 ; i < other.curr_size_; i++){
            array_[i] = other.array_[i];
        }
    }

    ABS& operator=(const ABS& rhs){ //copy assignment
		if (this == &rhs){
			return *this;
		}
        delete[] array_;
        capacity_ = rhs.capacity_;
        curr_size_ = rhs.curr_size_;
        array_ = new T[capacity_];
        for (size_t i = 0 ; i < rhs.curr_size_; i++){
            array_[i] = rhs.array_[i];
        }
        return *this;
    }

    ABS(ABS&& other) noexcept{ //move constructor
        array_ = other.array_;
        capacity_ = other.capacity_;
        curr_size_ = other.curr_size_;

        other.array_ = nullptr;
        other.capacity_ = 0;
        other.curr_size_ = 0;
    }

    ABS& operator=(ABS&& rhs) noexcept{ //move assignment
		if (this == &rhs){
			return *this;
		}
        delete[] array_;
        array_ = rhs.array_;
        capacity_ = rhs.capacity_;
        curr_size_ = rhs.curr_size_;

        rhs.array_ = nullptr;
        rhs.capacity_ = 0;
        rhs.curr_size_ = 0;

        return *this;
    }

    ~ABS() noexcept { //destructor
        delete[] array_;
        capacity_ = 0;
        curr_size_ = 0;
    }

    // Get the number of items in the ABS
    [[nodiscard]] size_t getSize() const noexcept override{
        return curr_size_;
    }

    // Get the max size of the ABS
    [[nodiscard]] size_t getMaxCapacity() const noexcept{
        return capacity_;
    }

    // Return underlying data for the stack
    [[nodiscard]] T* getData() const noexcept{
        return array_;
    }

    // Push item onto the stack
    void push(const T& data) override{
        if (curr_size_ == capacity_){
            size_t newCapacity = capacity_ * scale_factor_;
            T* newArray = new T[newCapacity];
            for (size_t i = 0; i < curr_size_; i ++){
                newArray[i] = array_[i];
            }
            delete[] array_;
            array_ = newArray;
            capacity_ = newCapacity;
        }
        array_[curr_size_] = data;
        curr_size_++;
    }

    T peek() const override{
        if (curr_size_ == 0){
            throw std::runtime_error("Nothing in array to peek");
        }
        return array_[curr_size_ - 1];
    }

    T pop() override{
        if (curr_size_ == 0){
            throw std::runtime_error("Nothing in array to pop");
        }
        T value = array_[curr_size_];
        curr_size_--;
        if (curr_size_ <= capacity_/4){
            std::size_t oldCapacity = capacity_;
            capacity_ /= 2;
            T* tempArray = new T[capacity_];
            for (std::size_t i = 0 ; i < curr_size_; i++){
                tempArray[i] = array_[i];
            }
            delete[] array_;
            array_ = tempArray;
        }
        return value;
    }

    void PrintForward() const{
        for (size_t i = 0; i < curr_size_; i++){
            std::cout << array_[i] << std::endl;
        }
    }

    void PrintReverse(){
        for (int i = curr_size_ - 1; i > 0; i--){
            std::cout << array_[i] << std::endl;
        }
    }

private:
    size_t capacity_;
    size_t curr_size_;
    T* array_;
    static constexpr size_t scale_factor_ = 2;
};
