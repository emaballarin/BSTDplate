#include "random.hpp"
#include <iostream>

int main(){
  Rand_int ri{97,122,0};

  //char c{static_cast<char>(ri())};
  char c(ri());
  std::cout << c << "comp"<< (c<'b') << '\n';
}
