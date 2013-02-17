#include <QtCore/QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDropEvent>
#include <QUrl>
#include <QtConcurrentRun>
#include <QMouseEvent>

#include "main_window.h"
#include "tab_page.h"
#include "resize_dialog.h"

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.actionAbout_Qt, SIGNAL(triggered()),
            this, SLOT(aboutQT()));
    connect(ui.actionAbout, SIGNAL(triggered()),
            this, SLOT(about()));
    connect(ui.action_Open, SIGNAL(triggered()),
            this, SLOT(chooseFiles()));
    connect(ui.action_Resize, SIGNAL(triggered()),
            this, SLOT(resizeDialog()));
    connect(ui.actionZoom_In, SIGNAL(triggered()),
            this, SLOT(zoomIn()));
    connect(ui.actionZoom_Out, SIGNAL(triggered()),
            this, SLOT(zoomOut()));
    connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
    connect(ui.tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(displayCurrentTab()));
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    chooseFiles();
}

void MainWindow::zoomIn()
{
    TabPage* page = qobject_cast<TabPage*>(ui.tabWidget->currentWidget());
    page->zoomIn();
}

void MainWindow::zoomOut()
{
    TabPage* page = qobject_cast<TabPage*>(ui.tabWidget->currentWidget());
    page->zoomOut();
}

void MainWindow::resizeDialog()
{
    TabPage* page = qobject_cast<TabPage*>(ui.tabWidget->currentWidget());

    ResizeDialog dialog(page->getHeight(), page->getWidth(), this);
    connect(&dialog, 
            SIGNAL(dialogFinished(int, int, int, bool, int, bool)),
            this, SLOT(resizeOpts(int, int, int, bool, int, bool)));
    dialog.exec();
}

void MainWindow::resizeOpts(int height, int width, int longest,
        bool isLongest, int filesize, bool applyToAll)
{
    if (applyToAll) {
        for (int i=0; i<ui.tabWidget->count(); ++i) {
            TabPage* page = 
            qobject_cast<TabPage*>(ui.tabWidget->widget(i)); 
            auto future = QtConcurrent::run(page, &TabPage::save,
                height, width, true, longest);
        }
    } else {
        TabPage* page = 
            qobject_cast<TabPage*>(ui.tabWidget->currentWidget()); 
        auto future = QtConcurrent::run(page, &TabPage::save,
                height, width, isLongest, longest);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* event)
{
    qDebug() << "Drop event";
    auto mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QStringList pathList;
        auto urlList = mimeData->urls();
        for (int i=0; i<urlList.size(); ++i) {
            pathList.append(urlList.at(i).toLocalFile());
        }
        addTabs(pathList);
    }
}

void MainWindow::displayCurrentTab()
{
    if (ui.tabWidget->count() == 0) {
        return;
    }
    TabPage* page = qobject_cast<TabPage*>(ui.tabWidget->currentWidget());
    Q_ASSERT(page != nullptr);
    page->loadImage();
}


void MainWindow::closeTab(int index)
{
    auto page = ui.tabWidget->widget(index);
    ui.tabWidget->removeTab(index);
    delete page;
}

void MainWindow::aboutQT()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::about()
{
    QMessageBox::about(this, "ImgResize", tr("Resize your images."));
}

void MainWindow::addTabs(const QStringList& pathList)
{
    auto it = pathList.begin();
    while (it != pathList.end()) {
        auto page = new TabPage(*it);
        lastTabIdx = ui.tabWidget->addTab(page,
                QFileInfo(*it).fileName());
        qDebug() << "Tab added: " << *it;
        ++it;
    }
    ui.tabWidget->setCurrentIndex(lastTabIdx);
}

void MainWindow::chooseFiles()
{
    auto files = QFileDialog::getOpenFileNames(
            this,
            tr("Select one or more files to open"),
            ".",
            "Images (*.png *.jpg *.jpeg *.nef)");
    addTabs(files);
}
