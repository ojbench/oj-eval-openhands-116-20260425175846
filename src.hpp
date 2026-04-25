
#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>
#include <utility>
#include <vector>

enum class instruction{UP, DOWN, LEFT, RIGHT, NONE};
//the instruction of the snake

const int MaxWidth = 20;

struct Snake; // Forward declaration

struct Map{
  // store the necessary information of the map
  bool wall[MaxWidth][MaxWidth];
  bool food[MaxWidth][MaxWidth];
  int width, height;
  
  int get_height(){
    //return the height of the map
    return height;
  }
  
  int get_width(){
    //return the width of the map
    return width;
  }
  
  bool is_food(int x, int y){
    //return true if the cell at (x,y) is food
    if (x < 0 || x >= height || y < 0 || y >= width) {
      return false;
    }
    return food[x][y];
  }
  
  void eat_food(int x, int y){
    //eat the food at (x,y)
    if (x >= 0 && x < height && y >= 0 && y < width) {
      food[x][y] = false;
    }
  }
  
  bool is_wall(int x, int y){
    //return true if the cell at (x,y) is wall
    if (x < 0 || x >= height || y < 0 || y >= width) {
      return false;
    }
    return wall[x][y];
  }

  void initialize(Snake *snake);
  void print(Snake *snake);
};

bool is_food(Map* map, int x, int y);
//return true if the cell at (x,y) is food

bool is_wall(Map* map, int x, int y);
//return true if the cell at (x,y) is wall

void eat_food(Map* map, int x, int y);
//remove the food at (x,y)

int get_height(Map* map);
//return the height of the map

int get_width(Map* map);
//return the width of the map

struct Snake{
  // store the necessary information of the snake
  std::vector<std::pair<int, int>> body;
  instruction current_direction;
  
  void initialize(int x, int y, instruction ins){
    // (x,y) isthe position of the head of the snake, ins is the initial orientation of the snake
    body.clear();
    body.push_back({x, y});
    current_direction = ins;
  }
  
  int get_length(){
    //return the length of the snake
    return body.size();
  }
  
  bool move(Map* map, instruction ins){
    //the current map and the next instruction
    //return false if the snake is dead
    //return true if the snake is alive
    
    // Check for opposite direction
    if (ins == instruction::NONE) {
      ins = current_direction;
    } else {
      // Check if trying to move in opposite direction
      if ((current_direction == instruction::UP && ins == instruction::DOWN) ||
          (current_direction == instruction::DOWN && ins == instruction::UP) ||
          (current_direction == instruction::LEFT && ins == instruction::RIGHT) ||
          (current_direction == instruction::RIGHT && ins == instruction::LEFT)) {
        return false; // Death by opposite direction
      }
      current_direction = ins;
    }
    
    // Calculate new head position
    int new_x = body[0].first;
    int new_y = body[0].second;
    
    switch (current_direction) {
      case instruction::UP:
        new_x--;
        break;
      case instruction::DOWN:
        new_x++;
        break;
      case instruction::LEFT:
        new_y--;
        break;
      case instruction::RIGHT:
        new_y++;
        break;
      case instruction::NONE:
        break; // Should not reach here
    }
    
    // Check boundaries
    if (new_x < 0 || new_x >= map->get_height() || 
        new_y < 0 || new_y >= map->get_width()) {
      return false; // Death by boundary
    }
    
    // Check wall collision
    if (map->is_wall(new_x, new_y)) {
      return false; // Death by wall
    }
    
    // Check self collision (but not with the tail if we're going to eat food)
    bool will_eat_food = map->is_food(new_x, new_y);
    for (size_t i = 0; i < body.size(); i++) {
      if (body[i].first == new_x && body[i].second == new_y) {
        // If we're about to eat food, the tail will stay, so don't check collision with tail
        if (will_eat_food && i == body.size() - 1) {
          continue;
        }
        return false; // Death by self collision
      }
    }
    
    // Move snake
    body.insert(body.begin(), {new_x, new_y});
    
    // Check if food is eaten
    if (will_eat_food) {
      map->eat_food(new_x, new_y);
      // Don't remove tail - snake grows
    } else {
      // Remove tail - normal movement
      body.pop_back();
    }
    
    return true;
  }
  
  std::pair<int, std::pair<int,int>*> get_snake(){
    //return the length of the snake and a pointer to the array of the position of the snake
    //you can store the head as the first element
    
    int length = body.size();
    std::pair<int, int>* positions = new std::pair<int, int>[length];
    
    for (int i = 0; i < length; i++) {
      positions[i] = body[i];
    }
    
    return {length, positions};
  }
};

// Map method implementations
void Map::initialize(Snake *snake){
  char str[MaxWidth];
  int head_x = -1 , head_y = -1;
  instruction ins;
  
  // Read map dimensions
  std::cin >> height >> width;
  
  // Initialize arrays
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      wall[i][j] = false;
      food[i][j] = false;
    }
  }
  
  // Read map layout
  for (int i = 0; i < height; i++) {
    std::cin >> str;
    for (int j = 0; j < width; j++) {
      if (str[j] == '#') {
        wall[i][j] = true;
      } else if (str[j] == '*') {
        food[i][j] = true;
      } else if (str[j] == '@') {
        head_x = i;
        head_y = j;
      }
    }
  }
  
  // Read initial direction
  char dir_char;
  std::cin >> dir_char;
  switch (dir_char) {
    case 'U':
      ins = instruction::UP;
      break;
    case 'D':
      ins = instruction::DOWN;
      break;
    case 'L':
      ins = instruction::LEFT;
      break;
    case 'R':
      ins = instruction::RIGHT;
      break;
    default:
      ins = instruction::UP; // Default
      break;
  }
  
  snake->initialize(head_x, head_y, ins);
}

void Map::print(Snake *snake){
  auto snake_body = snake->get_snake();
  
  // Create display grid
  char display[MaxWidth][MaxWidth];
  
  // Initialize with empty spaces
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (wall[i][j]) {
        display[i][j] = '#';
      } else if (food[i][j]) {
        display[i][j] = '*';
      } else {
        display[i][j] = '.';
      }
    }
  }
  
  // Place snake body
  for (int i = 0; i < snake_body.first; i++) {
    int x = snake_body.second[i].first;
    int y = snake_body.second[i].second;
    if (i == 0) {
      display[x][y] = '@'; // Head
    } else {
      display[x][y] = 'o'; // Body
    }
  }
  
  // Print the map
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      std::cout << display[i][j];
    }
    std::cout << std::endl;
  }
  
  // Clean up allocated memory
  delete[] snake_body.second;
}

struct Game{
  Map *map;
  Snake *snake;
  int score;
  int round;
  void initialize(){
    map = new Map();
    snake = new Snake();
    map->initialize(snake);
    score = 0;
    round = 0;
  }
  bool step()
  {
    char str[MaxWidth];
    std::cin >> str;
    instruction ins;
    switch(str[0]){
      case 'U':
        ins = instruction::UP;
        break;
      case 'D':
        ins = instruction::DOWN;
        break;
      case 'L':
        ins = instruction::LEFT;
        break;
      case 'R':
        ins = instruction::RIGHT;
        break;
      default:
        ins = instruction::NONE;
    }
    if(snake->move(map, ins)){
      score++;
    }else{
      return false;
    }
    return true;
  }
  void print(){
    std::cout<< "Round: " << round << std::endl;
    map->print(snake);
  }
  void play()
  {
    while(step()){
      round++;
      print();
    }
    score += snake->get_length() ;
    std::cout << "Game Over" << std::endl;
    std::cout << "Score: " << score << std::endl;
  }
};
#endif
