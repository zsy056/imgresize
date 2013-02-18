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
    void resizeOpts(int height, int width, int longest, bool isLongest,
            int fileSize, bool applyToAll);
    void zoomIn();
    void zoomOut();
    void setTabActionsEnabled(bool flag);
    void closeCurrentTab();
    void loadErrorHandler(const QString& fullpath);

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



