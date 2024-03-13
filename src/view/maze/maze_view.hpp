#ifndef MAZE_VIEW_MAZE_MAZE_VIEW_HPP
#define MAZE_VIEW_MAZE_MAZE_VIEW_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QTextStream>
#include <QFile>

#include "seeker.hpp"
#include "builder.hpp"

namespace s21 {
class MazeWidget : public QGraphicsView {
public:
    using Way = std::list<std::pair<int, int>>;
    using Matrix = std::vector<std::vector<int>>;
    using Coords = std::vector<std::pair<int, int>>;

    MazeWidget(QGraphicsView *wdg = nullptr);
    MazeWidget(MazeWidget &&wdg) = delete;
    MazeWidget(const MazeWidget &wdg) = delete;
    ~MazeWidget();

    void DrawMaze();
    void CreateMaze(int rows, int cols);
    void OpenFile(const QString &path);
    void SaveFile(const QString &path) const;

protected:
    void DrawWay();
    void ClearScene();
    void ClearVec(QVector<QGraphicsEllipseItem *> &vec) const;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QGraphicsScene *scene_;

    Builder builder_;
    Seeker seeker_;

    int rows_{1};
    int cols_{1};
    float cell_size_v_{};
    float cell_size_h_{};

    Way way_;
    Coords coords_;
    Matrix v_walls_{{0}};
    Matrix h_walls_{{0}};

    const float wdg_size_{606.0f};

    QVector<QGraphicsEllipseItem *> dots_;
    QVector<QGraphicsEllipseItem *> way_dots_;
};
}  // namespace s21

#endif  // MAZE_VIEW_MAZE_MAZE_VIEW_HPP
