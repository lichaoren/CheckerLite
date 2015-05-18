/*
 * Game.h
 *
 *  Created on: Mar 28, 2015
 *      Author: chao
 */

#ifndef GAME_H_
#define GAME_H_

#include "Board.h"
#include <list>
#include <regex>
#include <boost/smart_ptr.hpp>

class Game {
public:
  Game& getinstance();
  virtual ~Game();

  move readinput() const;

  void play();
//  use alpha-beta pruning to search future cases in given
//  depth, find the best case then move
  int solve();
  void isend(int);

  Game();
private:
//  enumerate possible cases for given side
  void enumjumps(int, Board*, std::list<boost::scoped_ptr<Board*>>*) {}
  void enumjumps(int, Board*, std::list<Board*>*);
  void enummoves(int, Board*, std::list<Board*>*);
//  alpha beta pruning (side, board, depth, alpha value, beta value)
  int prune(int, Board*, int, int, int);
//  check if the game ends
  int eval(Board*);

  Board m_board;
  std::list<Board> m_cases;
};

#endif /* GAME_H_ */
