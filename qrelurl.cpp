#include "qrelurl.h"
#include <QDebug>

QRelUrl::QRelUrl(const QUrl &other) : QUrl(other)
{
    /*if(other.path() == projectPath.absolutePath())
    {
        isRel = true;
    }
    else
    {
        isRel = false;
    }
    */
    isRel = false;
}
