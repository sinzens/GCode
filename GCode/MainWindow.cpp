#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Slide.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QThread>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Slide slide;
    slide.show();
    ui->setupUi(this);
    slide.progressAdd(40);
    delayToShow(500);
    this->setWindowFlags(Qt::FramelessWindowHint);
    initDockWidget();
    slide.progressAdd(10);
    delayToShow(250);
    initMenuBar();
    slide.progressAdd(10);
    delayToShow(250);
    initWindowStatus();
    slide.progressAdd(10);
    delayToShow(250);
    initMouseStatus();
    slide.progressAdd(10);
    delayToShow(100);
    initEventFilter();
    slide.progressAdd(10);
    delayToShow(100);
    initStylesheet();
    slide.progressAdd(10);
    delayToShow(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initDockWidget()
{
    ui->centralwidget->setVisible(false);

    splitDockWidget(ui->FileListDock, ui->ConsoleDock, Qt::Vertical);
    splitDockWidget(ui->FileListDock, ui->EditorDock, Qt::Horizontal);
    this->resizeDocks({ui->FileListDock, ui->EditorDock}, {1, 4}, Qt::Horizontal);
    this->resizeDocks({ui->FileListDock, ui->ConsoleDock}, {2, 1}, Qt::Vertical);
}

void MainWindow::initMenuBar()
{
    ui->menubar->setVisible(false);

    QVector<QMenu*> menuList;
    QVector<QToolButton*> menuButtonList;

    menuList << ui->menuFile << ui->menuEdit <<
                ui->menuBuild << ui->menuTool <<
                ui->menuHelp;

    menuButtonList << ui->fileMenuButton << ui->editMenuButton <<
                      ui->buildMenuButton << ui->toolMenuButton <<
                      ui->helpMenuButton;

    for(int i = 0; i < menuList.length(); i++)
    {
        menuButtonList[i]->setDefaultAction(menuList[i]->menuAction());
        ui->titleBar->addWidget(menuButtonList[i]);

        for(QAction* action : menuList[i]->actions())
        {
            action->setShortcutVisibleInContextMenu(true);
        }
    }

    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->titleBar->addWidget(spacer);

    ui->titleBar->addWidget(ui->minButton);
    ui->titleBar->addWidget(ui->switchButton);
    ui->titleBar->addWidget(ui->closeButton);
}

void MainWindow::initWindowStatus()
{
    switch(this->windowState())
    {
    case Qt::WindowMaximized:
        ui->switchButton->setIcon(QIcon("://icons/resH.png"));
        break;
    default:
        ui->switchButton->setIcon(QIcon("://icons/maxH.png"));
        break;
    }
}

void MainWindow::initMouseStatus()
{
    this->mouseDrag = false;
    this->mousePos = QPoint(0, 0);
}

void MainWindow::initEventFilter()
{
    this->installEventFilter(this);
    ui->titleBar->installEventFilter(this);
    ui->statusbar->installEventFilter(this);
}

void MainWindow::initStylesheet()
{
    this->styleTheme.theme = DarkTheme;

    QVector<QFile> qssList;
    /*qssList << QFile(":/dark/stylesheets/MainWindowD.qss")
            << QFile(":/dark/stylesheets/PushButtonD.qss");*/
    QFile file1(":/dark/stylesheets/MainWindowD.qss");

    /*for(QFile & qssFile : qssList)
    {
        qssFile.open(QFile::ReadOnly);
        this->styleTheme.stylesheetList << qssFile.readAll();
        qssFile.close();
    }

    for(QString & style : this->styleTheme.stylesheetList)
    {
        qDebug() << style;
    }*/
}

void MainWindow::delayToShow(int mesc)
{
    QEventLoop loop;
    QTimer::singleShot(mesc, &loop, SLOT(quit()));
    loop.exec();
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    if(object == ui->titleBar || object == ui->statusbar)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        switch(event->type())
        {
        case QEvent::MouseButtonPress:
            mouseEvent->button() == Qt::LeftButton ?
            mouseDrag = true : mouseDrag = false;
            mousePos = mouseEvent->globalPos() - this->pos();
            break;
        case QEvent::MouseButtonRelease:
            mouseDrag = false;
            break;
        case QEvent::MouseMove:
            if(mouseDrag)
            {
                this->windowState() == Qt::WindowMaximized ?
                switchStatus() : doNothing();
                this->move(mouseEvent->globalPos() - mousePos);
            }
            break;
        case QEvent::MouseButtonDblClick:
            switchStatus();
            break;
        default:
            break;
        }
    }
    return QMainWindow::eventFilter(object, event);
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    /*Q_UNUSED(eventType)
    MSG* param = static_cast<MSG*>(message);
    switch(param->message)
    {
        case WM_NCHITTEST:
        {
            int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
            int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();
            *result = HTCAPTION;
            switch(mouseAt(QPoint(nX, nY)))
            {
            case LeftBorder:
                *result = HTLEFT;
                break;
            case RightBorder:
                *result = HTRIGHT;
                break;
            case TopBorder:
                *result = HTTOP;
                break;
            case BottomBorder:
                *result = HTBOTTOM;
                break;
            case LeftTopCorner:
                *result = HTTOPLEFT;
                break;
            case RightTopCorner:
                *result = HTTOPRIGHT;
                break;
            case LeftBottomCorner:
                *result = HTBOTTOMLEFT;
                break;
            case RightBottomCorner:
                *result = HTBOTTOMRIGHT;
                break;
            default:
                break;
            }
            initRegionRect();
            if(*result == HTCAPTION)
            {
                return false;
            }
            return true;
        }
    }*/
    return QMainWindow::nativeEvent(eventType, message, result);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "即将退出\n\n确定吗");
    result == QMessageBox::Yes ? event->accept() : event->ignore();
}

void MainWindow::switchStatus()
{
    switch(this->windowState())
    {
    case Qt::WindowMaximized:
        this->showNormal();
        ui->switchButton->setIcon(QIcon("://icons/maxH.png"));
        break;
    default:
        this->showMaximized();
        ui->switchButton->setIcon(QIcon("://icons/resH.png"));
        break;
    }
}

