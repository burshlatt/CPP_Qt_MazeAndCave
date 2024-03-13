#ifndef MAZE_VIEW_VIEW_HPP
#define MAZE_VIEW_VIEW_HPP

#include <QFileDialog>
#include <QMainWindow>

#include <memory>

#include "maze_view.hpp"
#include "cave_view.hpp"

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
    std::unique_ptr<CaveWidget> cave_widget_;
};
}  // namespace s21

#endif  // MAZE_VIEW_VIEW_HPP
