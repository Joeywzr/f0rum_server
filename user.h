#ifndef USER_H
#define USER_H
#include <QString>
#include <QTextStream>
#include <QVector>
class User;
extern QVector<User> all_users;//所有用户
class User
{
public:
    User();
    int id;//ID
    QString username;//用户名
    QString password;//密码
    QString level;//等级
    int responsible_plate;//版主负责板块，管理员和普通用户为0
    int user_error = 0;//读入用户信息的错误个数

    //重载<<运算符
    friend QTextStream& operator << (QTextStream &fout, User &users)
    {
        for(int i = 0;i < all_users.size();i++)//将用户信息存入文件，每个用户的各种信息以空格相隔，不同用户用回车相隔
        {
            users = all_users[i];
            fout << QString::number(users.id) << " ";
            fout << users.username << " ";
            fout << users.password << " ";
            fout << users.level << " ";
            fout << users.responsible_plate << endl;
        }
        return fout;
    }
    //重载>>运算符
    friend QTextStream& operator >> (QTextStream &fin, User &users)
    {
        QString line;//每行信息
        QList<QString> strlist;//存取每个用户的各种信息
        bool flag = true;//用来判断用户信息是否出错
        while(!fin.atEnd())
        {
            line = fin.readLine();//按行读入
            strlist = line.split(" ");//把读入的行按空格分隔
            if(strlist.size() != 5)//如果每个用户信息不足5项，则报错，跳过读入该用户信息
                users.user_error++;
            else
            {
                for(int ii = 0;ii <= 4;ii++)
                    if(strlist[ii].size() == 0)
                        flag = false;
                users.id = strlist[0].toInt();
                if(users.id == 0)
                    flag = false;
                users.username = strlist[1];
                users.password = strlist[2];
                users.level = strlist[3];
                //用户等级信息错误
                if(users.level != "administrator" && users.level != "ordinary" && users.level != "moderator")
                    flag = false;

                users.responsible_plate = strlist[4].toInt();
                if(users.responsible_plate < 0 || users.responsible_plate > 5)//版主负责板块错误
                    flag = false;
                if(flag)
                    all_users.push_back(users);
                else
                {
                    users.user_error++;
                    flag = true;
                }
            }
        }
        return fin;
    }
};

#endif // USER_H
