#ifndef MUSIC_H
#define MUSIC_H

#include <QWidget>




#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QTextEdit>
#include <QMessageBox>
#include  <QMap>
#include <QTimer>
#include <QPaintEvent>

namespace Ui {
class music;
}

class music : public QWidget
{
    Q_OBJECT

public:
    explicit music(QWidget *parent = nullptr);
    ~music();
    QMediaPlayer *player;//创建播放家类
    QMediaPlaylist *play_list;//创建播放家列表类

    // QJsonArray arry ;//存储歌曲的数组






    // 保存本地添加的音乐信息的 QMap
    QMap<QString, QString> localMusicMap;

    QMap<qint64, QString> parseLyrics;//歌词和时间戳的容器
    void parseLyrics_fun();//将文件歌词读入

    QNetworkAccessManager *http;
     QNetworkAccessManager *http1;
    void SendRequest();//发送请求
    void handle_info(QNetworkReply *reply);//处理信号
    void SendRequest_lyric();//发送请求歌词
    QMap<QString,int> Map_song;  //储存歌名和歌的id
    QMap<QString,int>Map_song2;//存储加入列表时的歌名和id
   void handle_lyric(QNetworkReply *reply1);//处理歌词

    qint64 pausedPosition; // 保存播放器暂停时的播放位置

    // 设置音乐总时长的槽函数
   void setTotalMusicDuration(qint64 duration);
   // 更新进度条的槽函数
   void updateProgressBar(qint64 position);

//   // 进度条数值变化时的槽函数
//   void sliderValueChanged(int value);

//   // 拖拽进度条时的槽函数
//   void sliderMoved(int value);

   void paintEvent(QPaintEvent *) override;
signals:
    void return1();

private:
    Ui::music *ui;
};

#endif // MUSIC_H
