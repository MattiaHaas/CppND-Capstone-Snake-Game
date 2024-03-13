#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <mutex>
#include <string>
#include <future>
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
  std::vector<std::unique_ptr<Obstacle>> obstacles;
  std::mutex mutex;
  std::condition_variable cond_var;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  std::string high_score_file;
  std::string user_name;
  bool timer_active;
  int grid_width_;
  int grid_height_;
  
  std::thread obstacles_thread;

  void PlaceFood();
  void GenerateObstacles(int num_obstacles, int grid_width, int grid_height);
  void Update();
  void Timer();
};

#endif