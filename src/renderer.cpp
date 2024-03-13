#include "renderer.h"
#include "obstacle.h"
#include <iostream>
#include <string>
#include <memory>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  
  // Define grid cell size
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
        
  // Load textures
  LoadTextures();
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::LoadTextures() {
  
  // Load textures for food and obstacles
  food_texture = IMG_LoadTexture(sdl_renderer, "../data/textures/food.png");  
  obstacle_texture = IMG_LoadTexture(sdl_renderer, "../data/textures/obstacles.png");
  
  // Define block size of food
  block_food.w = block.w;
  block_food.h = block.h;
  
  // Define block size of obstacle
  block_obstacle.w = block.w;
  block_obstacle.h = block.h;
  
  // Render textures on SDL_Rects
  SDL_RenderCopy(sdl_renderer, food_texture, &block, &block_food);
  SDL_RenderCopy(sdl_renderer, obstacle_texture, &block, &block_obstacle);
  
}

void Renderer::RenderSnake(Snake const &snake){
  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::RenderFood(SDL_Point const &food){
  block_food.x = food.x * block_food.w;
  block_food.y = food.y * block_food.h;
  SDL_RenderCopy(sdl_renderer, food_texture, NULL, &block_food);
}

void Renderer::RenderObstacle(std::unique_ptr<Obstacle> const &obstacle){
  block_obstacle.x = obstacle->point.x * block_obstacle.w;
  block_obstacle.y = obstacle->point.y * block_obstacle.h;
  SDL_RenderCopy(sdl_renderer, obstacle_texture, NULL, &block_obstacle);
}

void Renderer::Render(Snake const snake, SDL_Point const &food, std::vector<std::unique_ptr<Obstacle>> const &obstacles) {
  
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  RenderFood(food);
  
  // Render snake
  RenderSnake(snake);
  
  // Render obstacles
  for (int i=0; i< obstacles.size(); ++i){
    RenderObstacle(obstacles[i]);
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
