#include "Dialog.h"
#include "ui_Dialog.h"

#include "MainWindow.h"
#include "OptionWindow.h"

#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

typedef void doNothing;

Dialog::Dialog(QWidget* parent, ParentType parentType_) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->parentType = parentType_;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->initTheme();
    this->initMouseStatus();
    this->initEventFilter();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initTheme()
{
    if(this->parentType == Main)
    {
        this->parent.parentMain = qobject_cast<MainWindow*>(this->parentWidget());
        this->styleTheme = parent.parentMain->theme();
    }
    if(this->parentType == Option)
    {
        this->parent.parentOption = qobject_cast<OptionWindow*>(this->parentWidget());
        this->styleTheme = parent.parentOption->parentWindow()->theme();
    }
    if(this->styleTheme == DarkTheme)
    {
        ui->closeButton->setIcon(QIcon("://icons/closeH.png"));
    }
    if(this->styleTheme == LightTheme)
    {
        ui->closeButton->setIcon(QIcon("://icons/closeH_D.png"));
    }
}

void Dialog::initMouseStatus()
{
    this->mouseDrag = false;
    this->mousePos = QPoint(0, 0);
}

void Dialog::initEventFilter()
{
    this->installEventFilter(this);
    ui->dialogTitleBar->installEventFilter(this);
}

void Dialog::setConfirmText(QString text)
{
    ui->confirmButton->setText(text);
}

void Dialog::setCancelText(QString text)
{
    ui->cancelButton->setText(text);
}

void Dialog::setButtonText(QString confirmText, QString cancelText)
{
    ui->confirmButton->setText(confirmText);
    ui->cancelButton->setText(cancelText);
}

void Dialog::setInfo(QString text)
{
    ui->infoLabel->setText(text);
}

void Dialog::hideConfirm()
{
    ui->confirmButton->setVisible(false);
}

void Dialog::hideCancel()
{
    ui->cancelButton->setVisible(false);
}

bool Dialog::eventFilter(QObject *object, QEvent *event)
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

void Dialog::paintEvent(QPaintEvent *event)
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
