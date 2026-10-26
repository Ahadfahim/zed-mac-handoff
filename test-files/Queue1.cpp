// Circular-array queue with CAPACITY slots, indexed 0 .. CAPACITY-1.
//   front_: index of the first (oldest) element
//   back_ : index of the last (newest) element
//
// empty queue:       front_ == -1 and back_ == -1
// one-element queue: front_ == back_ and front_ >= 0
// full queue:        (back_ + 1) % CAPACITY == front_   (all CAPACITY slots used)
//
// size:
//   empty                    -> 0
//   non-wrapped (back_ >= front_) -> back_ - front_ + 1
//   wrapped     (back_ <  front_) -> CAPACITY - (front_ - back_ - 1)

#include "Queue.hpp"

Queue::Queue() : front_(-1), back_(-1) { // empty queue
}

bool Queue::enqueue(int value) {
  if ((back_ + 1) % CAPACITY == front_) { // full
    return false;
  }

  back_ = (back_ +1) % CAPACITY;
  data_[back_] = value;

  if (front_ == -1) { // with the insertion, an empty queue has its first element.
    front_ = 0;
  }
  return true;
}

bool Queue::dequeue() {
  if (empty()) { 
    return false;
  }

  if (front_ == back_ && front_ >= 0) { // if the queue become empty after removal
    front_ = back_ = -1;
  }
  else {
    front_ = (front_ +1) % CAPACITY;
  }

  return true;
}

int Queue::top() const { // precondition, the queue is not empty()
  return data_[front_];
}

bool Queue::empty() const {
  return (front_ == -1 && back_ == -1);
  // (back_ + 1) % CAPACITY == front_ (full)
  
}

int Queue::size() const {
  if (back_ >= front_) { // non-wrapped queue
    if (front_ >= 0) { // non-empty queue
      return back_ - front_ + 1; // non-wrapped queue 
    }
    else { // front_ and back_ are both -1
      return 0;
    }
  }
  else { // (back_ < front_), for example, back_ is 0 and front_ is 2 
    return CAPACITY - (front_ - back_ -1);
  }
}
