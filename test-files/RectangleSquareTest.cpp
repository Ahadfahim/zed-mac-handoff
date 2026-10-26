//File name: /Users/laptopuser/Documents/courses_macbook_pro/cs235_f26/class_day_f26/day1_classOverView/code/RectangleSquareTest.cpp
#include <iostream>
#include "Rectangle.hpp"
#include "Square.hpp"

void print(Rectangle* rectPtr);

int main() {
  Rectangle rect(6, 8);
  Square sq(7);

  Square sq2 = sq;
  sq2.setSide(6);

  const int SIZE = 3;
  Rectangle shapes[SIZE] = {rect, sq, sq2};

//  std::cout << "side of square: " << sq.side() << '\n';

  // This does not preserve polymorphism.
  // sq and sq2 are sliced when copied into Rectangle objects.
  for (int i = 0; i < SIZE; i++) {
    print(&shapes[i]);
  }

  Rectangle* shapesPtr[SIZE] = {&rect, &sq, &sq2};

  for (int i = 0; i < SIZE; i++) {
    print(shapesPtr[i]);
  }

  return 0;
}

// Pass by pointer
void print(Rectangle* rectPtr) {
  rectPtr->print();
}
