#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_transBtn_clicked();//转换按钮事件

    void on_changeBtn_clicked();//是否需要空格按钮事件

    void on_closeBtn_clicked();//关闭按钮事件
    void resizeEvent(QResizeEvent *);//界面大小改变事件
    void closeEvent(QCloseEvent *);//关闭事件
private slots:
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void on_showMainAction();
    void on_exitAppAction();
    void on_resetBtn_clicked();

private:
    Ui::MainWindow *ui;
    bool hasSpace;//转换文字的时候需要空格不
    void setPos();//设置界面布局
    int uiWidth,uiHeight;//界面大小，宽度和高度
    QIcon icon;//系统图标也是托盘图标
    int spaceWidth=10;
private:
    QSystemTrayIcon *mSysTrayIcon;//托盘
    QMenu *mMenu;
    QAction *mShowMainAction;
    QAction *mExitAppAction;
private:
    void createActions();
    void createMenu();

};

#endif // MAINWINDOW_H
