/*
 * Solution.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: chao
 */

#include "Solution.h"

#include <iostream>
#include <cstring>
#include <fstream>

const int Solution::m_dx[8] = {-1,  0,  1, 1, 1, 0, -1, -1};
const int Solution::m_dy[8] = {-1, -1, -1, 0, 1, 1,  1,  0};

Solution::Solution() :
    m_dict(""),
    m_output("")
{
}

Solution::~Solution()
{
}

void Solution::LoadBoard(const char *src)
{
  std::ifstream ifs;
  int lineCount = 0, rowCount = 0;

  ifs.open(src, std::ifstream::in);

  while (!ifs.eof()) {
    std::string line;
    if (getline(ifs, line)) {
      rowCount = 0;
      for (std::string::const_iterator citer = line.begin(); citer != line.end(); ++citer) {
        if (*citer == ' ')
          continue;
        if (*citer >= 'A' && *citer <= 'Z')
          m_board[lineCount][rowCount++] = *citer + 0x20;
        else if (*citer >= 'a' && *citer <= 'z')
          m_board[lineCount][rowCount++] = *citer + 0x20;
      }
      if (N != rowCount)
        perror("invalid board\n");
    }
    ++lineCount;
  }

  if (N != lineCount)
    perror("invalid board\n");
}

void Solution::SetDict(const char *src)
{
  m_dict = src;
}

void Solution::SetOutput(const char* des)
{
  m_output = des;
}

void Solution::Play()
{
  std::ifstream ifs;
  std::ofstream ofs;
  std::string curr_word, prev_word;
  int prev_size = 0;
  int i, j, k, l, m;
  bool path[N*N][N][N];
  bool found = false;

  ifs.open(m_dict.c_str(), std::istream::in);
  ofs.open(m_output.c_str(), std::ios::out);

  while (getline(ifs, curr_word)) {
    std::string word;

    int size = curr_word.length() - 1;
    if (size < 3) continue;

    for (i = 0; i < size; ++i) {
      if (curr_word[i] == 'q' && i+1 < size && curr_word[i+1] == 'u') {
        word += 'q';
        ++i;
      }
      else word.append(std::string(1, curr_word[i]));
    }

    size = word.length();

    for (m = 0; m < size && m < prev_size; ++m)
      if (word[m] != prev_word[m]) break;

    if (0 == m) {
      for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
          if (m_board[i][j] == word[0])
            path[m][i][j] = true;
          else
            path[m][i][j] = false;
        }
      }
    }

    m = 1;

    for (k = m; k < size; ++k) {
      for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {

          path[k][i][j] = false;

          if (m_board[i][j] != word[k]) continue;

          for (l = 0; l < 8 && !path[k][i][j]; ++l) {

            int xx = i + m_dx[l];
            int yy = j + m_dy[l];

            if (xx < 0 || xx >= N ||
                yy < 0 || yy >= N) continue;

            if (path[k-1][xx][yy])
              path[k][i][j] = true;
          }
        }
      }
    }

    for (i = 0; i < N; ++i)
      for (j = 0; j < N; ++j)
        if (path[size-1][i][j])
          found = true;

    if (found) {
      for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
          if (word[0] != m_board[i][j])
            continue;

          memset(m_visited, false, sizeof(m_visited));
          m_visited[i][j] = true;

          if (FindPath(word.c_str(), size, 0, i, j)) {
            ofs << curr_word <<'\n';
            i = j = N;
          }
        }
      }
    }

    found = false;
    prev_word = word;
    prev_size = size;
  }
}

bool Solution::FindPath(const char* word, const int size,
                        int idx, int x, int y) {
  bool result = false;

  if (idx == size - 1)
    return true;

  for (int i = 0; i < 8; ++i) {
    int xx = x + m_dx[i];
    int yy = y + m_dy[i];

    if (xx >= 0 && xx < N && yy >= 0 && yy < N &&
        !m_visited[xx][yy] &&
        word[idx+1] == m_board[xx][yy]) {

      ++idx;
      m_visited[xx][yy] = true;

      result = FindPath(word, size, idx, xx, yy);

      if (result) return true;

      --idx;
      m_visited[xx][yy] = false;
    }
  }

  return result;
}
