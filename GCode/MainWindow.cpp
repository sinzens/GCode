#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Slide.h"
#include "OptionWindow.h"
#include "Dialog.h"
#include "FRWindow.h"
#include "AboutThisWindow.h"
#include "AboutUsWindow.h"

#include "Highlighter.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QTimer>
#include <QSettings>
#include <QProcess>
#include <QTextDocument>

#include <QTextCodec>

typedef void doNothing;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /*Slide slide;
    slide.show();
    slideProcess(&slide);*/
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    highlighter = new Highlighter(ui->codeEditor->document());
    initDockWidget();
    initMenuBar();
    initToolBar();
    initTabWidget();
    initSetting();
    initWindowStatus();
    initMouseStatus();
    initEventFilter();
    initStylesheet();
    initConsole();
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

    this->currentPage = ui->editorTab->currentIndex();
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
    ui->IOEditor->installEventFilter(this);
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

    this->oriColor = ui->codeEditor->textCursor().charFormat();
}

void MainWindow::initConsole()
{
    this->consoleProc = new QProcess(this);
    connect(this->consoleProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(this->consoleProc, SIGNAL(readyReadStandardError()), this, SLOT(readError()));
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

QString MainWindow::takeName(QString file)
{
    QStringList dirList = file.split("/");
    return dirList.last();
}

QString MainWindow::takeDir(QString file)
{
    QStringList dirList = file.split("/");
    int lastIndex = dirList.indexOf(dirList.last());
    return dirList.at(lastIndex - 1);
}

QFileInfoList MainWindow::getFileList(const QDir &dir)
{
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    /*for(QFileInfo & folder : dirList)
    {
        QString path = folder.absolutePath();
        QFileInfoList childList = getFileList(path);

        fileList.append(childList);
    }*/

    return fileList;
}

void MainWindow::generateFileTree(QString fileName)
{
    QString aboveDir = takeDir(fileName);
    QTreeWidgetItem* rootItem;
    QList<QTreeWidgetItem*> existList = ui->fileTree->findItems(aboveDir, Qt::MatchExactly);

    if(existList.isEmpty())
    {
        rootItem = new QTreeWidgetItem();
        rootItem->setText(0, aboveDir);
        ui->fileTree->addTopLevelItem(rootItem);
    }
    else
    {
        rootItem = existList.at(0);
    }

    QString filePath =  fileName.remove(takeName(fileName));
    QFileInfoList list = this->getFileList(filePath);
    for(QFileInfo & fileInfo : list)
    {
        QString name = fileInfo.fileName();

        bool hasItem = false;
        QTreeWidgetItemIterator i(rootItem);
        while(*i)
        {
            if((*i)->text(0) == name)
            {
                hasItem = true;
                break;
            }
            ++i;
        }

        if(!hasItem)
        {
            QTreeWidgetItem* childItem = new QTreeWidgetItem();
            childItem->setText(0, name);
            rootItem->addChild(childItem);
        }
    }
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
    if(object == ui->IOEditor)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        switch(event->type())
        {
        case QEvent::KeyPress:
            if(keyEvent->key() == Qt::Key_Return && this->consoleProc->state() == QProcess::Running)
            {
                QTextCursor cursor = ui->IOEditor->textCursor();
                QString input = cursor.block().text() + "\n";
                this->consoleProc->write(input.toUtf8().data());
            }
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
    if(index == ui->editorTab->count() - 1)
    {
        if(ui->editorTab->count() != 1)
        {
            ui->editorTab->setCurrentIndex(index - 1);
        }
    }
    else
    {
        ui->editorTab->setCurrentIndex(index + 1);
    }

    ui->editorTab->removeTab(index);
    if(ui->editorTab->count() == 0)
    {
        ui->codeEditor->clear();
    }
}

void MainWindow::changeTabTo(int index)
{
    ui->codeEditor->blockSignals(true);

    int lastIndex = this->currentPage;
    QString lastFile = ui->editorTab->tabText(lastIndex).remove("*");
    QString thisFile = ui->editorTab->tabText(index).remove("*");
    QString content = ui->codeEditor->toHtml();

    if(editorMap.contains(lastFile))
    {
        editorMap[lastFile].content = content;
    }
    else
    {
        CodeFile file = {lastFile, lastFile, content, true};
        editorMap.insert(lastFile, file);
    }

    ui->codeEditor->clear();
    this->currentPage = index;

    if(editorMap.contains(thisFile))
    {
        ui->codeEditor->setText(editorMap[thisFile].content);
    }

    ui->codeEditor->blockSignals(false);
}

void MainWindow::codeUnSave()
{
    QString tabTitle = ui->editorTab->tabText(this->currentPage).remove("*");
    ui->editorTab->setTabText(this->currentPage, tabTitle + "*");

    editorMap[tabTitle].saved = false;
}

void MainWindow::processFR(QString str1, QString str2, int flag)
{
    Dialog dialog(this, Main);
    dialog.setButtonText("确定", "取消");
    dialog.hideCancel();
    switch(flag)
    {
    case FindNext:
        if(str1.trimmed().isEmpty())
        {
            dialog.setInfo("被查找的字段不能为空");
            dialog.exec();
        }
        else
        {
            if(ui->codeEditor->find(str1))
            {
                QPalette palette = ui->codeEditor->palette();
                palette.setColor(QPalette::Highlight, palette.color(QPalette::Active, QPalette::Highlight));
                ui->codeEditor->setPalette(palette);
            }
            else
            {
                if(ui->codeEditor->find(str1, QTextDocument::FindBackward))
                {
                    ui->codeEditor->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
                    dialog.setInfo("已经到达文本末尾");
                    dialog.exec();

                }
                else
                {
                    dialog.setInfo("未找到要查找的字段");
                    dialog.exec();
                }
            }
        }
        break;
    case FindAll:
        if(str1.trimmed().isEmpty())
        {
            dialog.setInfo("被查找的字段不能为空");
            dialog.exec();
        }
        else
        {
            if(!ui->codeEditor->find(str1))
            {
                dialog.setInfo("未找到要查找的字段");
                dialog.exec();
                return;
            }

            bool found = false;

            QTextDocument* document = ui->codeEditor->document();
            QTextCursor highlight(document);
            QTextCursor cursor(document);

            cursor.beginEditBlock();

            QTextCharFormat color(highlight.charFormat());
            color.setBackground(ui->codeEditor->palette().color(QPalette::Active, QPalette::Highlight));
            while(!highlight.isNull() && !highlight.atEnd())
            {
                highlight = document->find(str1, highlight);
                if(!highlight.isNull())
                {
                    if(!found)
                    {
                        found = true;
                    }
                    highlight.mergeCharFormat(color);
                }
            }
            cursor.endEditBlock();
            if(!found)
            {
                dialog.setInfo("未找到要查找的字段");
                dialog.exec();
            }
        }
        break;
    case ReplaceNext:
        if(str1.trimmed().isEmpty() || str2.trimmed().isEmpty())
        {
            dialog.setInfo("被查找或替换的字段不能为空");
            dialog.exec();
        }
        else
        {
            if(ui->codeEditor->find(str1))
            {
                int index = 0;
                QString origin = ui->codeEditor->toPlainText();
                index = origin.indexOf(str1, 0);

                if(index != -1)
                {
                    origin = origin.replace(index, str1.length(), str2);
                    ui->codeEditor->setText(origin);
                }
            }
            else
            {
                if(ui->codeEditor->find(str1, QTextDocument::FindBackward))
                {
                    ui->codeEditor->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
                    dialog.setInfo("已经到达文本末尾");
                    dialog.exec();
                }
                else
                {
                    dialog.setInfo("未找到需要被替换的字段");
                    dialog.exec();
                }
            }
        }
        break;
    case ReplaceAll:
        if(str1.trimmed().isEmpty() || str2.trimmed().isEmpty())
        {
            dialog.setInfo("被查找或替换的字段不能为空");
            dialog.exec();
        }
        else
        {
            if(!ui->codeEditor->find(str1))
            {
                dialog.setInfo("未找到要查找的字段");
                dialog.exec();
                return;
            }

            QString origin = ui->codeEditor->toPlainText();
            origin.replace(str1, str2, Qt::CaseSensitive);
            ui->codeEditor->setText(origin);
        }
        break;
    }
}

void MainWindow::restoreFormat()
{
    ui->codeEditor->selectAll();
    QTextCursor cursor = ui->codeEditor->textCursor();
    cursor.setCharFormat(this->oriColor);
    cursor.clearSelection();
    ui->codeEditor->setTextCursor(cursor);
}

void MainWindow::readOutput()
{
    QString output = QString::fromLocal8Bit(this->consoleProc->readAllStandardOutput());
    ui->consoleTab->setCurrentWidget(ui->IOTab);
    ui->IOEditor->append(output);
}

void MainWindow::readError()
{
    QString error = QString::fromLocal8Bit(this->consoleProc->readAllStandardError());
    ui->consoleTab->setCurrentWidget(ui->errorTab);
    ui->errorBrowser->append(error);
}

void MainWindow::showNewFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "新建文件");
    //QFileDialog fileDialog(this);
    //fileDialog.exec();

    if(!fileName.isNull())
    {
        QWidget* newPage = new QWidget();
        QString fileKey = takeDir(fileName) + "/" + takeName(fileName);

        QFile file(fileName);
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        QString content = file.readAll();
        file.close();

        if(!editorMap.contains(fileKey))
        {
            CodeFile file = {fileKey, fileName, content, true};
            editorMap.insert(fileKey, file);
        }
        else
        {
            editorMap[fileKey].url = fileName;
            editorMap[fileKey].content = content;
            editorMap[fileKey].saved = true;
        }

        ui->editorTab->addTab(newPage, fileKey);
        ui->editorTab->setCurrentWidget(newPage);

        int tabNum = ui->editorTab->count();
        for(int i = 0; i < tabNum; i++)
        {
            if(ui->editorTab->tabText(i) == "空文件")
            {
                this->removeTab(i);
            }
        }

        this->generateFileTree(fileName);
    }
}

void MainWindow::showOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件");
    //QFileDialog fileDialog(this);
    //fileDialog.exec();

    if(!fileName.isNull())
    {
        QWidget* newPage = new QWidget();
        QString fileKey = takeDir(fileName) + "/" + takeName(fileName);

        QFile file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString content = file.readAll();
        file.close();

        if(!editorMap.contains(fileKey))
        {
            CodeFile file = {fileKey, fileName, content, true};
            editorMap.insert(fileKey, file);
        }
        else
        {
            editorMap[fileKey].url = fileName;
            editorMap[fileKey].content = content;
            editorMap[fileKey].saved = true;
        }

        ui->editorTab->addTab(newPage, fileKey);
        ui->editorTab->setCurrentWidget(newPage);

        int tabNum = ui->editorTab->count();
        for(int i = 0; i < tabNum; i++)
        {
            if(ui->editorTab->tabText(i).remove("*") == "空文件")
            {
                this->removeTab(i);
            }
        }

        this->generateFileTree(fileName);
    }
}

void MainWindow::save()
{
    if(ui->editorTab->tabText(this->currentPage).remove("*") == "空文件" ||
       ui->editorTab->count() == 0)
    {
        this->showSaveAs();
    }
    else
    {
        if(ui->editorTab->count() > 0)
        {
            QString tabTitle = ui->editorTab->tabText(this->currentPage).remove("*");
            ui->editorTab->setTabText(this->currentPage, tabTitle);

            QString path = editorMap[tabTitle].url;
            if(!path.endsWith(".c"))
            {
                path += ".c";
            }

            QFile file(path);
            file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();

            editorMap[tabTitle].saved = true;
        }
    }
}

void MainWindow::saveAll()
{
    for(int i = 0; i < ui->editorTab->count(); i++)
    {
        QString tabTitle = ui->editorTab->tabText(i).remove("*");
        ui->editorTab->setTabText(i, tabTitle);

        QString path = editorMap[tabTitle].url;
        if(!path.endsWith(".c"))
        {
            path += ".c";
        }

        QFile file(path);
        file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
        file.write(ui->codeEditor->toPlainText().toUtf8());
        file.close();

        editorMap[tabTitle].saved = true;
    }
}

void MainWindow::showSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "另存为");

    if(!fileName.isNull())
    {
        QWidget* newPage = new QWidget();
        QString fileKey = takeDir(fileName) + "/" + takeName(fileName);
        QString content = ui->codeEditor->toPlainText();

        QFile file(fileName);
        file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
        file.write(content.toUtf8());
        file.close();

        if(!editorMap.contains(fileKey))
        {
            CodeFile file = {fileKey, fileName, content, true};
            editorMap.insert(fileKey, file);
        }
        else
        {
            editorMap[fileKey].url = fileName;
            editorMap[fileKey].content = content;
            editorMap[fileKey].saved = true;
        }

        ui->editorTab->addTab(newPage, fileKey);
        ui->editorTab->setCurrentWidget(newPage);

        int tabNum = ui->editorTab->count();
        for(int i = 0; i < tabNum; i++)
        {
            if(ui->editorTab->tabText(i).remove("*") == "空文件")
            {
                this->removeTab(i);
            }
        }

        this->generateFileTree(fileName);
    }
}

void MainWindow::closeFile()
{
    this->removeTab(this->currentPage);
}

void MainWindow::closeAll()
{
    ui->editorTab->clear();
    ui->codeEditor->clear();
}

void MainWindow::undo()
{
    ui->codeEditor->undo();
}

void MainWindow::redo()
{
    ui->codeEditor->redo();
}

void MainWindow::cut()
{
    ui->codeEditor->cut();
}

void MainWindow::copy()
{
    ui->codeEditor->copy();
}

void MainWindow::paste()
{
    ui->codeEditor->paste();
}

void MainWindow::Delete()
{
    ui->codeEditor->textCursor().removeSelectedText();
}

void MainWindow::selectAll()
{
    ui->codeEditor->selectAll();
}

void MainWindow::showOption()
{
    OptionWindow* optionWindow = new OptionWindow(this);
    optionWindow->show();
}

void MainWindow::showFindReplace()
{
    FRWindow* frWindow = new FRWindow(this);
    connect(frWindow, SIGNAL(frRequested(QString, QString, int)), this, SLOT(processFR(QString, QString, int)));
    connect(frWindow, SIGNAL(finished(int)), this, SLOT(restoreFormat()));
    frWindow->show();
}

void MainWindow::compile()
{
    QString compilerEXE = this->compilerPath + "/gcc.exe ";
    QString sourceFile = editorMap[ui->editorTab->tabText(this->currentPage)].url;
    QString targetFile = sourceFile.remove(".c");
    QString compileCom = compilerEXE + sourceFile + ".c -o " + targetFile;

    QStringList command;
    command << "/c" << compileCom;

    ui->consoleTab->setCurrentWidget(ui->IOTab);
    ui->IOEditor->append("Processing command: " + compileCom + " ...");

    this->consoleProc->setProgram("cmd");
    this->consoleProc->setArguments(command);
    this->consoleProc->start();

    this->consoleProc->waitForStarted();
    this->consoleProc->write(compileCom.toUtf8().data());
    this->consoleProc->waitForFinished();
}

void MainWindow::runCode()
{
    this->compile();

    QString exePath = editorMap[ui->editorTab->tabText(this->currentPage)].url.remove(".c") + ".exe";

    ui->IOEditor->append("Starting " + exePath + " ...\n");
    this->consoleProc->setProgram(exePath);
    this->consoleProc->setArguments(QStringList() << "/c" << exePath);
    this->consoleProc->start();

    this->consoleProc->waitForStarted();
    //this->consoleProc->waitForFinished();
}

void MainWindow::debug()
{

}

void MainWindow::stopCode()
{
    this->consoleProc->kill();
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

