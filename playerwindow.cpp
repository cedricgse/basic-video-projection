#include "playerwindow.h"
#include "ui_playerwindow.h"

#include <QFileDialog>
#include <QScrollBar>

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);
    closeable = false;

    videoWidget = new VideoWidget();

    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->horizontalScrollBar()->blockSignals(true);
    ui->graphicsView->verticalScrollBar()->blockSignals(true);

    gScene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(gScene);
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    gScene->setBackgroundBrush(Qt::black);
    graphicsWidget = gScene->addWidget(videoWidget);
    videoWidget->setStyleSheet("background-color: black;");

    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setVideoOutput(videoWidget);
}

PlayerWindow::~PlayerWindow()
{
    delete ui;
}

QMediaPlayer* PlayerWindow::getPlayer() const
{
    return mediaPlayer;
}

int PlayerWindow::getX() const
{
    return (int) graphicsWidget->x();
}

int PlayerWindow::getY() const
{
    return (int) graphicsWidget->y();
}

int PlayerWindow::getWidth() const
{
    return videoWidget->width();
}

int PlayerWindow::getHeight() const
{
    return videoWidget->height();
}

qreal PlayerWindow::getAngle() const
{
    return graphicsWidget->rotation();
}

/**
 * @brief Setzt den Parameter, um das Fenster zu schliessen
 *
 * Fenster soll nur ueber Eintrag im ControlWindows geschlossen werden koennen
 *
 * @param close
 */
void PlayerWindow::setCloseable(bool close)
{
    closeable = close;
}

/**
 * @brief Schliessen nur wenn closeable true ist
 *
 * Um unabsichtliches Schliessen zu verhinden, kann nur durch den Eintrag im File-Menu des ControlWindow geschlossen werden
 *
 * @param event
 */
void PlayerWindow::closeEvent(QCloseEvent* event)
{
    if(!closeable) event->ignore();
    else event->accept();
}

void PlayerWindow::setPosition(int x, int y, int w, int h, qreal angle)
{
    videoWidget->setFixedSize(w, h);
    graphicsWidget->setPos(x, y);
    graphicsWidget->setRotation(angle);
}
