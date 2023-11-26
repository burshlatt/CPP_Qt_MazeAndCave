#include <QApplication>
#include <QIcon>

#include "view/view.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/images/maze_icon.png"));
  s21::MazeView window;
  window.show();
  return app.exec();
}
