#ifndef POST_H
#define POST_H
#include "comment.h"
#include <QVector>
#include <QTextStream>
typedef enum
{
    novice=1,
    technology,
    resources,
    relax,
    appeal
}Category;

class Post;
extern QHash<Category,QVector<Post>> all_post;//所有帖子
extern Comment com;
class Post
{
public:
    Post();
    QString state;//帖子板块
    int id = 0;//帖子id
    QString poster_name;//发送者用户名
    QString title;//帖子标题
    QString content;//帖子内容
    QString time;//帖子时间
    int comment_num = 0;//评论数量
    QVector<Comment> comment;//帖子评论
    int post_error = 0;

    //重载Post类<<运算符
    friend QTextStream& operator << (QTextStream &fout, Post &posts)
    {
        bool is_first = true;//是否是每个帖子的的开头
        for(int i = 1;i <= 5;i++)
        {
            Category state;
            QString s;
            switch (i) {
            case 1:
                state = novice;s = "novice";break;
            case 2:
                state = technology;s = "technology";break;
            case 3:
                state = resources;s = "resources";break;
            case 4:
                state = relax;s = "relax";break;
            case 5:
                state = appeal;s = "appeal";break;
            default:break;
            }
            QVector<Post> p = all_post[state];
            for(int j = 0;j < p.size();j++)
            {
                posts = p[j];
                if(!is_first)
                    fout << " ### ";   //把每个帖子用" ### "分隔
                fout << s << " $$$ ";  //把每个帖子的内部信息用" $$$ "分隔
                fout << QString::number(posts.id) << " $$$ ";
                fout << posts.poster_name << " $$$ ";
                fout << posts.time << " $$$ ";
                fout << QString::number(posts.comment_num) << " $$$ ";
                fout << posts.title << " $$$ ";
                fout << posts.content;
                if(posts.comment.size() != 0)
                    fout << " $$$ ";
                for(int k = 0;k < posts.comment.size();k++)
                {
                    fout << posts.comment[k].username << " $$$ ";
                    fout << posts.comment[k].time << " $$$ ";
                    fout << posts.comment[k].content;
                    if(k != posts.comment.size() - 1)
                        fout << " $$$ ";
                }
                is_first = false;
            }
        }
        return fout;
    }
    //重载Post类>>运算符
    friend QTextStream& operator >> (QTextStream &fin, Post &posts)
    {
        QString str;
        str = fin.readAll();
        QList<QString> list1, list2;
        QList<QString>::iterator i;
        bool flag = true;
        list1 = str.split(" ### ");//按" ### "分隔读取各个帖子
        if(list1.size() <= 1)
            return fin;
        for(i = list1.begin();i != list1.end();i++)
        {
            list2 = (*i).split(" $$$ ");//按" $$$ "分隔读取帖子内部信息
            if(list2.size() != 7 + list2[4].toInt()*3)//如果帖子内部信息数量不对应
                posts.post_error++;
            else
            {
                for(int ii = 0;ii < list2.size();ii++)
                    if(list2[ii].size() == 0)
                        flag = false;
                posts.state = list2[0];
                if(posts.state != "novice" &&posts.state != "technology" &&posts.state != "resources" &&posts.state != "relex" && posts.state != "appeal")
                    flag = false;
                posts.id = list2[1].toInt();
                posts.poster_name = list2[2];
                posts.time = list2[3];
                posts.comment_num = list2[4].toInt();
                posts.title = list2[5];
                posts.content = list2[6];
                //读取评论信息
                for(int j = 7;j < list2.size();j++)
                {
                    if(j % 3 == 1)
                        com.username = list2[j];
                    else if(j % 3 == 2)
                        com.time = list2[j];
                    else if(j % 3 == 0)
                    {
                        com.content = list2[j];
                        posts.comment.push_back(com);
                    }
                }
                if(flag)//如果没错误就读入该帖子信息
                {
                    if(posts.state == "novice")
                        all_post[novice].push_back(posts);
                    else if(posts.state == "technology")
                        all_post[technology].push_back(posts);
                    else if(posts.state == "resources")
                        all_post[resources].push_back(posts);
                    else if(posts.state == "relax")
                        all_post[relax].push_back(posts);
                    else if(posts.state == "appeal")
                        all_post[appeal].push_back(posts);
                }
                else//若帖子有错则跳过该帖子不读入
                {
                    posts.post_error++;
                    flag = true;
                }
                posts.comment.clear();
            }
        }
        return fin;
    }
};

#endif // POST_H
