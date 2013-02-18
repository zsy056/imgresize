#ifndef _DRAG_MOVE_SCROLL_AREA_H_
#define _DRAG_MOVE_SCROLL_AREA_H_

#include <QScrollArea>
#include <QPoint>

class DragMoveScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    DragMoveScrollArea(QWidget* parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

private:
    QPoint mousePos;
};

#endif
