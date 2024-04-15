#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QCameraInfo>

namespace Ui {
class camera;
}

class camera : public QWidget
{
    Q_OBJECT

public:
    explicit camera(QWidget *parent = nullptr);
    ~camera();
    QCamera *camera1;//摄像头
    QList<QCameraInfo> CameraInfoList;//选择设备
    QCameraViewfinder *viewfinder;//显示画面
    //QCameraInfo CameraInfo;//获取可用设备


    QCameraImageCapture*   ImageCapture;// 创建拍照对象



signals:
    void return1();//返回信号
private slots:


        void closecamera();//关闭摄像头
//    void on_btn_camera_clicked();

//    void on_pushButton_clicked();

//    void on_btn_picture_clicked();

private:
    Ui::camera *ui;
};

#endif // CAMERA_H
