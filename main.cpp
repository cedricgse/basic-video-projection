#include "controlwindow.h"

#include <QApplication>
#include "QtAwesome/QtAwesome.h"
#include "qrelurl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControlWindow w;
    QtAwesome* awesome = new QtAwesome();
    awesome -> initFontAwesome();
    awesome -> setDefaultOption("color", QColor(200, 200, 200));
    PlayerWindow p;
    w.setWindowIcon(awesome->icon(fa::filemovieo));
    p.setWindowIcon(awesome->icon(fa::filemovieo));
    w.init(&p, awesome);
    w.show();
    p.show();
    return a.exec();
}
