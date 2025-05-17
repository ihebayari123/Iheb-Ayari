#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Main Application Window");
    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *welcomeLabel = new QLabel("Welcome to the Main Application!", this);
    layout->addWidget(welcomeLabel);

    QPushButton *exitButton = new QPushButton("Exit", this);
    layout->addWidget(exitButton);

    connect(exitButton, &QPushButton::clicked, this, &QMainWindow::close);
}