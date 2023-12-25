#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QFrame *frame = ui->videoFrame;

    videoPlayer = new VideoPlayer(ui->videoFrame, this);
    // 嵌入 videoPlayer 的 videoWidget 到 frame 中
    videoPlayer->getVideoWidget()->setParent(frame);
    // 设置 videoWidget 的大小与 frame 的大小相同
    videoPlayer->getVideoWidget()->resize(frame->size());
    // 为 frame 设置一个新的布局并添加 videoWidget
    QVBoxLayout *layout = new QVBoxLayout(frame);
    layout->addWidget(videoPlayer->getVideoWidget());
    layout->setMargin(0);
    frame->setLayout(layout);
    videoPlayer->show();  // 显示 VideoPlayer

    // Inside your Widget class constructor or initialization function
    connect(videoPlayer, &VideoPlayer::positionChangedSignal, this, &Widget::updatePlaySlider);
    connect(ui->playerslider, &QSlider::sliderMoved, this, &Widget::on_playerslider_sliderMoved);
    connect(ui->volumeslider, &QSlider::sliderMoved, this, &Widget::on_volumeslider_sliderMoved);


}

void Widget::on_file_clicked()
{

}


void Widget::on_pause_clicked()
{

}

void Widget::updatePlaySlider(qint64 position) {
    ui->playerslider->setMaximum(videoPlayer->getDuration());
    ui->playerslider->setValue(position);
}

// Slot to handle playerslider movement
void Widget::on_playerslider_sliderMoved(int position) {
    videoPlayer->setPosition(position);
}

// Slot to handle volumeslider movement
void Widget::on_volumeslider_sliderMoved(int position) {
    videoPlayer->setVolume(position);
}

Widget::~Widget()
{
    delete ui;
    delete videoPlayer;
}

