#include <SFML/Graphics.hpp>
#include <array>
#include <chrono>
#include <iostream>
#include <thread>

#include "Mandel.hpp"

#pragma GCC optimize("Ofast")

using namespace std;
using namespace sf;
Image image;

const int WIDTH = 1280;
const int HEIGHT = 720;
const int maxThreads = 16;

void threadMandel(Mandelbrot& Mandel) {
  array<thread, maxThreads> threads;
  int sectionWidth = WIDTH / maxThreads;

  for (int i = 0; i < maxThreads; i++) {
    threads[i] = thread(&Mandelbrot::loadField, &Mandel, sectionWidth * i, sectionWidth * (i + 1));
  }

  for (auto& threadi : threads) {
    threadi.join();
  }
}

int main() {
  image.create(WIDTH, HEIGHT);
  Mandelbrot Mandel(WIDTH, HEIGHT);
  Mandel.maxIter = 1200;

  int frame = 0;
  while (frame < 2) {
    threadMandel(Mandel);
    Mandel.drawToImage(image);
    Mandel.zoomPlot(-1.157754, -0.299008, 1.03f);

    image.saveToFile("../images/image" + to_string(frame) + ".png");
    frame += 1;
  }

  return 0;
}