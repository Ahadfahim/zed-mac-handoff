#include <iostream> // std::cout
#include "Question.hpp"

Question::Question() : text_(""), answer_("") {
}

Question::Question(const std::string& text, const std::string& answer) : text_(text), answer_(answer) {
}

std::string Question::text() const {
  return text_; 
}

bool Question::checkAnswer(const std::string& reply) const {
  return answer_ == reply;
}

void Question::setText(const std::string& text) {
  text_ = text;
}

void Question::setAnswer(const std::string& answer) {
  answer_ = answer;
}

void Question::print() const {
  std::cout << text_ << '\n';
}
