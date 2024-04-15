#ifndef SMART_FURNITURE_H
#define SMART_FURNITURE_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
namespace Ui {
class smart_furniture;
}

class smart_furniture : public QWidget
{
    Q_OBJECT

public:
    explicit smart_furniture(QWidget *parent = nullptr);
    ~smart_furniture();
     void paintEvent(QPaintEvent *)override;//绘图事件
     void TCP_fun();

     QTcpServer *server;//服务器对象
     QTcpSocket *socket;//套接字通对象

signals:
    void return1();

private:
    Ui::smart_furniture *ui;
};

#endif // SMART_FURNITURE_H
