#pragma once
#include <iostream>
using namespace std;



template <typename T>
class LinkedList {
public:

	struct Node {
		T data;
		Node* prev;
		Node* next;
	};

	// Behaviors
	void printForward() const {
		Node* current = head;
		while (current != nullptr){
			std::cout << current->data << " ";
			current = current->next;
		}
	}
	void printReverse() const {
		Node* current = tail;
		while (current != nullptr){
			std::cout << current->data << " ";
			current = current->prev;
		}
	}

	// Accessors
	[[nodiscard]] unsigned int getCount() const{
		return count;
	}

	Node* getHead(){
		return head;
	}
	const Node* getHead() const{
		return head;
	}
	Node* getTail(){
		return tail;
	}
	const Node* getTail() const{
		return tail;
	}

	// Insertion
	void addHead(const T& data){
		Node* newHead = new Node{data, nullptr, head};
		if (head != nullptr) {head->prev = newHead;}
		else {tail = newHead;}
		head = newHead;
		count++;
	}
	void addTail(const T& data){
		Node* newTail = new Node{data, tail, nullptr};
		if (tail != nullptr) {tail->next = newTail;}
		else {head = newTail;}
		tail = newTail;
		count++;
	}

	// Removal
	bool removeHead(){
		if (head == nullptr) {return false;}
		Node* old = head;
		head = head->next;
		delete old;
		count--;
		if (head == nullptr){
			tail = nullptr;
		}
		else {
			head->prev = nullptr;
		}
		return true;
	}

	bool removeTail(){
		if (tail == nullptr) {return false;}
		Node* old = tail;
		tail = tail->prev;
		delete old;
		count--;
		if (tail == nullptr) {head = nullptr;}
		else {tail->next = nullptr;}
		return true;
	}

	void clear(){
		while (head != nullptr){
			removeHead();
		}
	}

	// Operators
	LinkedList<T>& operator=(LinkedList<T>&& other) noexcept{ //move assignment 
		if (this == &other){
			return *this;
		}
		clear();
		head = other.head;
		tail = other.tail;
		count = other.count;
		
		other.head = nullptr;
		other.tail = nullptr;
		other.count = 0;

		return *this;
	}

	LinkedList<T>& operator=(const LinkedList<T>& rhs){ //copy assignment 
		if (this == &rhs){
			return *this;
		}
		clear();
		Node* current = rhs.head;
		while(current != nullptr){
			addTail(current->data);
			current = current->next;
		}
		return *this;
	}

	// Construction/Destruction
	LinkedList() : head(nullptr), tail(nullptr), count(0) {} //default constructor

	LinkedList(const LinkedList<T>& list){ //copy constructor
		Node* current = list.head;
		while(current != nullptr){
			addTail(current->data);
			current = current->next;
		}
	}

	LinkedList(LinkedList<T>&& other) noexcept{ //move constructor
		head = other.head;
		tail = other.tail;
		count = other.count;

		other.head = nullptr;
		other.tail = nullptr;
		other.count = 0;
	}

	~LinkedList(){
		clear();
	}

private:
	// Stores pointers to first and last nodes and count
	Node* head = nullptr;
	Node* tail = nullptr;
	unsigned int count = 0;
};


