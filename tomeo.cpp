//
//    ______
//   /_  __/___  ____ ___  ___  ____
//    / / / __ \/ __ `__ \/ _ \/ __ \
//   / / / /_/ / / / / / /  __/ /_/ /
//  /_/  \____/_/ /_/ /_/\___/\____/
//              video for sports enthusiasts...
//
//

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QVideoWidget>
#include <QSlider>
#include <QToolButton>
#include <QMenu>
#include <QListWidget>
#include <QWidgetAction>
#include "the_player.h"
#include "the_button.h"

        // read in videos and thumbnails to this directory
        std::vector<TheButtonInfo> getInfoIn (std::string loc) {

    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

        if (f.contains("."))

#if defined(_WIN32)
        if (f.contains(".wmv"))  { // windows
#else
        if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                QImage sprite = imageReader->read(); // read the thumbnail
                if (!sprite.isNull()) {
                    QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                    QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                    out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                }
                else
                    qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // Collect all the videos in the folder
    std::vector<TheButtonInfo> videos;
    if (argc == 2)
        videos = getInfoIn(std::string(argv[1]));

    if (videos.size() == 0) {
        QMessageBox::information(
            nullptr,
            QString("Tomeo"),
            QString("No videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);
    }

    // The widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // The QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // A row of buttons
    QWidget *buttonWidget = new QWidget();
    std::vector<TheButton*> buttons;
    QHBoxLayout *layout = new QHBoxLayout(buttonWidget);

    // Create the buttons
    for (size_t i = 0; i < videos.size(); ++i) {
        TheButton *button = new TheButton(buttonWidget);
        QObject::connect(button, SIGNAL(jumpTo(TheButtonInfo*)), player, SLOT(jumpTo(TheButtonInfo*)));
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    // Tell the player what buttons and videos are available
    player->setContent(&buttons, &videos);

    // Create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("Tomeo");
    window.setMinimumSize(800, 680);

    // Add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(buttonWidget);

    // Create the play/pause button and add it to the layout
    TheButton *playPauseButton = new TheButton(&window);
    playPauseButton->setText("Pause");
    playPauseButton->setPlayer(player);
    // Set a fixed, smaller size for the button
    playPauseButton->setFixedSize(100, 50); // Adjust the size as needed

    // Create a layout to center the play/pause button
    QWidget *buttonContainer = new QWidget();
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->addStretch();    // Spacer to push the button to the center
    buttonLayout->addWidget(playPauseButton);
    buttonLayout->addStretch();    // Spacer to push the button to the center

    top->addWidget(playPauseButton);

    QSlider *progressSlider = new QSlider(Qt::Horizontal, &window);
    progressSlider->setRange(0, player->duration()); // 初始范围

    // 连接播放器的信号到进度条
    QObject::connect(player, &QMediaPlayer::positionChanged, [progressSlider](qint64 position) {
        progressSlider->setValue(static_cast<int>(position));
    });
    QObject::connect(player, &QMediaPlayer::durationChanged, [progressSlider](qint64 duration) {
        progressSlider->setRange(0, static_cast<int>(duration));
    });

    // 当进度条移动时，更新视频播放位置
    QObject::connect(progressSlider, &QSlider::sliderMoved, [player](int position) {
        player->setPosition(position);
    });
    top->addWidget(progressSlider);

    QPushButton *forwardButton = new QPushButton("Forward 5s", &window);
    QPushButton *backwardButton = new QPushButton("Backward 5s", &window);

    // 连接按钮的点击事件到相应的槽
    QObject::connect(forwardButton, &QPushButton::clicked, [player]() {
        player->setPosition(player->position() + 5000); // 前进五秒
    });
    QObject::connect(backwardButton, &QPushButton::clicked, [player]() {
        qint64 newPosition = player->position() - 5000;
        player->setPosition(newPosition > 0 ? newPosition : 0); // 后退五秒，不要小于0
    });

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(backwardButton);
    controlLayout->addWidget(forwardButton);

    top->addLayout(controlLayout);

    QSlider *volumeSlider = new QSlider(Qt::Horizontal, &window);
    volumeSlider->setRange(0, 100);  // 音量范围从 0 到 100
    volumeSlider->setValue(10);      // 默认音量值设为 50
    volumeSlider->setFixedSize(200, 30); // 设置滑动条的宽度为 200，高度为 30
    QObject::connect(volumeSlider, &QSlider::valueChanged, player, &QMediaPlayer::setVolume);

    top->addWidget(volumeSlider);

    QPushButton *replayButton = new QPushButton("Replay", &window);

    // 连接重新播放按钮的点击事件到播放器的槽函数
    QObject::connect(replayButton, &QPushButton::clicked, [player]() {
        player->setPosition(0); // 将播放位置重置到起始点
        player->play();         // 开始播放
    });
    top->addWidget(replayButton);

    QPushButton *prevButton = new QPushButton("Previous", &window);
    QPushButton *nextButton = new QPushButton("Next", &window);

    size_t currentIndex = 0;  // 当前视频索引

    auto playVideo = [&](size_t index) {
        if (index < videos.size()) {
            currentIndex = index;
            TheButtonInfo &info = videos.at(index);
            player->setMedia(*info.url);
            player->play();
        }
    };

    // 连接按钮的点击事件到槽函数
    QObject::connect(prevButton, &QPushButton::clicked, [&]() {
        if (currentIndex > 0) {
            playVideo(currentIndex - 1);
        }
    });
    QObject::connect(nextButton, &QPushButton::clicked, [&]() {
        if (currentIndex < videos.size() - 1) {
            playVideo(currentIndex + 1);
        }
    });



    controlLayout->addWidget(prevButton);
    controlLayout->addWidget(nextButton);
    top->addLayout(controlLayout);

    QToolButton *fileMenuButton = new QToolButton(&window);
    fileMenuButton->setText("Select Video");

    QListWidget *fileList = new QListWidget(&window);
    for (size_t i = 0; i < videos.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(videos[i].url->fileName(), fileList);
        item->setData(Qt::UserRole, QVariant::fromValue(*videos[i].url)); // 以值存储

    }

    QObject::connect(fileList, &QListWidget::itemClicked, [player](QListWidgetItem *item) {
        QUrl url = item->data(Qt::UserRole).value<QUrl>(); // 以值检索
        player->setMedia(url);

    });

    QWidgetAction *popupAction = new QWidgetAction(fileMenuButton);
    popupAction->setDefaultWidget(fileList);

    QMenu *popupMenu = new QMenu(fileMenuButton);
    popupMenu->addAction(popupAction);
    fileMenuButton->setMenu(popupMenu);
    fileMenuButton->setPopupMode(QToolButton::InstantPopup);


    top->addWidget(fileMenuButton);
    // Show the main window
    window.show();

    // Execute the application
    return app.exec();
}
