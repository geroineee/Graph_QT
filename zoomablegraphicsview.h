#ifndef ZOOMABLEGRAPHICSVIEW_H
#define ZOOMABLEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>

#include <QApplication>

class ZoomableGraphicsView : public QGraphicsView
{
public:
    ZoomableGraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent), minZoomFactor(0.5), maxZoomFactor(2.0) {}

    void setMinZoomFactor(qreal factor) { minZoomFactor = factor; }
    void setMaxZoomFactor(qreal factor) { maxZoomFactor = factor; }

protected:
    void wheelEvent(QWheelEvent *event) override
    {
        if (QApplication::keyboardModifiers() == Qt::ControlModifier)
        {
            qreal factor = transform().scale(1.2, 1.2).mapRect(QRectF(0, 0, 1, 1)).width();

            // Увеличение масштаба
            if (event->angleDelta().y() > 0 && factor < maxZoomFactor)
            {
               scale(1.2, 1.2);
            }

            // Уменьшение масштаба
            else if (event->angleDelta().y() < 0 && factor > minZoomFactor)
            {
               scale(1 / 1.2, 1 / 1.2);
            }
        }
        else
        {
            QGraphicsView::wheelEvent(event);
        }
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::ControlModifier)
        {
            setDragMode(QGraphicsView::ScrollHandDrag);
        }
        QGraphicsView::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            setDragMode(QGraphicsView::NoDrag);
        }
        QGraphicsView::mouseReleaseEvent(event);
    }

private:
    qreal minZoomFactor;
    qreal maxZoomFactor;
};

#endif // ZOOMABLEGRAPHICSVIEW_H
