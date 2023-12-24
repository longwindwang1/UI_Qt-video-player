#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this); // 创建播放器实例

    QWidget *placeholderWidget = ui->stackedWidget->findChild<QWidget*>("videoPlaceholderWidget");
    if (placeholderWidget) {
        QVideoWidget *videoWidget = new QVideoWidget(placeholderWidget);
        placeholderWidget->layout()->addWidget(videoWidget); // 确保占位符有布局
        player->setVideoOutput(videoWidget);
    }

    QVideoWidget *videoWidget = ui->stackedWidget->findChild<QVideoWidget*>("videoPlaceholderWidget");
    if (videoWidget) {
        player->setVideoOutput(videoWidget);
    }

    // 如果您希望在程序启动时就加载并播放视频，可以在这里设置
    QString videoPath = "videos/c.mp4"; // 视频文件的路径
    QUrl videoUrl = QUrl::fromLocalFile(videoPath);
    player->setMedia(videoUrl); // 设置播放器媒体
    player->play(); // 开始播放
}


Widget::~Widget()
{
    delete ui;
}

void Widget::on_file_clicked() {
    QString filter = "Video Files (*.mp4 *.wmv *.MOV)"; // 支持的视频文件格式
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Video File"), "", filter);

    if (player && !fileName.isEmpty()) {
        QUrl videoUrl = QUrl::fromLocalFile(fileName);
        player->setMedia(videoUrl);
        player->play();
    }
}

void Widget::on_play_clicked() {
    if (player->state() == QMediaPlayer::PlayingState) {
        // 如果正在播放，暂停视频
        player->pause();
    } else {
        // 如果视频已暂停或停止，开始播放
        player->play();
    }
}



void Widget::on_volume_clicked() {
    if (player->isMuted()) {
        // 如果当前已静音，取消静音
        player->setMuted(false);
    } else {
        // 如果当前未静音，设置为静音
        player->setMuted(true);
    }
}

void Widget::on_volumeslider_actionTriggered(int action) {
    // 获取滑块的当前值
    int volume = ui->volumeslider->value();

    // 设置播放器的音量
    player->setVolume(volume);
}

void Widget::on_backward_clicked() {
    // 获取当前播放位置（以毫秒为单位）
    qint64 currentPosition = player->position();

    // 计算新的播放位置，确保不小于 0
    qint64 newPosition = qMax(currentPosition - 5000, qint64(0));

    // 设置播放器到新的播放位置
    player->setPosition(newPosition);
}

void Widget::on_forward_clicked() {
    // 获取当前播放位置（以毫秒为单位）
    qint64 currentPosition = player->position();

    // 计算新的播放位置
    qint64 newPosition = currentPosition + 5000;

    // 确保新位置不超过视频的总长度
    newPosition = qMin(newPosition, player->duration());

    // 设置播放器到新的播放位置
    player->setPosition(newPosition);
}


void Widget::on_playslider_actionTriggered(int action) {
    // 如果动作是由于用户拖动滑块导致的
    if (action == QAbstractSlider::SliderMove) {
        int position = ui->playslider->value(); // 获取滑块的当前值（以毫秒为单位）

        // 设置播放器的播放位置
        player->setPosition(position);
    }
}





