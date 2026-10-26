#include "Queue.hpp"

Queue::Queue() : front_(-1), size_(0) {
}

bool Queue::enqueue(int value) {
  if (size_ == CAPACITY) {
    return false;
  }

  // size_ < CAPACITY
  // front_ is the index of the first element.
  if (front_ == -1) { // empty queue before the insertion
    front_ = 0;
  }

  // The next insertion index is size_ positions after front_,
  // wrapping around when necessary.
  int insertIdx = (front_ + size_) % CAPACITY;
  data_[insertIdx] = value;

  size_++;
  return true;
}

bool Queue::dequeue() {
  if (size_ == 0) {
    return false;
  }

  size_--;
  if (size_ == 0) { // queue becomes empty after removal
    front_ = -1;
  }
  else {
    front_ = (front_ +1) % CAPACITY; 
  }
  return true;
}

int Queue::top() const { // precondition !empty();
  return data_[front_];
}

int Queue::size() const {
  return size_;
}

bool Queue::empty() const {
  return size_ == 0;
}
