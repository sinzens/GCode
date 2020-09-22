#include "FRWindow.h"
#include "ui_FRWindow.h"

#include "MainWindow.h"

#include <QMouseEvent>
#include <QPainter>

typedef void doNothing;

FRWindow::FRWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FRWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->initTheme();
    this->initMouseStatus();
    this->initEventFilter();
}

FRWindow::~FRWindow()
{
    delete ui;
}

void FRWindow::initTheme()
{
    this->parent = qobject_cast<MainWindow*>(this->parentWidget());
    this->styleTheme = parent->theme();
    if(this->styleTheme == DarkTheme)
    {
        ui->closeButton->setIcon(QIcon("://icons/closeH.png"));
    }
    if(this->styleTheme == LightTheme)
    {
        ui->closeButton->setIcon(QIcon("://icons/closeH_D.png"));
    }
}

void FRWindow::initMouseStatus()
{
    this->mouseDrag = false;
    this->mousePos = QPoint(0, 0);
}

void FRWindow::initEventFilter()
{
    this->installEventFilter(this);
    ui->dialogTitleBar->installEventFilter(this);
}

bool FRWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->dialogTitleBar)
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
            mouseDrag ?
            this->move(mouseEvent->globalPos() - mousePos) : doNothing();
            break;
        default:
            break;
        }
    }
    return QDialog::eventFilter(object, event);
}

void FRWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if(this->styleTheme == DarkTheme)
    {
        painter.setPen(QPen(QColor(50, 50, 50), 4));
    }
    if(this->styleTheme == LightTheme)
    {
        painter.setPen(QPen(QColor(190, 190, 190), 4));
    }
    painter.setBrush(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRect(rect);
    QDialog::paintEvent(event);
}

void FRWindow::findNext()
{
    emit frRequested(ui->findEdit->text(), ui->replaceEdit->text(), FindNext);
}

void FRWindow::findAll()
{
    emit frRequested(ui->findEdit->text(), ui->replaceEdit->text(), FindAll);
}

void FRWindow::replaceNext()
{
    emit frRequested(ui->findEdit->text(), ui->replaceEdit->text(), ReplaceNext);
}

void FRWindow::replaceAll()
{
    emit frRequested(ui->findEdit->text(), ui->replaceEdit->text(), ReplaceAll);
}
