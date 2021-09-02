#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "Mandel.hpp"

#pragma GCC optimize("Ofast")

using namespace std;
using namespace sf;

const int WIDTH = 960;
const int HEIGHT = 600;
const int maxThreads = 16;

int main() {
  // Init
  RenderWindow window(VideoMode(WIDTH, HEIGHT), "Mandelbrot", Style::Close);
  window.setPosition(Vector2i(300, 60));
  window.setFramerateLimit(30);

  Image image;
  Texture texture;
  Sprite sprite;
  image.create(WIDTH, HEIGHT);

  Mandelbrot Mandel(WIDTH, HEIGHT);

  string textStr;
  Text text;
  Font font;
  font.loadFromFile("../media/GeosansLight.ttf");
  text.setFont(font);
  text.setFillColor(Color::White);
  text.setCharacterSize(24);

  // Threading
  array<thread, maxThreads> threads;
  int threadWidth = WIDTH / maxThreads;

  // Loop
  Vector2i panPosition;
  bool moving = false;

  while (window.isOpen()) {
    Vector2i position = Mouse::getPosition(window);
    Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case Event::Closed:
          window.close();
          break;
        case Event::MouseButtonReleased:
          if (event.mouseButton.button == Mouse::Button::Left) {
            moving = false;
          }
        case Event::MouseButtonPressed:
          if (event.mouseButton.button == Mouse::Button::Left) {
            panPosition = position;
            moving = true;
          }
      }
    }

    // Zoom
    if (Keyboard::isKeyPressed(Keyboard::A)) Mandel.zoomPlot(position, 0.96);
    if (Keyboard::isKeyPressed(Keyboard::Q)) Mandel.zoomPlot(position, 1.04);

    // Change iterations
    if (Keyboard::isKeyPressed(Keyboard::W)) Mandel.maxIter += 10;
    if (Keyboard::isKeyPressed(Keyboard::S)) Mandel.maxIter -= 10;

    // Panning
    if (Mouse::isButtonPressed(Mouse::Left) && moving) {
      Mandel.panPlot(panPosition - position);
      panPosition = position;
    }

    // Update and Time
    auto timer1 = chrono::high_resolution_clock::now();

    for (int i = 0; i < threads.size(); i++) {
      threads[i] = thread(&Mandelbrot::loadField, &Mandel, threadWidth * i, threadWidth * (i + 1));
    }
    for (auto& threadi : threads) {
      threadi.join();
    }

    Mandel.drawToImage(image);

    auto timer2 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedTime = timer2 - timer1;

    textStr.clear();
    textStr += "Time per frame: " + to_string(elapsedTime.count());
    textStr += "\n Iterations: " + to_string(Mandel.maxIter);

    double real, imag;
    Mandel.screenToPlot(position.x, position.y, real, imag);
    textStr += "\nPosition: " + to_string(real) + " | " + to_string(imag);
    text.setString(textStr);

    texture.loadFromImage(image);
    sprite.setTexture(texture);

    window.clear();
    window.draw(sprite);
    window.draw(text);
    window.display();
  }

  return 0;
}