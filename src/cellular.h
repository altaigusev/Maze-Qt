#pragma once
#include <fstream>
// #include <iostream>
#include <random>

class Cellular {
 private:
  int width_;
  int height_;
  char *field_;
  static constexpr int MAX_FIELD_SIZE = 50;
  int lifeLimit_;
  int deathLimit_;
  int probability_;

 public:
  Cellular(int width, int height);
  Cellular(int width, int height, int lifeLimit, int deathLimit,
           int probability);
  Cellular(const Cellular &other) = delete;
  Cellular(Cellular &&other) = delete;
  Cellular &operator=(Cellular &other) = delete;
  ~Cellular();

  int operator()(int row, int col);
  int getPoint(int row, int col);
  void randomFill();
  // void printField();
  bool life_or_dead();
  char *getField();
  void setLifeLimit(int limit);
  void setDeathLimit(int limit);
  void setProbability(int prob);
  void writeMap(std::string filename);
  void readMap(std::string filename);
  int getWidth() const;
  int getHeight() const;
};
