#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "snake.h"
#include "obstacle.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food, std::vector<std::unique_ptr<Obstacle>> const &obstacles);
  void UpdateWindowTitle(int score, int fps);

 private:
  void RenderSnake(Snake const &snake);
  void RenderFood(SDL_Point const &food);  
  void RenderObstacle(std::unique_ptr<Obstacle> const &obstacle);
  
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  SDL_Rect block;
};

#endif