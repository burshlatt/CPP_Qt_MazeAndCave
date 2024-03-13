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

void MazeWidget::SaveFile(const QString &path) const {
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << rows_ << " " << cols_ << "\n";
        for (const auto &row : v_walls_) {
            for (int val : row)
                out << val << " ";
            out << "\n";
        }
        out << "\n";
        for (const auto &row : h_walls_) {
            for (int val : row)
                out << val << " ";
            out << "\n";
        }
        file.close();
    }
}

void MazeWidget::OpenFile(const QString &path) {
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in >> rows_ >> cols_;
        int value = 0;
        for (int i = 0; i < 2; ++i) {
            Matrix matrix;
            for (int row = 0; row < rows_; ++row) {
                std::vector<int> row_vec;
                for (int col = 0; col < cols_; ++col) {
                    in >> value;
                    row_vec.push_back(value);
                }
                matrix.push_back(row_vec);
            }
            if (i == 0)
                v_walls_ = matrix;
            else
                h_walls_ = matrix;
        }
        file.close();
        DrawMaze();
    }
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
    builder_.BuildMaze(rows, cols);
    v_walls_ = builder_.GetVerticalWalls();
    h_walls_ = builder_.GetHorizontalWalls();
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

    scene_->addRect(0, 0, cols_ * cell_size_h_, rows_ * cell_size_v_, wall_pen);
}

void MazeWidget::DrawWay() {
    for (const auto &coords : way_) {
        auto [x, y] = coords;
        float ellipse_w = 0.2 * cell_size_h_;
        float ellipse_h = 0.2 * cell_size_v_;
        float x_offset = (cell_size_h_ - ellipse_w) / 2;
        float y_offset = (cell_size_v_ - ellipse_h) / 2;
        QPen pen(Qt::red, 2);
        QGraphicsEllipseItem *way_dot = scene_->addEllipse(x * cell_size_h_ + x_offset, y * cell_size_v_ + y_offset, ellipse_w, ellipse_h, pen);
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
    int x = event->pos().x() / cell_size_h_;
    int y = event->pos().y() / cell_size_v_;
    float ellipse_w = 0.5 * cell_size_h_;
    float ellipse_h = 0.5 * cell_size_v_;
    float x_offset = (cell_size_h_ - ellipse_w) / 2;
    float y_offset = (cell_size_v_ - ellipse_h) / 2;
    QPen pen(Qt::blue, 2);
    QGraphicsEllipseItem *dot = scene_->addEllipse(x * cell_size_h_ + x_offset, y * cell_size_v_ + y_offset, ellipse_w, ellipse_h, pen);
    dot->setBrush(Qt::blue);
    dots_.append(dot);
    coords_.emplace_back(x, y);
    if (dots_.size() == 2) {
        way_ = seeker_.Solve(rows_, cols_, v_walls_, h_walls_, coords_[0], coords_[1]);
        DrawWay();
    }
}
}  // namespace s21
