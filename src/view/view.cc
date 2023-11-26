#include "view.h"
#include "ui_view.h"

namespace s21 {
MazeView::MazeView(QWidget *parent) : QMainWindow(parent), ui_(new Ui::MazeView) {
    ui_->setupUi(this);
    setFixedSize(910, 660);

    maze_widget_ = std::make_unique<MazeWidget>(ui_->graphicsView);

    connect(ui_->btnOpenFile, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getOpenFileName(this, "Open maze", QString(), "Maze file (*.txt)");
        maze_widget_->OpenFile(path);
    });

    connect(ui_->btnSaveFile, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getSaveFileName(this, "Save maze", QString(), "Maze file (*.txt)");
        maze_widget_->SaveFile(path);
    });

    connect(ui_->btnGenerate, &QPushButton::clicked, this, [this]() {
        int rows = ui_->RowsValue->value();
        int cols = ui_->ColsValue->value();
        maze_widget_->CreateMaze(rows, cols);
        maze_widget_->DrawMaze();
    });
}

MazeView::~MazeView() { delete ui_; }
}  // namespace s21
