// Copyright 2021 NNTU-CS
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
  for (int i = 0; i < len - 1; i++) {
    int target = value - arr[i];
    int low = i + 1;
    int high = len - 1;
    while (low <= high) {
      int mid = low + (high - low) / 2;
      if (arr[mid] == target) {
        count++;
        int temp = mid + 1;
        while (temp <= high && arr[temp] == target) {
          count++;
          temp++;
        }
        temp = mid - 1;
        while (temp >= low && arr[temp] == target) {
          count++;
          temp--;
        }
        break;
      } else if (arr[mid] < target) {
        low = mid + 1;
      } else {
        high = mid - 1;
      }
    }
  }
  return count;
}

int countPairs3(int *arr, int len, int value) {
  int count = 0;
  int i = 0;
  int j = len - 1;
  while (i < j) {
    if (arr[i] + arr[j] == value) {
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
    } else if (arr[i] + arr[j] < value) {
      i++;
    } else {
      j--;
    }
  }
  return count;
}
