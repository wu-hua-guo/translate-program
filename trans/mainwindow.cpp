#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QMessageBox>
#include <QIcon>
#include <QDebug>
#define tab "\t"
using namespace std;

QString str2qstr(const string &str)
{
    return QString::fromLocal8Bit(str.data());
}

string qstr2str(const QString &qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setWindowFlags(Qt::Tool);
    this->hasSpace=true;
    //自动填充背景
    setAutoFillBackground(true);
    QPalette myPalette(this->palette());
    myPalette.setBrush(QPalette::Background, QBrush(QColor(100, 230, 120,60)));
    this->setPalette(myPalette);


    //设置图标
    icon= QIcon(":/image/src/folder.ico");
    //ui->closeBtn->setIcon(QIcon(":/image/src/close.ico"));
    ui->resetBtn->setIcon(QIcon(":/image/src/reset.ico"));
    ui->transBtn->setIcon(QIcon(":/image/src/exchange.ico"));

    //设置提示符
    ui->deleteStrEdit->setToolTip(tr("删除特殊字符串"));


    //设置程序图标
    this->setWindowIcon(icon);

    //ui->statusBar->hide();//ui的状态栏隐藏
    //ui->mainToolBar->hide();//工具栏隐藏
    //ui->menuBar->hide();

    this->uiHeight=620;
    this->uiWidth=980;
    setPos();

    //
    shwoTrayIcon();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_transBtn_clicked()
{
    QString inputStr=ui->inputText->toPlainText();
    auto strU16=inputStr.toStdU16String();

    for(auto &value:strU16)
    {
        if ((value&0xff00)==0xff00) {
            value &=0x00ff;
            value +=0x20;
        }

    }
    QString newQstr(QString::fromStdU16String(strU16));
    //qDebug()<<newQstr;

    newQstr.replace(ui->deleteStrEdit->text(),"");
    if (this->hasSpace) {
        newQstr.replace("\t","");
    }else{
        newQstr.replace("\n","").replace(" ","").replace("\t","");
    }
    ui->outText->setPlainText(newQstr);
    //qDebug()<<newQstr;

}

void MainWindow::on_changeBtn_clicked()
{
    if (this->hasSpace) {
        this->hasSpace=false;
        ui->transBtn->setText(tr("换行转换"));
    }else{
        this->hasSpace=true;
        ui->transBtn->setText(tr("无换行转换"));
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type()==QEvent::WindowStateChange) {
        //qDebug()<<windowState();
        if (windowState()&Qt::WindowMinimized) {
            this->hide();
            setWindowState(Qt::WindowMinimized);
            //qDebug()<<windowState();
        }
    }

    //QMainWindow::changeEvent(event);
}

void MainWindow::setPos()
{
    //this->setGeometry([(0,0),uiWidth*uiHeight]);
    this->resize(uiWidth,uiHeight);
    int textWidth=uiWidth-2*spaceWidth;
    int textHeight=(uiHeight-3*spaceWidth-35)/2;
    //输入框位置
    ui->inputText->setGeometry(spaceWidth,spaceWidth,textWidth,textHeight);
    ui->outText->setGeometry(spaceWidth,textHeight+2*spaceWidth,textWidth,textHeight);

    //按钮位置
    ui->changeBtn->setGeometry(spaceWidth,uiHeight-40,90,30);
    ui->deleteStrEdit->setGeometry(spaceWidth+100,uiHeight-40,100,30);
    //ui->resetBtn->setGeometry(uiWidth-335,uiHeight-35,80,25);
    ui->resetBtn->setGeometry(uiWidth-215,uiHeight-40,90,30);
    ui->transBtn->setGeometry(uiWidth-100,uiHeight-40,90,30);

    //QPixmap pixmap=QPixmap(":/image/src/yellow.jpg").scaled(this->size());



}
/*
 *显示托盘图标
 *
 */
void MainWindow::shwoTrayIcon()
{
    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(this->icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(tr("文献换行处理"));
    //给QSystemTrayIcon添加槽函数
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    //建立托盘操作的菜单
    createActions();
    createMenu();

    //在系统托盘显示此对象
    mSysTrayIcon->show();
}

void MainWindow::createActions()
{
    mShowMainAction = new QAction(tr("显示主界面"),this);
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));

    mExitAppAction = new QAction(tr("退出"),this);
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));
}

void MainWindow::createMenu()
{
    mMenu = new QMenu(this);
    mMenu->addAction(mShowMainAction);
    mMenu->addSeparator();
    mMenu->addAction(mExitAppAction);

    mSysTrayIcon->setContextMenu(mMenu);
}

//void MainWindow::on_closeBtn_clicked()
//{
//    QMessageBox msgBox;
//    QIcon icon(":/image/src/warningt.ico");
//    msgBox.setWindowTitle("警告!!");
//    msgBox.setText("是否退出程序？");
//    msgBox.setWindowIcon(icon);
//    //msgBox.setInformativeText("Do you want to save your changes?");
//    msgBox.setStandardButtons(QMessageBox::Yes |  QMessageBox::No);
//    msgBox.setDefaultButton(QMessageBox::No);
//    int ret = msgBox.exec();
//    if (ret==QMessageBox::Yes) {
//        exit(0);
//    }

//}

void MainWindow::resizeEvent(QResizeEvent *)
{
    //qDebug()<<this->size();
    uiHeight=this->height();
    uiWidth=this->width();
    setPos();

    //qDebug()<<uiHeight<<" "<<uiWidth;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //删除托盘，删除界面，退出系统
    delete mSysTrayIcon;
    this->close();
    exit(0);
    //    QMessageBox msgBox;
    //    QIcon icon(":/image/src/warningt.ico");
    //    msgBox.setWindowTitle("警告!!");
    //    msgBox.setText("退出程序或者最小化？");
    //    msgBox.setWindowIcon(icon);

    //    QPushButton *minBtn= msgBox.addButton(tr("最小化"),QMessageBox::YesRole);
    //    QPushButton *exitBtn=msgBox.addButton(tr("退出"),QMessageBox::NoRole);
    //    QPushButton *cancleBtn=msgBox.addButton(tr("取消"),QMessageBox::RejectRole);
    //    msgBox.setDefaultButton(cancleBtn);
    //    msgBox.exec();


    //    if (msgBox.clickedButton()==exitBtn) {
    //        event->accept();
    //    }else if(msgBox.clickedButton()==minBtn){
    //        //隐藏程序主窗口
    //        this->hide();

    //        //新建QSystemTrayIcon对象
    //        mSysTrayIcon = new QSystemTrayIcon(this);
    //        //新建托盘要显示的icon
    //        //将icon设到QSystemTrayIcon对象中
    //        mSysTrayIcon->setIcon(this->icon);
    //        //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    //        mSysTrayIcon->setToolTip(tr("文献换行处理"));
    //        //给QSystemTrayIcon添加槽函数
    //        connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    //        //建立托盘操作的菜单
    //        createActions();
    //        createMenu();

    //        //在系统托盘显示此对象
    //        mSysTrayIcon->show();
    //        event->ignore();
    //    }else{
    //        event->ignore();
    //    }

}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    //qDebug()<<reason;
    switch(reason){
    //单击就可以显示了
    case QSystemTrayIcon::Trigger:
        //qDebug()<<"DoubleClick"<<endl;
        on_showMainAction();
        break;
    default:
        break;
    }
}
//显示主界面事件
void MainWindow::on_showMainAction()
{
    if(!this->isActiveWindow())
    {
        this->activateWindow();
        //qDebug()<<isActiveWindow();
    }
    //qDebug()<<windowState();
    if (windowState()&Qt::WindowMinimized) {
        this->showNormal();
        setWindowState(Qt::WindowActive);
        //setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        //setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    //qDebug()<<windowState();
    //mSysTrayIcon->hide();
}

void MainWindow::on_exitAppAction()
{
    //删除托盘，删除界面，退出系统
    //delete mSysTrayIcon;
    this->close();
    exit(0);
}


void MainWindow::on_resetBtn_clicked()
{

    this->uiHeight=620;
    this->uiWidth=980;

    //this->setWindowFlags(this->windowFlags()| Qt::FramelessWindowHint);

    setPos();
    ui->inputText->clear();
    ui->outText->clear();
}
