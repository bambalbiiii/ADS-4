// Copyright 2021 NNTU-CS
#include <algorithm>
#include "alg.h"

int countPairs1(int *arr, int len, int value) {
  int count = 0;
  for (int i = 0; i < len - 1; i++) {
    for (int j = i + 1; j < len; j++) {
      if (arr[i] + arr[j] == value) {
        count++;
      }
    }
  }
  return count;
}

int countPairs2(int *arr, int len, int value) {
  int count = 0;
  int i = 0, j = len - 1;
  while (i < j) {
    int sum = arr[i] + arr[j];
    if (sum == value) {
      if (arr[i] == arr[j]) {
        int n = j - i + 1;
        count += n * (n - 1) / 2;
        break;
      }
      int left = 1, right = 1;
      while (i + 1 < j && arr[i] == arr[i + 1]) {
        left++;
        i++;
      }
      while (j - 1 > i && arr[j] == arr[j - 1]) {
        right++;
        j--;
      }
      count += left * right;
      i++;
      j--;
    } else if (sum < value) {
      i++;
    } else {
      j--;
    }
  }
  return count;
}

int countPairs3(int *arr, int len, int value) {
  int count = 0;
  int i = 0;
  while (i < len) {
    int target = value - arr[i];
    if (target < arr[i]) break;
    int* lo = std::lower_bound(arr + i + 1, arr + len, target);
    int* hi = std::upper_bound(arr + i + 1, arr + len, target);
    int found = static_cast<int>(hi - lo);
    if (found > 0) {
      if (arr[i] == target) {
        int n = found + 1;
        count += n * (n - 1) / 2;
        i += n;
        continue;
      }
      count += found;
    }
    int* next = std::upper_bound(arr + i, arr + len, arr[i]);
    i = static_cast<int>(next - arr);
  }
  return count;
}
