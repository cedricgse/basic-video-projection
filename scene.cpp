#include "scene.h"

#include <QMessageBox>
#include <QDebug>

Scene::Scene(QString pName, QUrl pFile) : name(pName), file(pFile), x(-1), y(-1), w(-1), h(-1)
{
}

Scene::Scene()
{
}

Scene::~Scene()
{
}

QVector<QVector<Scene*>::iterator> Scene::find(QVector<Scene*>::iterator begin, QVector<Scene*>::iterator end, QString findName)
{
    QVector<QVector<Scene*>::iterator> res;
    for(auto it = begin; it != end; it++)
    {
        if((*it)->getName() == findName) res.append(it);
    }
    return res;
}

bool Scene::checkName(Scene* scene, QVector<Scene*>* sceneList, QString name)
{
    auto res = Scene::find(sceneList->begin(), sceneList->end(), name);
    if(res.length() > 0)
    {
        if(res.length() > 1 || **res.begin() != scene)
        {
            QMessageBox msg("Fehler", "Name bereits vergeben", QMessageBox::NoIcon, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
            msg.exec();
            return false;
        }
    }
    return true;
}

bool Scene::checkFile(QUrl fileURL)
{
    if(fileURL.isEmpty()) return true;
    QFile file(fileURL.toString());
    if(file.exists() && file.open(QFile::ReadOnly) && (file.fileName().endsWith(".mp4", Qt::CaseSensitivity::CaseInsensitive)
                                                       || file.fileName().endsWith(".mov", Qt::CaseSensitivity::CaseInsensitive)
                                                       || file.fileName().endsWith(".m4v", Qt::CaseSensitivity::CaseInsensitive)))
    {
        file.close();
        return true;
    }
    QMessageBox msg("Fehler", "Datei fehlerhaft", QMessageBox::NoIcon, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
    msg.exec();
    return false;
}

void Scene::write(QTextStream &out) const
{
    out << "01" << name << "\n" << "02" << file.toString() << "\n"
        << "03" << x << "\n" << "04" << y << "\n" << "05" << w << "\n" << "06" << h << "\n"
        << "07" << angle << "\n";
}

bool Scene::read(QTextStream &in)
{
    QString input = in.readLine();
    bool ok = false;
    int type = input.leftRef(2).toInt(&ok);
    if(ok)
    {
        input.remove(0, 2);
        switch(type)
        {
        case 1:
            name = input;
            break;
        case 2:
            file = QUrl(input);
            break;
        case 3:
            x = input.toInt();
            break;
        case 4:
            y = input.toInt();
            break;
        case 5:
            w = input.toInt();
            break;
        case 6:
            h = input.toInt();
            break;
        case 7:
            angle = input.toDouble();
            break;
        }
        return true;
    }
    else return false;
}

void Scene::setName(QString pName)
{
    name = pName;
}

void Scene::setFile(QUrl pFile)
{
    file = pFile;
}

void Scene::setPositioning(int x, int y, int w, int h, qreal angle)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->angle = angle;
}

QString Scene::getName()
{
    return name;
}

QUrl Scene::getFile()
{
    return file;
}

int Scene::getX()
{
    return x;
}

int Scene::getY()
{
    return y;
}

int Scene::getWidth()
{
    return w;
}

int Scene::getHeight()
{
    return h;
}

qreal Scene::getAngle()
{
    return angle;
}
