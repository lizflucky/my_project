#ifndef CHOICE_MODEL_H
#define CHOICE_MODEL_H
#include <weather.h>
#include <camera.h>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <music.h>
#include <smart_furniture.h>


namespace Ui {
class choice_model;
}

class choice_model : public QWidget
{
    Q_OBJECT

public:
    explicit choice_model(QWidget *parent = nullptr);
    ~choice_model();
    weather* weather1;//创建天气的类
    camera* camera1;//创建摄像头的类
    music* music1;//创建音乐播放器的类
    smart_furniture * smart_furniture1;//创建智能家具类
    void paintEvent(QPaintEvent *)override;//绘图事件

private:
    Ui::choice_model *ui;
};

#endif // CHOICE_MODEL_H
