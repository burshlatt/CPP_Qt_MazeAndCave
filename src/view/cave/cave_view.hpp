#ifndef MAZE_AND_CAVE_VIEW_CAVE_CAVE_VIEW_HPP
#define MAZE_AND_CAVE_VIEW_CAVE_CAVE_VIEW_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "cave_controller.hpp"

namespace s21 {
class CaveWidget : public QGraphicsView {
public:
    using Matrix = std::vector<std::vector<int>>;
    using Coords = std::vector<std::pair<int, int>>;

    CaveWidget(QGraphicsView *wdg = nullptr);
    CaveWidget(CaveWidget &&wdg) = delete;
    CaveWidget(const CaveWidget &wdg) = delete;
    ~CaveWidget();

public:
    Settings GetSettings() const;

public:
    void DrawCave();
    void CreateCaveStep();
    void CreateCave(Settings settings);
    void SaveFile(const QString &path) const;
    void OpenFile(const QString &path);
    void ClearWalls();

protected:
    void ClearScene();

private:
    CaveController cave_;
    Settings settings_;

    QGraphicsScene *scene_;
    QVector<QGraphicsRectItem*> walls_;

    float cell_w_{};
    float cell_h_{};

    static constexpr const float wdg_size_{606.0f};
};
}  // namespace s21

#endif  // MAZE_VIEW_CAVE_CAVE_VIEW_HPP
