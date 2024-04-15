#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include "QDebug"
#include "QPushButton"
#include "QLabel"
#include <QMessageBox>
#include <cstdlib>
#include <ctime>
#include <QFile>
#include <QSslSocket>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->setWindowTitle("登录");
    this->setWindowIcon(QIcon(":/new/prefix1/main/登录.jpg"));

   model = new choice_model();

    //程序启动，将文件中的账号密码手机号插入info，info1容器中
    QFile file("D:\\c++qt\\qt_daima\\qt_project\\acount_passwd.txt");  //读取文件
    file.open(QIODevice::ReadOnly); //以只读的方式打开文件
    while(!file.atEnd())//遍历文件
    {
        QByteArray data=file.readLine();//按行读取
        QString info_data = data;
        int iops = info_data.indexOf('\t');//找到‘\t’返回其下标
        int iops1 = info_data.indexOf(':');//找到‘：’返回其下标
        QString acount_key = info_data.mid(0,iops);//提取账号
        QString passwd_value = info_data.mid(iops + 1, iops1 - iops - 1);//提取密码
        QString phone_value = info_data.mid(iops1+1);//提取手机号
        phone_value = phone_value.replace("\r\n","");//清除“\r\n”

        info.insert(acount_key,passwd_value);//将账号和手机号插入info容器中
        info1.insert(acount_key,phone_value);//将账号和手机号插入info1容器中
    }


    //注册账号
    connect(ui->btn_register,&QPushButton::clicked,this,[=](){


        this->hide();//将主界面隐藏


        //创建注册对话框

        //初始化对话框 并将各个控件摆好位置
        QDialog *dia = new QDialog;
        QLabel* leb1 = new QLabel("账号",dia);
        QLabel*leb2 = new QLabel("密码",dia);
        QLabel*leb3 = new QLabel("确认密码",dia);
        QLabel*leb4 = new QLabel("手机号码",dia);
        leb1->move(0,0);
        leb2->move(0,50);
        leb3->move(0,100);
        leb4->move(0,150);
        QLineEdit *act=new QLineEdit(dia);
        QLineEdit *pad=new QLineEdit(dia);
        QLineEdit *surepad=new QLineEdit(dia);
        QLineEdit*phone=new QLineEdit(dia);
        act->move(50,0);
        pad->move(50,50);
        surepad->move(80,100);
        phone->move(80,150);

        dia->resize(300,300);//设置对话固定大小为300X300
        dia->setWindowTitle("注册");//对话框标题
        dia->setAttribute(Qt::WA_DeleteOnClose);//在关闭窗口时自动删除窗口部件对象。
        //设置成不能输入中文
        pad->setAttribute(Qt::WA_InputMethodEnabled,false);
        surepad->setAttribute(Qt::WA_InputMethodEnabled,false);
        phone->setAttribute(Qt::WA_InputMethodEnabled,false);


        //确定 取消按钮的初始化
        QPushButton *but1 = new QPushButton("确定",dia);
        QPushButton *but2 = new QPushButton("取消",dia);
        but1->move(0,250);
        but2->move(200,250);

        // 设计随机数种子，以确保每次运行程序时生成的随机数都不同
        srand(unsigned(time(nullptr)));

        while(true)
        {
            // 生成一个 100000到 999999之间的随机整数
            this-> randomNumber = 100000+rand() % 900000;
            if(!rand1.contains(randomNumber))//检查rand1中是否存在重复的随机数
            {
                //rand1.push_back(randomNumber);
                //设计账号为6位的随机数
                act->setText(QString::number(randomNumber));
                act->setReadOnly(true);
                break;

            }

        }


        //注册信息输入好后，按下确认键
        connect(but1,&QPushButton::clicked,this,[=](){


            QString acount=act->text();
            QString passwd=pad->text();
            QString surepasswd=surepad->text();
            QString linephone=phone->text();

            //因为账号是随机生成的，且不可能有重复的，所以不需要判断账号是否重复

            if(passwd==surepasswd)//第一次输入的密码与第二次输入的密码相同
            {

                QFile file("D:\\c++qt\\qt_daima\\qt_project\\acount_passwd.txt");//存储账号密码
                file.open(QIODevice::WriteOnly | QIODevice::Append);
                if (file.isOpen() )
                {
                    // 文件打开成功，可以写入数据到文件中
                    file.write(acount.toUtf8()+'\t');
                    file.write(passwd.toUtf8()+':');
                    file.write(linephone.toUtf8()+"\r\n");



                    info.insert(acount,passwd);
                    info1.insert(acount,linephone);
                    rand1.push_back(randomNumber);

                    QMessageBox::information(this,"注册","注册成功");
                    file.close();

                    dia->close();
                    this->show();

                }
                else
                {
                    // 文件打开失败，无法写入数据到文件中
                    QMessageBox::information(this,"注册","注册失败");

                }

            }
            else {
                QMessageBox::information(this,"注册","注册账号失败");
                pad->clear();
                surepad->clear();
            }

        });
        connect(but2,&QPushButton::clicked,this,[=](){
            dia->close();
            this->show();
        });

        dia->exec();
    });


    //忘记密码
    connect(ui->btn_forget,&QPushButton::clicked,this,[=](){


        this->hide();//将主界面隐藏

        //创建忘记密码对话框

        //初始化对话框 并将各个控件摆好位置
        QDialog *dia = new QDialog;
        QLabel* leb1 = new QLabel("账号",dia);
        QLabel*leb2 = new QLabel("修改密码",dia);
        QLabel*leb3 = new QLabel("确认密码",dia);
        QLabel*leb4 = new QLabel("手机号码",dia);
        leb1->move(0,0);
        leb2->move(0,50);
        leb3->move(0,100);
        leb4->move(0,150);
        QLineEdit *act=new QLineEdit(dia);
        QLineEdit *changepad=new QLineEdit(dia);
        QLineEdit *surepad=new QLineEdit(dia);
        QLineEdit*phone=new QLineEdit(dia);
        act->move(50,0);
        changepad->move(80,50);
        surepad->move(80,100);
        phone->move(80,150);

        dia->resize(300,300);//设置对话固定大小为300X300
        dia->setWindowTitle("忘记密码");//对话框标题
        dia->setAttribute(Qt::WA_DeleteOnClose);//在关闭窗口时自动删除窗口部件对象。

        //设置成不能输入中文
        act->setAttribute(Qt::WA_InputMethodEnabled,false);
        changepad->setAttribute(Qt::WA_InputMethodEnabled,false);
        surepad->setAttribute(Qt::WA_InputMethodEnabled,false);
        phone->setAttribute(Qt::WA_InputMethodEnabled,false);


        //确定 取消按钮的初始化
        QPushButton *but1 = new QPushButton("确定",dia);
        QPushButton *but2 = new QPushButton("取消",dia);
        but1->move(0,250);
        but2->move(200,250);


        //验证信息输入好后，按下确认键
        connect(but1,&QPushButton::clicked,this,[=](){

            //读取输入的账号密码号码
            QString acount=act->text();
            QString passwd=changepad->text();
            QString surepasswd=surepad->text();
            QString lienphone=phone->text();

            QFile file("D:\\c++qt\\qt_daima\\qt_project\\acount_passwd.txt");  //读取文件
            file.open(QIODevice::ReadWrite);//以读写的方式打开

            if(file.isOpen())//判断是否打开成功
            {
                while(!file.atEnd())//遍历文件
                {
                    //按行读取文件  将文件中的账号密码 手机号码 分隔出来
                    QByteArray data=file.readLine();
                    QString info_data = data;
                    int iops = info_data.indexOf('\t');
                    int iops1 = info_data.indexOf(':');
                    QString key = info_data.mid(0,iops);//提取账号
                    QString passwd_value = info_data.mid(iops + 1, iops1 - iops - 1);//提取密码
                    QString phone_value = info_data.mid(iops1+1);//提取手机号
                    phone_value = phone_value.replace("\r\n","");
                    qDebug()<<key<<passwd_value<<phone_value;

                    //判断两次输入的修改的密码是否一致
                    if(passwd==surepasswd)
                    {

                        //判断输入的账号和手机号码和文件中的是否一致（即是否正确）
                        if(key==acount && this->info1[acount]==lienphone )
                        {
                            //将密码重新赋值，即修改密码
                            info[acount]=passwd;
                            //关闭文件 重新以只读和清空写的方式打开
                            file.close();
                            file.open(QIODevice::WriteOnly | QIODevice::Truncate);
                            //遍历info容器，将账号密码手机号重新插入写入到文件中
                            for(auto i=info.begin();i!=info.end();++i)
                            {


                                file.write(i.key().toUtf8()+'\t');
                                file.write(i.value().toUtf8()+':');
                                file.write(info1.value(i.key()).toUtf8()+"\r\n");
                            }

                            QMessageBox::information(this,"忘记密码","密码修改成功");
                            file.close();
                            dia->close();
                            this->show();
                            return ;
                        }

                    }


                    else
                    {
                        QMessageBox::information(this,"忘记密码","两次密码输入不一致");
                        changepad->clear();
                        surepad->clear();
                        break;
                    }

                }


                QMessageBox::information(this,"忘记密码","输入的账号或手机号不正确");
                changepad->clear();
                surepad->clear();




            }
            else
            {
                QMessageBox::information(this,"忘记密码","修改失败");
            }


        });
        //取消按钮
        connect(but2,&QPushButton::clicked,this,[=](){
            dia->close();
            this->show();
        });
        dia->exec();

    });




    //登录
    connect(ui->btn_login,&QPushButton::clicked,this,[=](){


        //获取登录的账号密码
        QString login_acount = ui->lineEdit_account->text();
        QString login_passward = ui->lineEdit_password->text();


        QFile file("D:\\c++qt\\qt_daima\\qt_project\\acount_passwd.txt");  //读取文件
        file.open(QIODevice::ReadOnly); //以只读的方式打开文件
        while(!file.atEnd())//遍历文件
        {
            QByteArray data=file.readLine();//按行读取
            QString info_data = data;
            int iops = info_data.indexOf('\t');//找到‘\t’返回其下标
            int iops1 = info_data.indexOf(':');//找到‘：’返回其下标
            QString acount_key = info_data.mid(0,iops);//提取账号
            QString passwd_value = info_data.mid(iops + 1, iops1 - iops - 1);//提取密码


            if(login_acount==acount_key&&login_passward==passwd_value)
            {
                QMessageBox::information(this,"登录","登录成功");
                this->hide();
                file.close();

                model->show();

                return ;

            }

        }
        QMessageBox::information(this,"登录","账号或者密码错误");
        ui->lineEdit_account->clear();
        ui->lineEdit_password->clear();



    });
    //设置成不能输入中文
    ui->lineEdit_account->setAttribute(Qt::WA_InputMethodEnabled,false);
    ui->lineEdit_password->setAttribute(Qt::WA_InputMethodEnabled,false);
    //隐藏密码
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);






}

MainWindow::~MainWindow()
{
    delete ui;
}

