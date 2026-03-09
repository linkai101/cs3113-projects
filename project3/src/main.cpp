/**
* Author: Linkai Wu
* Assignment: Lunar Lander
* Date due: 03/14/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "core/game.h"

int main(void) {
  Game game(1200, 675, "Dustrider");
  game.init();
  game.run();
  return 0;
}