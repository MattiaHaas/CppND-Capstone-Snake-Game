#include "game.h"
#include "obstacle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <chrono>
#include <algorithm>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      grid_width_(static_cast<int>(grid_width - 1)),
      grid_height_(static_cast<int>(grid_height - 1)),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  // Ask user to enter name
  std::cout << "Please enter your username: ";
  std::cin >> user_name;
        
  // Define score file location
  high_score_file = "../data/high_scores.txt";
  
  // Place food
  PlaceFood();
        
  // Place obstacles
  GenerateObstacles(2, grid_width_, grid_height_);
}

void Game::Timer()
{
  timer_active = true;
  const int timer_time = 15;
  auto start_time = std::chrono::high_resolution_clock::now();
  std::unique_lock<std::mutex> lock(mutex);
  while (timer_active)
  {
    lock.unlock();
    auto current_time = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
    if (elapsed_seconds >= timer_time)
    {
      // Timer is up
      timer_active = false;
      break;
    }
    lock.lock();
    // Wait for a short interval or until the condition_variable is notified
    cond_var.wait_for(lock, std::chrono::milliseconds(800));
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  timer_active = false;

  while (running) {
    frame_start = SDL_GetTicks();
    
    // Check if timer is still running
    if (!timer_active)
    { 
      // Delete current obstacles and generate new obstacles
      obstacles.clear();
      
      GenerateObstacles(2, grid_width_, grid_height_);
      timer_active = true;
      obstacles_thread = std::thread(&Game::Timer, this);
      obstacles_thread.detach();
    }

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, obstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::GenerateObstacles(int num_obstacles, int grid_width, int grid_height) {
  
  // Generate obstacle
  for (int i=0; i<num_obstacles; ++i){
  obstacles.emplace_back(std::make_unique<Obstacle>(grid_width, grid_height));
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
  
  // Check if snake crashes with any of the objects
  for (int i=0; i< obstacles.size(); ++i){
    obstacles[i]->Update();
    if (obstacles[i]->point.x == new_x && obstacles[i]->point.y == new_y) {
    	snake.alive = false;
  	}
  }
  
}

void Game::SaveScore(){
  std::ofstream score_file(high_score_file, std::ios_base::app | std::ios_base::out);

  if (score>0){
    score_file << user_name << ";" << score << std::endl;
  }
}

void Game::PrintHighScore(){
  std::string user_name;
  int score;
  
  int high_score = 0;
  std::string high_score_user = "";
  
  std::ifstream filestream(high_score_file);
  std::string line;
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ';', ' ');
      std::istringstream linestream(line);
      while (linestream >> user_name >> score) {
        if (score > high_score) {
          std::replace(user_name.begin(), user_name.end(), '_', ' ');
          high_score = score;
          high_score_user = user_name;
        }
      }
    }
  }
  std::cout << "The player " << high_score_user << " has the high score " << high_score << std::endl;
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }