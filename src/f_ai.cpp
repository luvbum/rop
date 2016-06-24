#include "f_ai.h"
#include "sdl_wrapper.h"
#include "entity.h"
#include "game.h"
#include "helper.h"
#include "map.h"
#include "f_mortal.h"


PlayerAi::PlayerAi(int speed) {
  this->speed = speed;
  this->energy = 10;
}

PlayerAi::~PlayerAi(){

}

void PlayerAi::Input(Entity *a){
  int dx = 0;
  int dy = 0;
  // Handle Inputs
  Keys key = Game::sdl.Input();
  switch(key){
  case K_UP:
    dy = -1;
    break;
  case K_DOWN:
    dy = 1;
    break;
  case K_RIGHT:
    dx = 1;
    break;
  case K_LEFT:
    dx = -1;
    break;
  case E_QUIT:
    Game::isRunning = false;
    break;
  case K_q:
    Game::isRunning = false;
    break;
  case K_c:
    Map::closeDoor(a->pos);
    break;
  default:
    break;
  }

  if(dx != 0 || dy != 0){
    if(Move(a, P(a->pos.x+dx, a->pos.y+dy))){
      Game::newTurn();
    }
  }   
}

void PlayerAi::Act(Entity *a) {
  Input(a);
  Map::computeFov();
  // end of turn
  energy -= 10;
}

bool PlayerAi::Move(Entity *a, P p){
  // check for door
  if(Map::cells[p.x][p.y]._glyph == '+'){
    Map::openDoor(p);
    return true;
  }
  
  // Check collision with walls
  else if(!Map::cells[p.x][p.y]._block && p.x < globals::MAP_WIDTH && p.y < globals::MAP_HEIGHT && p.x >= 0 && p.y >= 0){
    a->pos.set(p);
    return true;
  }
  return false;
}

void PlayerAi::newTurn(){
  energy += speed;
}

MonsterAi::MonsterAi(int speed){
  this->energy = 0;
  this->speed = speed;
}

void MonsterAi::Act(Entity *a) {
  //if Dead
  if(a->mortal && a->mortal->isDead()){
    return;
  }
  
  // If player is in field of view
  if(Map::cells[a->pos.x][a->pos.y].isSeen){
    //attack or move
    
  }
  else{ // wander
    Wander(a);
  }
  energy -= 10;
}

void MonsterAi::Wander(Entity *a){
  int dir = Rng::randInt(0,4);
  int dx = 0;
  int dy = 0;
  switch(dir){
  case 0://north
    dy = -1;
    break;
  case 1: //east
    dx = 1;
    break;
  case 2: //south
    dy = 1;
    break;
  case 3:// west
    dx = -1;
    break;
  }
  
  Move(a, P(a->pos.x+dx, a->pos.y+dy));    

}

void MonsterAi::Move(Entity *a, P p) {

  // Check collision with walls
  if(!Map::cells[p.x][p.y]._block && p.x < globals::MAP_WIDTH && p.y < globals::MAP_HEIGHT && p.x >= 0 && p.y >= 0){
    a->pos.set(p);
  }
  
}

void MonsterAi::newTurn(){
  energy += speed;
}

					

				       