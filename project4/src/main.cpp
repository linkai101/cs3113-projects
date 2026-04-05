/**
* Author: Linkai Wu
* Assignment: Rise of the AI
* Date due: 2026-04-04, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "game.h"

int main(void) {
  Game game(1200, 675, "High Tide Heist");
  game.init();
  game.run();
  return 0;
}