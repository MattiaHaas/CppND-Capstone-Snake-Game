#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <string>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "obstacle.h"


class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void SaveScore();
  void PrintHighScore();

 private:
  Snake snake;
  SDL_Point food;
  std::unique_ptr<Obstacle> obstacle;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  std::string high_score_file;
  std::string user_name;

  void PlaceFood();
  void GenerateObstacle(int grid_width, int grid_height);
  void Update();
};

#endif