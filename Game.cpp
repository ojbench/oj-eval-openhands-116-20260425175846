
#include "src.hpp"
bool is_food(Map* map, int x, int y){
  return map->is_food(x,y);
}
bool is_wall(Map* map, int x, int y){
  return map->is_wall(x,y);
}
void eat_food(Map* map, int x, int y){
  map->eat_food(x, y);
}
int get_height(Map* map){
  return map->get_height();
}
int get_width(Map* map){
  return map->get_width();
}
int main()
{
	Game game;
	game.initialize();
	game.play();
  return 0;
}
