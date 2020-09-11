#include "OptionWindow.h"
#include "ui_OptionWindow.h"

#include <QMouseEvent>
#include <QListView>

typedef void doNothing;

OptionWindow::OptionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OptionWindow)
{
    ui->setupUi(this);
    ui->themeComboBox->setView(new QListView());
    ui->fontComboBox->setView(new QListView());
    this->setWindowModality(Qt::WindowModal);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->initTitleBar();
    this->initSplitter();
    this->initWindowStatus();
    this->initMouseStatus();
    this->initEventFilter();
}

OptionWindow::~OptionWindow()
{
    delete ui;
}

void OptionWindow::initTitleBar()
{
    ui->dockWidget->setVisible(false);
    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->titleBar->addWidget(spacer);

    ui->titleBar->addWidget(ui->minButton);
    ui->titleBar->addWidget(ui->switchButton);
    ui->titleBar->addWidget(ui->closeButton);
}

void OptionWindow::initSplitter()
{
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);
}

void OptionWindow::initWindowStatus()
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

void OptionWindow::initMouseStatus()
{
    this->mouseDrag = false;
    this->mousePos = QPoint(0, 0);
}

void OptionWindow::initEventFilter()
{
    this->installEventFilter(this);
    ui->titleBar->installEventFilter(this);
    ui->statusbar->installEventFilter(this);
}

bool OptionWindow::eventFilter(QObject *object, QEvent *event)
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

void OptionWindow::switchStatus()
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

void OptionWindow::confirmOption()
{
    this->close();
}

void OptionWindow::applyOption()
{

}

void OptionWindow::cancelOption()
{
    this->close();
}

void OptionWindow::handleStack()
{
    if(ui->optionTree->currentItem()->text(0) == "字体")
    {
        ui->optionStack->setCurrentWidget(ui->fontPage);
    }
    if(ui->optionTree->currentItem()->text(0) == "主题")
    {
        ui->optionStack->setCurrentWidget(ui->themePage);
    }
}
