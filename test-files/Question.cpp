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
