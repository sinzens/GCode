#include "AboutThisWindow.h"
#include "ui_AboutThisWindow.h"

#include "MainWindow.h"

#include <QMouseEvent>

typedef void doNothing;

AboutThisWindow::AboutThisWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutThisWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->initTheme();
    this->initTitleBar();
    this->initWindowStatus();
    this->initMouseStatus();
    this->initEventFilter();
}

AboutThisWindow::~AboutThisWindow()
{
    delete ui;
}

void AboutThisWindow::initTheme()
{
    this->parent = qobject_cast<MainWindow*>(this->parentWidget());
    this->styleTheme = parent->theme();
    if(this->styleTheme == DarkTheme)
    {
        ui->minButton->setIcon(QIcon("://icons/minH.png"));
        ui->closeButton->setIcon(QIcon("://icons/closeH.png"));
    }
    if(this->styleTheme == LightTheme)
    {
        ui->minButton->setIcon(QIcon("://icons/minH_D.png"));
        ui->closeButton->setIcon(QIcon("://icons/closeH_D.png"));
    }
}

void AboutThisWindow::initTitleBar()
{
    ui->dockWidget->setVisible(false);
    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->titleBar->addWidget(spacer);

    ui->titleBar->addWidget(ui->minButton);
    ui->titleBar->addWidget(ui->switchButton);
    ui->titleBar->addWidget(ui->closeButton);
}

void AboutThisWindow::initWindowStatus()
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

void AboutThisWindow::initMouseStatus()
{
    this->mouseDrag = false;
    this->mousePos = QPoint(0, 0);
}

void AboutThisWindow::initEventFilter()
{
    this->installEventFilter(this);
    ui->titleBar->installEventFilter(this);
    ui->statusbar->installEventFilter(this);
}

bool AboutThisWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->titleBar || object == ui->statusbar)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        switch(event->type())
        {
        case QEvent::MouseButtonPress:
            mouseEvent->button() == Qt::LeftButton ?
            this->mouseDrag = true : this->mouseDrag = false;
            this->mousePos = mouseEvent->globalPos() - this->pos();
            break;
        case QEvent::MouseButtonRelease:
            this->mouseDrag = false;
            break;
        case QEvent::MouseMove:
            if(mouseDrag)
            {
                this->windowState() == Qt::WindowMaximized ?
                this->switchStatus() : doNothing();
                this->move(mouseEvent->globalPos() - mousePos);
            }
            break;
        case QEvent::MouseButtonDblClick:
            this->switchStatus();
            break;
        default:
            break;
        }
    }
    return QMainWindow::eventFilter(object, event);
}

void AboutThisWindow::switchStatus()
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
