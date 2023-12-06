#ifndef CW2_THE_BUTTON_H
#define CW2_THE_BUTTON_H

#include <QPushButton>
#include <QUrl>
#include <QMediaPlayer> // Include QMediaPlayer

class TheButtonInfo {
public:
    QUrl* url; // video file to play
    QIcon* icon; // icon to display

    TheButtonInfo(QUrl* url, QIcon* icon) : url(url), icon(icon) {}
};

class TheButton : public QPushButton {
    Q_OBJECT

public:
    TheButtonInfo* info;
    explicit TheButton(QWidget *parent = nullptr);

    void init(TheButtonInfo* i);
    void setPlayer(QMediaPlayer *player); // Method to set the media player

private slots:
    void clicked();
    void togglePlayPause(); // Slot for toggling play and pause

private:
    QMediaPlayer *player; // Media player reference
    bool isPlaying; // Track play/pause state

signals:
    void jumpTo(TheButtonInfo*);
};

#endif // CW2_THE_BUTTON_H
