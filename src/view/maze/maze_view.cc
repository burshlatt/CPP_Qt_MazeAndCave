#include "maze_view.hpp"

namespace s21 {
MazeWidget::MazeWidget(QGraphicsView *wdg) : QGraphicsView(wdg) {
    scene_ = new QGraphicsScene(this);

    DrawMaze();
}

MazeWidget::~MazeWidget() {
    ClearScene();

    delete scene_;
}

std::pair<int, int> MazeWidget::GetSettings() const {
    return std::make_pair(rows_, cols_);
}

void MazeWidget::SaveFile(const QString &path) const {
    maze_.SaveConfig(path.toStdString());
}

void MazeWidget::OpenFile(const QString &path) {
    maze_.LoadConfig(path.toStdString());

    rows_ = maze_.GetRows();
    cols_ = maze_.GetCols();
    v_walls_ = maze_.GetVerticalWalls();
    h_walls_ = maze_.GetHorizontalWalls();

    DrawMaze();
}

void MazeWidget::ClearVec(QVector<QGraphicsEllipseItem *> &vec) const {
    for (int i = 0; i < vec.size(); i++) {
        delete vec[i];
        vec[i] = nullptr;
    }

    vec.clear();
}

void MazeWidget::ClearScene() {
    ClearVec(dots_);
    ClearVec(way_dots_);

    scene_->clear();
    coords_.clear();
}

void MazeWidget::CreateMaze(int rows, int cols) {
    rows_ = rows;
    cols_ = cols;

    maze_.BuildMaze(rows, cols);

    v_walls_ = maze_.GetVerticalWalls();
    h_walls_ = maze_.GetHorizontalWalls();
}

void MazeWidget::DrawMaze() {
    ClearScene();
    setScene(scene_);

    cell_size_v_ = wdg_size_ / rows_;
    cell_size_h_ = wdg_size_ / cols_;

    QPen wall_pen(Qt::green, 2);

    for (int row = 0; row < rows_; row++)
        for (int col = 0; col < cols_; col++)
            if (v_walls_[row][col] == 1)
                scene_->addLine((col + 1) * cell_size_h_, row * cell_size_v_, (col + 1) * cell_size_h_, (row + 1) * cell_size_v_, wall_pen);

    for (int row = 0; row < rows_; row++)
        for (int col = 0; col < cols_; col++)
            if (h_walls_[row][col] == 1)
                scene_->addLine(col * cell_size_h_, (row + 1) * cell_size_v_, (col + 1) * cell_size_h_, (row + 1) * cell_size_v_, wall_pen);

    scene_->addRect(0, 0, wdg_size_, wdg_size_, wall_pen);
}

void MazeWidget::DrawWay() {
    for (const auto &coords : way_) {
        auto [x, y]{coords};

        float ellipse_w{0.2f * cell_size_h_};
        float ellipse_h{0.2f * cell_size_v_};
        float x_offset{(cell_size_h_ - ellipse_w) / 2.0f};
        float y_offset{(cell_size_v_ - ellipse_h) / 2.0f};

        QPen pen(Qt::red, 2);
        QGraphicsEllipseItem* way_dot{scene_->addEllipse(x * cell_size_h_ + x_offset, y * cell_size_v_ + y_offset, ellipse_w, ellipse_h, pen)};

        way_dot->setBrush(Qt::red);
        way_dots_.append(way_dot);
    }
}

void MazeWidget::mousePressEvent(QMouseEvent *event) {
    if (dots_.size() == 2) {
        coords_.clear();
        ClearVec(dots_);
        ClearVec(way_dots_);
    }

    int x{static_cast<int>(event->pos().x() / cell_size_h_)};
    int y{static_cast<int>(event->pos().y() / cell_size_v_)};

    float ellipse_w{0.5f * cell_size_h_};
    float ellipse_h{0.5f * cell_size_v_};
    float x_offset{(cell_size_h_ - ellipse_w) / 2.0f};
    float y_offset{(cell_size_v_ - ellipse_h) / 2.0f};

    QPen pen(Qt::blue, 2);
    QGraphicsEllipseItem* dot{scene_->addEllipse(x * cell_size_h_ + x_offset, y * cell_size_v_ + y_offset, ellipse_w, ellipse_h, pen)};

    dot->setBrush(Qt::blue);
    dots_.append(dot);
    coords_.emplace_back(x, y);

    if (dots_.size() == 2) {
        way_ = maze_.FindWay(rows_, cols_, v_walls_, h_walls_, coords_[0], coords_[1]);
        DrawWay();
    }
}
}  // namespace s21
