#ifndef MAZE_AND_CAVE_VIEW_VIEW_HPP
#define MAZE_AND_CAVE_VIEW_VIEW_HPP

#include <QTimer>
#include <QTabBar>
#include <QFileDialog>
#include <QMainWindow>

#include <memory>

#include "maze_view.hpp"
#include "cave_view.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {
class View : public QMainWindow {
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    ~View();

private:
    void MazeSaveConfig();
    void MazeLoadConfig();
    void GenerateMaze();

    Settings GetSettings();

    void CaveSaveConfig();
    void CaveLoadConfig();
    void GenerateCave();
    void GenerateStep();
    void GenerateStepTimer();

    void WindowChange();

private:
    Ui::View* ui_;
    QTimer* timer_;

    std::unique_ptr<MazeWidget> maze_widget_;
    std::unique_ptr<CaveWidget> cave_widget_;
};
}  // namespace s21

#endif  // MAZE_AND_CAVE_VIEW_VIEW_HPP
