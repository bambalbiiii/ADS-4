// Copyright 2021 NNTU-CS
#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include "alg.h"
#include "lodepng.h"
 
struct Color { unsigned char r, g, b; };
 
const int W = 900, H = 600;
const int ML = 80, MR = 30, MT = 30, MB = 60;
 
void setPixel(std::vector<unsigned char>& img, int x, int y, Color c) {
  if (x < 0 || x >= W || y < 0 || y >= H) return;
  int idx = (y * W + x) * 4;
  img[idx] = c.r;
  img[idx+1] = c.g;
  img[idx+2] = c.b;
  img[idx+3] = 255;
}
 
void drawLine(std::vector<unsigned char>& img,
              int x0, int y0, int x1, int y1, Color c) {
  int dx = abs(x1-x0), dy = abs(y1-y0);
  int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
  int err = dx - dy;
  while (true) {
    setPixel(img, x0, y0, c);
    if (x0 == x1 && y0 == y1) break;
    int e2 = 2 * err;
    if (e2 > -dy) { err -= dy; x0 += sx; }
    if (e2 < dx)  { err += dx; y0 += sy; }
  }
}
 
void drawChar(std::vector<unsigned char>& img,
              int x, int y, char ch, Color c) {
  static const unsigned char font[10][5] = {
    {0x7E, 0x42, 0x42, 0x42, 0x7E},
    {0x00, 0x44, 0x7E, 0x40, 0x00},
    {0x72, 0x52, 0x52, 0x52, 0x4E},
    {0x24, 0x42, 0x52, 0x52, 0x3C},
    {0x1E, 0x10, 0x10, 0x7E, 0x10},
    {0x2E, 0x4A, 0x4A, 0x4A, 0x32},
    {0x3C, 0x52, 0x52, 0x52, 0x20},
    {0x02, 0x62, 0x12, 0x0A, 0x06},
    {0x3C, 0x52, 0x52, 0x52, 0x3C},
    {0x0C, 0x52, 0x52, 0x52, 0x3C}
  };
  if (ch >= '0' && ch <= '9') {
    const auto& g = font[ch-'0'];
    for (int col = 0; col < 5; col++) {
      for (int row = 0; row < 8; row++) {
        if (g[col] & (1 << row)) {
          setPixel(img, x+col, y+row, c);
        }
      }
    }
  }
}
 
void drawText(std::vector<unsigned char>& img,
              int x, int y, const std::string& s, Color c) {
  for (char ch : s) {
    drawChar(img, x, y, ch, c);
    x += 7;
  }
}
 
void savePlot(const std::string& filename,
              const std::vector<int>& ns,
              const std::vector<double>& t1,
              const std::vector<double>& t2,
              const std::vector<double>& t3,
              bool logScale) {
  std::vector<unsigned char> img(W * H * 4, 255);
 
  int pw = W - ML - MR, ph = H - MT - MB;
 
  drawLine(img, ML, MT, ML, MT+ph, {0, 0, 0});
  drawLine(img, ML, MT+ph, ML+pw, MT+ph, {0, 0, 0});
 
  auto toY = [&](double v, double vmin, double vmax) {
    return MT + ph - static_cast<int>((v - vmin) / (vmax - vmin) * ph);
  };
 
  double vmin = 0, vmax = 0;
  if (logScale) {
    auto logmax = [](double acc, double v) {
      return v > 0 ? std::max(acc, std::log10(v)) : acc;
    };
    auto logmin = [](double acc, double v) {
      return v > 0 ? std::min(acc, std::log10(v)) : acc;
    };
    vmax = std::accumulate(t1.begin(), t1.end(), vmax, logmax);
    vmax = std::accumulate(t2.begin(), t2.end(), vmax, logmax);
    vmax = std::accumulate(t3.begin(), t3.end(), vmax, logmax);
    vmin = vmax;
    vmin = std::accumulate(t1.begin(), t1.end(), vmin, logmin);
    vmin = std::accumulate(t2.begin(), t2.end(), vmin, logmin);
    vmin = std::accumulate(t3.begin(), t3.end(), vmin, logmin);
  } else {
    vmax = *std::max_element(t1.begin(), t1.end());
  }
 
  auto getV = [&](double v) {
    return logScale && v > 0 ? std::log10(v) : v;
  };
 
  Color c1{220, 50, 50}, c2{50, 150, 50}, c3{50, 50, 220};
  int n = static_cast<int>(ns.size());
  double xmin = ns.front(), xmax = ns.back();
 
  for (int i = 1; i < n; i++) {
    int x0 = ML + static_cast<int>((ns[i-1]-xmin)/(xmax-xmin)*pw);
    int x1 = ML + static_cast<int>((ns[i]  -xmin)/(xmax-xmin)*pw);
    drawLine(img,
      x0, toY(getV(t1[i-1]), vmin, vmax),
      x1, toY(getV(t1[i]), vmin, vmax), c1);
    drawLine(img,
      x0, toY(getV(t2[i-1]), vmin, vmax),
      x1, toY(getV(t2[i]), vmin, vmax), c2);
    drawLine(img,
      x0, toY(getV(t3[i-1]), vmin, vmax),
      x1, toY(getV(t3[i]), vmin, vmax), c3);
  }
 
  for (int i = 0; i < n; i += 4) {
    int x = ML + static_cast<int>((ns[i]-xmin)/(xmax-xmin)*pw);
    drawLine(img, x, MT+ph, x, MT+ph+4, {0, 0, 0});
    drawText(img, x-10, MT+ph+8, std::to_string(ns[i]), {0, 0, 0});
  }
 
  lodepng::encode(filename, img, W, H);
}
 
double measure(int (*f)(int*, int, int), int* arr, int len, int value) {
  auto s = std::chrono::high_resolution_clock::now();
  f(arr, len, value);
  auto e = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::micro>(e - s).count();
}
 
int main() {
  int arr[] = {10, 20, 30, 30, 40, 50};
  std::cout << countPairs1(arr, 6, 60) << std::endl;
  std::cout << countPairs2(arr, 6, 60) << std::endl;
  std::cout << countPairs3(arr, 6, 60) << std::endl;
 
  std::vector<int> ns;
  std::vector<double> t1, t2, t3;
 
  for (int n = 1000; n <= 20000; n += 1000) {
    int* a = new int[n];
    for (int i = 0; i < n; i++) {
      a[i] = rand() % 1000;
    }
    std::sort(a, a + n);
 
    ns.push_back(n);
    t1.push_back(measure(countPairs1, a, n, 500));
    t2.push_back(measure(countPairs2, a, n, 500));
    t3.push_back(measure(countPairs3, a, n, 500));
 
    delete[] a;
  }
 
  savePlot("../Plots/lin.png", ns, t1, t2, t3, false);
  savePlot("../Plots/log.png", ns, t1, t2, t3, true);
 
  std::cout << "Plots saved." << std::endl;
  return 0;
}
