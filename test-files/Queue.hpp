// Queue.hpp
#ifndef QUEUE_HPP
#define QUEUE_HPP
class Queue {
  public:
    Queue();
    bool enqueue(int value);
    bool dequeue();
    int top() const; // precondition !empty();
    int size() const;
    bool empty() const;

  private:
    const static int CAPACITY = 4; // cannot omit keyword static
    int data_[CAPACITY];
    int front_;
    int size_;
};
#endif
