#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QCloseEvent>
#include <post.h>
#include <user.h>

extern QHash<Category,QVector<Post>> all_post;//所有帖子
extern QVector<User> all_users;
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
    void start_listen();
    void new_connect();
    void sign_in();
    void send_messages();
    void load_file();
    void send_user_data();
    void send_post_data();
    void closeEvent(QCloseEvent *event);
    void disconnect_info();
    void on_quit_clicked();

private:
    Ui::MainWindow *ui;
    QFile file1;//文件对象
    QFile file2;
    qint64 file1size;//文件大小
    qint64 file2size;
    qint64 sendsize;//已经发送文件的大小
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;
    QTimer timer;
    QString console;
};

#endif // MAINWINDOW_H
