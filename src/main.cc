#include <QApplication>
#include <QIcon>

#include "view.hpp"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  app.setWindowIcon(QIcon(":/images/maze_icon.png"));

  s21::View window;
  window.show();

  return app.exec();
}
