// Queue.hpp
#ifndef QUEUE_HPP
#define QUEUE_HPP
class Queue {
  public:
    Queue(); // empty queue
    bool enqueue(int value);
    bool dequeue();
    int top() const; // precondition, the queue is not empty()
    bool empty() const;
    int size() const;

  private:
    const static int CAPACITY = 4; // cannot omit keyword static
    int data_[CAPACITY]; // circular array
    int front_;
    int back_;
};
#endif
