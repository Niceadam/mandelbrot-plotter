#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

using namespace sf;

struct Mandelbrot {
  int Width;
  int Height;
  int maxIter = 100;

  std::vector<int> Field;

  long double realMin = -1.9, realMax = 0.6;
  long double imagMin = -1, imagMax = 1;

  Mandelbrot(int Width, int Height);

  void drawToImage(Image& image);
  Color colorMap(int iter);
  void loadField(int startWidth = 0, int endWidth = 0);
  void loadFieldAVX(int startWidth = 0, int endWidth = 0);

  void screenToPlot(int iS, int jS, double& iW, double& jW);
  void panPlot(Vector2i displace);
  void zoomPlot(Vector2i position, double zoom);
  void zoomPlot(double real, double imag, double zoom);
};

Mandelbrot::Mandelbrot(int Width, int Height) : Width(Width), Height(Height) { Field.resize(Width * Height); }

void Mandelbrot::screenToPlot(int iS, int jS, double& iW, double& jW) {
  iW = realMin + (realMax - realMin) * (double)iS / Width;
  jW = imagMin + (imagMax - imagMin) * (double)jS / Height;
}

void Mandelbrot::panPlot(Vector2i displace) {
  double real, imag;
  screenToPlot(displace.x, displace.y, real, imag);
  real -= realMin;
  imag -= imagMin;

  realMin += real;
  realMax += real;
  imagMin += imag;
  imagMax += imag;
};

void Mandelbrot::zoomPlot(Vector2i position, double zoom) {
  double real, imag;
  screenToPlot(position.x, position.y, real, imag);

  realMin = real + (realMin - real) / zoom;
  realMax = real + (realMax - real) / zoom;
  imagMin = imag + (imagMin - imag) / zoom;
  imagMax = imag + (imagMax - imag) / zoom;
};

void Mandelbrot::zoomPlot(double real, double imag, double zoom) {
  realMin = real + (realMin - real) / zoom;
  realMax = real + (realMax - real) / zoom;
  imagMin = imag + (imagMin - imag) / zoom;
  imagMax = imag + (imagMax - imag) / zoom;
};

void Mandelbrot::loadField(int startWidth, int endWidth) {
  if (endWidth == 0) endWidth = Width;

  double Creal, Cimag;
  double real, imag;
  double real_2, imag_2;
  int count;

  for (int i = startWidth; i < endWidth; i++) {
    for (int j = 0; j < Height; j++) {
      screenToPlot(i, j, Creal, Cimag);

      real = 0, imag = 0, real_2 = 0, imag_2 = 0;
      for (count = 0; count < maxIter; count++) {
        imag = 2 * real * imag + Cimag;
        real = real_2 - imag_2 + Creal;
        real_2 = real * real;
        imag_2 = imag * imag;

        if (real_2 + imag_2 > 4) break;
      }

      Field[i + j * Width] = count;
    }
  }
}

void Mandelbrot::loadFieldAVX(int startWidth, int endWidth) { ; }

void Mandelbrot::drawToImage(Image& image) {
  for (int i = 0; i < Width; i++) {
    for (int j = 0; j < Height; j++) {
      int count = Field[i + j * Width];
      image.setPixel(i, j, colorMap(count));
    }
  }
}

Color Mandelbrot::colorMap(int iter) {
  float n = (float)iter;

  if (iter == maxIter) return Color(0, 0, 0);

  float r = 0.5 * sin(0.1f * n) + 0.5f;
  float g = 0.5 * sin(0.1f * n + 2.094f) + 0.5f;
  float b = 0.5 * sin(0.1f * n + 4.188f) + 0.5f;

  return Color(r * 255, g * 255, b * 255);
}