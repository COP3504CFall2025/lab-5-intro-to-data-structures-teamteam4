#pragma once
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include "Interfaces.hpp"

// Technically bad, but size_t isn't likely to conflict with any client code.
using std::size_t;

template<typename T>
class ABQ : public QueueInterface<T>{

    size_t capacity_;
    size_t curr_size_;
    T* array_;
    static constexpr size_t scale_factor_ = 2;

public:
    // Constructors + Big 5
    ABQ(): capacity_(1), curr_size_(0), array_(new T[capacity_]) {}

    explicit ABQ(const size_t capacity){
        capacity_ = capacity;
        curr_size_ = 0;
        array_ = new T[capacity_];
    }

    ABQ(const ABQ& other){ //copy constructor
        capacity_ = other.capacity_;
        curr_size_ = other.curr_size_;
        array_ = new T[other.capacity_];
        for (size_t i = 0 ; i < other.curr_size_; i++){
            array_[i] = other.array_[i];
        }
    }

    ABQ& operator=(const ABQ& rhs){ //copy assignment
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

    ABQ(ABQ&& other) noexcept{ //move constructor
        array_ = other.array_;
        capacity_ = other.capacity_;
        curr_size_ = other.curr_size_;

        other.array_ = nullptr;
        other.capacity_ = 0;
        other.curr_size_ = 0;
    }

    ABQ& operator=(ABQ&& rhs) noexcept{ //move assignment
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

    ~ABQ() noexcept { //destructor
        delete[] array_;
        capacity_ = 0;
        curr_size_ = 0;
    }

    // Getters
    [[nodiscard]] size_t getSize() const noexcept override{
        return curr_size_;
    }
    [[nodiscard]] size_t getMaxCapacity() const noexcept{
        return capacity_;
    }
    [[nodiscard]] T* getData() const noexcept{
        return array_;
    }

    // Insertion
    void enqueue(const T& data) override{
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

    // Access
    T peek() const override{
        if (curr_size_ == 0){
            throw std::runtime_error("Nothing in array to peek");
        }
        return array_[0];
    }

    // Deletion
    T dequeue() override{
        if (curr_size_ == 0){
            throw std::runtime_error("Nothing in array to dequeue");
        }
        T temp = array_[0];
        for (size_t i = 0; i < curr_size_ - 1; i++){
            array_[i] = array_[i+1];
        }
        curr_size_--;
        return temp;
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
