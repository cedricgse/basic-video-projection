#ifndef QRELURL_H
#define QRELURL_H

#include <QUrl>
#include <QDir>

class QRelUrl : public QUrl
{
public:
    QRelUrl(const QUrl &other);
    static QDir projectPath;

private:
    bool isRel;
};

#endif // QRELURL_H
