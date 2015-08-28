/*
 * Solution.h
 *
 *  Created on: Jan 25, 2015
 *      Author: chao
 */


#ifndef SOLUTION_H_
#define SOLUTION_H_

#define N 4

#include <string>

class Solution {
public:
  Solution();
  ~Solution();

  void LoadBoard(const char*);
  void Play();

  void SetDict(const char*);
  void SetOutput(const char*);
  void SetSolver(const int);

private:
  static const int m_dx[8];
  static const int m_dy[8];
  std::string m_dict;
  std::string m_output;

  char m_board[N][N];
  bool m_visited[N][N];

  bool FindPath(const char*, const int,
                int, int, int);
};

#endif /* SOLUTION_H_ */
