#include "obstacle.h"
#include <random>
#include <iostream>

Obstacle::Obstacle(std::size_t grid_width, std::size_t grid_height){
  // Define the size of the window
  grid_width = grid_width;  
  grid_height = grid_height;
  
  // Place obstacle on random field inside of window
  std::random_device os_seed;
  const uint_least32_t seed = os_seed();
  
  std::mt19937 generator(seed);
  std::uniform_real_distribution<double> x_pos(0, grid_width);  
  std::uniform_real_distribution<double> y_pos(0, grid_height);
  
  float obstacle_x = x_pos(generator);
  float obstacle_y = y_pos(generator);
  
  point.x = static_cast<int>(obstacle_x);
  point.y = static_cast<int>(obstacle_y);
  
  // Get random direction
  std::uniform_int_distribution dir(0, 3);
  direction = Direction{dir(generator)};
}

Obstacle& Obstacle::operator=(const Obstacle &source){
  if (this == &source)
    return *this;

  speed = source.speed;
  obstacle_x = source.obstacle_x;
  obstacle_y = source.obstacle_y;
  
  point = source.point;
  grid_width = source.grid_width;
  grid_height = source.grid_height;
  direction = source.direction;

  return *this;
}

void Obstacle::Update() {
  switch (direction) {
    case Direction::kUp:
      obstacle_y -= speed;
      break;

    case Direction::kDown:
      obstacle_y += speed;
      break;

    case Direction::kLeft:
      obstacle_x -= speed;
      break;

    case Direction::kRight:
      obstacle_x += speed;
      break;
  }
  
  // Wrap the Snake around to the beginning if going off of the screen.
  obstacle_x = fmod(obstacle_x + grid_width, grid_width);
  obstacle_y = fmod(obstacle_y + grid_height, grid_height);
  
  point.x = static_cast<int>(obstacle_x);
  point.y = static_cast<int>(obstacle_y);
}