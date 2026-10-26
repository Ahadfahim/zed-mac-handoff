// QueueTest.cpp
#include "Queue.hpp"
#include <iostream>
#include <string>
#include <cstdlib> // srand, rand
#include <ctime> // time

int main() {
  Queue que;
  for (int i = 1; i <= 6; i++) { // CAPACITY is 4 in Queue
    std::cout << "Insert " << i << " to the queue.\n";
    bool success = que.enqueue(i);
    if (success) {
      std::cout << "  After insertion, current size of queue: " << que.size() << '\n';
    }
    else {
      std::cout << "  The queue is full, cannot insert.\n";
    }
  }

  std::cout << "\n";
  while (!que.empty()) {
    int value = que.top();
    que.dequeue();
    std::cout << "Remove top element " << value << " from queue\n"; 
    std::cout << "  current size of queue: " << que.size() << '\n';
  }

  std::cout << "\n";
  for (int i = 7; i <= 9; i++) { // CAPACITY is 5 in Queue
    std::cout << "Insert " << i << " to the queue.\n";
    que.enqueue(i);
    std::cout << "  After insertion, current size of queue: " << que.size() << '\n'; 
  }

  // Remove a random number in [0, que.size()] elements from the queue.
  srand(time(NULL));
  int numElmsToRemove = rand() % (que.size() + 1); // rand() % (que.size() +1) generates a random integer in [0, que.size()]. If you do not see a desired number in a few random tries, set numElmsToRemove to be that desired value, say 2.
  std::cout << "\nRemove " << numElmsToRemove << " elements from the queue\n";
  int j = 0;
  while (!que.empty() && j < numElmsToRemove) {
    int value = que.top();
    que.dequeue();
    std::cout << "Remove top element " << value << " from queue\n"; 
    std::cout << "  After removal, current size of queue: " << que.size() << '\n';
    j++;
  }

  std::cout << "\n";
  for (int i = 10; i <= 15; i++) { // CAPACITY is 5 in Queue
    std::cout << "Insert " << i << " to the queue.\n";
    bool success = que.enqueue(i);
    if (success) {
      std::cout << "  After insertion, current size of queue: " << que.size() << '\n'; 
    }
    else {
      std::cout << "  The queue is full, cannot insert.\n";
    }
  }

  std::cout << "\n";
  while (!que.empty()) {
    int value = que.top();
    que.dequeue();
    std::cout << "Remove top element " << value << " from queue\n"; 
    std::cout << "  After removal, current size of queue: " << que.size() << '\n';
  }

  return 0;
}
