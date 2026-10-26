// printReverse.cpp
// purpose: print a string in reverse order
#include <iostream>
#include <string>

void printReverse(std::string s);

int main() {
  std::string strs[] = {"", "abc d"};
  for (const auto s: strs) {
    std::cout << "original string: \"" << s << "\"\n";
    std::cout << "print the reversed string\n";
    printReverse(s); 

    std::cout << "\n";
  }

  return 0;
}

void printReverse(std::string s) {
  if (s.empty()) {
    return;
  }

  printReverse(s.substr(1)); // s.substr(1) is the substring of s starting from the second character. That is, the substring of s without the first character.
  std::cout << s[0]; 
}
