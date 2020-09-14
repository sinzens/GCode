#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Slide.h"
#include "OptionWindow.h"
#include "Dialog.h"
#include "FRWindow.h"
#include "AboutThisWindow.h"
#include "AboutUsWindow.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTimer>

typedef void doNothing;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Slide slide;
    slide.show();
    slideProcess(&slide);
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    initDockWidget();
    initMenuBar();
    initToolBar();
    initTabWidget();
    initSetting();
    initWindowStatus();
    initMouseStatus();
    initEventFilter();
    initStylesheet();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slideProcess(Slide* slide)
{
    const int slideTime = 7;

    for(int i = 1; i <= slideTime; i++)
    {
        if(i == 1)
        {
            slide->progressAdd(40);
            delayToShow(500);
        }
        else if(i <= 4)
        {
            slide->progressAdd(10);
            delayToShow(250);
        }
        else if(i <= 6)
        {
            slide->progressAdd(10);
            delayToShow(100);
        }
        else
        {
            slide->progressAdd(10);
            delayToShow(2000);
        }
    }
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

    ui->titleBar->addWidget(ui->iconLabel);

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

void MainWindow::initToolBar()
{
    ui->leftToolBar->addWidget(ui->openButton);
    ui->leftToolBar->addWidget(ui->saveButton);
    ui->leftToolBar->addWidget(ui->saveAsButton);
    ui->leftToolBar->addWidget(ui->saveAllButton);

    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->leftToolBar->addWidget(spacer);

    ui->leftToolBar->addWidget(ui->runButton);
    ui->leftToolBar->addWidget(ui->debugButton);
    ui->leftToolBar->addWidget(ui->stopButton);
}

void MainWindow::initTabWidget()
{
    int tabNum = ui->editorTab->count();
    for(int i = 0; i < tabNum; i++)
    {
        QWidget* tab = ui->editorTab->widget(i);
        tab->setVisible(false);
    }
}

void MainWindow::initWindowStatus()
{
    switch(this->windowState())
    {
    case Qt::WindowMaximized:
        if(this->styleTheme == DarkTheme)
        {
            ui->switchButton->setIcon(QIcon("://icons/resH.png"));
        }
        if(this->styleTheme == LightTheme)
        {
            ui->switchButton->setIcon(QIcon("://icons/resH_D.png"));
        }
        break;
    default:
        if(this->styleTheme == DarkTheme)
        {
            ui->switchButton->setIcon(QIcon("://icons/maxH.png"));
        }
        if(this->styleTheme == LightTheme)
        {
            ui->switchButton->setIcon(QIcon("://icons/maxH_D.png"));
        }
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

void MainWindow::initSetting()
{
    QFileInfo fileInfo("config.gni");
    if(!fileInfo.exists())
    {
        Dialog dialog(this);
        dialog.setButtonText("确定", "取消");
        dialog.hideCancel();
        dialog.setInfo("未能找到配置文件config.gni");
        dialog.exec();
        this->styleTheme = DarkTheme;
    }

    this->setting = new QSettings("config.gni", QSettings::IniFormat);
    ui->codeEditor->setFont(QFont(setting->value("Edit/font").toString()));
    this->compilerPath = setting->value("Compile/compiler").toString();

    QString themeName = setting->value("Edit/theme").toString();
    if(themeName == "黑色主题")
    {
        this->styleTheme = DarkTheme;
    }
    if(themeName == "白色主题")
    {
        this->styleTheme = LightTheme;
    }
}

void MainWindow::initStylesheet()
{
    if(this->styleTheme == DarkTheme)
    {
        QFile qssFile(":/dark/stylesheets/MainWindowD.qss");
        qssFile.open(QFile::ReadOnly);
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();

        ui->minButton->setIcon(QIcon("://icons/minH.png"));
        ui->closeButton->setIcon(QIcon("://icons/closeH.png"));

        ui->runButton->setIcon(QIcon("://icons/runD.png"));
        ui->debugButton->setIcon(QIcon("://icons/debugD.png"));
        ui->stopButton->setIcon(QIcon("://icons/stopD.png"));

        ui->openButton->setIcon(QIcon("://icons/openD.png"));
        ui->saveButton->setIcon(QIcon("://icons/saveD.png"));
        ui->saveAllButton->setIcon(QIcon("://icons/save_allD.png"));
        ui->saveAsButton->setIcon(QIcon("://icons/save_asD.png"));
    }
    if(this->styleTheme == LightTheme)
    {
        QFile qssFile(":/light/stylesheets/MainWindowL.qss");
        qssFile.open(QFile::ReadOnly);
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();

        ui->minButton->setIcon(QIcon("://icons/minH_D.png"));
        ui->closeButton->setIcon(QIcon("://icons/closeH_D.png"));

        ui->runButton->setIcon(QIcon("://icons/run.png"));
        ui->debugButton->setIcon(QIcon("://icons/debug.png"));
        ui->stopButton->setIcon(QIcon("://icons/stop.png"));

        ui->openButton->setIcon(QIcon("://icons/open.png"));
        ui->saveButton->setIcon(QIcon("://icons/save.png"));
        ui->saveAllButton->setIcon(QIcon("://icons/save_all.png"));
        ui->saveAsButton->setIcon(QIcon("://icons/save_as.png"));
    }
}

void MainWindow::delayToShow(int mesc)
{
    QEventLoop loop;
    QTimer::singleShot(mesc, &loop, SLOT(quit()));
    loop.exec();
}

void MainWindow::changeEditorFont(QFont font)
{
    ui->codeEditor->setFont(font);
}

void MainWindow::changeCompilerPath(QString path)
{
    this->compilerPath = path;
}

void MainWindow::changeThemeTo(UiStyle theme)
{
    switch(theme)
    {
    case LightTheme:
        this->styleTheme = LightTheme;
        break;
    case DarkTheme:
        this->styleTheme = DarkTheme;
        break;
    }
    this->initStylesheet();
    this->initWindowStatus();
}

UiStyle MainWindow::theme()
{
    return this->styleTheme;
}

QSettings *MainWindow::settings()
{
    return this->setting;
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
    Dialog dialog(this);
    dialog.setButtonText("确定", "取消");
    dialog.setInfo("即将退出GCode\n确定吗");
    int result = dialog.exec();
    result == QDialog::Accepted ?
                event->accept() : event->ignore();
}

void MainWindow::switchStatus()
{
    switch(this->windowState())
    {
    case Qt::WindowMaximized:
        this->showNormal();
        if(this->styleTheme == DarkTheme)
        {
            ui->switchButton->setIcon(QIcon("://icons/maxH.png"));
        }
        if(this->styleTheme == LightTheme)
        {
            ui->switchButton->setIcon(QIcon("://icons/maxH_D.png"));
        }
        break;
    default:
        this->showMaximized();
        if(this->styleTheme == DarkTheme)
        {
            ui->switchButton->setIcon(QIcon("://icons/resH.png"));
        }
        if(this->styleTheme == LightTheme)
        {
            ui->switchButton->setIcon(QIcon("://icons/resH_D.png"));
        }
        break;
    }
}

void MainWindow::removeTab(int index)
{
    ui->editorTab->removeTab(index);
}

void MainWindow::showOption()
{
    OptionWindow* optionWindow = new OptionWindow(this);
    optionWindow->show();
}

void MainWindow::showFindReplace()
{
    FRWindow* frWindow = new FRWindow(this);
    frWindow->show();
}

void MainWindow::showAboutThis()
{
    AboutThisWindow* aboutThisWindow = new AboutThisWindow(this);
    aboutThisWindow->show();
}

void MainWindow::showAboutUs()
{
    AboutUsWindow* aboutUsWindow = new AboutUsWindow(this);
    aboutUsWindow->show();
}

