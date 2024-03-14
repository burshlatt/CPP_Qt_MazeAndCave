#include "view.hpp"
#include "ui_view.h"

namespace s21 {
View::View(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::View),
    timer_(new QTimer)
{
    ui_->setupUi(this);
    setFixedSize(910, 660);

    QTabBar* tab{ui_->tabWidget->tabBar()};
    tab->setStyleSheet("QTabBar::tab::first { border: 2px solid #82D625; }");

    maze_widget_ = std::make_unique<MazeWidget>(ui_->gvMaze);
    cave_widget_ = std::make_unique<CaveWidget>(ui_->gvCave);

    connect(ui_->tabWidget, &QTabWidget::currentChanged, this, &View::WindowChange);

    connect(ui_->btnSaveFileMaze, &QPushButton::clicked, this, &View::MazeSaveConfig);
    connect(ui_->btnOpenFileMaze, &QPushButton::clicked, this, &View::MazeLoadConfig);
    connect(ui_->btnGenerateMaze, &QPushButton::clicked, this, &View::GenerateMaze);

    connect(ui_->btnSaveFileCave, &QPushButton::clicked, this, &View::CaveSaveConfig);
    connect(ui_->btnOpenFileCave, &QPushButton::clicked, this, &View::CaveLoadConfig);
    connect(ui_->btnGenerateCave, &QPushButton::clicked, this, &View::GenerateCave);
    connect(ui_->btnGenerateStep, &QPushButton::clicked, this, &View::GenerateStep);

    connect(timer_, &QTimer::timeout, this, &View::GenerateStep);
    connect(ui_->btnStartGenerate, &QPushButton::clicked, this, &View::GenerateStepTimer);
}

View::~View() {
    delete timer_;
    delete ui_;
}

void View::WindowChange() {
    int index{ui_->tabWidget->currentIndex()};
    QTabBar* tab{ui_->tabWidget->tabBar()};

    if (index == 0) {
        tab->setStyleSheet("QTabBar::tab::last { border: 2px solid gray; }");
        tab->setStyleSheet("QTabBar::tab::first { border: 2px solid #82D625; }");
    } else if (index == 1) {
        tab->setStyleSheet("QTabBar::tab::first { border: 2px solid gray; }");
        tab->setStyleSheet("QTabBar::tab::last { border: 2px solid #82D625; }");
    }
}

void View::MazeSaveConfig() {
    QString path{QFileDialog::getSaveFileName(this, "Save maze", QString(), "Maze file (*.txt)")};

    if (path != "")
        maze_widget_->SaveFile(path);
}

void View::MazeLoadConfig() {
    QString path{QFileDialog::getOpenFileName(this, "Open maze", QString(), "Maze file (*.txt)")};

    if (path != "") {
        maze_widget_->OpenFile(path);

        auto [rows, cols]{maze_widget_->GetSettings()};

        ui_->sbMazeRows->setValue(rows);
        ui_->sbMazeCols->setValue(cols);
    }
}

void View::GenerateMaze() {
    int rows{ui_->sbMazeRows->value()};
    int cols{ui_->sbMazeCols->value()};

    maze_widget_->CreateMaze(rows, cols);
    maze_widget_->DrawMaze();
}

void View::CaveSaveConfig() {
    QString path{QFileDialog::getSaveFileName(this, "Save cave", QString(), "Cave file (*.txt)")};

    if (path != "")
        cave_widget_->SaveFile(path);
}

void View::CaveLoadConfig() {
    QString path{QFileDialog::getOpenFileName(this, "Open cave", QString(), "Cave file (*.txt)")};

    if (path != "") {
        cave_widget_->OpenFile(path);
        cave_widget_->DrawCave();

        Settings sett{cave_widget_->GetSettings()};

        ui_->sbCaveRows->setValue(static_cast<int>(sett.rows));
        ui_->sbCaveCols->setValue(static_cast<int>(sett.cols));
        ui_->sbLiveChance->setValue(static_cast<int>(sett.live_chance));
        ui_->sbPopulation->setValue(static_cast<int>(sett.generation_count));
        ui_->sbBornFrom->setValue(static_cast<int>(sett.born_limit.first));
        ui_->sbBornTo->setValue(static_cast<int>(sett.born_limit.second));
        ui_->sbSurviveFrom->setValue(static_cast<int>(sett.live_limit.first));
        ui_->sbSurviveTo->setValue(static_cast<int>(sett.live_limit.second));
    }
}

Settings View::GetSettings() {
    Settings settings;

    settings.rows = ui_->sbCaveRows->value();
    settings.cols = ui_->sbCaveCols->value();
    settings.live_chance = ui_->sbLiveChance->value();
    settings.generation_count = ui_->sbPopulation->value();
    settings.live_limit = std::make_pair(ui_->sbSurviveFrom->value(), ui_->sbSurviveTo->value());
    settings.born_limit = std::make_pair(ui_->sbBornFrom->value(), ui_->sbBornTo->value());

    return settings;
}

void View::GenerateCave() {
    Settings settings{std::move(GetSettings())};

    cave_widget_->CreateCave(settings);
    cave_widget_->DrawCave();

    ui_->lbPopulation->setText(QString::number(settings.generation_count));
}

void View::GenerateStep() {
    Settings settings{std::move(GetSettings())};

    cave_widget_->CreateCaveStep();
    cave_widget_->DrawCave();

    auto current_population{ui_->lbPopulation->text().toUInt()};

    ui_->lbPopulation->setText(QString::number(++current_population));
}

void View::GenerateStepTimer() {
    if (!timer_->isActive()) {
        timer_->start(ui_->sbMs->value());
        ui_->btnStartGenerate->setText("Stop");
    } else {
        timer_->stop();
        ui_->btnStartGenerate->setText("Start");
    }
}
}  // namespace s21
