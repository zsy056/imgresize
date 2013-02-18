#include <QMouseEvent>
#include <QScrollBar>

#include "drag_move_scroll_area.h"

DragMoveScrollArea::DragMoveScrollArea(QWidget* parent):
    QScrollArea(parent)
{
}

void DragMoveScrollArea::mousePressEvent(QMouseEvent* e)
{
    mousePos = e->pos();
}

void DragMoveScrollArea::mouseMoveEvent(QMouseEvent* e)
{
    QPoint diff = e->pos() - mousePos;
    mousePos = e->pos();
    verticalScrollBar()->setValue(verticalScrollBar()->value()
            - diff.y());
    horizontalScrollBar()->setValue(horizontalScrollBar()->value()
            - diff.x());
}
