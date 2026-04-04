// Copyright 2021 NNTU-CS
#include <iostream>
#include "alg.h"

int main() {
  int arr[] = {10, 20, 30, 30, 40, 50};
  int len = 6;
  int value = 60;

  std::cout << countPairs1(arr, len, value) << std::endl;
  std::cout << countPairs2(arr, len, value) << std::endl;
  std::cout << countPairs3(arr, len, value) << std::endl;

  return 0;
}
