#include "choice_model.h"
#include "ui_choice_model.h"

choice_model::choice_model(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::choice_model)
{
    ui->setupUi(this);




    this->setWindowTitle("菜单");
    this->setWindowIcon(QIcon(":/new/prefix1/main/菜单.jpg"));

    smart_furniture1 = new smart_furniture();
    weather1 = new weather();
    music1 = new  music();
    camera1 = new camera();


    //进入到智能家居
    connect(ui->btn_smart,&QPushButton::clicked,this,[=](){
        this->hide();
        smart_furniture1->show();
    });

    //进入到音乐播放器
    connect(ui->btn_music,&QPushButton::clicked,this,[=](){

        this->hide();
        music1->show();

    });

    //进入天气预报
    connect(ui->btn_weather,&QPushButton::clicked,this,[=](){

        this->hide();
        weather1 ->show();

    });
   // 进入摄像头
    connect(ui->btn_camera,&QPushButton::clicked,this,[=](){


        this->hide();
        camera1 ->show();

    });

    //进入到天气预报，返回键
    connect(weather1,&weather::return1,this,[=](){
        weather1->hide();
        this->show();
    });


    //进入到摄像头，返回键
    connect(camera1,&camera::return1,this,[=](){
        camera1->hide();
        this->show();

    });


    //进入到音乐播放器，返回键
    connect(music1,&music::return1,this,[=](){
        music1->hide();
        this->show();

    });
    //进入到智能家居，返回键
    connect(smart_furniture1,&smart_furniture::return1,this,[=](){
        smart_furniture1->hide();
        this->show();

    });

}

choice_model::~choice_model()
{
    delete ui;
}
void choice_model::paintEvent(QPaintEvent *)
{
    QPainter pa(this);
    pa.drawPixmap(0,0,1000,500,QPixmap(":/new/prefix1/model/3.jpg"));
}
