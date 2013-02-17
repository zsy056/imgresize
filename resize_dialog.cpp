#include "resize_dialog.h"

ResizeDialog::ResizeDialog(int height, int width, QWidget* parent):
    QDialog(parent), height(height), width(width),
    longest(qMax(height, width))
{
    ui.setupUi(this);
    ui.sbHeight->setValue(height);
    ui.sbWidth->setValue(width);
    ui.sbLongEdge->setValue(longest);

    connect(this, SIGNAL(accepted()), this, SLOT(accepted()));
    connect(ui.sbHeight, SIGNAL(valueChanged(int)),
            this, SLOT(heightChanged(int)));
    connect(ui.sbWidth, SIGNAL(valueChanged(int)),
            this, SLOT(widthChanged(int)));
    connect(ui.sbLongEdge, SIGNAL(valueChanged(int)),
            this, SLOT(longEdgeChanged(int)));
}

void ResizeDialog::heightChanged(int newHeight)
{
    if (height == newHeight) {
        return;
    }
    if (ui.cbKeepRatio->isChecked()) {
        Q_ASSERT(height != 0);
        width *= newHeight*1.0 / height;
        ui.sbWidth->setValue(width); 
    }
    height = newHeight;
    if (height > width) {
        longest = height;
        ui.sbLongEdge->setValue(height);
    }
}

void ResizeDialog::widthChanged(int newWidth)
{
    if (width == newWidth) {
        return;
    }
    if (ui.cbKeepRatio->isChecked()) {
        Q_ASSERT(width != 0);
        height *= newWidth*1.0 / width;
        ui.sbHeight->setValue(height);
    }
    width = newWidth;
    if (width > height) {
        longest = width;
        ui.sbLongEdge->setValue(longest);
    }
}

void ResizeDialog::longEdgeChanged(int newSize)
{
    if (newSize == longest) {
        return;
    }
    if (height > width) {
        Q_ASSERT(height != 0);
        width *= newSize*1.0 / height;
        height = newSize;
    } else {
        Q_ASSERT(width != 0);
        height *= newSize*1.0 / width;
        width = newSize;
    }
    ui.sbHeight->setValue(height);
    ui.sbWidth->setValue(width);
}

void ResizeDialog::accepted()
{
    emit dialogFinished(ui.sbHeight->value(), ui.sbWidth->value(),
            ui.sbLongEdge->value(), ui.rbSetLongest->isChecked(),
            ui.sbFileSize->value(), ui.cbApplyToAll->isChecked());
}
