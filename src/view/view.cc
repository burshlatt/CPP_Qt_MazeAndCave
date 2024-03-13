#include "view.hpp"
#include "ui_view.h"

namespace s21 {
MazeView::MazeView(QWidget *parent) : QMainWindow(parent), ui_(new Ui::MazeView) {
    ui_->setupUi(this);
    setFixedSize(910, 660);

    maze_widget_ = std::make_unique<MazeWidget>(ui_->gvMaze);
    cave_widget_ = std::make_unique<CaveWidget>(ui_->gvCave);

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

    connect(ui_->btnGenerateCave, &QPushButton::clicked, this, [this]() {
        Settings settings;

        settings.rows = ui_->sbCaveRows->value();
        settings.cols = ui_->sbCaveCols->value();
        settings.live_chance = ui_->sbLiveChance->value();
        settings.generation_count = ui_->sbPopulation->value();
        settings.live_limit = std::make_pair(ui_->sbSurviveFrom->value(), ui_->sbSurviveTo->value());
        settings.born_limit = std::make_pair(ui_->sbBornFrom->value(), ui_->sbBornTo->value());

        cave_widget_->CreateCave(settings);
        cave_widget_->DrawCave();
    });

    connect(ui_->btnGenerateStep, &QPushButton::clicked, this, [this]() {
        Settings settings;

        settings.rows = ui_->sbCaveRows->value();
        settings.cols = ui_->sbCaveCols->value();
        settings.live_chance = ui_->sbLiveChance->value();
        settings.generation_count = ui_->sbPopulation->value();
        settings.live_limit = std::make_pair(ui_->sbSurviveFrom->value(), ui_->sbSurviveTo->value());
        settings.born_limit = std::make_pair(ui_->sbBornFrom->value(), ui_->sbBornTo->value());

        cave_widget_->CreateCave(settings);
        cave_widget_->DrawCave();
    });
}

MazeView::~MazeView() { delete ui_; }
}  // namespace s21
