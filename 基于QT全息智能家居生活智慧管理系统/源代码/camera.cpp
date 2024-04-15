#include "camera.h"
#include "ui_camera.h"

#include <QCameraInfo>
#include <QComboBox>
#include <QDateTime>




camera::camera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camera)
{
    ui->setupUi(this);

    this->setWindowTitle("摄像头");
    this->setWindowIcon(QIcon(":/new/prefix1/model/摄像头.jpg"));

    setWindowTitle("拍摄照片");
    setFixedSize(800,600);





    ui->btn_picture->setEnabled(false);//拍照键使能开始无法使用

    // 获取可用的摄像头设备信息列表
    CameraInfoList = QCameraInfo::availableCameras();
    QList<QCameraInfo>::const_iterator it;
   //遍历，将摄像头信息逐个添加到下拉列表当中
   for(it=CameraInfoList.begin();it!=CameraInfoList.end(); it++)
    {
        qDebug() << "Device name:" << (*it).deviceName();
        qDebug() << "Description:" << (*it).description();
        qDebug() << "Position:" << (*it).position();
        qDebug() << "Orientation:" << (*it).orientation();
        qDebug() << "---------------------";

        // 在ComboBox中添加设备描述
        ui->comboBox->addItem((*it).description());
    }
   //创建取景器并将其附在要显示的区域
   QCameraInfo CameraInfo = CameraInfoList.at(ui->comboBox->currentIndex());//获取到要打开的设备的名称


   camera1 =new QCamera(CameraInfo);//创建摄像头并将其附在要显示的区域

   //QVBoxLayout *layout = new QVBoxLayout(this);


   viewfinder = new QCameraViewfinder(ui->widget);

    camera1->setViewfinder(viewfinder);


  ImageCapture = new QCameraImageCapture(camera1);//捕获相机图像

    //打开摄像头
   connect(ui->btn_camera, &QPushButton::clicked, this, [=](){
       if(ui->btn_camera->text()=="打开摄像头")
       {
           ui->btn_picture->setEnabled(true);//允许拍照
//           获取到要打开的设备的名称
           QCameraInfo CameraInfo = CameraInfoList.at(ui->comboBox->currentIndex());

            camera1 =new QCamera(CameraInfo);//创建摄像头并将其附在要显示的区域
           viewfinder = new QCameraViewfinder(ui->widget);
            camera1->setViewfinder(viewfinder);

             viewfinder->resize(591,312);
           //显示摄像头取景器
           viewfinder->show();//开启摄像头
           camera1->start();

           ui->btn_camera->setText("关闭摄像头");

       }
       else if(ui->btn_camera->text()=="关闭摄像头")
       {

           ui->btn_picture->setEnabled(false);
           camera1->stop();
           viewfinder->hide();
           ui->btn_camera->setText("打开摄像头");
       }


   });



    //拍照
    connect(ui->btn_picture,&QPushButton::clicked,this,[=](){
        ImageCapture = new QCameraImageCapture(camera1);//捕获相机图像
        QDateTime dateTime(QDateTime::currentDateTime());
        QString time = dateTime.toString("yyyy-MM-dd-hh-mm-ss");
        camera1->setCaptureMode(QCamera::CaptureStillImage);//设置相机模式为静态图像模式
        QString picture_adr ="D:\\c++qt\\qt_daima\\qt_project\\picture\\"+time;
        //QString name=QFileDialog::getSaveFileName();//要保存的地址和文件名
        camera1->searchAndLock();//锁定相机设备
        ImageCapture->capture(picture_adr);//存储的照片名
        camera1->unlock();//释放锁定相机设备
        QMessageBox::information(this,"成功","拍照成功");

    });

    //返回上一级

    connect(ui->btn_return1,&QPushButton::clicked,this,&camera::return1);

    connect(ui->btn_return1,&QPushButton::clicked,this,&camera::closecamera);







}






camera::~camera()
{
    delete ui;
}





//void camera::on_btn_camera_clicked()
//{
// mycamera->start();
//}

void camera::closecamera()
{
    camera1->stop();

     this->close();
}




//void camera::on_pushButton_clicked()
//{
//    mycamera->stop();
//}



//void camera::on_btn_picture_clicked()
//{
//    QMessageBox::information(this,"拍照","拍照成功");
//    ImageCapture = new QCameraImageCapture(mycamera);

//       mycamera->searchAndLock();


//       QDateTime time = QDateTime::currentDateTime();
//       QString name = time.toString("yy-MM-dd hh-mm-ss");
//       ImageCapture->capture("D:\\c++qt\\qt_daima\\qt_project\\picture\\"+name +".ipg");
//       mycamera->unlock();
//}
