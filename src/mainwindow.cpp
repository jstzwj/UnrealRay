#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    engine.render();
    Film * film=&*engine.integrator->camera->film;
    film->writeImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    Film * film=&*engine.integrator->camera->film;
    QImage qimg=dynamic_cast<QImageFilm *>(film)->getImage();


    QPainter painter(this);

    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);

     // 绘制图标
     painter.drawPixmap(rect(), QPixmap::fromImage(qimg));
}
