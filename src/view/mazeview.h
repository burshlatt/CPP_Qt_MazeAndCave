#ifndef A1_MAZE_1_VIEW_MAZEVIEW_H
#define A1_MAZE_1_VIEW_MAZEVIEW_H

#include <QFileDialog>
#include <QMainWindow>
#include <memory>

#include "mazewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MazeView;
}
QT_END_NAMESPACE

namespace s21 {
class MazeView : public QMainWindow {
    Q_OBJECT

    public:
        MazeView(QWidget *parent = nullptr);
        ~MazeView();

    private:
        Ui::MazeView *ui_;
        std::unique_ptr<MazeWidget> maze_widget_;
};
}  // namespace s21

#endif  // A1_MAZE_1_VIEW_MAZEVIEW_H
