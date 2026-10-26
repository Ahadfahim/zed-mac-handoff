#include <iostream> // std::cout
#include "ChoiceQuestion.hpp"

ChoiceQuestion::ChoiceQuestion() : Question() {
}

ChoiceQuestion::ChoiceQuestion(const std::string& text) : Question(text, "") {
}

void ChoiceQuestion::addChoice(const std::string& option, bool isAnswer) {
  options.push_back(option);
  if (isAnswer) {
    setAnswer(std::to_string(options.size()));
  }
}

void ChoiceQuestion::print() const { // override is not allowed outside a class definition
  Question::print(); // Call superclass print() to print the question text. 

  // options.size() returns size_t, an unsigned integer type.
  for (size_t i = 0; i < options.size(); i++) {
    std::cout << i+1 << ". " << options[i] << '\n';
  }
}
