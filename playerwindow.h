#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include "videowidget.h"

namespace Ui {
class PlayerWindow;
}

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow();
    void setCloseable(bool);
    QMediaPlayer* getPlayer() const;
    void setPosition(int x, int y, int w, int h, qreal angle);
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    qreal getAngle() const;

private:
    Ui::PlayerWindow *ui;
    QGraphicsScene* gScene;
    QMediaPlayer *mediaPlayer;
    VideoWidget* videoWidget;
    QGraphicsProxyWidget *graphicsWidget;
    bool closeable;
    void closeEvent(QCloseEvent* event);
};

#endif // PLAYERWINDOW_H
