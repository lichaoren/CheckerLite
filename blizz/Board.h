/*
 * Board.h
 *
 *  Created on: Apr 1, 2015
 *      Author: chao
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <ostream>
#include <vector>

class vec2i {
public:
  explicit vec2i(int a = 0, int b = 0) {
    v[0] = a;
    v[1] = b;
  }

  const int& operator[](const int i) const { return v[i]; }
        int& operator[](const int i)       { return v[i]; }
  vec2i& operator=(const vec2i& rh) {
    if (this != &rh) {
      v[0] = rh[0];
      v[1] = rh[1];
    }
    return *this;
  }
  const vec2i operator+(const vec2i& r) const {
    return vec2i(v[0]+r[0], v[1]+r[1]);
  }
  const vec2i operator-(const vec2i& r) const {
    return vec2i(v[0]-r[0], v[1]-r[1]);
  }
  const bool operator==(const vec2i& r) const {
    return (v[0] == r[0]) && (v[1] == r[1]);
  }
private:
  int v[2];
};


struct move {
  vec2i from, to;
};

enum {PLAYER, AI, BLANK};
class Board {
public:
//  enum {};
  Board();
  Board(const Board&);
  virtual ~Board();

  void value(const int v) { m_value = v; }
  int value() const { return m_value; }
  void turn(const int t) { m_turn = t; }
  int turn() const { return m_turn; }
  void record(const std::string& str) { m_record = str; }
  const std::string& record() const { return m_record; }
        std::string& record()       { return m_record; }
  void reset();
  void takemove(int, move);
  void takejump(int, move);
  int verifymove(int, move) const;
  int islegal(int, move) const;
  int isjump(int, move) const;
  int hasjump(int, int, int) const;
  int hasjump(int) const;
  int hasmove(int, move) const;

  bool hasleftmove(int, int, int) const;
  bool hasrightmove(int, int, int) const;
  bool hasleftjump(int, int, int) const;
  bool hasrightjump(int, int, int) const;

  vec2i leftmove(int, int, int);
  vec2i rightmove(int, int, int);
//  m_value changed after jump
  vec2i leftjump(int, int, int);
  vec2i rightjump(int, int, int);

  char mat(int i) const { return m_mat[i]; }
  int operator()(int x, int y) const { return m_data[x][y]; }
  int& operator()(int x, int y) { return m_data[x][y]; }
  void operator=(const Board&);
private:
  static const char* m_mat;
  int m_value;
  int m_turn;
  int m_data[8][8];
  std::string m_record;
};

std::ostream& operator<<(std::ostream&, const Board&);

struct lessboardkey {
  inline bool operator() (const Board* l, const Board* r) {
    return l->value() <= r->value();
  }
};

struct greaterboardkey {
  inline bool operator() (const Board* l, const Board* r) {
    return l->value() >= r->value();
  }
};
#endif /* BOARD_H_ */
