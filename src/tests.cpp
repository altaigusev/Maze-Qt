#include <gtest/gtest.h>

#include "cellular.h"
#include "labyrinth.h"
#include "row.h"

TEST(rowTest, test1) {
  Row newRow1(50, 50);
  Row newRow2(10, 10);
  newRow1 = newRow2;
}

TEST(mazeTest, test1) {
  Labyrinth newLab(50, 50);
  newLab.labyrinthLoader("MazeSamples/123.maze");
  ASSERT_THROW(newLab.findWay(0, 1, 45, 3), std::invalid_argument);
}

TEST(mazeTest, test2) {
  Labyrinth newLab(50, 50);
  ASSERT_THROW(newLab.labyrinthLoader("MazeSamples/123"), std::runtime_error);
}

TEST(mazeTest, test3) {
  ASSERT_THROW(Labyrinth newLab(0, 0), std::invalid_argument);
}

TEST(mazeTest, test4) {
  Labyrinth newLab(10, 10);
  newLab.labyrinthLoader("MazeSamples/123.maze");
  newLab.findWay(0, 1, 9, 5);
  newLab.labyrinthSaver("MazeSamples/456.maze");
}

TEST(mazeTest, test5) {
  Labyrinth newLab(10, 10);
  newLab.labyrinthLoader("MazeSamples/123.maze");
  newLab.findWay(9, 9, 1, 1);
}

TEST(mazeTest, test6) {
  Labyrinth newLab(10, 10);
  newLab.labyrinthLoader("MazeSamples/123.maze");
  ASSERT_THROW(newLab.findWay(14, 9, 1, 1), std::invalid_argument);
}

TEST(mazeTest, test7) {
  Labyrinth newLab(10, 10);
  newLab.labyrinthLoader("MazeSamples/123.maze");
  newLab.findWay(1, 9, 9, 1);
}

TEST(mazeTest, test8) {
  Labyrinth newLab(10, 10);
  newLab.labyrinthLoader("MazeSamples/123.maze");
  ASSERT_THROW(newLab.findWay(1, 9, 14, 1), std::invalid_argument);
}

TEST(mazeTest, test9) {
  Labyrinth newLab(10, 10);
  newLab.labyrinthLoader("MazeSamples/123.maze");
  ASSERT_THROW(newLab.findWay(1, 567, 1, 6), std::invalid_argument);
}

TEST(mazeTest, test10) {
  Labyrinth newLab(10, 10);
  newLab.labyrinthLoader("MazeSamples/123.maze");
  newLab.findWay(1, 9, 4, 1);
}

TEST(mazeTest, test11) {
  Labyrinth newLab(10, 10);
  Labyrinth newLab1(20, 20);
  newLab = newLab1;
}

TEST(cellularTest, test1) {
  ASSERT_THROW(Cellular newCell(1, 51), std::out_of_range);
}

TEST(cellularTest, test2) {
  ASSERT_THROW(Cellular newCell(10, 51, 3, 5, 2), std::out_of_range);
}

TEST(cellularTest, test3) {
  Cellular newCell(10, 10);
  newCell.randomFill();
  ASSERT_EQ(newCell.getHeight(), 10);
  ASSERT_EQ(newCell.getWidth(), 10);
}

TEST(cellularTest, test4) {
  ASSERT_THROW(Cellular newCell(-4, 1, 3, 5, 2), std::out_of_range);
}

TEST(cellularTest, test5) {
  Cellular newCell(50, 50, 3, 5, 2);
  newCell.readMap("CaveSamples/map50x50.cave");
  ASSERT_EQ(newCell(3, 4), 1);
  ASSERT_EQ(newCell.getPoint(3, 4), 1);
  newCell.life_or_dead();
}

TEST(cellularTest, test6) {
  Cellular newCell(50, 50, 3, 5, 2);
  newCell.readMap("CaveSamples/map50x50.cave");
  ASSERT_THROW(newCell.getPoint(3, 67), std::invalid_argument);
}

TEST(cellularTest, test7) {
  Cellular newCell(50, 50, 3, 5, 2);
  newCell.readMap("CaveSamples/map50x50.cave");
  newCell.setLifeLimit(6);
  newCell.setDeathLimit(6);
  newCell.setProbability(6);
}

TEST(cellularTest, test8) {
  Cellular newCell(50, 50, 3, 5, 2);
  newCell.readMap("CaveSamples/map50x50.cave");
  ASSERT_THROW(newCell.setLifeLimit(56), std::invalid_argument);
  ASSERT_THROW(newCell.setDeathLimit(56), std::invalid_argument);
}

TEST(cellularTest, test9) {
  Cellular newCell(50, 50);
  newCell.randomFill();
  newCell.writeMap("CaveSamples/test.cave");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
