#ifndef RECTANGLE_HPP // Do not write ifndef as ifdef, otherwise, Rectangle class is not recognized.
#define RECTANGLE_HPP
class Rectangle {
  public: 
    Rectangle(); // Set length and width to be 2 and 1, respectively.
    Rectangle(double length, double width); // parameterized constructor
    Rectangle(const Rectangle& rect);
    // Overload the assignment operator.
    Rectangle& operator=(const Rectangle& rect);
    double length() const;
    double width() const;
    double area() const;
    double perimeter() const;
    void setLength(double length);
    void setWidth(double width);
    void print() const;

  private:
    double length_;
    double width_;
};
#endif
