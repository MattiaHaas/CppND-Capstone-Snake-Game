#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <random>
#include <string>
#include "SDL.h"

class Obstacle {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };
  
  Obstacle(int grid_width, int grid_height);
  Obstacle &operator=(Obstacle &&source);

  void Update();
  int getGridWidth();
  int getGridHeight();
  void setGridWidth(int width);
  void setGridHeight(int height);

  float speed{0.05f};
  float obstacle_x;
  float obstacle_y;
  
  SDL_Point point;

 private:
  int grid_width;
  int grid_height;
  Direction direction;
};

#endif