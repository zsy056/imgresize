#ifndef _TAB_PAGE_H_
#define _TAB_PAGE_H_

#include <QFutureWatcher>

#include "ui_tab_page.h"

class TabPage : public QWidget
{
    Q_OBJECT
public:
    TabPage(const QString& fullpath, QWidget* parent = nullptr);
    void loadImage();
    void loadImage(const QString& fullpath);
    bool isLoaded(const QString& fullpath) const;
    void save(int height, int width, const QString& outdir, int quality);
    int getHeight() const;
    int getWidth() const;
    QString getFullpath() const;

public slots:
    void zoomOut();
    void zoomIn();
    //void displayFit();

signals:
    void loadFinish();
    void loadError(const QString& fullpath);

private slots:
    void displayImage();

private:
    Ui::TabPage ui;
    qreal displayScaleFactor;
    QString fullpath;
    QImage image;
    QFutureWatcher<void> loadWatcher;
    static const QString SAVE_PREFIX;

    void doLoadImage(const QString& fullpath);
    void displayScale(double scale);
    void adjustScrollBar(QScrollBar* scrollbar, double scale);
};

#endif
