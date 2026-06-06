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
  for (int i = 0; i < len - 1; i++) {
    int target = value - arr[i];
    if (target < arr[i]) break;
    int low = i + 1, high = len - 1;
    while (low <= high) {
      int mid = low + (high - low) / 2;
      if (arr[mid] == target) {
        int left = mid, right = mid;
        while (left > i + 1 && arr[left - 1] == target) {
          left--;
        }
        while (right < high && arr[right + 1] == target) {
          right++;
        }
        if (arr[i] == target) {
          int n = right - i + 1;
          count += n * (n - 1) / 2;
          i = right;
        } else {
          count += right - left + 1;
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
