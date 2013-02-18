#include <QtCore/QDebug>
#include <QMessageBox>
#include <QtConcurrentRun>
#include <QMovie>
#include <QDir>
#include <QScrollBar>

#include "tab_page.h"

const QString TabPage::SAVE_PREFIX(tr("Resized-"));

TabPage::TabPage(const QString& fullpath, QWidget* parent):
    QWidget(parent), displayScaleFactor(1.0), fullpath(fullpath)
{
    ui.setupUi(this);
    QMovie* loadingMovie = new QMovie(":/image/images/loading.gif");
    ui.label->setMovie(loadingMovie);
    loadingMovie->start();

    ui.label->setBackgroundRole(QPalette::Base);
    ui.scrollArea->setBackgroundRole(QPalette::Dark);

    connect(&loadWatcher, SIGNAL(finished()), this, SLOT(displayImage()));
    connect(this, SIGNAL(loadFinish()), this, SLOT(displayImage()));
}         

QString TabPage::getFullpath() const
{
    return fullpath;
}

void TabPage::displayScale(double scale)
{
    if ((displayScaleFactor > 3.0 && scale > 1.0)
            || (displayScaleFactor < 0.1 && scale < 1.0)) {
        return;
    }
    //ui.scrollArea->setWidgetResizable(false);
    Q_ASSERT(ui.label->pixmap() != nullptr);
    displayScaleFactor *= scale;
    auto scaledSize = displayScaleFactor * ui.label->pixmap()->size();
    //ui.scrollArea->resize(scaledSize);
    ui.label->resize(scaledSize);
    // ensure the alignment of label is correct
    ui.label->setMinimumSize(scaledSize);
    ui.label->setMaximumSize(scaledSize);

    adjustScrollBar(ui.scrollArea->horizontalScrollBar(), scale);
    adjustScrollBar(ui.scrollArea->verticalScrollBar(), scale);

    qDebug() << "Scaled" << displayScaleFactor;
}

void TabPage::adjustScrollBar(QScrollBar* scrollbar, double scale)
{
    scrollbar->setValue(scale * scrollbar->value()
            + ((scale-1) * scrollbar->pageStep()/2));
}

void TabPage::zoomIn()
{
    displayScale(1.25);
}

void TabPage::zoomOut()
{
    displayScale(0.8);
}

int TabPage::getHeight() const
{
    return image.height();
}

int TabPage::getWidth() const
{
    return image.width();
}

void TabPage::save(int height, int width, const QString& outdir, int quality)
{
    if (image.isNull()) {
        doLoadImage(fullpath);
        emit loadFinish();
    }
    if (image.isNull()) {
        qDebug() << fullpath << "Not saved";
        return;
    }
    auto scaledImg = image.scaled(width, height,
            Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    auto path = QDir(outdir).absoluteFilePath(
            SAVE_PREFIX + QFileInfo(fullpath).baseName()+".jpg");
    scaledImg.save(path, 0, quality);
    qDebug() << "Save done" << path;
}

bool TabPage::isLoaded(const QString& fullpath) const
{
    return !image.isNull() && this->fullpath == fullpath;
}

void TabPage::loadImage(const QString& fullpath)
{
    auto future = QtConcurrent::run(this,
            &TabPage::doLoadImage, fullpath);
    loadWatcher.setFuture(future);
}

void TabPage::loadImage()
{
    if (isLoaded(fullpath)) {
        return;
    }
    loadImage(fullpath);
}

void TabPage::doLoadImage(const QString& fullpath)
{
    this->fullpath = fullpath;
    image = QImage(fullpath);
    if (image.isNull()) {
        emit loadError(fullpath);
    }
    qDebug() << "Image loaded:" << fullpath;
}

void TabPage::displayImage()
{
    if (image.isNull()) {
        return;
    }
    if (image.height() > image.width() && image.height() > 900) {
        ui.label->setPixmap(
                QPixmap::fromImage(image.scaledToHeight(900)));
    } else if (image.width() > 900) {
        ui.label->setPixmap(
                QPixmap::fromImage(image.scaledToWidth(900)));
    }
}

/*void TabPage::displayFit()
{
    Q_ASSERT(ui.label->pixmap() != nullptr);
    auto oheight = ui.label->pixmap()->height();
    auto owidth = ui.label->pixmap()->width();
    double scale = 1.0;
    if (oheight > owidth && oheight > 900) {
        scale = 900.0 / oheight;
    } else if (owidth > 900) {
        scale = 900.0 / owidth;
    }
    displayScale(scale);
}   */
