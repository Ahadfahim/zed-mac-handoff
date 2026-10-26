//File name: /Users/laptopuser/Documents/courses_macbook_pro/cs235_f26/class_day_f26/day1_classOverView/code/question/v2_ChoiceQuestion/ChoiceQuestion.hpp
#ifndef CHOICE_QUESTION_HPP
#define CHOICE_QUESTION_HPP
#include "Question.hpp"
#include <vector>
class ChoiceQuestion : public Question {
  public:
    ChoiceQuestion(); 
    ChoiceQuestion(const std::string& text); 
    void addChoice(const std::string& option, bool isAnswer);
    void print() const override; 
    // (1) When a method inherited from a base class,
    //     behaves differently in the current derived class,
    //     it is recommended to add keyword override in the end of the method, 
    //     though override can be omitted.
    // (2) override lets the compiler verify that
    //     this overrides a virtual method from base class.

  private:
    std::vector<std::string> options;
};
#endif
