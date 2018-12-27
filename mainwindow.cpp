#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file1( "user.txt" );
    file1.open( QFile::ReadOnly | QFile::Text );
    QTextStream fin1(&file1);
    User users;
    fin1 >> users;
    file1.close();

    if(users.user_error != 0)
    {
        QString error = "存在"+QString::number(users.user_error)+"个用户数据出现错误，已删除错误用户数据！";
        QMessageBox::StandardButton button;
        button = QMessageBox::information(this, tr("提示"),
                                          error);
        users.user_error = 0;
    }

    QFile file2("post.txt");
    file2.open(QFile::ReadOnly | QFile::Text);
    QTextStream fin2(&file2);
    Post posts;
    fin2 >> posts;
    file2.close();
    if(posts.post_error != 0)
    {
        QString error = "存在"+QString::number(posts.post_error)+"个帖子数据出现错误，已删除错误帖子数据！";
        QMessageBox::StandardButton button;
        button = QMessageBox::information(this, tr("提示"),
                                          error);
        posts.post_error = 0;
    }

    this->connect(ui->listen,SIGNAL(clicked(bool)),this,SLOT(start_listen()));
}

void MainWindow::start_listen()
{
    tcpserver = new QTcpServer(this);
    qDebug()<<"开始监听";
    //监听任何连上19999端口的ip
    tcpserver->listen(QHostAddress::Any,19999);
    QString ip =  tcpsocket->peerAddress().toString();
    quint16 port = tcpsocket->peerPort();
    QString str = QString("[%1:%2] 成功连接").arg(ip).arg(port);
    qDebug()<< str;
    //新连接信号触发，调用槽函数，这个跟信号函数一样可以随便取
    connect(tcpserver,SIGNAL(newConnection()),this,SLOT(load_file()));
}

void MainWindow::load_file()
{

    file1size = file1.size();
    file2size = file2.size();

//    connect(&timer,&QTimer::timeout,[=](){ timer.stop();send_user_data();});

//    QString head = QString("0&%1&%2").arg("user.txt").arg(file1size);
//    qint64 len = tcpsocket->write(head.toUtf8());
//    if( len < 0 )
//    {
//        qDebug()<<"用户头文件没有发送成功";
//        file1.close();
//    }
//    timer.start(20);//防止粘包
//    file1.close();
//    file2.close();
}


void MainWindow::send_user_data()
{
//    qint64 len = 0;
//    do{
//        len = 0;
//        char buf[4*1024] = {0};//每次发送数据大小
//        len = file1.read(buf,sizeof(buf));//读文件
//        len = tcpsocket->write(buf,len);//发文件
//        sendsize += len;
//    }while(len > 0);

//    if(sendsize != file1size)
//    {
//        file1.close();
//        qDebug()<<"用户文件未发送完全";
//        return ;
//    }
//    qDebug()<<"用户文件发送完毕";
//    file1.close();

//    connect(&timer,&QTimer::timeout,[=](){ timer.stop();send_post_data();});

//    QString head = QString("0&%1&%2").arg("post.txt").arg(file2size);
//    len = tcpsocket->write(head.toUtf8());
//    if( len < 0 )
//    {
//        qDebug()<<"帖子头文件没有发送成功";
//        file2.close();
//    }
//    timer.start(20);//防止粘包
}

void MainWindow::send_post_data()
{
//    qint64 len = 0;
//    do{
//        len = 0;
//        char buf[4*1024] = {0};//每次发送数据大小
//        len = file2.read(buf,sizeof(buf));//读文件
//        len = tcpsocket->write(buf,len);//发文件
//        sendsize += len;
//    }while(len > 0);

//    if(sendsize != file2size)
//    {
//        file2.close();
//        qDebug()<<"帖子文件未发送完全";
//        return ;
//    }

//    qDebug()<<"帖子文件发送完毕";
//    file2.close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QFile user("user.txt");
    QFile post("post.txt");

    user.open( QFile::ReadWrite | QFile :: Truncate );
    QTextStream fout1(&user);
    User users;
    fout1 << users;
    user.close();

    post.open( QFile::ReadWrite | QFile :: Truncate );
    QTextStream fout2(&post);
    Post posts;
    fout2 << posts;
    post.close();

    event->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}
