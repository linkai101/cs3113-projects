/**
* Author: Linkai Wu
* Assignment: Project 5
* Date due: 2026-04-24, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "game.h"

int main(void) {
  Game game(1600, 900, "Holdout");
  game.init();
  game.run();
  return 0;
}