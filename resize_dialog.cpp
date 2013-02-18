#include <QtCore/qmath.h>
#include <QSettings>
#include <QFileDialog>

#include "resize_dialog.h"

static const QString QUALITY_KEY("resize-quality");
static const QString SAMEDIR_KEY("resize-samedir");
static const QString OUTDIR_KEY("resize-outdir");

ResizeDialog::ResizeDialog(int height, int width, QWidget* parent):
    QDialog(parent), aspectRatio(height*1.0 / width), updating(false)
{
    ui.setupUi(this);
    ui.sbHeight->setValue(height);
    ui.sbWidth->setValue(width);
    ui.sbLongEdge->setValue(qMax(height, width));
    ui.sbFileSize->setValue(estimateFileSize());

    QSettings settings;
    ui.leOutDir->setText(settings.value(OUTDIR_KEY, ".").toString());
    auto samedir = settings.value(SAMEDIR_KEY, true).toBool();
    if (!samedir) {
        ui.rbSelectDir->setChecked(true);
    }
    ui.sbQuality->setValue(settings.value(QUALITY_KEY, 100).toInt());

    connect(this, SIGNAL(accepted()), this, SLOT(accepted()));
    connect(ui.sbHeight, SIGNAL(valueChanged(int)),
            this, SLOT(heightChanged(int)));
    connect(ui.sbWidth, SIGNAL(valueChanged(int)),
            this, SLOT(widthChanged(int)));
    connect(ui.sbLongEdge, SIGNAL(valueChanged(int)),
            this, SLOT(longEdgeChanged(int)));
    connect(ui.sbFileSize, SIGNAL(valueChanged(int)),
            this, SLOT(fileSizeChanged(int)));
    connect(ui.pbChoose, SIGNAL(clicked()), this, SLOT(chooseDir()));
}

void ResizeDialog::chooseDir()
{
    auto dir = QFileDialog::getExistingDirectory(this,
            tr("Choose Directory"), ui.leOutDir->text());
    if (dir != "") {
        ui.leOutDir->setText(dir);
    }
}


void ResizeDialog::fileSizeChanged(int size)
{
    if (updating)
        return;
    updating = true;
    auto width = qSqrt(size*8*1024.0 / (aspectRatio * 12));
    ui.sbWidth->setValue(width);
    auto height = width * aspectRatio;
    ui.sbHeight->setValue(height);
    ui.sbLongEdge->setValue(aspectRatio > 1.0 ? height : width);
    updating = false;
}

int ResizeDialog::estimateFileSize()
{
    return ui.sbHeight->value() * ui.sbWidth->value() * 12 / (8 * 1024);
}

void ResizeDialog::heightChanged(int newHeight)
{
    if (updating)
        return;
    updating = true;
    if (ui.cbKeepRatio->isChecked()) {
        Q_ASSERT(aspectRatio != 0);
        ui.sbWidth->setValue(newHeight / aspectRatio); 
    }
    ui.sbLongEdge->setValue(
            qMax(ui.sbHeight->value(), ui.sbWidth->value()));
    ui.sbFileSize->setValue(estimateFileSize());
    updating = false;
}

void ResizeDialog::widthChanged(int newWidth)
{
    if (updating)
        return;
    updating = true;
    if (ui.cbKeepRatio->isChecked()) {
        ui.sbHeight->setValue(newWidth * aspectRatio);
    }
    ui.sbLongEdge->setValue(
            qMax(ui.sbHeight->value(), ui.sbWidth->value()));
    ui.sbFileSize->setValue(estimateFileSize());
    updating = false;
}

void ResizeDialog::longEdgeChanged(int newSize)
{
    if (updating)
        return;
    updating = true;
    if (aspectRatio > 1.0) {
        ui.sbHeight->setValue(newSize);
        ui.sbWidth->setValue(newSize / aspectRatio);
    } else {
        ui.sbWidth->setValue(newSize);
        ui.sbHeight->setValue(newSize * aspectRatio);
    }
    ui.sbFileSize->setValue(estimateFileSize());
    updating = false;
}

void ResizeDialog::accepted()
{
    emit dialogFinished(ui.sbHeight->value(), ui.sbWidth->value(),
            ui.cbApplyToAll->isChecked(), ui.rbSameDir->isChecked(),
            ui.leOutDir->text(), ui.sbQuality->value());
    QSettings settings;
    settings.setValue(QUALITY_KEY, ui.sbQuality->value());
    settings.setValue(SAMEDIR_KEY, ui.rbSameDir->isChecked());
    settings.setValue(OUTDIR_KEY, ui.leOutDir->text());
}
