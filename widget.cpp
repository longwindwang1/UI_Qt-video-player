#include "widget.h"
#include "ui_widget.h"
#include <QApplication>
#include <QMediaPlayer>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->playPauseButton, &QPushButton::clicked, this, &Widget::togglePlayPause);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_file_clicked()
{

}


void Widget::on_play_clicked() {

}

