#include <iostream> // std::cout
#include "Square.hpp"

Square::Square() : Rectangle(1, 1) {
}

//Square::Square(double side) : side > 0 ? Rectangle(side, side) : Square() { // WRONG

// Correct
Square::Square(double side) : Rectangle(side > 0 ? side : 1, side > 0 ? side : 1) {
}

// WRONG:
// Calling Rectangle(side, side) inside the constructor body
// creates a temporary Rectangle. It does not initialize
// the Rectangle part of the Square object.
//
// Similarly, Square() inside the constructor body creates
// a temporary Square. To call a particular base-class
// constructor, use the initializer list.
// Square::Square(double side) {
//   if (side > 0) {
//     Rectangle(side, side);
//   }
//   else {
//     Square();
//   }
// }

double Square::side() const {
  return length(); // length() is inherited from Rectangle
}

void Square::setSide(double side) {
  if (side > 0) {
    setLength(side);
    setWidth(side);
  }
}

//void Square::print() const override { override is not allowed outside a class definition
void Square::print() const {
  std::cout << "square side: " << side() << '\n';
  std::cout << "area: " << area() << '\n';
  std::cout << "perimeter: " << perimeter() << "\n\n";
}
