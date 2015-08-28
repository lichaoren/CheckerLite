/*
 * Game.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: chao
 */

#include "Game.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>

Game& Game::getinstance() {
  static Game instance;
  return instance;
}

Game::Game() :
  m_cases()
{
  std::cout << m_board;
}

Game::~Game() {}

void Game::enumjumps(int side, Board* board, std::list<Board*>* cases) {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (side == board->operator ()(i, j)) {
        if (board->hasleftjump(side, i, j)) {
//          need to find all possible jump paths
          cases->push_back(new Board(*board));
          cases->back()->leftjump(side, i, j);
        }
        if (board->hasrightjump(side, i, j)) {
          cases->push_back(new Board(*board));
          cases->back()->rightjump(side, i, j);
        }
      }
    }
  }
}
void Game::enummoves(int side, Board* board, std::list<Board*>* cases) {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (side == board->operator ()(i, j)) {
        if (board->hasleftmove(side, i, j)) {
          cases->push_back(new Board(*board));
          cases->back()->leftmove(side, i, j);
        }
        if (board->hasrightmove(side, i, j)) {
          cases->push_back(new Board(*board));
          cases->back()->rightmove(side, i, j);
        }
      }
    }
  }
}

void Game::isend(int done) {
    if (done == 0)
      std::cout << "tie" << std::endl;
    else if (done < 0)
      std::cout << "AI won" << std::endl;
    else
      std::cout << "player won" << std::endl;
}

int Game::prune(int turn, Board* board, int depth, int alpha, int beta) {
  if (0 == depth)
    return board->value();

  if (turn == PLAYER) {
    std::list<Board*> cases;
    enumjumps(turn, board, &cases);

    if (cases.size() > 0)
    std::cout << cases.front()->value() <<std::endl;

    if (cases.size() == 0) {
      enummoves(turn, board, &cases);
      if (cases.size() == 0)
        isend(board->value());
     std::cout << cases.size() << " walk cases found" <<std::endl;
    }

    cases.sort(greaterboardkey());

    std::list<Board*>::iterator itr;
    for (itr = cases.begin(); itr != cases.end(); ++itr) {
      alpha = std::max(alpha, prune(1 - turn, *itr, depth-1, alpha, beta));
      if (beta <= alpha) {
        cases.erase(itr, cases.end());
        break;
      }
    }
    for (itr = cases.begin(); itr != cases.end(); ++itr) {
      if((*itr)->value() >= board->value())
        (*board) = (**itr);
    }
    return alpha;
  }
  else {
    std::list<Board*> cases;
    enumjumps(turn, board, &cases);
    std::cout << cases.size() << " jump cases found" <<std::endl;

    if (cases.size() > 0)
    std::cout << cases.front()->value() <<std::endl;
    if (cases.size() == 0) {
     enummoves(turn, board, &cases);
     if (cases.size() == 0)
       isend(board->value());
     std::cout << cases.size() << " walk cases found" <<std::endl;
    }

    cases.sort(lessboardkey());

    std::list<Board*>::iterator itr;
    for (itr = cases.begin(); itr != cases.end(); ++itr) {
      beta = std::min(beta, prune(1 - turn, *itr, depth - 1, alpha, beta));
      if (beta <= alpha) {
        cases.erase(itr, cases.end());
        break;
      }
    }
    for (itr = cases.begin(); itr != cases.end(); ++itr) {
      if ((*itr)->value() <= board->value())
        board->operator =((**itr));
    }
    return beta;
  }
}

/*template <typename I>
I random_element(I begin, I end)
{
    const unsigned long n = std::distance(begin, end);
    const unsigned long divisor = (RAND_MAX + 1) / n;

    unsigned long k;
    do { k = std::rand() / divisor; } while (k >= n);

    return std::advance(begin, k);
}*/

int Game::solve() {
  int v = prune(AI, &m_board, 2, -12, 12);
  std::cout << "prune value is " << v << std::endl;
  return v;
}

void Game::play() {
  int done = 0;
  move m;

  while (0 == done) {
    m = readinput();
    int valid = m_board.verifymove(PLAYER, m);

    while (valid != 0) {
      if (1 == valid)
        std::cout << "illegal move, re input move" << std::endl;
      else if (2 == valid)
        std::cout << "jump is mandatory than current move" << std::endl;
      m = readinput();
      valid = m_board.verifymove(PLAYER, m);
    }

    m_board.takemove(PLAYER, m);
    std::cout << m_board;

    if (m_board.isjump(PLAYER, m) == 0) {
      vec2i t = m.to;
      int c = m_board.hasjump(PLAYER, t[0], t[1]);
      while (c > 0) {
        if (c == 3) {
          std::cout << "choose next jump direction, l or r" << std::endl;
          char in = getchar();

          if (in == 'l')
            t = m_board.leftjump(PLAYER, t[0], t[1]);
          else
            t = m_board.rightjump(PLAYER, t[0], t[1]);
        }
        else if (c == 1) {
          std::cout << "left jump" << std::endl;
          t = m_board.leftjump(PLAYER, t[0], t[1]);
        }
        else {
          std::cout << "right jump" << std::endl;
          t = m_board.rightjump(PLAYER, t[0], t[1]);
        }
        std::cout << m_board;
        c = m_board.hasjump(PLAYER, t[0], t[1]);
      }
    }

    solve();
    std::cout << m_board;

  }
}

move Game::readinput() const {
  std::string m;
  std::cout << "input move, e.g. a4 to b3" << std::endl;
  getline(std::cin, m);
  move rt;
  rt.from[0] = m[0] - 'a';
  rt.from[1] = m[1] - '1';
  rt.to[0] = m[6] - 'a';
  rt.to[1] = m[7] - '1';
  return rt;
}


