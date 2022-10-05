#pragma once

#include <QFileDialog>
#include <QGraphicsScene>
#include <QLabel>
#include <QMainWindow>
#include <QTimer>

#include "cellular.h"
#include "labyrinth.h"
#include "mazecavescene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_mazeRadioButton_clicked();
  void on_cavesRadioButton_clicked();
  void on_generateMazeButton_clicked();
  void on_generateCavesButton_clicked();
  void on_loadCavesButton_clicked();
  void on_saveCavesButton_clicked();
  void on_loadMazeButton_clicked();
  void on_saveMazeButton_clicked();
  void on_oneStepButton_clicked();
  void on_autoStepButton_clicked();
  void timedPaintCells();
  void firstPointSlot(double x, double y);
  void secondPointSlot(double x, double y);

 private:
  Ui::MainWindow *ui;
  MazeCaveScene *scene;
  QLabel errorMessage;

  // maze block
  int width_, height_;
  Labyrinth *maze;
  void paintLines();
  void paintRight(double w_step, double h_step, int i, int j, QPen pen);
  void paintDown(double w_step, double h_step, int i, int j, QPen pen);
  void paintWay(double w_step, double h_step,
                std::vector<std::pair<int, int>> vec, QPen pen);
  std::pair<int, int> firstPoint;
  std::pair<int, int> secondPoint;
  int pointCount;
  bool linesPainted;

  // caves block
  int widthCaves_, heightCaves_, deathLimit_, birthLimit_, probability_,
      iteration_;
  void paintCells();

  Cellular *cells;

  QTimer *timer;
};
