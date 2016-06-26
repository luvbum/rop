#include "game.h"
#include "globals.h"
#include "sdl_wrapper.h"
#include "entity.h"
#include "map.h"
#include "f_ai.h"
#include "f_mortal.h"
#include "gui.h"
#include "f_fighter.h"

namespace Game{

  bool isRunning = false;
  sdlEngine sdl;

  std::unique_ptr<Entity> player = nullptr;
  std::vector<std::unique_ptr<Entity>> actors;
  void Init(){
    // Init player + player components
    std::unique_ptr<Entity> pEntity(new Entity(P(0,0), "Player", '@', White));
    player = std::move(pEntity);
    
    std::unique_ptr<Ai> pAi(new PlayerAi(10));
    player->ai = std::move(pAi);

    std::unique_ptr<Fighter> pFight(new Fighter(10));
    player->fighter = std::move(pFight);

    std::unique_ptr<Mortal> pMortal(new PlayerMortal(20, 0, "Dead Player"));
    player->mortal = std::move(pMortal);
    
    Map::cleanMap();
    Map::createMap();
    
    isRunning = true;
  }

  void runStartMenu(){
    // Do important stuff here.
  }

  void runGame(){
    
    while(isRunning){
      /* Handle Rendering
	 renderMap
	 renderGui
	 renderMonsters
	 renderPlayer	 
      */
      Render();
           
      
      /* Handle Game Logic
	 handleFov
	 handleCollisions
	 handleCombat
      */
      Tick();
    }
    
  }

  void Render(){


    sdl.clear();
    Map::renderMap();

    // Render actors
    for(auto& a : actors){
      if(Map::cells[a->pos.x][a->pos.y].isSeen)
	a->Render();
    }
    
    player->Render();
    Gui::RenderGui();

    sdl.flip();

  }

  void Input(){

  }
  
  void Tick(){  
    
    //if(player->ai->energy >= 10){
      player->Update();
      //}
    
    for(auto& a : actors){
      if(a->ai){
	while(a->ai->energy >= globals::TURN_COST && !a->mortal->isDead()){
	  a->Update();
	}
      }
    }
  }

  void newTurn(){

    // New Turn, add more energy to each actor
    player->ai->newTurn();
    for(auto& a : actors){
      if(a->ai)
	a->ai->newTurn();
    }
    
  }
  
  void menuState(){

  }
  
}
