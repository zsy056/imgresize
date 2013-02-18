#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include "ui_main_window.h"

class TabPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void aboutQT();
    void about();
    void chooseFiles();
    void closeTab(int index);
    void displayCurrentTab();
    void resizeDialog();
    void resizeOpts(int height, int width, bool applyToAll, bool sameDir,
            const QString& outdir, int quality);
    void zoomIn();
    void zoomOut();
    void setTabActionsEnabled(bool flag);
    void closeCurrentTab();
    void loadErrorHandler(const QString& fullpath);
    void saveImage(int index, int height, int width, bool sameDir,
            const QString& outdir, int quality) ;

protected:
    void dropEvent(QDropEvent* event);
    void dragEnterEvent(QDragEnterEvent *event);
    void mouseDoubleClickEvent(QMouseEvent* event);

private:
    Ui::MainWindow ui;
    int lastTabIdx;

    void addTabs(const QStringList& pathList);
    void setTabIcon(int index);
};

#endif



