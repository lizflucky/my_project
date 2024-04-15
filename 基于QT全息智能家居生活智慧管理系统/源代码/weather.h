#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QMap>
#include <QLabel>
#include <QPaintEvent>
#include <QImage>

namespace Ui {
class weather;
}

class weather : public QWidget
{
    Q_OBJECT

public:
    explicit weather(QWidget *parent = nullptr);
    ~weather();

    QNetworkAccessManager * http;
    QMap<QString,QString>city_info;
    void SendRequest(); //发送请求
    void read_file();//读取文件信息
    void handle_info(QNetworkReply*reply);//处理信息
    void set_picture(QLabel* weather,QLabel* picture);//给对应的天气设置gif图片
    void paintEvent(QPaintEvent *)override;//绘图事件
signals:

    void return1();


private:
    Ui::weather *ui;
};

#endif // WEATHER_H
