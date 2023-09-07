#include "controlwindow.h"
#include "ui_controlwindow.h"
#include <QFileDialog>
#include <QTime>
#include <QMessageBox>
#include "playerpositiondialog.h"
#include <iostream>
#include <fstream>

#define PARAMETER 7

ControlWindow::ControlWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ControlWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);

    closeable = false;
}

ControlWindow::~ControlWindow()
{
    delete ui;
}

void ControlWindow::init(PlayerWindow* p, QtAwesome* pAwesome)
{
    player = p;
    awesome = pAwesome;
    mediaPlayer = player->getPlayer();
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(playStateChanged(QMediaPlayer::State)));
    connect(mediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));


    ui->toolButton_play->setIcon(awesome->icon(fa::play));
    ui->toolButton_back->setIcon(awesome->icon(fa::fastbackward));
    ui->toolButton_forward->setIcon(awesome->icon(fa::fastforward));
    ui->toolButton_stop->setIcon(awesome->icon(fa::stop));

    projectDir.setPath("");
}

void ControlWindow::fillSceneList(QListWidget* widget, int currentRow)
{
    widget->clear();
    for(auto it = sceneList.begin(); it != sceneList.end(); it++)
    {
        widget->addItem((*it)->getName());
    }
    widget->setCurrentRow(currentRow);
}

void ControlWindow::fillSceneView(Scene *scene)
{
    if(scene == nullptr)
    {
        ui->lineEdit_scene_name->setText("");
        ui->lineEdit_scene_file->setText("");
    }
    else
    {
        ui->lineEdit_scene_name->setText(scene->getName());
        ui->lineEdit_scene_file->setText(scene->getFile().toString());
    }
}

Scene* ControlWindow::getCurrentScene()
{
    auto res = Scene::find(sceneList.begin(), sceneList.end(), ui->listWidget_scenes->currentItem()->text());
    return **res.begin();
}

/**
 * @brief Schliessen nur wenn closeable true ist
 *
 * Um unabsichtliches Schliessen zu verhinden, kann nur durch den Eintrag im File-Menu geschlossen werden
 *
 * @param event
 */
void ControlWindow::closeEvent(QCloseEvent *event)
{
    if(!closeable) event->ignore();
    else event->accept();
}

/**
 * @brief Schliessen beider Fenster
 *
 * Um unabsichtliches Schliessen zu verhindern, kann nur durch diesen Eintrag geschlossen werden. Dazu wird closeable auf true gesetzt
 * und die Fenster werden geschlossen
 */
void ControlWindow::on_actionBeenden_triggered()
{
    closeable = true;
    player->setCloseable(true);
    player->close();
    this->close();
}

/**
 * @brief Vollbildmodus des Player Fensters ein und ausschalten
 */
void ControlWindow::on_actionVollbild_triggered()
{
    if(player->isFullScreen())
    {
        player->showNormal();
        ui->actionVollbild->setChecked(false);
    }
    else
    {
        player->showFullScreen();
        ui->actionVollbild->setChecked(true);
    }
}

/**
 * @brief Play Pause
 */
void ControlWindow::on_toolButton_play_clicked()
{
    if(mediaPlayer->state() == QMediaPlayer::PausedState)
    {
        mediaPlayer->play();
    }
    else if (mediaPlayer->state() == QMediaPlayer::PlayingState)
    {
        mediaPlayer->pause();
    }
}

/**
 * @brief Stop
 */
void ControlWindow::on_toolButton_stop_clicked()
{
    if(mediaPlayer->state() != QMediaPlayer::StoppedState) mediaPlayer->stop();
}

/**
 * @brief Wird aufgerufen, wenn der MediaPlayer das Signal PositionChanged wirft
 *
 * Ueberprueft, ob der Slider das richtige Maximum eingestellt hat, setzt diese ansonsten und setzt den Slider an die richtige Stelle
 *
 * @param time
 */
void ControlWindow::positionChanged(qint64 time)
{
    QAbstractSlider* slider = ui->horizontalSlider_time;
    qint64 duration = mediaPlayer->duration();
    if(slider->maximum() != mediaPlayer->duration()) slider->setMaximum(duration);
    slider->setValue(time);
    QTime labelTime((time/3600000) % 60, (time/60000) % 60, (time/1000) % 60);
    QTime labelDuration((duration/3600000) % 60, (duration/60000) % 60, (duration/1000) % 60);
    ui->label_timestamp->setText(QString("%1/%2").arg(labelTime.toString("hh:mm:ss"), labelDuration.toString("hh:mm:ss")));
}

/**
 * @brief Trennt die Verbindung des positionChanged Signals damit der Slider nicht waehrend des Bewegens springt
 */
void ControlWindow::on_horizontalSlider_time_sliderPressed()
{
    disconnect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
}

/**
 * @brief Setzt die neue Position im MediaPlayer und verbindet das positionChanged Signal erneut
 */
void ControlWindow::on_horizontalSlider_time_sliderReleased()
{
    mediaPlayer->setPosition(ui->horizontalSlider_time->sliderPosition());
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
}

/**
 * @brief Aendert das Icon des Play Pause Buttons je nach neuem Status des Players
 * @param newState
 */
void ControlWindow::playStateChanged(QMediaPlayer::State newState)
{
    if(newState == QMediaPlayer::PlayingState) ui->toolButton_play->setIcon(awesome->icon(fa::pause));
    else ui->toolButton_play->setIcon(awesome->icon(fa::play));
}

void ControlWindow::mediaStatusChanged(QMediaPlayer::MediaStatus state)
{
    if(state == QMediaPlayer::EndOfMedia)
    {
        mediaPlayer->setMedia(QMediaContent());
    }
}

/**
 * @brief Fügt eine Neue Szene hinzu und lässt die SzenenListenAnzeige neu laden
 */
void ControlWindow::on_pushButton_New_clicked()
{
    int i = 1;
    QString name;
    name = QString("NeueSzene%1").arg(i);
    while(Scene::find(sceneList.begin(), sceneList.end(), name).length() != 0)
    {
        i++;
        name = QString("NeueSzene%1").arg(i);
    }
    Scene* scene = new Scene(name);
    sceneList.append(scene);
    ui->listWidget_scenes->addItem(name);
    ui->listWidget_scenes->setCurrentRow(sceneList.length()-1);
}

/**
 * @brief Loescht die ausgewaehlte Szene
 *
 * Fragt, ob wirklich geloescht werden soll und waehlt eine neue aktuelle Szene aus
 */
void ControlWindow::on_pushButton_Delete_clicked()
{
    if(ui->listWidget_scenes->currentItem() != nullptr)
    {
        QMessageBox msg;
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setText(QString("Möchten Sie %1 wirklich löschen?").arg(ui->listWidget_scenes->currentItem()->text()));
        msg.setWindowTitle("Löschen");
        msg.setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");
        int reply = msg.exec();
        if(reply == QMessageBox::Yes)
        {
            auto res = Scene::find(sceneList.begin(), sceneList.end(), ui->listWidget_scenes->currentItem()->text());
            int pos = *res.begin() - sceneList.begin();
            sceneList.erase(*res.begin());
            if(pos == sceneList.length()) fillSceneList(ui->listWidget_scenes, pos-1);
            else fillSceneList(ui->listWidget_scenes, pos);
        }
    }
}

/**
 * @brief Aktualisiert die Anzeige der Szeneneigenschaften, wenn eine neue ausgewaehlt wird
 *
 * Aktualisiert mit den Eigenschaften der aktuellen Szene oder mit leeren Feldern, wenn die Szenenliste leer ist
 *
 * @param currentRow
 */
void ControlWindow::on_listWidget_scenes_currentRowChanged(int currentRow)
{
    if(currentRow != -1)
    {
        Scene* currentScene = getCurrentScene();
        fillSceneView(currentScene);
        if(!ui->actionBearbeitungsmodus->isChecked())
        {
            if(currentScene->getX() != -1 && (currentScene->getX() != player->getX()
                    || currentScene->getY() != player->getY()
                    || currentScene->getWidth() != player->getWidth()
                    || currentScene->getHeight() != player->getHeight()
                    || currentScene->getAngle() != player->getAngle()))
            {
                player->setPosition(currentScene->getX(), currentScene->getY(), currentScene->getWidth(), currentScene->getHeight(), currentScene->getAngle());
            }
            if(!currentScene->getFile().isEmpty())
            {
                mediaPlayer->setMedia(QMediaContent(currentScene->getFile()));
                mediaPlayer->play();
            }
            else
            {
                mediaPlayer->setMedia(QMediaContent());
            }
        }
    }
    else fillSceneView(nullptr);
}

void ControlWindow::on_pushButton_Save_clicked()
{
    Scene* scene = getCurrentScene();
    if(Scene::checkName(scene, &sceneList, ui->listWidget_scenes->currentItem()->text()))
    {
        scene->setName(ui->lineEdit_scene_name->text());
        ui->listWidget_scenes->currentItem()->setText(ui->lineEdit_scene_name->text());
    }
    else
    {
        return;
    }
    if(Scene::checkFile(QUrl(ui->lineEdit_scene_file->text())))
    {
        scene->setFile(QUrl(ui->lineEdit_scene_file->text()));
    }
    else
    {
        return;
    }
}

void ControlWindow::on_toolButton_back_clicked()
{
    if(ui->listWidget_scenes->currentRow() != 0) ui->listWidget_scenes->setCurrentRow(ui->listWidget_scenes->currentRow()-1);
}

void ControlWindow::on_toolButton_forward_clicked()
{
    if(ui->listWidget_scenes->currentRow() != sceneList.length()-1) ui->listWidget_scenes->setCurrentRow(ui->listWidget_scenes->currentRow()+1);
}

void ControlWindow::on_toolButton_file_clicked()
{
    QUrl path = QFileDialog::getOpenFileUrl(this, "Datei öffnen", QUrl(), tr("Media (*.mov *.mp4 *.m4v)"));
    ui->lineEdit_scene_file->setText(path.toString(QUrl::PreferLocalFile | QUrl::RemoveScheme));
}

void ControlWindow::on_actionBearbeitungsmodus_triggered()
{
    if(!ui->actionBearbeitungsmodus->isChecked())
    {
        ui->pushButton_New->setEnabled(false);
        ui->pushButton_Save->setEnabled(false);
        ui->pushButton_Delete->setEnabled(false);
        ui->lineEdit_scene_name->setEnabled(false);
        ui->lineEdit_scene_file->setEnabled(false);
        ui->toolButton_file->setEnabled(false);
    }
    else
    {
        ui->pushButton_New->setEnabled(true);
        ui->pushButton_Save->setEnabled(true);
        ui->pushButton_Delete->setEnabled(true);
        ui->lineEdit_scene_name->setEnabled(true);
        ui->lineEdit_scene_file->setEnabled(true);
        ui->toolButton_file->setEnabled(true);
    }
}

void ControlWindow::on_actionPosition_triggered()
{
    PlayerPositionDialog p(player->getX(), player->getY(), player->getWidth(), player->getHeight(), player->getAngle(), &sceneList, this);
    int res = p.exec();
    if(res == 0) return;
    else
    {
        player->setPosition(p.getX(), p.getY(), p.getWidth(), p.getHeight(), p.getAngle());
    }
}

PlayerWindow* ControlWindow::getPlayer()
{
    return player;
}

void ControlWindow::on_actionProjekt_speichern_triggered()
{
    qDebug() << "Save";
    if(projectDir.path() == "")
    {
        qDebug() << "New Dir";
        projectDir.setPath(QFileDialog::getExistingDirectory());
    }
    QFile file(projectDir.absolutePath()+"/scenes.bvp");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Open";
        QTextStream stream(&file);
        for(auto it = sceneList.begin(); it != sceneList.end(); it++)
        {
            (*it)->write(stream);
        }
    }
    else
    {
        QMessageBox msg("Fehler", "Kann nicht schreiben", QMessageBox::NoIcon, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        msg.exec();
    }
}

void ControlWindow::on_actionProjekt_oeffnen_triggered()
{
    projectDir.setPath(QFileDialog::getExistingDirectory());
    QFile file(projectDir.absolutePath()+"/scenes.bvp");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        Scene* scene;
        sceneList.clear();
        while(!stream.atEnd())
        {
            scene = new Scene();
            for(int i = 0; i<PARAMETER; i++)
            {
                if(!scene->read(stream))
                {
                    QMessageBox msg("Fehler", "Inhalt fehlerhaft", QMessageBox::NoIcon, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                    msg.exec();
                    return;
                }
            }
            sceneList.append(scene);
        }
        fillSceneList(ui->listWidget_scenes, 0);
    }
    else
    {
        QMessageBox msg("Fehler", "Kann nicht öffnen", QMessageBox::NoIcon, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        msg.exec();
    }
}
