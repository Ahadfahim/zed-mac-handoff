// printReverse2.cpp
#include <iostream>
#include <string>

void printReverse(const std::string& str, std::size_t i);

int main() {
  std::string strs[] = {"", "abc d"};
  for (const auto s: strs) {
    std::cout << "original string: \"" << s << "\"\n";
    std::cout << "print the reversed string\n";
    printReverse(s, 0); 

    std::cout << "\n";
  }
  
  return 0;
}

void printReverse(const std::string& str, std::size_t i) {
  if (i == str.length()) {
    return;
  }

  printReverse(str, i+1);
  std::cout << str[i];
}
