#include <iostream>

// https://ko.wikipedia.org/wiki/%EA%B7%B8%EB%A0%88%EC%9D%B4_%EB%B6%80%ED%98%B8
// https://www.youtube.com/watch?v=5iCzSUU_ffw

int gray_code(int n)
{
  return n ^ (n >>1);
}

int main(){
  for (int i =0; i < 9; i++)
    std::cout << gray_code(i) << std::endl;
}
