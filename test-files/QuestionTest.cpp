//File name: /Users/laptopuser/Documents/courses_macbook_pro/cs235_f26/class_day_f26/day1_classOverView/code/question/QuestionTest.cpp
//name: Tong Yi
//email: ty680@hunter.cuny.edu
#include <iostream>
#include <string>
#include "Question.hpp"

void testQuestion(const Question& ques);

int main() {
  Question usaFounded("When was the USA founded?", "1776");
  testQuestion(usaFounded);

  Question onePlusTwo("What is 1 + 2?", "3");
  testQuestion(onePlusTwo);

  Question usaCapital("What is the capital city of the USA?", "Washington DC");
  testQuestion(usaCapital);
  return 0;
}


void testQuestion(const Question& ques) {
  std::cout << ques.text() << '\n';
  std::cout << "Enter an answer: ";
  std::string reply;
  // std::cin >> reply;
  std::getline(std::cin, reply); // reply might be more than one word
  std::cout << std::boolalpha << ques.checkAnswer(reply) << "\n\n";


}
