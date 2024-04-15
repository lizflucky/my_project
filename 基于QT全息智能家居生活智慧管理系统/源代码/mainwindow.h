#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QVector>
#include <choice_model.h>
#include "choice_model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     QMap<QString,QString> info;//创建一个QMap容器info，用来储存账号密码
      QMap<QString,QString> info1;//创建一个QMap容器info1，用来储存账号和手机号
     QVector<int> rand1;//用来存储随机数，防止生成重复的随机数
     int randomNumber;//随机数

     choice_model*model;



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
