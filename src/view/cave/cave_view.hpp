#ifndef MAZE_VIEW_CAVE_CAVE_VIEW_HPP
#define MAZE_VIEW_CAVE_CAVE_VIEW_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QTextStream>
#include <QFile>

#include <memory>

#include "generator.hpp"

namespace s21 {
class CaveWidget : public QGraphicsView {
public:
    using Matrix = std::vector<std::vector<int>>;
    using Coords = std::vector<std::pair<int, int>>;

    CaveWidget(QGraphicsView *wdg = nullptr);
    CaveWidget(CaveWidget &&wdg) = delete;
    CaveWidget(const CaveWidget &wdg) = delete;
    ~CaveWidget();

    void DrawCave();
    void CreateCave(Settings settings);
    void CreateCaveStep(Settings settings);
    void OpenFile(const QString &path);
    void SaveFile(const QString &path) const;

protected:
    void ClearScene();

private:
    QGraphicsScene *scene_;

    Settings settings_;

    Generator generator_;

    float cell_size_v_{};
    float cell_size_h_{};

    std::unique_ptr<Cave> cave_;

    static constexpr const float wdg_size_{606.0f};
};
}  // namespace s21

#endif  // MAZE_VIEW_CAVE_CAVE_VIEW_HPP
