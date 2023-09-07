#ifndef SCENE_H
#define SCENE_H

#include <QString>
#include <QUrl>
#include "qrelurl.h"
#include <QTextStream>

class Scene
{
public:
    Scene(QString pName, QUrl pFile = QUrl());
    Scene();
    ~Scene();

    static QVector<QVector<Scene*>::iterator> find(QVector<Scene*>::iterator begin, QVector<Scene*>::iterator end, QString findName);
    static bool checkName(Scene* scene, QVector<Scene*>* sceneList, QString name);
    static bool checkFile(QUrl fileURL);

    void setName(QString pName);
    void setFile(QUrl pFile);
    void setPositioning(int x, int y, int w, int h, qreal angle);

    void write(QTextStream &out) const;
    bool read(QTextStream &in);

    QString getName();
    QUrl getFile();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    qreal getAngle();

private:
    QString name;
    QUrl file;
    int x, y, w, h;
    qreal angle;
};

#endif // SCENE_H
