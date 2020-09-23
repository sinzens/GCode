#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCharFormat>
#include <QDir>
#include <QMap>

enum UiStyle
{
    LightTheme,
    DarkTheme
};

struct CodeFile
{
    QString name;
    QString url;
    QString content;
    bool saved;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Slide;
class QSettings;
class QProcess;
class Highlighter;
class QTreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() Q_DECL_OVERRIDE;
    void slideProcess(Slide* slide);
    inline void initDockWidget();
    inline void initMenuBar();
    inline void initToolBar();
    inline void initTabWidget();
    inline void initWindowStatus();
    inline void initMouseStatus();
    inline void initEventFilter();
    inline void initSetting();
    inline void initStylesheet();
    inline void initConsole();

    inline void delayToShow(int mesc);

    void changeEditorFont(QFont font);
    void changeCompilerPath(QString path);
    void changeThemeTo(UiStyle theme);

    QString takeName(QString file);
    QString takeDir(QString file);
    QFileInfoList getFileList(const QDir & dir);

    void newFile(QString fileName);
    void openFile(QString fileName);
    void saveAsFile(QString fileName);
    void generateFileTree(QString fileName);
    void generateRecent(QString fileName);

    UiStyle theme();
    QSettings* settings();
    QStringList keyList();

protected:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;
    bool nativeEvent(const QByteArray &eventType, void* message, long* result) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

protected slots:
    void switchStatus();
    void skipToLine(QString line);

    void removeTab(int index);
    void changeTabTo(int index);
    void tagToFile(QTreeWidgetItem* item, int column);

    void codeUnSave();

    void processFR(QString str1, QString str2, int flag);
    void restoreFormat();

    void readOutput();
    void readError();

    void showNewFile();
    void showOpenFile();
    void save();
    void saveAll();
    void showSaveAs();
    void closeFile();
    void closeAll();

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void Delete();
    void selectAll();
    void showFindReplace();

    void compile();
    void runCode();
    void debug();
    void stopCode();

    void showOption();

    void showAboutThis();
    void showAboutUs();

private:
    Ui::MainWindow* ui;

    UiStyle styleTheme;

    QString compilerPath;
    QProcess* consoleProc;
    bool debuggerRunning;

    bool mouseDrag;
    QPoint mousePos;

    QSettings* setting;

    QMap<QString, CodeFile> editorMap;
    int currentPage;

    QTextCharFormat oriColor;

    Highlighter* highlighter;
    QStringList keywordList;
};
#endif // MAINWINDOW_H
