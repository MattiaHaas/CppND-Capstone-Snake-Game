#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <random>
#include <string>
#include "SDL.h"

class Obstacle {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };
  
  Obstacle(std::size_t grid_width, std::size_t grid_height);
  Obstacle &operator=(const Obstacle &source);

  void Update();

  float speed{0.0f};
  float obstacle_x;
  float obstacle_y;
  
  SDL_Point point;

 private:
  std::size_t grid_width;
  std::size_t grid_height;
  Direction direction;
};

#endif