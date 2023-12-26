#ifndef WIDGET_H
#define WIDGET_H

#include <QApplication>
#include <QStackedWidget>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFileDialog>
#include <QProcess>
#include "videoplayer.h"  // 包含 VideoPlayer 类的头文件
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlayer>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QShortcut>



QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:


    void on_file_clicked();

    void on_pause_clicked();

    void on_playerslider_sliderMoved(int position);
    void on_volumeslider_sliderMoved(int position);
    void updatePlaySlider(qint64 position);

    void on_volume_clicked();

    void on_forward_clicked();

    void on_backward_clicked();

    void updateVideoNameLabel(const QString &name);

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_fullscreen_clicked();

private:
    Ui::Widget *ui;
    VideoPlayer *videoPlayer;  // 添加 VideoPlayer 类型的成员变量
    bool isPlaying = true;
    bool isMuted = true;

    QMediaPlayer *player;
    QList<QString> videoFiles; // 用于存储视频文件路径的列表
    int currentVideoIndex;
    void loadVideoFiles(const QString &folderPath);  // Function to load video files
    void playVideo(int index);        // Function to play a video by index

protected:
    void keyPressEvent(QKeyEvent *event) override;

};
#endif // WIDGET_H


