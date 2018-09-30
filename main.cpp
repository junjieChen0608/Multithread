#include <iostream>
#include <thread>
#include <mutex>

inline void myName(const std::string& name) {
  std::cout << "My name is " << name << "\n";
}

inline void myAge(const int& age) {
  std::cout << "I am " << age << " years old\n";
}

inline void selfIntro(const std::string& name, const int& age) {
  myName(name);
  myAge(age);
}

int main() {
  std::string name = "JJC";
  int age = 27;
  std::thread selfIntroductrionThread(selfIntro, name, age);

  if (selfIntroductrionThread.joinable()) {
    selfIntroductrionThread.join();
  }
  return 0;
}