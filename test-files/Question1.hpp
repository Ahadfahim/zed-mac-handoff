//File name: /Users/laptopuser/Documents/courses_macbook_pro/cs235_f26/class_day_f26/day1_classOverView/code/question/Question.hpp
#ifndef QUESTION_HPP
#define QUESTION_HPP
#include <string>
class Question {
  public:
    Question();
    Question(const std::string& text, const std::string& answer);
    std::string text() const;
    bool checkAnswer(const std::string& reply) const;
    // add in v2
    void setText(const std::string& text);
    void setAnswer(const std::string& answer);
    
    virtual void print() const;
    // Keyword virtual is needed for runtime polymorphism.
    // Put virtual before the return type of print method
    // means print method might behave differently in a derived class
    // than when it is in base class.
    // With virtual keyword, derived print() is called through Question& (pass by reference) or Question* (pass by pointer).

  private:
    std::string text_;
    std::string answer_;
};
#endif
