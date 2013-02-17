#ifndef _RESIZE_DIALOG_H_
#define _RESIZE_DIALOG_H_

#include "ui_resize_dialog.h"

class ResizeDialog : public QDialog
{
    Q_OBJECT
public:
    ResizeDialog(int height, int width, QWidget* parent = nullptr);
    
signals:
    void dialogFinished(int height, int width, int longest, bool isLongest,
            int fileSize, bool applyToAll);

private slots:
    void accepted();
    void heightChanged(int newHeight);
    void widthChanged(int newWidth);
    void longEdgeChanged(int newSize);

private:
    Ui::ResizeDialog ui;
    int height;
    int width;
    int longest;
};

#endif
