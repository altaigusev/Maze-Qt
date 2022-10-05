#include "cellular.h"

int Cellular::getWidth() const { return width_; }

int Cellular::getHeight() const { return height_; }

Cellular::Cellular(int width, int height)
    : width_(width), height_(height), field_(nullptr) {
  if (width > MAX_FIELD_SIZE || height > MAX_FIELD_SIZE) {
    throw std::out_of_range("Max size of width or height equals 50");
  }
  field_ = new char[width_ * height_]{};
  probability_ = 1;
}

Cellular::Cellular(int width, int height, int lifeLimit, int deathLimit,
                   int probability)
    : width_(width),
      height_(height),
      lifeLimit_(lifeLimit),
      deathLimit_(deathLimit),
      probability_(probability) {
  if (width > MAX_FIELD_SIZE || height > MAX_FIELD_SIZE) {
    throw std::out_of_range("Max size of width or height equals 50");
  }

  if (width < 0 || height < 0) {
    throw std::out_of_range("Size must be more than 0");
  }
  field_ = new char[width_ * height_]{};
}

Cellular::~Cellular() {
  if (field_ != nullptr) delete field_;
}

int Cellular::operator()(int row, int col) {
  if (row > height_ || col > width_ || row < 0 || col < 0) {
    throw std::invalid_argument("Invalid index");
  }
  return field_[row * width_ + col];
}

int Cellular::getPoint(int row, int col) {
  if (row > height_ || col > width_ || row < 0 || col < 0) {
    throw std::invalid_argument("Invalid point index");
  }
  return field_[row * width_ + col];
}

void Cellular::randomFill() {
  std::mt19937 rng(std::random_device{}());
  bool default_field = 0;
  if (probability_ < 0) {
    default_field = 1;
  }
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      if (std::uniform_int_distribution<>{0, 99}(rng) < probability_)
        field_[i * width_ + j] = char(default_field);
      else
        field_[i * width_ + j] = char(!default_field);
    }
  }
}

// void Cellular::printField() {
//   for (int i = 0; i < height_; i++) {
//     for (int j = 0; j < width_; j++) {
//       if (field_[i * width_ + j])
//         std::cout << ' ';
//       else
//         std::cout << '*';
//     }
//     std::cout << std::endl;
//   }
// }

bool Cellular::life_or_dead() {
  char *tmp_field = new char[width_ * height_]{};
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      int cellSum = 0;
      for (int m = i - 1; m <= i + 1; m++) {
        for (int n = j - 1; n <= j + 1; n++) {
          if (m < 0 || n < 0 || m >= height_ || n >= width_ ||
              (m == i && n == j))
            continue;
          cellSum += field_[m * width_ + n];
        }
      }
      if (field_[i * width_ + j] == 1 && cellSum < lifeLimit_)
        tmp_field[i * width_ + j] = 0;
      else if (field_[i * width_ + j] == 0 && cellSum > deathLimit_)
        tmp_field[i * width_ + j] = 1;
      else
        tmp_field[i * width_ + j] = field_[i * width_ + j];
    }
  }
  bool isEqual = true;
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      if (field_[i * width_ + j] != tmp_field[i * width_ + j]) isEqual = false;
    }
  }
  if (isEqual) {
    delete[] tmp_field;
    return false;
  }
  delete[] field_;
  field_ = tmp_field;
  tmp_field = nullptr;

  return true;
}

char *Cellular::getField() { return field_; }

void Cellular::setLifeLimit(int limit) {
  if (limit < 0 || limit > 7)
    throw std::invalid_argument("Life limit could be between 0 and 7");
  lifeLimit_ = limit;
}

void Cellular::setDeathLimit(int limit) {
  if (limit < 0 || limit > 7)
    throw std::invalid_argument("Death limit could be between 0 and 7");
  deathLimit_ = limit;
}

void Cellular::setProbability(int prob) { probability_ = prob; }

void Cellular::writeMap(std::string filename) {
  std::ofstream caveFile;
  caveFile.open(filename);
  if (!caveFile.is_open()) {
    throw std::runtime_error("The file to writeMap the cave is not open");
  }
  caveFile << width_ << " " << height_ << "\n";
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_ - 1; j++) {
      caveFile << int(field_[i * width_ + j]) << " ";
    }
    caveFile << int(field_[i * width_ + width_ - 1]) << "\n";
  }
  caveFile.close();
}

void Cellular::readMap(std::string filename) {
  std::ifstream caveFile;
  caveFile.open(filename);
  if (!caveFile.is_open()) {
    throw std::runtime_error("The file to readMap the cave is not open");
  }
  if (!(caveFile.bad() || caveFile.peek() == EOF)) {
    caveFile >> width_;
    caveFile >> height_;
    if (field_ != nullptr) {
      delete field_;
      field_ = new char[width_ * height_]{};
    }
    for (int i = 0; i < height_ * width_; i++) {
      caveFile >> field_[i];
      field_[i] = field_[i] - 48;
    }
  }
  caveFile.close();
}
