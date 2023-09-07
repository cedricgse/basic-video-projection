#ifndef PLAYERPOSITIONDIALOG_H
#define PLAYERPOSITIONDIALOG_H

#include <QDialog>
#include "controlwindow.h"

namespace Ui {
class PlayerPositionDialog;
}

class PlayerPositionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerPositionDialog(int x, int y, int w, int h, qreal angle, QVector<Scene *> *pSceneList, ControlWindow *pParent);
    ~PlayerPositionDialog();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    qreal getAngle();

private slots:
    void on_pushButton_apply_clicked();

private:
    Ui::PlayerPositionDialog *ui;
    QVector<Scene*>* sceneList;
    ControlWindow* parent;
};

#endif // PLAYERPOSITIONDIALOG_H
