#include "labyrinth.h"

#include <fstream>
#include <sstream>

Labyrinth::Labyrinth(int countAllRows, int lenghtRow) {
  if (lenghtRow == 0 || countAllRows == 0) {
    throw std::invalid_argument(
        "ERROR on Labyrinth. It is impossible to create a labyrinth of zero "
        "dimension.");
  }
  m_countAllRows = countAllRows;
  m_lenghtRow = lenghtRow;
  initRows();
}

Labyrinth::Labyrinth(const Labyrinth& other) { *this = other; }

Labyrinth::Labyrinth(Labyrinth&& other) { *this = std::move(other); }

Labyrinth::~Labyrinth() {}

void Labyrinth::initRows() {
  for (int i = 0; i < m_countAllRows; ++i) {
    if (i == 0) {
      Row first(m_lenghtRow, 0);
      first.generationRow();
      m_rows.push_back(first);
    } else if (i == m_countAllRows - 1) {
      Row last(m_rows[i - 1]);
      last.generationRow();
      last.generationLastRow();
      m_rows.push_back(last);
    } else {
      Row innerRow(m_rows[i - 1]);
      innerRow.generationRow();
      m_rows.push_back(innerRow);
    }
  }
}

void Labyrinth::labyrinthSaver(std::string path) {
  std::fstream fs;
  fs.open(path, std::fstream::out);

  if (!fs.is_open()) {
    throw std::runtime_error("The file to save the maze is not open");
  }

  fs << m_countAllRows << " " << m_lenghtRow;
  fs << "\n";

  for (int i = 0; i < m_countAllRows; ++i) {
    std::vector<bool> right = m_rows[i].getRightSeparator();

    for (int j = 0; j < (int)right.size(); ++j) {
      fs << right[j];
      if (j < (int)right.size() - 1) {
        fs << " ";
      }
    }

    fs << "\n";
  }
  fs << "\n";

  for (int i = 0; i < m_countAllRows; ++i) {
    std::vector<bool> down = m_rows[i].getDownSeparator();

    for (int j = 0; j < (int)down.size(); ++j) {
      fs << down[j];
      if (j < (int)down.size() - 1) {
        fs << " ";
      }
    }

    fs << "\n";
  }
  fs.close();
}

void Labyrinth::labyrinthLoader(std::string path) {
  std::ifstream fs;
  fs.open(path, std::fstream::in);

  if (!fs.is_open()) {
    throw std::runtime_error("The file to load the maze is not open");
  }

  int countAllRows = 1;
  fs >> countAllRows;
  int lenghtRow = 1;
  fs >> lenghtRow;

  if (countAllRows < 0 || countAllRows > 50 || lenghtRow < 0 ||
      lenghtRow > 50) {
    throw std::invalid_argument("ERROR on labyrinthLoader. File incorrect!");
  }
  Labyrinth loaderLab(countAllRows, lenghtRow);

  for (int i = 0; i < countAllRows; ++i) {
    std::vector<bool> right;

    for (int j = 0; j < lenghtRow; ++j) {
      int tmp;
      fs >> tmp;
      right.push_back(tmp);
    }

    loaderLab.m_rows[i].setRightSeparator(right);
  }

  for (int i = 0; i < countAllRows; ++i) {
    std::vector<bool> down;

    for (int j = 0; j < lenghtRow; ++j) {
      int tmp;
      fs >> tmp;
      down.push_back(tmp);
    }

    loaderLab.m_rows[i].setDownSeparator(down);
  }

  *this = std::move(loaderLab);

  fs.close();
}

Labyrinth& Labyrinth::operator=(const Labyrinth& other) {
  if (this == &other) {
    throw std::invalid_argument(
        "ERROR on operator=(Row &other). The current and added Row are one "
        "object");
  }
  this->m_lenghtRow = other.m_lenghtRow;
  this->m_countAllRows = other.m_countAllRows;
  this->m_rows = other.m_rows;
  return *this;
}

Labyrinth& Labyrinth::operator=(Labyrinth&& other) {
  if (this == &other) {
    throw std::invalid_argument(
        "ERROR on operator=(Row &other). The current and added Row are one "
        "object");
  }
  std::swap(this->m_lenghtRow, other.m_lenghtRow);
  std::swap(this->m_countAllRows, other.m_countAllRows);
  std::swap(this->m_rows, other.m_rows);
  return *this;
}

std::vector<std::pair<int, int>> Labyrinth::findWay(int rowNumberStart,
                                                    int posInRowStart,
                                                    int rowNumberEnd,
                                                    int posInRowEnd) {
  if (rowNumberEnd > m_countAllRows || rowNumberEnd < 0) {
    throw std::invalid_argument(
        "ERROR on findWay. The point is outside the maze.");
  }

  if (posInRowEnd > m_lenghtRow || posInRowEnd < 0) {
    throw std::invalid_argument(
        "ERROR on findWay. The point is outside the maze.");
  }

  if (rowNumberStart > m_countAllRows || rowNumberStart < 0) {
    throw std::invalid_argument(
        "ERROR on findWay. The point is outside the maze.");
  }

  if (posInRowStart > m_lenghtRow || posInRowStart < 0) {
    throw std::invalid_argument(
        "ERROR on findWay. The point is outside the maze.");
  }

  bool tmp =
      recursionFind({true, true, true, true}, {rowNumberStart, posInRowStart},
                    {rowNumberEnd, posInRowEnd});
  if (tmp == false) {
    throw std::invalid_argument("ERROR on findWay. Invalid maze.");
  }
  return m_direct;
}

bool Labyrinth::recursionFind(std::tuple<bool, bool, bool, bool> side,
                              std::pair<int, int> start,
                              std::pair<int, int> end) {
  bool rez = false;

  if (start.first - 1 >= 0 &&
      m_rows[start.first - 1].getDownSeparator()[start.second] == false &&
      std::get<0>(side) == true) {
    if (start.first - 1 == end.first && start.second == end.second) {
      rez = true;
      m_direct.push_back({start.first - 1, start.second});
      m_direct.push_back({start.first, start.second});
      return rez;
    }
    rez =
        recursionFind({true, false, true, true},
                      {start.first - 1, start.second}, {end.first, end.second});
  }

  if (start.first + 1 < m_countAllRows &&
      m_rows[start.first].getDownSeparator()[start.second] == false &&
      std::get<1>(side) == true && rez == false) {
    if (start.first + 1 == end.first && start.second == end.second) {
      rez = true;
      m_direct.push_back({start.first + 1, start.second});
      m_direct.push_back({start.first, start.second});
      return rez;
    }
    rez =
        recursionFind({false, true, true, true},
                      {start.first + 1, start.second}, {end.first, end.second});
  }

  if (start.second - 1 >= 0 &&
      m_rows[start.first].getRightSeparator()[start.second - 1] == false &&
      std::get<2>(side) == true && rez == false) {
    if (start.first == end.first && start.second - 1 == end.second) {
      rez = true;
      m_direct.push_back({start.first, start.second - 1});
      m_direct.push_back({start.first, start.second});
      return rez;
    }
    rez =
        recursionFind({true, true, true, false},
                      {start.first, start.second - 1}, {end.first, end.second});
  }

  if (start.second + 1 < m_lenghtRow &&
      m_rows[start.first].getRightSeparator()[start.second] == false &&
      std::get<3>(side) == true && rez == false) {
    if (start.first == end.first && start.second + 1 == end.second) {
      rez = true;
      m_direct.push_back({start.first, start.second + 1});
      m_direct.push_back({start.first, start.second});
      return rez;
    }
    rez =
        recursionFind({true, true, false, true},
                      {start.first, start.second + 1}, {end.first, end.second});
  }

  if (rez == true) {
    m_direct.push_back({start.first, start.second});
  }
  return rez;
}
