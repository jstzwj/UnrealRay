#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QImage img;
    Film<QImage> film(img);
    scene.setFilm(film);
    scene.render();

    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);

     // 绘制图标
     painter.drawPixmap(rect(), QPixmap::fromImage(film.getData()));
}
