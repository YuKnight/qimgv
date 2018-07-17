#ifndef THUMBNAILSTRIP_H
#define THUMBNAILSTRIP_H

#include <QApplication>
#include <QLabel>
#include <QBoxLayout>
#include <QScrollArea>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QMutex>
#include <QTimer>
#include <QPainter>
#include <QResizeEvent>
#include <cmath>

#include "gui/customwidgets/thumbnailview.h"
#include "sourcecontainers/thumbnail.h"

class ThumbnailStrip : public ThumbnailView
{
    Q_OBJECT
public:
    explicit ThumbnailStrip(QWidget *parent = nullptr);

private:
    int panelSize;

    int current, thumbnailSpacing;

    QTimer loadTimer;

    void updateThumbnailPositions(int start, int end);
    void updateThumbnailPositions();
    void fitSceneToContents();
    void setThumbnailSize(int);
    void updateThumbnailSize();

    void setupLayout();
signals:

public slots:
    void loadVisibleThumbnailsDelayed();
    void highlightThumbnail(int pos);

    //void removeItemAt(int pos);
    //void createLabelAt(int pos);

protected:
    virtual void resizeEvent(QResizeEvent *event);
    void ensureThumbnailVisible(int pos);
    void addItemToLayout(ThumbnailLabel *widget, int pos);
    void removeItemFromLayout(int pos);
    ThumbnailLabel *createThumbnailWidget();
};

#endif // THUMBNAILSTRIP_H
