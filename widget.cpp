#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>
#include <QShortcut>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , isPlaying(true)
    , isMuted(true)
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
    //fullscreen


    // Inside your Widget class constructor or initialization function
    connect(videoPlayer, &VideoPlayer::positionChangedSignal, this, &Widget::updatePlaySlider);
    connect(ui->playerslider, &QSlider::sliderMoved, this, &Widget::on_playerslider_sliderMoved);
    connect(ui->volumeslider, &QSlider::sliderMoved, this, &Widget::on_volumeslider_sliderMoved);

    videoPlayer->play(); // Start playing video on application start
    ui->pause->setIcon(QIcon(":/icons/play.png"));

    videoPlayer->setVolume(0); // Mute volume on application start
    ui->volume->setIcon(QIcon(":/icons/volumeDisable.png"));
    ui->volumeslider->setMaximum(100); // Set the maximum value of volumeslider to 100
    ui->volumeslider->setValue(0);

    // widget.cpp, inside the Widget constructor
    connect(ui->forward, &QPushButton::clicked, this, &Widget::on_forward_clicked);
    connect(ui->backward, &QPushButton::clicked, this, &Widget::on_backward_clicked);
    // Inside Widget constructor
    connect(videoPlayer, &VideoPlayer::videoNameChanged, this, &Widget::updateVideoNameLabel);

    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+Shift+F"), this);
    connect(shortcut, &QShortcut::activated, this, &Widget::on_file_clicked);
    // 为 on_pushButton_clicked 添加快捷键
    QShortcut *shortcutAccount = new QShortcut(QKeySequence("Ctrl+A"), this);
    connect(shortcutAccount, &QShortcut::activated, this, &Widget::on_pushButton_clicked);

    // 为 on_pushButton_5_clicked 添加快捷键
    QShortcut *shortcutShare = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(shortcutShare, &QShortcut::activated, this, &Widget::on_pushButton_5_clicked);

    // 为 on_pushButton_4_clicked 添加快捷键
    QShortcut *shortcutLike = new QShortcut(QKeySequence("Ctrl+L"), this);
    connect(shortcutLike, &QShortcut::activated, this, &Widget::on_pushButton_4_clicked);

    QShortcut *shortcutFull = new QShortcut(QKeySequence("Ctrl+X"), this);
    connect(shortcutFull, &QShortcut::activated, this, &Widget::on_fullscreen_clicked);
}
void Widget::updateVideoNameLabel(const QString &name) {
    QLabel *label = findChild<QLabel *>("videoname");
    if (label) {
        label->setText(name);
    }
}

void Widget::on_file_clicked() {
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Video Directory"), QDir::homePath());

    if (!folderPath.isEmpty()) {
        QDir dir(folderPath);
        QStringList fileFilter;
        fileFilter << "*.mp4" << "*.wmv" << "*.mov"; // 支持的视频文件格式
        QFileInfoList fileList = dir.entryInfoList(fileFilter, QDir::Files);

        if (fileList.isEmpty()) {
            qDebug() << "No video files found in the directory.";
            return; // 如果没有找到视频文件，就提前退出
        }

        videoFiles.clear(); // 清空之前的列表
        for (const QFileInfo &fileInfo : fileList) {
            if (fileInfo.exists() && fileInfo.isFile()) {
                videoFiles.append(fileInfo.absoluteFilePath());
            } else {
                qDebug() << "File does not exist or is not a valid video file:" << fileInfo.absoluteFilePath();
            }
        }

        // 如果有 VideoPlayer 类的实例，调用它的方法来播放视频
        // 例如，如果 VideoPlayer 是这个 Widget 类的一个成员变量：
        videoPlayer->loadVideoFiles(folderPath); // 假设 VideoPlayer 有这个方法
        videoPlayer->playVideo(0); // 开始播放第一个视频
    } else {
        qDebug() << "No folder was selected.";
    }
}



void Widget::on_pause_clicked()
{
    if (isPlaying) {
        videoPlayer->pause();
        ui->pause->setIcon(QIcon(":/icons/stop.png")); // Assuming the icon is in a resource file
        isPlaying = false;
    } else {
        videoPlayer->play();
        ui->pause->setIcon(QIcon(":/icons/play.png"));
        isPlaying = true;
    }
}

void Widget::on_volume_clicked() {
    if (isMuted) {
        videoPlayer->setVolume(50); // Set the volume to 50 when unmuted
        ui->volume->setIcon(QIcon(":/icons/volume.png")); // Icon when volume is on
        ui->volumeslider->setValue(50); // Update the volumeslider position to 50
        isMuted = false;
    } else {
        videoPlayer->setVolume(0); // Mute the volume
        ui->volume->setIcon(QIcon(":/icons/volumeDisable.png")); // Icon when volume is off
        ui->volumeslider->setValue(0); // Update the volumeslider position to 0
        isMuted = true;
    }
}

void Widget::on_forward_clicked() {
    qint64 currentPosition = videoPlayer->getPosition();
    qint64 duration = videoPlayer->getDuration();
    // Ensure we don't go beyond the video's length
    if(currentPosition + 5000 < duration) {
        videoPlayer->setPosition(currentPosition + 5000);
    } else {
        videoPlayer->setPosition(duration);
    }
}

void Widget::on_backward_clicked() {
    qint64 currentPosition = videoPlayer->getPosition();
    // Ensure we don't go before the start of the video
    if(currentPosition - 5000 > 0) {
        videoPlayer->setPosition(currentPosition - 5000);
    } else {
        videoPlayer->setPosition(0);
    }
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


void Widget::on_pushButton_clicked()
{
    QMessageBox::information(this, "Account Information", "user's account");
}


void Widget::on_pushButton_5_clicked()
{
    QMessageBox::information(this, "Sharing", "You can share videos to QQ/Wechat");
}



void Widget::on_pushButton_4_clicked()
{
    QMessageBox::information(this, "Like", "Added to like list!");
}

void Widget::on_fullscreen_clicked() {
    if (this->isFullScreen()) {
        // 退出全屏模式
        this->showNormal();
        // 恢复所有非视频部分的元素
        ui->header->show();
        ui->bottom->show();
    } else {
        // 进入全屏模式
        this->showFullScreen();
        // 隐藏所有非视频部分的元素
        ui->header->hide();
        ui->bottom->hide();
    }
}

void Widget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape && this->isFullScreen()) {
        on_fullscreen_clicked(); // 如果已经全屏，那么调用全屏按钮的槽函数来退出全屏
    } else {
        QWidget::keyPressEvent(event); // 否则调用默认的事件处理
    }
}

