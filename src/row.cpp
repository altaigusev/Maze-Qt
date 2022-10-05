#include "row.h"

#include <algorithm>
#include <iterator>
#include <random>
#include <set>

Row::Row(int lenght, int number) {
  lenghtRow = lenght;
  numRow = number;
}

Row::Row(const Row& prevRow) { *this = prevRow; }

Row::Row(Row&& otherRow) { *this = std::move(otherRow); }

Row::~Row() {}

void Row::initRow() {
  if (numRow == 0) {
    for (int i = 0; i < lenghtRow; ++i) {
      numInRow.push_back(i + 1);
    }
    numRow += 1;
  } else {
    numRow += 1;
    rightSeparator.clear();
    for (int i = 0; i < (int)numInRow.size(); ++i) {
      if (downSeparator[i] == true) {
        numInRow[i] = *max_element(numInRow.begin(), numInRow.end()) + 1;
      }
    }
    downSeparator.clear();
  }
}

void Row::generationdownSeparator() {
  std::set<int> element;

  for (int i = 0; i < lenghtRow; ++i) {
    element.insert(numInRow[i]);
    downSeparator.push_back(false);
  }

  for (int i = 0; i < lenghtRow; ++i) {
    if (i == lenghtRow - 1) {
      if (element.count(numInRow[i]) == 1) downSeparator[i] = false;

    } else {
      if (element.count(numInRow[i]) == 1) {
        int countEqualsNum = 0;
        int countTrueDown = 0;
        int lastPosition = i;

        for (int j = i; j < lenghtRow; ++j) {
          if (numInRow[i] == numInRow[j]) {
            ++countEqualsNum;
            downSeparator[j] = randomChenge();
            if (downSeparator[j]) ++countTrueDown;
            lastPosition = j;
          }
        }

        if (countEqualsNum == countTrueDown)
          downSeparator[lastPosition] = false;
        element.erase(numInRow[i]);
      }
    }
  }
}

void Row::generationRightSeparator() {
  for (int i = 0; i < lenghtRow - 1; ++i) {
    if (randomChenge() == true || numInRow[i] == numInRow[i + 1]) {
      rightSeparator.push_back(true);
    } else {
      int check = numInRow[i + 1];
      for (int j = 0; j < lenghtRow; ++j) {
        if (numInRow[j] == check) {
          numInRow[j] = numInRow[i];
        }
      }
      rightSeparator.push_back(false);
    }
  }
  rightSeparator.push_back(true);
}

void Row::generationRow() {
  initRow();
  generationRightSeparator();
  generationdownSeparator();
}

void Row::generationLastRow() {
  downSeparator.clear();
  for (int i = 0; i < lenghtRow; ++i) {
    downSeparator.push_back(true);
  }

  for (int i = 0; i < lenghtRow - 1; ++i) {
    if (numInRow[i] != numInRow[i + 1]) {
      rightSeparator[i] = false;
      int check = numInRow[i + 1];

      for (int j = 0; j < lenghtRow; ++j) {
        if (numInRow[j] == check) {
          numInRow[j] = numInRow[i];
        }
      }
    }
  }

  rightSeparator[lenghtRow - 1] = true;
}

bool Row::randomChenge() {
  std::mt19937 rng(std::random_device{}());
  return std::uniform_int_distribution<>{0, 1}(rng);
}

// void Row::printRow() {
//   for (int i = 0; i < lenghtRow; ++i) {
//     if (rightSeparator[i] == true && downSeparator[i] == true) {
//       std::cout << "_|";
//     } else if (rightSeparator[i] == true && downSeparator[i] == false) {
//       std::cout << " |";
//     } else if (rightSeparator[i] == false && downSeparator[i] == true) {
//       std::cout << "_ ";
//     } else {
//       std::cout << "  ";
//     }
//   }

//   std::cout << std::endl;
// }

Row& Row::operator=(const Row& other) {
  if (this == &other) {
    throw std::invalid_argument(
        "ERROR on operator=(Row &other). The current and added Row are one "
        "object");
  }

  this->lenghtRow = other.lenghtRow;
  this->numRow = other.numRow;
  this->numInRow = other.numInRow;
  this->downSeparator = other.downSeparator;
  this->rightSeparator = other.rightSeparator;
  return *this;
}

Row& Row::operator=(Row&& other) {
  if (this == &other) {
    throw std::invalid_argument(
        "ERROR on operator=(Row &&other). The current and added Row are one "
        "object");
  }

  std::swap(this->lenghtRow, other.lenghtRow);
  std::swap(this->numRow, other.numRow);
  std::swap(this->numInRow, other.numInRow);
  std::swap(this->downSeparator, other.downSeparator);
  std::swap(this->rightSeparator, other.rightSeparator);
  return *this;
}
