#include <iostream>
// https://iq.opengenus.org/addition-using-bitwise-operations/
int adder(int a, int b) {
  while (b != 0) {
    int carry = a & b;
    a = a ^ b;
    b = carry << 1;
  }
  return a;
}

int main() {
  std::cout << adder(1, 2) << std::endl;
}
