#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QVideoWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    QList<QUrl> videoUrls;
    ~Widget();

private slots:


    void on_file_clicked();

    void on_play_clicked();

    void on_volume_clicked();

    void on_volumeslider_actionTriggered(int action);

    void on_backward_clicked();

    void on_forward_clicked();

    void on_playslider_actionTriggered(int action);


private:
    Ui::Widget *ui;
    QMediaPlayer *player;  // 新增播放器实例

};
#endif // WIDGET_H
