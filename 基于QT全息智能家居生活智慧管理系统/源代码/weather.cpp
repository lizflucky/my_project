#include "weather.h"
#include "ui_weather.h"

#include <QSslSocket>
#include <QNetworkAccessManager>  //管理类
#include <QNetworkRequest>        //请求类
#include <QNetworkReply>          //响应类
#include <QUrl>                   //超链接
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>
#include <QMovie>
#include <QPainter>

weather::weather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::weather)
{

    ui->setupUi(this);

    this->setWindowTitle("天气预报");
    this->setWindowIcon(QIcon(":/new/prefix1/model/天气.jpg"));

    this->setWindowTitle("天气预报");
    this->setFixedSize(1157,503);
    read_file();//程序一启动就读取文件

    this->http=new QNetworkAccessManager(this);

    // 点击搜索按钮 发送请求
   connect(ui->btn_search,&QPushButton::clicked,this,&weather::SendRequest);

    connect(http,&QNetworkAccessManager::finished,this,&weather::handle_info);
    ui->lineEdit_city->setText("长沙市");
    ui->btn_search->click();
    //返回键
    connect(ui->btn_return1,&QPushButton::clicked,this,&weather::return1);



}

weather::~weather()
{
    delete ui;
}

void weather::SendRequest()
{
    QString ip = "http://t.weather.sojson.com/api/weather/city/"+this->city_info[ui->lineEdit_city->text()];//获取城市ip
    //发送请求 ，网址支持 get请求
    this->http->get(QNetworkRequest(QUrl(ip)));
}

void weather::read_file()
{
    //读取资源文件内容
    QFile file(":/new/prefix1/city_info.txt");
    //以读的方式打开
    if(!file.open(QIODevice::ReadOnly))
    {
        //打开文件失败
        QMessageBox::critical(this,"失败","资源文件打开失败");

    }


    //文件打开成功 读取文件中的信息，将其插入的city_info容器中
    // 城市作为key值   城市id作为value
    while(!file.atEnd())
    {
        QByteArray data = file.readLine();
        QString info = data;

        int ipos = info.indexOf('\t');//找到\t的下标
        QString value=info.mid(0,ipos);//求子串 找到value值
        QString key = info.mid(ipos+1);//获取从下标ipos+1开始的所有字符

        key = key.replace("\r\n","");//去掉换行符

        this->city_info.insert(key,value);

    }
}

void weather::handle_info(QNetworkReply*reply)
{
    //读取信息
    //看状态码  是否为200

    int statu = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statu!=200)
    {
        QMessageBox::information(this,"错误","网络异常");
        return ;
    }
    //请求成功
    //解析数据
    QByteArray data = reply->readAll();

    //将数据转化为 Json文档
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(doc.isObject())
    {
        QJsonObject obj = doc.object();

        //解析当前时间和城市
        ui->label_nowtime->setText(obj["time"].toString());//当前时间
        QJsonObject citymes = obj["cityInfo"].toObject();
        ui->city->setText("城市:"+citymes["city"].toString());//当前城市
        //解析当前温度和提醒标语
         QJsonObject data = obj["data"].toObject();
        ui->nowT->setText("当前温度:"+data["wendu"].toString()+"℃");//当前温度
        ui->Cur_banner->setText("注意:"+data["ganmao"].toString());//注意事项
        ui->air->setText("空气质量:"+data["quality"].toString());//当前空气质量



        //解析forecast 第一天
        QJsonArray forecast = data["forecast"].toArray();
        QJsonObject day1 = forecast[0].toObject();

        ui->now_weather->setText("天气:"+day1["type"].toString());//天气状况
        ui->label_time1->setText("今天");//星期
        ui->label_date1->setText(day1["ymd"].toString());//日期
        ui->label_weather1->setText(day1["type"].toString());//天气状况
        ui->label_highT1->setText(day1["high"].toString());//最高温度
        ui->label_lowT1->setText(day1["low"].toString());//最低温度
        ui->label_wind1->setText(day1["fx"].toString());//风的名称
        ui->label_wind_scale1->setText(day1["f1"].toString());//风的等级
        ui->remind->setText("提醒:"+day1["notice"].toString());//提示语



        //解析forecast 第二天
        QJsonObject day2 = forecast[1].toObject();
        ui->label_time2->setText("明天");//星期
        ui->label_date2->setText(day2["ymd"].toString());//日期
        ui->label_weather2->setText(day2["type"].toString());//天气状况
        ui->label_highT2->setText(day2["high"].toString());//最高温度
        ui->label_lowT2->setText(day2["low"].toString());//最低温度
        ui->label_wind2->setText(day2["fx"].toString());//风的名称
        ui->label_wind_scale2->setText(day2["f1"].toString());//风的等级

        //解析forecast 第三天
        QJsonObject day3 = forecast[2].toObject();
        ui->label_time3->setText(day3["week"].toString());//星期
        ui->label_date3->setText(day3["ymd"].toString());//日期
        ui->label_weather3->setText(day3["type"].toString());//天气状况
        ui->label_highT3->setText(day3["high"].toString());//最高温度
        ui->label_lowT3->setText(day3["low"].toString());//最低温度
        ui->label_wind3->setText(day3["fx"].toString());//风的名称
        ui->label_wind_scale3->setText(day3["f1"].toString());//风的等级

        //解析forecast 第四天
        QJsonObject day4 = forecast[3].toObject();
        ui->label_time4->setText(day4["week"].toString());//星期
        ui->label_date4->setText(day4["ymd"].toString());//日期
        ui->label_weather4->setText(day4["type"].toString());//天气状况
        ui->label_highT4->setText(day4["high"].toString());//最高温度
        ui->label_lowT4->setText(day4["low"].toString());//最低温度
        ui->label_wind4->setText(day4["fx"].toString());//风的名称
        ui->label_wind_scale4->setText(day4["f1"].toString());//风的等级

        //解析forecast 第五天
        QJsonObject day5 = forecast[4].toObject();
        ui->label_time5->setText(day5["week"].toString());//星期
        ui->label_date5->setText(day5["ymd"].toString());//日期
        ui->label_weather5->setText(day5["type"].toString());//天气状况
        ui->label_highT5->setText(day5["high"].toString());//最高温度
        ui->label_lowT5->setText(day5["low"].toString());//最低温度
        ui->label_wind5->setText(day5["fx"].toString());//风的名称
        ui->label_wind_scale5->setText(day5["f1"].toString());//风的等级

        //解析forecast 第六天
        QJsonObject day6 = forecast[5].toObject();
        ui->label_time6->setText(day6["week"].toString());//星期
        ui->label_date6->setText(day6["ymd"].toString());//日期
        ui->label_weather6->setText(day6["type"].toString());//天气状况
        ui->label_highT6->setText(day6["high"].toString());//最高温度
        ui->label_lowT6->setText(day6["low"].toString());//最低温度
        ui->label_wind6->setText(day6["fx"].toString());//风的名称
        ui->label_wind_scale6->setText(day6["f1"].toString());//风的等级

        //解析forecast 第七天
        QJsonObject day7 = forecast[6].toObject();
        ui->label_time7->setText(day7["week"].toString());//星期
        ui->label_date7->setText(day7["ymd"].toString());//日期
        ui->label_weather7->setText(day7["type"].toString());//天气状况
        ui->label_highT7->setText(day7["high"].toString());//最高温度
        ui->label_lowT7->setText(day7["low"].toString());//最低温度
        ui->label_wind7->setText(day7["fx"].toString());//风的名称
        ui->label_wind_scale7->setText(day7["f1"].toString());//风的等级


        //给对应的天气设置gif图片
        set_picture(ui->label_weather1,ui->label_picture1);//第一天
        set_picture(ui->label_weather2,ui->label_picture2);//第二天
        set_picture(ui->label_weather3,ui->label_picture3);//第三天
        set_picture(ui->label_weather4,ui->label_picture4);//第四天
        set_picture(ui->label_weather5,ui->label_picture5);//第五天
        set_picture(ui->label_weather6,ui->label_picture6);//第六天
        set_picture(ui->label_weather7,ui->label_picture7);//第七天

        set_picture(ui->label_weather1,ui->picture);//当天


    }

}

void weather::set_picture(QLabel* weather,QLabel* picture)
{
    // 创建 QMovie1 对象
    QMovie *movie1 = new QMovie;

    if(weather->text()=="晴")
    {

        // 加载 GIF 图像到 QMovie 对象
        movie1->setFileName(":/new/prefix1/天气/晴.gif");
        // 将 QMovie 对象设置为 QLabel 的动画
        picture->setMovie(movie1);
        // 启动动画
        movie1->start();

    }
    else if(weather->text()=="阴")
    {
        // 加载 GIF 图像到 QMovie 对象
         movie1->setFileName(":/new/prefix1/天气/阴天1.gif");
        // 将 QMovie 对象设置为 QLabel 的动画
        picture->setMovie(movie1);
        // 启动动画
        movie1->start();
    }
    else if(weather->text()=="多云")
    {
        // 加载 GIF 图像到 QMovie 对象
         movie1->setFileName(":/new/prefix1/天气/多云.gif");
        // 将 QMovie 对象设置为 QLabel 的动画
        picture->setMovie(movie1);
        // 启动动画
        movie1->start();

    }
    else if(weather->text()=="小雨")
    {
        // 加载 GIF 图像到 QMovie 对象
         movie1->setFileName(":/new/prefix1/天气/小雨.gif");
        // 将 QMovie 对象设置为 QLabel 的动画
        picture->setMovie(movie1);
        // 启动动画
        movie1->start();
    }
    else if(weather->text()=="中雨")
    {
        // 加载 GIF 图像到 QMovie 对象
         movie1->setFileName(":/new/prefix1/天气/中雨.gif");
        // 将 QMovie 对象设置为 QLabel 的动画
        picture->setMovie(movie1);
        // 启动动画
        movie1->start();
    }

    else if(weather->text()=="大雨")
    {
        // 加载 GIF 图像到 QMovie 对象
         movie1->setFileName(":/new/prefix1/天气/大雨.gif");
        // 将 QMovie 对象设置为 QLabel 的动画
        picture->setMovie(movie1);
        // 启动动画
        movie1->start();
    }
    else if(weather->text()=="霾")
    {
        // 加载 GIF 图像到 QMovie 对象
         movie1->setFileName(":/new/prefix1/天气/霾.jpg");
        // 将 QMovie 对象设置为 QLabel 的动画
        picture->setMovie(movie1);
        // 启动动画
        movie1->start();
    }
}

void weather::paintEvent(QPaintEvent *)
{
    QPainter pa(this);
    pa.drawPixmap(0,0,1157,503,QPixmap(":/new/prefix1/天空/1.jpg"));
}

