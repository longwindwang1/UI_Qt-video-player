//
//
//
#include "the_button.h"

TheButton::TheButton(QWidget *parent)
    : QPushButton(parent), player(nullptr), isPlaying(false) {
    setIconSize(QSize(200,110));
    connect(this, SIGNAL(released()), this, SLOT(clicked())); // Existing connection
}

void TheButton::init(TheButtonInfo* i) {
    setIcon(*(i->icon));
    info = i;
}

void TheButton::setPlayer(QMediaPlayer *p) {
    player = p;
    connect(this, SIGNAL(released()), this, SLOT(togglePlayPause())); // Connect to the new slot
}

void TheButton::clicked() {
    emit jumpTo(info);
}

void TheButton::togglePlayPause() {
    if (!player) return; // Check if player is set

    if (isPlaying) {
        player->pause();
        setText("Play");
    } else {
        player->play();
        setText("Pause");
    }
    isPlaying = !isPlaying;
}
