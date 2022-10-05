#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "row.h"

class Labyrinth {
 public:
  Labyrinth(int countAllRows, int lenghtRow);
  Labyrinth(const Labyrinth& other);
  Labyrinth(Labyrinth&& other);
  ~Labyrinth();
  Labyrinth& operator=(const Labyrinth& other);
  Labyrinth& operator=(Labyrinth&& other);

  std::vector<Row> getRow() { return m_rows; }
  void setRow(const std::vector<Row>& rows) { m_rows = rows; }
  int getLenghtRow() { return m_lenghtRow; }
  int getCountAllRows() { return m_countAllRows; }
  void setLenghtRow(int lenghtRow) { m_lenghtRow = lenghtRow; }
  void setCountAllRows(int countAllRows) { m_countAllRows = countAllRows; }
  void cleanDirect() { m_direct.clear(); }

  void labyrinthSaver(std::string path);
  void labyrinthLoader(std::string path);

  std::vector<std::pair<int, int>> findWay(int rowNumberStart,
                                           int posInRowStart, int rowNumberEnd,
                                           int posInRowEnd);

 private:
  bool recursionFind(std::tuple<bool, bool, bool, bool> side,
                     std::pair<int, int> start, std::pair<int, int> end);
  void initRows();
  std::vector<Row> m_rows;
  std::vector<std::pair<int, int>> m_direct;
  int m_lenghtRow;
  int m_countAllRows;
};
