#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>

#include "mainwindow.h"


class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = nullptr);
    void setMainPtr(MainWindow *m);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    MainWindow *main;
};




GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setAcceptDrops(true);
}

void GraphicsView::setMainPtr(MainWindow *m)
{
    this->main = m;
}

void GraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void GraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void GraphicsView::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        QString file = event->mimeData()->urls().at(0).path();
        main->load(file);
        event->accept();
    }
}

#endif // GRAPHICSVIEW_H
