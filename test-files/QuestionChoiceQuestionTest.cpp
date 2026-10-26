//File name: /Users/laptopuser/Documents/courses_macbook_pro/cs235_f26/class_day_f26/day1_classOverView/code/question/v2_ChoiceQuestion/QuestionChoiceQuestionTest.cpp
//name: Tong Yi
//email: ty680@hunter.cuny.edu
#include <iostream>
#include <string>
#include "Question.hpp"
#include "ChoiceQuestion.hpp"

void testQuestion(Question ques);
void testQuestionByReference(const Question& ques);
void testQuestionByPointer(Question* quesPtr);

int main() {
  Question usaFounded("When was the USA founded?", "1776");
  testQuestion(usaFounded);
  // Same visible output for a Question object, but passed differently.
  // testQuestionByReference(usaFounded);
  // testQuestionByPointer(&usaFounded);

  ChoiceQuestion twoPlusThree("What is 2 + 3 ?");
  twoPlusThree.addChoice("3", false); 
  twoPlusThree.addChoice("4", false); 
  twoPlusThree.addChoice("5", true);
  twoPlusThree.addChoice("6", false);

  testQuestion(twoPlusThree); // slicing
  testQuestionByReference(twoPlusThree); // no slicing
  testQuestionByPointer(&twoPlusThree); // no slicing

  return 0;
}

void testQuestion(Question ques) {
  ques.print();
  std::cout << "Enter your answer: ";
  std::string reply;
  std::getline(std::cin, reply);

  std::cout << std::boolalpha << ques.checkAnswer(reply) << "\n\n";
}


void testQuestionByReference(const Question& ques) {
  ques.print();
  std::cout << "Enter your answer: ";
  std::string reply;
  std::getline(std::cin, reply);

  std::cout << std::boolalpha << ques.checkAnswer(reply) << "\n\n";
}

void testQuestionByPointer(Question* quesPtr) {
  quesPtr->print();
  std::cout << "Enter your answer: ";
  std::string reply;
  std::getline(std::cin, reply);

  std::cout << std::boolalpha << quesPtr->checkAnswer(reply) << "\n\n";
}
