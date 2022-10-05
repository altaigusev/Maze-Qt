#pragma once

#include <vector>

class Row {
 public:
  Row(int lenght, int number);
  Row(const Row& prevRow);
  Row(Row&& otherRow);
  ~Row();

  void generationRow();
  void generationLastRow();

  Row& operator=(const Row& other);
  Row& operator=(Row&& other);

  std::vector<bool> getRightSeparator() { return rightSeparator; }
  std::vector<bool> getDownSeparator() { return downSeparator; }

  void setRightSeparator(const std::vector<bool>& right) {
    rightSeparator = right;
  }
  void setDownSeparator(const std::vector<bool>& down) { downSeparator = down; }

  // void printRow();

 private:
  std::vector<int> numInRow;
  std::vector<bool> rightSeparator;
  std::vector<bool> downSeparator;
  bool randomChenge();
  void initRow();
  void generationRightSeparator();
  void generationdownSeparator();
  int lenghtRow;
  int numRow;
};
