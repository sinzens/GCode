#include "OptionWindow.h"
#include "ui_OptionWindow.h"

#include "MainWindow.h"
#include "Dialog.h"

#include <QMouseEvent>
#include <QListView>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>

typedef void doNothing;
typedef bool changeNothing;

OptionWindow::OptionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OptionWindow)
{
    ui->setupUi(this);
    ui->themeComboBox->setView(new QListView());
    ui->fontComboBox->setView(new QListView());
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->initTheme();
    this->initTitleBar();
    this->initSplitter();
    this->initWindowStatus();
    this->initMouseStatus();
    this->initEventFilter();
    this->readSetting();
}

OptionWindow::~OptionWindow()
{
    delete ui;
}

void OptionWindow::initTheme()
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

void OptionWindow::readSetting()
{
    this->optionChanged = false;
    this->optionConfirmed = true;
    ui->applyButton->setEnabled(false);
    ui->fontComboBox->setCurrentFont(QFont(parent->settings()->value("Edit/font").toString()));
    ui->themeComboBox->setCurrentIndex(ui->themeComboBox->findText(
            parent->settings()->value("Edit/theme").toString()));
    ui->compilerPath->setText(parent->settings()->value("Compile/compiler").toString());

    ui->fontPreviewer->setFontFamily(ui->fontComboBox->currentText());
    ui->fontPreviewer->setFontPointSize(30);
    ui->fontPreviewer->setText("Hello, world!\n你好，世界!");
}

MainWindow *OptionWindow::parentWindow()
{
    return this->parent;
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

void OptionWindow::confirmOption()
{
    parent->settings()->setValue("Edit/font", ui->fontComboBox->currentFont());
    parent->settings()->setValue("Edit/theme", ui->themeComboBox->currentText());
    parent->settings()->setValue("Compile/compiler", ui->compilerPath->text());

    this->optionConfirmed = true;
    this->applyOption();
    this->close();
}

void OptionWindow::applyOption()
{
    ui->applyButton->setEnabled(false);
    this->optionChanged = false;

    parent->changeEditorFont(ui->fontComboBox->currentFont());
    parent->changeCompilerPath(ui->compilerPath->text());

    QString themeName = ui->themeComboBox->currentText();
    if(themeName == "黑色主题")
    {
        parent->changeThemeTo(DarkTheme);
        this->initTheme();
        this->initWindowStatus();
    }
    if(themeName == "白色主题")
    {
        parent->changeThemeTo(LightTheme);
        this->initTheme();
        this->initWindowStatus();
    }

    ui->fontPreviewer->setFontFamily(ui->fontComboBox->currentText());
    ui->fontPreviewer->setFontPointSize(30);
    ui->fontPreviewer->setText("Hello, world!\n你好，世界!");
}

void OptionWindow::cancelOption()
{
    if(!optionConfirmed)
    {
        Dialog dialog(this, Option);
        dialog.setButtonText("确定", "取消");
        dialog.setInfo("如不点击确定, 将不会保存\n确定吗");
        int result = dialog.exec();
        result == QDialog::Accepted ?
        this->close() : changeNothing();
    }
    else
    {
        this->close();
    }
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
    if(ui->optionTree->currentItem()->text(0) == "编译器")
    {
        ui->optionStack->setCurrentWidget(ui->compilerPage);
    }
}

void OptionWindow::optionHasChanged()
{
    if(!this->isHidden())
    {
        ui->applyButton->setEnabled(true);
        this->optionChanged = true;
        this->optionConfirmed = false;
    }
}

void OptionWindow::showOpenDir()
{
    QString fileDir = QFileDialog::getExistingDirectory(this, "打开文件");

    if(!fileDir.isNull())
    {
        ui->compilerPath->setText(fileDir);
    }
}
