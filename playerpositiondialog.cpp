#include "playerpositiondialog.h"
#include "ui_playerpositiondialog.h"

PlayerPositionDialog::PlayerPositionDialog(int x, int y, int w, int h, qreal angle, QVector<Scene*>* pSceneList, ControlWindow *pParent) :
    QDialog(pParent),
    ui(new Ui::PlayerPositionDialog), sceneList(pSceneList), parent(pParent)
{
    ui->setupUi(this);
    ui->spinBox_x->setValue(x);
    ui->spinBox_y->setValue(y);
    ui->spinBox_w->setValue(w);
    ui->spinBox_h->setValue(h);
    ui->doubleSpinBox_angle->setValue(angle);

    pParent->fillSceneList(ui->listWidget_scenes, 0);

    this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

PlayerPositionDialog::~PlayerPositionDialog()
{
    delete ui;
}

int PlayerPositionDialog::getX()
{
    return ui->spinBox_x->value();
}

int PlayerPositionDialog::getY()
{
    return ui->spinBox_y->value();
}

int PlayerPositionDialog::getWidth()
{
    return ui->spinBox_w->value();
}

int PlayerPositionDialog::getHeight()
{
    return ui->spinBox_h->value();
}

qreal PlayerPositionDialog::getAngle()
{
    return ui->doubleSpinBox_angle->value();
}

void PlayerPositionDialog::on_pushButton_apply_clicked()
{
    QList<QListWidgetItem*> selected = ui->listWidget_scenes->selectedItems();
    if(selected.length() != 0)
    {
        Scene* res;
        QVector<QVector<Scene*>::iterator> resVec;
        for(auto it = selected.begin(); it != selected.end(); it++)
        {
             resVec = Scene::find(sceneList->begin(), sceneList->end(), it.i->t()->text());
             res = **resVec.begin();
             res->setPositioning(getX(), getY(), getWidth(), getHeight(), getAngle());
        }
        parent->getPlayer()->setPosition(getX(), getY(), getWidth(), getHeight(), getAngle());
    }
}
