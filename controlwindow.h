#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include "playerwindow.h"
#include "QtAwesome/QtAwesome.h"
#include "scene.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ControlWindow; }
QT_END_NAMESPACE

class ControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    ControlWindow(QWidget *parent = nullptr);
    ~ControlWindow();
    void init(PlayerWindow *p, QtAwesome* pAwesome);
    void fillSceneList(QListWidget *widget, int currentRowPos);
    PlayerWindow* getPlayer();

private slots:
    void on_actionBeenden_triggered();

    void on_actionVollbild_triggered();

    void on_toolButton_play_clicked();

    void on_toolButton_stop_clicked();

    void positionChanged(qint64);

    void on_horizontalSlider_time_sliderReleased();

    void on_horizontalSlider_time_sliderPressed();

    void playStateChanged(QMediaPlayer::State);

    void mediaStatusChanged(QMediaPlayer::MediaStatus);

    void on_pushButton_New_clicked();

    void closeEvent(QCloseEvent* event);

    void on_pushButton_Delete_clicked();

    void on_listWidget_scenes_currentRowChanged(int currentRow);

    void on_pushButton_Save_clicked();

    void on_toolButton_back_clicked();

    void on_toolButton_forward_clicked();

    void on_toolButton_file_clicked();

    void on_actionBearbeitungsmodus_triggered();

    void on_actionPosition_triggered();

    void on_actionProjekt_speichern_triggered();

    void on_actionProjekt_oeffnen_triggered();

private:
    Ui::ControlWindow *ui;
    PlayerWindow *player;
    QMediaPlayer *mediaPlayer;
    bool closeable;
    QtAwesome* awesome;
    QVector<Scene*> sceneList;
    QDir projectDir;

    void fillSceneView(Scene* scene);
    Scene* getCurrentScene();

};
#endif // CONTROLWINDOW_H
