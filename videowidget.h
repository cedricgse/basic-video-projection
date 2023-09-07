#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>

class VideoWidget : public QVideoWidget
{
public:
    explicit VideoWidget(QWidget *parent = nullptr);
};

#endif // VIDEOWIDGET_H
