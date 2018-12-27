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
    console.append("开始监听...\n");
    ui->message->setPlainText(console);
    //监听任何连上19999端口的ip
    tcpserver->listen(QHostAddress::Any,19999);    
    //新连接信号触发，调用槽函数
    connect(tcpserver,SIGNAL(newConnection()),this,SLOT(new_connect()));
}

void MainWindow::new_connect()
{
    tcpsocket = new QTcpSocket(this);
    QString ip =  tcpsocket->peerAddress().toString();
    quint16 port = tcpsocket->peerPort();
    if(ip.isEmpty())
        ip = "127.0.0.1";
    QString str = QString("[%1:%2] 成功连接!\n").arg(ip).arg(port);
    console.append(str);
    ui->message->setPlainText(console);
    tcpsocket = tcpserver->nextPendingConnection();
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(sign_in()));
    connect(tcpsocket,SIGNAL(disconnected()),this,SLOT(disconnect_info()),Qt::QueuedConnection);
}

void MainWindow::sign_in()
{
    QByteArray login_info = tcpsocket->readAll();
    QList<QString> login_list = QVariant(login_info).toString().split(" ");
    QString username_input = login_list[0];
    QString password_input = login_list[1];
    QString username_wrong = "UDE";
    QString password_wrong = "PIW";

    User u;
    for(int i = 0;i < all_users.size();i++)
    {
        u = all_users[i];
        if(username_input == u.username)
        {
            if(password_input == u.password)
            {
                QString level = u.level;
                level.append(" ");
                QString responsible_plate = QString::number(u.responsible_plate);
                responsible_plate.append(" ");
                QString id = QString::number(u.id);
                tcpsocket->write(level.toStdString().c_str(),strlen(level.toStdString().c_str()));
                tcpsocket->write(responsible_plate.toStdString().c_str(),strlen(responsible_plate.toStdString().c_str()));
                tcpsocket->write(id.toStdString().c_str(),strlen(id.toStdString().c_str()));
                console.append("登录成功！\n");
                ui->message->setPlainText(console);
                return;
            }
            tcpsocket->write(password_wrong.toStdString().c_str(),strlen(password_wrong.toStdString().c_str()));
            console.append("密码错误！\n");
            ui->message->setPlainText(console);
            return;
        }
    }
    tcpsocket->write(username_wrong.toStdString().c_str(),strlen(username_wrong.toStdString().c_str()));
    console.append("用户名不存在！\n");
    ui->message->setPlainText(console);
    return;
}

void MainWindow::send_messages()
{

}

void MainWindow::load_file()
{

}


void MainWindow::send_user_data()
{

}

void MainWindow::send_post_data()
{

}

void MainWindow::disconnect_info()
{
    console.append("断开连接！\n");
    ui->message->setPlainText(console);
    tcpsocket->abort();
    tcpserver->close();
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
