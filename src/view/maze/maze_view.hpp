#ifndef MAZE_AND_CAVE_VIEW_MAZE_MAZE_VIEW_HPP
#define MAZE_AND_CAVE_VIEW_MAZE_MAZE_VIEW_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>

#include "maze_controller.hpp"

namespace s21 {
class MazeWidget : public QGraphicsView {
private:
    using Matrix = std::vector<std::vector<int>>;
    using Way    = std::list<std::pair<int, int>>;
    using Coords = std::vector<std::pair<int, int>>;

public:
    MazeWidget(QGraphicsView *wdg = nullptr);
    MazeWidget(MazeWidget &&wdg) = delete;
    MazeWidget(const MazeWidget &wdg) = delete;
    ~MazeWidget();

public:
    std::pair<int, int> GetSettings() const;

public:
    void DrawMaze();
    void CreateMaze(int rows, int cols);
    void OpenFile(const QString &path);
    void SaveFile(const QString &path) const;

private:
    void DrawWay();
    void ClearScene();
    void ClearVec(QVector<QGraphicsEllipseItem *> &vec) const;
    void mousePressEvent(QMouseEvent *event) override;

private:
    MazeController maze_;

    Way way_;
    Coords coords_;
    Matrix v_walls_{{0}};
    Matrix h_walls_{{0}};

    int rows_{1};
    int cols_{1};
    float cell_size_v_{};
    float cell_size_h_{};

    const float wdg_size_{606.0f};

    QGraphicsScene* scene_;

    QVector<QGraphicsEllipseItem*> dots_;
    QVector<QGraphicsEllipseItem*> way_dots_;
};
}  // namespace s21

#endif  // MAZE_AND_CAVE_VIEW_MAZE_MAZE_VIEW_HPP
