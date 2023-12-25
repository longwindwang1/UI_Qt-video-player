#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QStringList>
#include <QFrame>

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QFrame *videoFrame, QWidget *parent = nullptr);
    ~VideoPlayer();
    QVideoWidget *getVideoWidget() const;
    void setVolume(int volume);
    int getVolume() const;
    void setPosition(qint64 position);
    qint64 getPosition() const;
    qint64 getDuration() const;

signals:
    void durationChangedSignal(qint64 duration);
    void positionChangedSignal(qint64 position);

private slots:
    void playNextVideo();
    void playPreviousVideo();
    void durationChanged(qint64 duration);
    void handlePositionChanged(qint64 position);
    void onPlayerStateChanged(QMediaPlayer::State state);

private:
    QFrame *videoFrame;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QStringList videoFiles;
    int currentVideoIndex;

    void loadVideoFiles(const QString &folderPath);
    void playVideo(int index);
    void playPreview(int index);

    QPoint lastMousePos;  // 记录鼠标按下的位置
    bool isDragging = false;




protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

};


#endif // VIDEOPLAYER_H

