#ifndef _RESIZE_DIALOG_H_
#define _RESIZE_DIALOG_H_

#include "ui_resize_dialog.h"

class ResizeDialog : public QDialog
{
    Q_OBJECT
public:
    ResizeDialog(int height, int width, QWidget* parent = nullptr);
    
signals:
    void dialogFinished(int height, int width, bool applyToAll,
            bool sameDir, const QString& outdr, int quality);

private slots:
    void accepted();
    void heightChanged(int newHeight);
    void widthChanged(int newWidth);
    void longEdgeChanged(int newSize);
    void fileSizeChanged(int size);
    void chooseDir();

private:
    Ui::ResizeDialog ui;
    qreal aspectRatio;
    bool updating;

    int estimateFileSize();
};

#endif
