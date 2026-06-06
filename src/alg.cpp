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
    int a = arr[i];
    int target = value - a;
    if (target < a) break;
    int iEnd = static_cast<int>(
      std::upper_bound(arr + i, arr + len, a) - arr);
    int aCnt = iEnd - i;
    if (a == target) {
      count += aCnt * (aCnt - 1) / 2;
    } else {
      int lo = static_cast<int>(
        std::lower_bound(arr + iEnd, arr + len, target) - arr);
      int hi = static_cast<int>(
        std::upper_bound(arr + iEnd, arr + len, target) - arr);
      count += aCnt * (hi - lo);
    }
    i = iEnd;
  }
  return count;
}
