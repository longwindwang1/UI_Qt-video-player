#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>

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

    void on_play_clicked();

private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
