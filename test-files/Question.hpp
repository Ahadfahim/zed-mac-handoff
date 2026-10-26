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

  private:
    std::string text_;
    std::string answer_;
};
#endif
