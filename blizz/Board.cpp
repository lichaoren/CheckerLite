/*
 * Board.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: chao
 */

#include "Board.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <boost/smart_ptr.hpp>

static const char mat_data[] = {'o', 'x', '.'};
const char* Board::m_mat = mat_data;

vec2i offset[2][4] = {
    {vec2i(-1, 1), vec2i(-1, -1), vec2i(-2, 2), vec2i(-2, -2)},
    {vec2i(1, 1), vec2i(1, -1),vec2i(2, 2), vec2i(2, -2)}
};

Board::Board() :
  m_value(0)
{
  reset();
}

Board::Board(const Board& rh) :
  m_value(rh.value()),
  m_turn(PLAYER)
{
  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
      m_data[i][j] = rh.m_data[i][j];
}

Board::~Board() {
}

bool Board::hasleftmove(int side, int x, int y) const
{
  if (m_data[x][y] == side) {
    int x1 = x + offset[side][0][0],
        y1 = y + offset[side][0][1];
     if (x1 >= 0 && x1 <= 7 &&
         y1 >= 0 && y1 <= 7 &&
         m_data[x1][y1] == 2)
       return true;
  }
  return false;
}

bool Board::hasrightmove(int side, int x, int y) const
{
  if (m_data[x][y] == side) {
    int dx = x + offset[side][1][0],
        dy = y + offset[side][1][1];
     if (dx >= 0 && dx <= 7 &&
         dy >= 0 && dy <= 7 &&
         m_data[dx][dy] == 2)
       return true;
  }
  return false;
}

bool Board::hasleftjump(int side, int x, int y) const
{
  if (m_data[x][y] == side) {
    int x1 = x + offset[side][0][0],
        y1 = y + offset[side][0][1],
        x2 = x + offset[side][2][0],
        y2 = y + offset[side][2][1];

     if (x2 >= 0 && x2 <= 7 &&
         y2 >= 0 && y2 <= 7 &&
         m_data[x1][y1] == 1 - side &&
         m_data[x2][y2] == 2 )
       return true;
  }
  return false;
}

bool Board::hasrightjump(int side, int x, int y) const
{
  if (m_data[x][y] == side) {
    int x1 = x + offset[side][1][0],
        y1 = y + offset[side][1][1],
        x2 = x + offset[side][3][0],
        y2 = y + offset[side][3][1];

     if (x2 >= 0 && x2 <= 7 &&
         y2 >= 0 && y2 <= 7 &&
         m_data[x1][y1] == 1 - side &&
         m_data[x2][y2] == 2 )
       return true;
  }
  return false;
}

vec2i Board::leftmove(int side, int x, int y) {
  m_data[x][y] = 2;
  m_data[x + offset[side][0][0]][y + offset[side][0][1]] = side;
  return vec2i(x + offset[side][0][0], y + offset[side][0][1]);
}

vec2i Board::rightmove(int side, int x, int y) {
  m_data[x][y] = 2;
  m_data[x + offset[side][1][0]][y + offset[side][1][1]] = side;
  return vec2i(x + offset[side][1][0], y + offset[side][1][1]);
}

vec2i Board::leftjump(int side, int x, int y) {
  m_data[x][y] = 2;
  m_data[x + offset[side][0][0]][y + offset[side][0][1]] = 2;
  m_data[x + offset[side][2][0]][y + offset[side][2][1]] = side;
  m_value += side == PLAYER ? 1 : -1;
  return vec2i(x + offset[side][2][0], y + offset[side][2][1]);
}

vec2i Board::rightjump(int side, int x, int y) {
  m_data[x][y] = 2;
  m_data[x + offset[side][1][0]][y + offset[side][1][1]] = 2;
  m_data[x + offset[side][3][0]][y + offset[side][3][1]] = side;
   m_value += side == 1 ? 1 : -1;
  return vec2i(x + offset[side][3][0], y + offset[side][3][1]);
}

void Board::operator =(const Board& rh) {
  m_value = rh.m_value;

  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
      m_data[i][j] = rh.m_data[i][j];
}

// 0 = player, 1 = AI, 2 = blank
void Board::reset() {
  for (int i = 0; i < 8; ++i)
       for (int j = 0; j < 8; ++j)
         m_data[i][j] = 2;

     int k = 0;
     for(int i = 0; i < 8; ++i) {
       if (i < 3 || i > 4) {
         int c = i < 3 ? 1 : 0;
         for(int j = k; j < 8; j+=2) {
           m_data[i][j] = c;
         }
       }
       k = 1 - k;
     }
}

void Board::takemove(int side, move m) {
  m_data[m.from[0]][m.from[1]] = BLANK;
  if (abs(m.to[0] - m.from[0]) == 2) {
    m_data[(m.to[0]+m.from[0])/2][(m.to[1]+m.from[1])/2] = BLANK;
    m_value += side == PLAYER ? 1 : -1;
  }
  m_data[m.to[0]][m.to[1]] = side;
}

void Board::takejump(int side, move m) {
  m_data[m.to[0]][m.to[1]] = side;
  m_data[m.from[0]][m.from[1]] = BLANK;
  m_data[(m.from[0] - m.to[0])/2][(m.from[1] - m.to[1])/2] = BLANK;
  m_value += side == PLAYER ? 1 : -1;
}

//verify input move:
//return 0. accept
//       1. illegal
//       2. has jump
int Board::verifymove(int side, move m) const {
  if ( 0 != islegal(side, m) ) return 1;

  std::cout <<isjump(side, m) << " ";
  std::cout <<hasjump(side, m.from[0], m.from[1])<< std::endl;
  if (isjump(side, m) != 0
      && hasjump(side) == 0)
    return 2;

  return 0;
}

//verify a move is legal
//  1. picking a right piece
//  2. moving relative forward diagonally
//    3. destination :
//      a. in boundary
//      b. is blank
int Board::islegal(int side, move m) const {
  if (m_data[m.from[0]][m.from[1]] != side)
    return 1;
//  std::cout << "check1" <<std::endl;
  int tmp = 0;
  vec2i diff = m.to - m.from;
  for (int i = 0; i < 4; ++i)
    if (diff == offset[side][i])
      ++tmp;
//  std::cout << "check2 " << tmp <<std::endl;
  if (1 != tmp) return 1;
  if (   m.to[0] < 0 && m.to[0] > 7
      && m.to[1] < 0 && m.to[1] > 7)
    return 1;
  if (m_data[m.to[0]][m.to[1]] != BLANK)
    return 1;
  return 0;
}

int Board::isjump(int side, move m) const {
  if (2 == abs(m.from[0] - m.to[0]))
    return 0;
  return 1;
}

//check if jump available when the move is march
int Board::hasjump(int side, int x, int y) const {
  int v = 0;
  if (hasleftjump(side, x, y))
    v += 1;
  if (hasrightjump(side, x, y))
    v += 2;
  return v;
}

int Board::hasjump(int side) const {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if ((m_data[i][j] == side)
          && (hasleftjump(side, i, j) || hasrightjump(side, i, j))) {
        return 0;
      }
    }
  }
  return 1;
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
  os << "\\ 1 2 3 4 5 6 7 8" << std::endl;
  for(int i = 0; i < 8; ++i) {
    char c = 'A' + i;
    os << c << ' ';
    for (int j = 0; j < 8; ++j) {
      os << board.mat(board(i,j)) << ' ';
    }
    os << std::endl;
  }
  os << "------------------" << std::endl;
  os << "board value : " << board.value() <<std::endl;

  return os;
}
