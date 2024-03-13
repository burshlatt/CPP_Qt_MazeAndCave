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

void CaveWidget::SaveFile(const QString &path) const {
//    QFile file(path);
//    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        QTextStream out(&file);
//        out << rows_ << " " << cols_ << "\n";
//        for (const auto &row : v_walls_) {
//            for (int val : row)
//                out << val << " ";
//            out << "\n";
//        }
//        out << "\n";
//        for (const auto &row : h_walls_) {
//            for (int val : row)
//                out << val << " ";
//            out << "\n";
//        }
//        file.close();
//    }
}

void CaveWidget::OpenFile(const QString &path) {
//    QFile file(path);
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QTextStream in(&file);
//        in >> rows_ >> cols_;
//        int value = 0;
//        for (int i = 0; i < 2; ++i) {
//            Matrix matrix;
//            for (int row = 0; row < rows_; ++row) {
//                std::vector<int> row_vec;
//                for (int col = 0; col < cols_; ++col) {
//                    in >> value;
//                    row_vec.push_back(value);
//                }
//                matrix.push_back(row_vec);
//            }
//            if (i == 0)
//                v_walls_ = matrix;
//            else
//                h_walls_ = matrix;
//        }
//        file.close();
//        DrawCave();
//    }
}

void CaveWidget::ClearScene() {
    scene_->clear();
}

void CaveWidget::CreateCave(Settings settings) {
    settings_ = settings;
    cave_ = std::make_unique<Cave>(generator_.Generate(settings));
}

void CaveWidget::CreateCaveStep(Settings settings) {
//    settings_ = settings;
//    generator_.Generate(settings, *cave_);
}

//void View::on_StepGenerationCave_clicked() {
//    ng::CaveWrapper cave = ui->graphicsView->getCave();
//    controller_->Generate(getCaveSettingsFromView(), cave);
//    ui->graphicsView->setCave(std::move(cave));
//    ui->graphicsView->DrawCave();
//    auto current_population = ui->current_population->text().toUInt();
//    ui->current_population->setText(QString::number(++current_population));
//}

void CaveWidget::DrawCave() {
    ClearScene();
    setScene(scene_);
    cell_size_v_ = wdg_size_ / settings_.rows;
    cell_size_h_ = wdg_size_ / settings_.cols;

    QBrush wall_brush(Qt::green);
    QPen wall_pen(Qt::green, 2);

    for (int row = 0; row < settings_.rows; row++) {
        for (int col = 0; col < settings_.cols; col++) {
            if ((*cave_)(row, col) == 1) {
                QGraphicsRectItem *wall = new QGraphicsRectItem(col * cell_size_h_, row * cell_size_v_, cell_size_h_, cell_size_v_);
                wall->setBrush(wall_brush);
                wall->setPen(wall_pen);
                scene_->addItem(wall);
            }
        }
    }

    QGraphicsRectItem *background = new QGraphicsRectItem(0, 0, settings_.cols * cell_size_h_, settings_.rows * cell_size_v_);
    scene_->addItem(background);
}
}  // namespace s21
