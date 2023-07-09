#include <iostream>

// russian peasant method of multiplication
// https://www.youtube.com/watch?v=xrUCL7tGKaI

int multiplier(int a, int b) {
  int result;

  result = 0;
  while (b != 0) {
    if (b & 1)
      // result += a;
      result = result ^ a;
    a = a << 1;
    b = b >> 1;
  }
  return result;
}

int main() {
  std::cout << multiplier(2, 3) << std::endl;
}
