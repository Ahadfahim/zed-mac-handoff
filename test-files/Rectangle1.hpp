#ifndef RECTANGLE_HPP 
#define RECTANGLE_HPP
class Rectangle {
  public: 
    Rectangle(); // Set length and width to be 2 and 1, respectively.
    Rectangle(double length, double width);
    Rectangle(const Rectangle& rect);
    // Overload the assignment operator.
    Rectangle& operator=(const Rectangle& rect);
    double length() const;
    double width() const;
    double area() const;
    double perimeter() const;
    void setLength(double length);
    void setWidth(double width);

    // virtual before the return type of a method 
    // in a base class means a derived class 
    // may provide its own version of the corresponding method
    virtual void print() const;

  private:
    double length_;
    double width_;
};
#endif
