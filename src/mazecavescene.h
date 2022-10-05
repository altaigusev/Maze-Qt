#pragma once
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

class MazeCaveScene : public QGraphicsScene {
    Q_OBJECT
 public:
  explicit MazeCaveScene(QObject *parent = nullptr) : QGraphicsScene{parent} {}
  QPointF m_MousePosition;
 signals:
  void mouseLeft(double x, double y);
  void mouseRight();
 private:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
      double x = event->scenePos().x();
      double y = event->scenePos().y();
      emit mouseLeft(x, y);
    }

    event->accept();
    }
};
