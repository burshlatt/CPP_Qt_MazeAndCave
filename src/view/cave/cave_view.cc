#include "cave_view.hpp"

namespace s21 {
CaveWidget::CaveWidget(QGraphicsView *wdg) : QGraphicsView(wdg) {
    scene_ = new QGraphicsScene(this);
    DrawCave();
}

CaveWidget::~CaveWidget() {
    ClearScene();
    delete scene_;
}

void CaveWidget::ClearWalls() {
    for (std::size_t i{}; i < walls_.size(); ++i) {
        if (walls_[i]) {
            delete walls_[i];
            walls_[i] = nullptr;
        }
    }
}

void CaveWidget::ClearScene() {
    ClearWalls();
    scene_->clear();
}

Settings CaveWidget::GetSettings() const {
    return settings_;
}

void CaveWidget::SaveFile(const QString& path) const {
    cave_.SaveConfig(path.toStdString());
}

void CaveWidget::OpenFile(const QString& path) {
    cave_.LoadConfig(path.toStdString());
    settings_ = cave_.GetSettings();
}

void CaveWidget::CreateCave(Settings settings) {
    settings_ = settings;
    cave_.Generate(settings);
}

void CaveWidget::CreateCaveStep() {
    cave_.GenerateStep();
}

void CaveWidget::DrawCave() {
    ClearScene();
    setScene(scene_);

    cell_w_ = wdg_size_ / settings_.rows;
    cell_h_ = wdg_size_ / settings_.cols;

    QBrush wall_brush(Qt::green);
    QPen wall_pen(Qt::green, 1);

    for (int row = 0; row < settings_.rows; row++) {
        for (int col = 0; col < settings_.cols; col++) {
            if (cave_(row, col) == 1) {
                walls_.push_back(new QGraphicsRectItem(col * cell_h_, row * cell_w_, cell_h_, cell_w_));

                walls_.back()->setBrush(wall_brush);
                walls_.back()->setPen(wall_pen);
                scene_->addItem(walls_.back());
            }
        }
    }

    QPen rect_pen(Qt::green, 2);

    scene_->addRect(0, 0, wdg_size_, wdg_size_, rect_pen);
}
}  // namespace s21
