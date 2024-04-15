#include "smart_furniture.h"
#include "ui_smart_furniture.h"

#include <QMovie>
#include <QPaintEvent>
#include <QTimer>
#include <QSpinBox>
#include <QPainter>
#include <qmessagebox.h>


smart_furniture::smart_furniture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::smart_furniture)
{
    ui->setupUi(this);



    this->setWindowTitle("智能家居");
    this->setWindowIcon(QIcon(":/new/prefix1/model/智能家居.jpg"));
    QTimer*time = new QTimer(this);//定时器

    // 设置定时器的间隔时间（毫秒）
        int interval = 1000; // 每秒更新一次滑块的值
        time->setInterval(interval);

        // 启动定时器
        time->start();


        ui->SpinBox->setValue(30);

    QMovie *movie_light= new QMovie;//灯的gif
    QMovie *movie_fan = new QMovie;//风扇gif



    this->server = new QTcpServer(this);
    this->socket = nullptr;
    ui->btn_send->setEnabled(false);

    //设置默认端口号
    ui->lineEdit_ports->setText("20000");//设置默认端口号为20000

    connect(ui->btn_start,&QPushButton::clicked,this,[=](){
        if(ui->btn_start->text() == "启动服务器")
        {
            unsigned int ports_num = ui->lineEdit_ports->text().toUInt();
            if(this->server->listen(QHostAddress::Any,ports_num))
            {
                //服务器启动后，端口号不得更改
                ui->lineEdit_ports->setReadOnly(true);
                QMessageBox::information(this,"启动服务器","服务器启动成功");
                ui->btn_start->setText("关闭服务器");
                ui->btn_start->setEnabled(false);
            }
        }
        else
        {
            this->server->close();
            //this->socket->close();//可以断掉连接
            this->socket->disconnectFromHost();
            ui->btn_start->setText("启动服务器");
            QMessageBox::information(this,"关闭服务器","服务器关闭成功");
            ui->btn_send->setEnabled(false);
        }
    });

    //编写连接的逻辑
    connect(this->server,&QTcpServer::newConnection,this,[=](){
        //获取与客户端通信的套接字
        this->socket = this->server->nextPendingConnection();
         qDebug()<<"有新的客户连接";
       // statu->setText("已连接");
        ui->btn_send->setEnabled(true);
        ui->btn_start->setEnabled(true);
        //连接后，可以收发消息,只有当socket 初始化后我才能正常使用
        connect(this->socket,&QTcpSocket::readyRead,this,[=](){
              QByteArray data = this->socket->readAll();
              if(data=="1")
              {
                  if(ui->btn_linght->text()=="开灯")
                  {

                      // 加载 GIF 图像到 QMovie 对象
                       movie_light->setFileName(":/new/prefix1/furniture/灯.gif");
                      // 将 QMovie 对象设置为 QLabel 的动画
                      ui->label_light->setMovie(movie_light);
                      // 启动动画
                      movie_light->start();
                      ui->btn_linght->setText("关灯");
                  }
                  else if(ui->btn_linght->text()=="关灯")
                  {

                      // 停止并清除动画
                              movie_light->stop();
                              ui->label_light->clear();
                        ui->label_light->setPixmap(QPixmap(":/new/prefix1/furniture/灯1.png"));
                         ui->btn_linght->setText("开灯");
                  }
              }
              else if(data=="0")
              {
                  if(ui->btn_fan->text()=="开风扇")
                  {
                      // 加载 GIF 图像到 QMovie 对象
                       movie_fan->setFileName(":/new/prefix1/furniture/fgif.gif");
                      // 将 QMovie 对象设置为 QLabel 的动画
                      ui->label_fan->setMovie(movie_fan);
                      // 启动动画
                      movie_fan->start();
                      ui->btn_fan->setText("关风扇");

                  }
                  else if(ui->btn_fan->text()=="关风扇")
                  {
                               // 停止并清除动画
                              movie_fan->stop();
                              ui->label_fan->clear();
                      ui->label_fan->setPixmap(QPixmap(":/new/prefix1/furniture/f.jpg"));
                        ui->btn_fan->setText("开风扇");
                  }
              }

              else if(data=="11")
               {
                  if(ui->btn_linght->text()=="开灯")
                  emit ui->btn_linght->clicked();
                if(ui->btn_fan->text()=="开风扇")
                     emit ui->btn_fan->clicked();
              }
              else if(data=="10")
              {
                  if(ui->btn_linght->text()=="开灯")
                  emit ui->btn_linght->clicked();
                if(ui->btn_fan->text()=="关风扇")
                     emit ui->btn_fan->clicked();
              }
              else if(data=="01")
              {
                  if(ui->btn_linght->text()=="关灯")
                  emit ui->btn_linght->clicked();
                if(ui->btn_fan->text()=="开风扇")
                     emit ui->btn_fan->clicked();
              }
              else if(data=="00")
              {
                  if(ui->btn_linght->text()=="关灯")
                  emit ui->btn_linght->clicked();
                if(ui->btn_fan->text()=="关风扇")
                     emit ui->btn_fan->clicked();
              }
             qDebug()<<"客户端:" + data;
        });

        //当客户端与服务器断开连接的时候，socket会发送 disconnected 信号
        connect(this->socket,&QTcpSocket::disconnected,this,[=](){
            //statu->setText("未连接");
            ui->btn_send->setEnabled(false);
            //手动关闭一下套接字
            this->socket->close();
           qDebug()<<"客户端已断开连接";
        });
    });

    //发送按钮
    connect(ui->btn_send,&QPushButton::clicked,this,[=](){
        //QString data = ui->mes_send->toPlainText();
      // ui->mes_send->clear();
        this->socket->write("1");
        //qDebug()<<"服务器:" + data;
    });



    //开关灯
    connect(ui->btn_linght,&QPushButton::clicked,this,[=](){

        if( ui->btn_start->text()=="关闭服务器")
        {
            this->socket->write("1");
        }

        if(ui->btn_linght->text()=="开灯")
        {

            // 加载 GIF 图像到 QMovie 对象
             movie_light->setFileName(":/new/prefix1/furniture/灯.gif");
            // 将 QMovie 对象设置为 QLabel 的动画
            ui->label_light->setMovie(movie_light);
            // 启动动画
            movie_light->start();
            ui->btn_linght->setText("关灯");
        }
        else if(ui->btn_linght->text()=="关灯")
        {

            // 停止并清除动画
                    movie_light->stop();
                    ui->label_light->clear();
              ui->label_light->setPixmap(QPixmap(":/new/prefix1/furniture/灯1.png"));
               ui->btn_linght->setText("开灯");
        }
    });

    //开关风扇
    connect(ui->btn_fan,&QPushButton::clicked,this,[=](){

         if( ui->btn_start->text()=="关闭服务器")
         {
         this->socket->write("0");
         }
        if(ui->btn_fan->text()=="开风扇")
        {
            // 加载 GIF 图像到 QMovie 对象
             movie_fan->setFileName(":/new/prefix1/furniture/fgif.gif");
            // 将 QMovie 对象设置为 QLabel 的动画
            ui->label_fan->setMovie(movie_fan);
            // 启动动画
            movie_fan->start();
            ui->btn_fan->setText("关风扇");

        }
        else if(ui->btn_fan->text()=="关风扇")
        {
                     // 停止并清除动画
                    movie_fan->stop();
                    ui->label_fan->clear();
            ui->label_fan->setPixmap(QPixmap(":/new/prefix1/furniture/f.jpg"));
              ui->btn_fan->setText("开风扇");
        }
    });


    //定时器与spinbox

    connect(time, &QTimer::timeout, this,[=](){
        double currentValue = ui->SpinBox->value();
        if(currentValue>24)
        {
            ui->label_air->setPixmap(QPixmap(":/new/prefix1/furniture/冷.jpg"));
           ui->SpinBox->setValue(currentValue -0.5);
        }
        else if(currentValue<20)
        {
            ui->label_air->setPixmap(QPixmap(":/new/prefix1/furniture/热.jpg"));
           ui->SpinBox->setValue(currentValue +0.5);
        }
        else if(currentValue<=24&&currentValue>=20)
        {
             ui->label_air->setPixmap(QPixmap(":/new/prefix1/furniture/正常.jpg"));
        }
    });


   // 返回
    connect(ui->btn_return,&QPushButton::clicked,this,&smart_furniture::return1);
}

smart_furniture::~smart_furniture()
{
    delete ui;
}

void smart_furniture::paintEvent(QPaintEvent *)
{
    QPainter pa(this);
    pa.drawPixmap(0,0,783,475,QPixmap(":/new/prefix1/furniture/home.png"));
}

//void smart_furniture::TCP_fun()
//{


//    this->server = new QTcpServer(this);
//    this->socket = nullptr;
//    ui->btn_send->setEnabled(false);

//    //设置默认端口号
//    ui->lineEdit_ports->setText("20000");//设置默认端口号为20000

//    connect(ui->btn_start,&QPushButton::clicked,this,[=](){
//        if(ui->btn_start->text() == "启动服务器")
//        {
//            unsigned int ports_num = ui->lineEdit_ports->text().toUInt();
//            if(this->server->listen(QHostAddress::Any,ports_num))
//            {
//                //服务器启动后，端口号不得更改
//                ui->lineEdit_ports->setReadOnly(true);
//                QMessageBox::information(this,"启动服务器","服务器启动成功");
//                ui->btn_start->setText("关闭服务器");
//                ui->btn_start->setEnabled(false);
//            }
//        }
//        else
//        {
//            this->server->close();
//            //this->socket->close();//可以断掉连接
//            this->socket->disconnectFromHost();
//            ui->btn_start->setText("启动服务器");
//            QMessageBox::information(this,"关闭服务器","服务器关闭成功");
//            ui->btn_send->setEnabled(false);
//        }
//    });

//    //编写连接的逻辑
//    connect(this->server,&QTcpServer::newConnection,this,[=](){
//        //获取与客户端通信的套接字
//        this->socket = this->server->nextPendingConnection();
//         qDebug()<<"有新的客户连接";
//       // statu->setText("已连接");
//        ui->btn_send->setEnabled(true);
//        ui->btn_start->setEnabled(true);
//        //连接后，可以收发消息,只有当socket 初始化后我才能正常使用
//        connect(this->socket,&QTcpSocket::readyRead,this,[=](){
//              QByteArray data = this->socket->readAll();
//              if(data=="1")
//              {
//                  if(ui->btn_linght->text()=="开灯")
//                  {

//                      // 加载 GIF 图像到 QMovie 对象
//                       movie_light->setFileName(":/new/prefix1/furniture/灯.gif");
//                      // 将 QMovie 对象设置为 QLabel 的动画
//                      ui->label_light->setMovie(movie_light);
//                      // 启动动画
//                      movie_light->start();
//                      ui->btn_linght->setText("关灯");
//                  }
//                  else if(ui->btn_linght->text()=="关灯")
//                  {

//                      // 停止并清除动画
//                              movie_light->stop();
//                              ui->label_light->clear();
//                        ui->label_light->setPixmap(QPixmap(":/new/prefix1/furniture/灯1.png"));
//                         ui->btn_linght->setText("开灯");
//                  }
//              }
//              else if(data=="0")
//              {
//                  emit ui->btn_fan->clicked();
//              }

//              else if(data=="11")
//               {
//                  if(ui->btn_linght->text()=="开灯")
//                  emit ui->btn_linght->clicked();
//                if(ui->btn_fan->text()=="开风扇")
//                     emit ui->btn_fan->clicked();
//              }
//              else if(data=="10")
//              {
//                  if(ui->btn_linght->text()=="开灯")
//                  emit ui->btn_linght->clicked();
//                if(ui->btn_fan->text()=="关风扇")
//                     emit ui->btn_fan->clicked();
//              }
//              else if(data=="01")
//              {
//                  if(ui->btn_linght->text()=="关灯")
//                  emit ui->btn_linght->clicked();
//                if(ui->btn_fan->text()=="开风扇")
//                     emit ui->btn_fan->clicked();
//              }
//              else if(data=="00")
//              {
//                  if(ui->btn_linght->text()=="关灯")
//                  emit ui->btn_linght->clicked();
//                if(ui->btn_fan->text()=="关风扇")
//                     emit ui->btn_fan->clicked();
//              }
//             qDebug()<<"客户端:" + data;
//        });

//        //当客户端与服务器断开连接的时候，socket会发送 disconnected 信号
//        connect(this->socket,&QTcpSocket::disconnected,this,[=](){
//            //statu->setText("未连接");
//            ui->btn_send->setEnabled(false);
//            //手动关闭一下套接字
//            this->socket->close();
//           qDebug()<<"客户端已断开连接";
//        });
//    });

//    //发送按钮
//    connect(ui->btn_send,&QPushButton::clicked,this,[=](){
//        //QString data = ui->mes_send->toPlainText();
//      // ui->mes_send->clear();
//        this->socket->write("1");
//        //qDebug()<<"服务器:" + data;
//    });

////    connect(ui->btn_clear,&QPushButton::clicked,ui->mes_send,&QTextEdit::clear);

////    ui->btn_send->setDefault(true);

//}

