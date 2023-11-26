#ifndef A1_MAZE_1_VIEW_MAZEWIDGET_H
#define A1_MAZE_1_VIEW_MAZEWIDGET_H

#include <QFile>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QTextStream>

namespace s21 {
class MazeWidget : public QGraphicsView {
    public:
        using Way = std::list<std::pair<int, int>>;
        using Matrix = std::vector<std::vector<int>>;
        using Coords = std::vector<std::pair<int, int>>;

        MazeWidget(QGraphicsView *wdg = nullptr);
        MazeWidget(MazeWidget&& wdg) = delete;
        MazeWidget(const MazeWidget& wdg) = delete;
        ~MazeWidget();

        void DrawWay();
        void DrawMaze();
        void ClearScene();
        void SetWalls(int rows, int cols);
        void OpenFile(const QString &path);
        void SaveFile(const QString &path);
        void ClearVec(QVector<QGraphicsEllipseItem *> &vec);

        void mousePressEvent(QMouseEvent *event) override;

    private:
        QGraphicsScene *scene_;

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
        std::unique_ptr<MazeController> maze_controller_;
};
}  // namespace s21

#endif  // A1_MAZE_1_VIEW_MAZEWIDGET_H
