#include "videoplayer.h"
#include <QDir>
#include <QFileInfoList>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>


VideoPlayer::VideoPlayer(QFrame *videoFrame, QWidget *parent)
    : QWidget(parent),videoFrame(videoFrame),
    player(new QMediaPlayer(this)),
    videoWidget(new QVideoWidget(this)),
    currentVideoIndex(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(videoWidget);
    layout->setMargin(0);  // Remove margins if desired
    setLayout(layout);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);

    // Set up the video output and widget
    player->setVideoOutput(videoWidget);
    videoWidget->resize(this->size());
    videoWidget->setParent(this); // Ensure the video widget is a child of the VideoPlayer
    videoWidget->show();

    loadVideoFiles("E:/qtwk/UIcswk/videos"); // Replace with your folder path
    playVideo(currentVideoIndex);
    // ... other initialization ...
    videoFrame->installEventFilter(this);

    connect(player, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &VideoPlayer::handlePositionChanged);

    connect(player, &QMediaPlayer::stateChanged, this, &VideoPlayer::onPlayerStateChanged);
}

void VideoPlayer::onPlayerStateChanged(QMediaPlayer::State state) {
    if (state == QMediaPlayer::StoppedState) {
        player->setPosition(0); // Rewind to the start
        player->play(); // Start playing again
    }
}

void VideoPlayer::loadVideoFiles(const QString &folderPath)
{
    QDir dir(folderPath);
    QFileInfoList files = dir.entryInfoList(QStringList() << "*.wmv" << "*.mp4" << "*.avi", QDir::Files);
    foreach (const QFileInfo &file, files) {
        videoFiles << file.absoluteFilePath();
    }
}

void VideoPlayer::playVideo(int index)
{
    if (index >= 0 && index < videoFiles.size()) {
        player->setMedia(QUrl::fromLocalFile(videoFiles[index]));
        player->play();
    }
}

void VideoPlayer::playNextVideo()
{
    qDebug() << "playNextVideo called.";
    if (currentVideoIndex < videoFiles.size() - 1) {
        ++currentVideoIndex;
        playVideo(currentVideoIndex);
    }
}

void VideoPlayer::playPreviousVideo()
{
    qDebug() << "Current video index before decrement:" << currentVideoIndex;  // Add debug output
    if (currentVideoIndex > 0) {
        --currentVideoIndex;
        playVideo(currentVideoIndex);
    }
}

void VideoPlayer::playPreview(int index) {
    // Your implementation to load and show the preview frame
    // This could be as simple as setting the media to the player and seeking to the first frame
    // But do not play the media.
    if (index >= 0 && index < videoFiles.size()) {
        player->setMedia(QUrl::fromLocalFile(videoFiles[index]));
        player->setPosition(0); // Go to the beginning but don't play
    }
}
void VideoPlayer::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->pos();
    isDragging = true;
    player->pause();  // 暂停视频播放
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent *event) {
    int distance = event->pos().y() - lastMousePos.y();

    if (distance > 0) {
        // 向下滑动，播放上一个视频
        playPreviousVideo();
    } else if (distance < 0) {
        // 向上滑动，播放下一个视频
        playNextVideo();
    }

    isDragging = false;
    player->play();  // 继续视频播放
}

void VideoPlayer::mouseMoveEvent(QMouseEvent *event) {
    if (!isDragging) {
        return;  // 如果不是拖动状态，就不做任何处理
    }

    int distance = event->pos().y() - lastMousePos.y();

    // 根据滑动的距离来“预览”视频
    // 如果距离足够大，可以考虑加载下一个或上一个视频的第一帧进行预览
    // 这里需要一些逻辑来决定何时加载和显示预览
    // 例如：
    if (abs(distance) > 50) { // 50可以根据您的需要调整
        // 这里假设 playPreview 是一个加载并显示视频第一帧的方法
        if (distance > 0) {
            playPreview(currentVideoIndex - 1);
        } else {
            playPreview(currentVideoIndex + 1);
        }
    }
}

bool VideoPlayer::eventFilter(QObject *watched, QEvent *event) {
    if (watched != videoFrame) {
        return QWidget::eventFilter(watched, event);
    }

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    int dy = 0; // 定义变量来存储垂直距离的变化

    switch (event->type()) {
    case QEvent::MouseButtonPress:
        lastMousePos = mouseEvent->pos();
        isDragging = true;
        player->pause();  // 暂停视频播放
        return true;

    case QEvent::MouseMove:
        if (isDragging) {
            dy = mouseEvent->pos().y() - lastMousePos.y();
            if (abs(dy) > 50) { // 50可以根据您的需要调整
                if (dy > 0) {
                    playPreview(currentVideoIndex - 1);
                } else {
                    playPreview(currentVideoIndex + 1);
                }
            }
        }
        return true;

    case QEvent::MouseButtonRelease:
        if (isDragging) {
            dy = mouseEvent->pos().y() - lastMousePos.y();
            if (dy > 0) {
                playPreviousVideo();
            } else if (dy < 0) {
                playNextVideo();
            }
            isDragging = false;
            player->play();  // 继续视频播放
        }
        return true;

    default:
        return QWidget::eventFilter(watched, event);
    }
}

void VideoPlayer::setVolume(int volume) {
    player->setVolume(volume);
}

int VideoPlayer::getVolume() const {
    return player->volume();
}

void VideoPlayer::setPosition(qint64 position) {
    player->setPosition(position);
}

qint64 VideoPlayer::getPosition() const {
    return player->position();
}

qint64 VideoPlayer::getDuration() const {
    return player->duration();
}

// Emit a custom signal when the duration changes
void VideoPlayer::durationChanged(qint64 duration) {
    emit durationChangedSignal(duration);
}

// Emit a custom signal when the position changes
void VideoPlayer::handlePositionChanged(qint64 position) {
    emit positionChangedSignal(position);
}

VideoPlayer::~VideoPlayer()
{
    // No need to delete the UI object since it's not being used
    // Only delete player and videoWidget if they were not added to a layout
    // Qt deletes children widgets when the parent is deleted if they are added to a layout
    delete player;
    delete videoWidget;
}

QVideoWidget* VideoPlayer::getVideoWidget() const {
    return videoWidget;
}

