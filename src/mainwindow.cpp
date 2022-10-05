#include "mainwindow.h"

#include <QDebug>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->stackedWidget->setCurrentIndex(0);
  scene = new MazeCaveScene();
  scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

  ui->mainGraphicView->setScene(scene);
  ui->mainGraphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->mainGraphicView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  width_ = ui->spinBoxMazeWidth->value();
  height_ = ui->spinBoxMazeHeight->value();

  widthCaves_ = ui->spinBoxMazeWidth->value();
  heightCaves_ = ui->spinBoxMazeHeight->value();
  birthLimit_ = ui->spinBoxBirthLimit->value();
  deathLimit_ = ui->spinBoxDeathLimit->value();
  probability_ = ui->live_dead_cells_rate_slider->value();
  ui->statusbar->addWidget(&errorMessage);
  try {
    maze = new Labyrinth(height_, width_);
    cells = new Cellular(widthCaves_, heightCaves_, birthLimit_, deathLimit_,
                         probability_);
  } catch (std::invalid_argument e) {
    ui->statusbar->clearMessage();
    errorMessage.setText(e.what());
  } catch (std::out_of_range e) {
    ui->statusbar->clearMessage();
    errorMessage.setText(e.what());
  }
  pointCount = 0;
  iteration_ = 0;
  connect(scene, SIGNAL(mouseLeft(double, double)), this,
          SLOT(firstPointSlot(double, double)));
  linesPainted = false;
}

MainWindow::~MainWindow() {
  if (cells != nullptr) delete cells;
  delete ui;
}

void MainWindow::on_mazeRadioButton_clicked() {
  errorMessage.clear();
  if (cells != nullptr) {
    delete cells;
    cells = nullptr;
  }

  if (maze != nullptr) {
    delete maze;
    maze = nullptr;
  }

  if (timer != nullptr) {
    timer->stop();
    ui->autoStepButton->setText("Авто");
  }

  linesPainted = false;
  scene->clear();
  ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_cavesRadioButton_clicked() {
  iteration_ = 0;
  errorMessage.clear();
  if (cells != nullptr) {
    delete cells;
    cells = nullptr;
  }

  if (maze != nullptr) {
    delete maze;
    maze = nullptr;
  }

  if (timer != nullptr) {
    timer->stop();
    ui->autoStepButton->setText("Авто");
  }
  linesPainted = false;
  scene->clear();
  ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::paintLines() {
  double w_step = 500.0 / double(width_);
  double h_step = 500.0 / double(height_);
  QPen pen(Qt::white, 2);
  scene->addRect(0, 0, 500, 500, pen);
  if (maze != nullptr) {
    std::vector<Row> tmp = maze->getRow();
    for (int i = 0; i < maze->getCountAllRows(); ++i) {
      for (int j = 0; j < maze->getLenghtRow(); ++j) {
        bool rightSep = tmp.at(i).getRightSeparator().at(j);
        bool downSep = tmp.at(i).getDownSeparator().at(j);
        if (rightSep == true) paintRight(w_step, h_step, j, i, pen);
        if (downSep == true) paintDown(w_step, h_step, j, i, pen);
      }
    }
  }
}

void MainWindow::paintRight(double w_step, double h_step, int i, int j,
                            QPen pen) {
  scene->addLine(w_step * (i + 1), h_step * j, w_step * (i + 1),
                 h_step * (j + 1), pen);
}

void MainWindow::paintDown(double w_step, double h_step, int i, int j,
                           QPen pen) {
  scene->addLine(w_step * (i), h_step * (j + 1), w_step * (i + 1),
                 h_step * (j + 1), pen);
}

void MainWindow::paintWay(double w_step, double h_step,
                          std::vector<std::pair<int, int>> vec, QPen pen) {
  for (int a = (int)vec.size() - 1; a > 0; --a) {
    int i1 = vec.at(a).second;
    int j1 = vec.at(a).first;
    int i2 = vec.at(a - 1).second;
    int j2 = vec.at(a - 1).first;
    int coordX1 = w_step * i1 + w_step / 2;
    int coordY1 = h_step * j1 + h_step / 2;
    int coordX2 = w_step * i2 + w_step / 2;
    int coordY2 = h_step * j2 + h_step / 2;
    scene->addLine(coordX1, coordY1, coordX2, coordY2, pen);
  }
}

void MainWindow::paintCells() {
  double w_step = 500.0 / double(widthCaves_);
  double h_step = 500.0 / double(heightCaves_);
  QPen pen(Qt::white, 1);
  QBrush brushDead(Qt::white, Qt::SolidPattern);
  QBrush brushAlive(Qt::gray, Qt::SolidPattern);
  for (int i = 0; i < widthCaves_; i++) {
    for (int j = 0; j < heightCaves_; j++) {
      QBrush brush = brushDead;
      try {
        if (cells->getPoint(j, i) == 0) brush = brushAlive;
      } catch (std::invalid_argument e) {
        ui->statusbar->clearMessage();
        errorMessage.setText(e.what());
      }

      scene->addRect(w_step * i, h_step * j, w_step * (i + 1), h_step * (j + 1),
                     pen, brush);
    }
  }
}

void MainWindow::timedPaintCells() {
  if (cells != nullptr) {
    if (cells->life_or_dead() == true) {
      paintCells();
      iteration_++;
    } else {
      ui->autoStepButton->setText("Авто");
      timer->stop();
      iteration_ = 0;
    }
  }
  if (iteration_ > 50) {
    ui->autoStepButton->setText("Авто");
    timer->stop();
    iteration_ = 0;
  }
}

void MainWindow::firstPointSlot(double x, double y) {
  if (linesPainted == true && ui->mazeRadioButton->isChecked()) {
    double w_step = 500.0 / double(width_);
    double h_step = 500.0 / double(height_);
    int xx = x / w_step;
    int yy = y / h_step;
    QPen pen(Qt::green, 2);
    if (pointCount == 0) {
      firstPoint.first = xx;
      firstPoint.second = yy;
      scene->addLine(w_step * (xx) + w_step / 2, h_step * yy + h_step / 2,
                     w_step * (xx) + w_step / 2, h_step * yy + h_step / 2, pen);
      ++pointCount;
    } else if (pointCount == 1) {
      secondPoint.first = xx;
      secondPoint.second = yy;
      scene->addLine(w_step * (xx) + w_step / 2, h_step * yy + h_step / 2,
                     w_step * (xx) + w_step / 2, h_step * yy + h_step / 2, pen);

      if (maze != nullptr) {
        try {
          std::vector<std::pair<int, int>> rez =
              maze->findWay(firstPoint.second, firstPoint.first,
                            secondPoint.second, secondPoint.first);
          paintWay(w_step, h_step, rez, pen);
        } catch (std::invalid_argument e) {
          errorMessage.setText(e.what());
        }
      }
      ++pointCount;
    } else if (pointCount == 2) {
      scene->clear();
      paintLines();
      pointCount = 0;
      if (maze != nullptr) maze->cleanDirect();
    }
  }
}

void MainWindow::secondPointSlot(double x, double y) {
  double w_step = 500.0 / double(widthCaves_);
  double h_step = 500.0 / double(heightCaves_);
  int xx = x / w_step;
  int yy = y / h_step;
  secondPoint.first = xx;
  secondPoint.second = yy;
}

void MainWindow::on_generateMazeButton_clicked() {
  scene->clear();
  width_ = ui->spinBoxMazeWidth->value();
  height_ = ui->spinBoxMazeHeight->value();
  if (maze != nullptr) delete maze;
  try {
    maze = new Labyrinth(height_, width_);
    paintLines();
    linesPainted = true;
  } catch (std::invalid_argument e) {
    errorMessage.setText(e.what());
  }
}

void MainWindow::on_generateCavesButton_clicked() {
  scene->clear();
  widthCaves_ = ui->spinBoxCavesWidth->value();
  heightCaves_ = ui->spinBoxCavesHeight->value();
  birthLimit_ = ui->spinBoxBirthLimit->value();
  deathLimit_ = ui->spinBoxDeathLimit->value();
  probability_ = ui->live_dead_cells_rate_slider->value();
  if (cells != nullptr) delete cells;
  try {
    cells = new Cellular(widthCaves_, heightCaves_, birthLimit_, deathLimit_,
                         probability_);
    cells->randomFill();
    paintCells();
  } catch (std::out_of_range e) {
    errorMessage.setText(e.what());
  } catch (std::invalid_argument e) {
    errorMessage.setText(e.what());
  }
}

void MainWindow::on_loadCavesButton_clicked() {
  QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                  QDir::homePath(), "*.cave");
  if (!filepath.isEmpty()) {
    try {
      cells->readMap(filepath.toStdString());
      ui->spinBoxCavesHeight->setValue(cells->getHeight());
      heightCaves_ = cells->getHeight();
      ui->spinBoxCavesWidth->setValue(cells->getWidth());
      widthCaves_ = cells->getWidth();
      paintCells();
    } catch (std::out_of_range e) {
      errorMessage.setText(e.what());
    } catch (std::invalid_argument e) {
      errorMessage.setText(e.what());
    } catch (std::runtime_error e) {
      errorMessage.setText(e.what());
    }
  }
}

void MainWindow::on_saveCavesButton_clicked() {
  QString filepath = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                  QDir::homePath(), "*.cave");
  if (!filepath.isEmpty()) {
    try {
      cells->writeMap(filepath.toStdString());
    } catch (std::runtime_error e) {
      errorMessage.setText(e.what());
    }
  }
}

void MainWindow::on_loadMazeButton_clicked() {
  QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                  QDir::homePath(), "*.maze");
  if (!filepath.isEmpty() && maze != nullptr) {
    try {
      maze->labyrinthLoader(filepath.toStdString());
      scene->clear();
      ui->spinBoxMazeHeight->setValue(maze->getCountAllRows());
      ui->spinBoxMazeWidth->setValue(maze->getLenghtRow());
      width_ = ui->spinBoxMazeWidth->value();
      height_ = ui->spinBoxMazeHeight->value();
      paintLines();
      linesPainted = true;
    } catch (std::invalid_argument e) {
      errorMessage.setText(e.what());
    } catch (std::runtime_error e) {
      errorMessage.setText(e.what());
    }
  }
}

void MainWindow::on_saveMazeButton_clicked() {
  QString filepath = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                  QDir::homePath(), "*.maze");
  if (!filepath.isEmpty() && maze != nullptr) {
    try {
      maze->labyrinthSaver(filepath.toStdString());
    } catch (std::runtime_error e) {
      errorMessage.setText(e.what());
    }
  }
}

void MainWindow::on_oneStepButton_clicked() {
  if (cells != nullptr) {
    cells->life_or_dead();
    paintCells();
  }
}

void MainWindow::on_autoStepButton_clicked() {
  if (cells != nullptr) {
    unsigned long delay = ui->spinBoxDelay->value();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timedPaintCells()));
    timer->start(delay);
    ui->autoStepButton->setText("Труд");
  }
}
