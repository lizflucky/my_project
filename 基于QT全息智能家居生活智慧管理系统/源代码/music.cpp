#include "music.h"
#include "ui_music.h"

#include <QPainter>

music::music(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::music)
{
    ui->setupUi(this);


    this->setWindowTitle("音乐播放器");
    this->setWindowIcon(QIcon(":/new/prefix1/model/音乐.jpg"));
    this->setWindowTitle("音乐播放器");

    this->player = new QMediaPlayer(this);
    this->play_list = new QMediaPlaylist(this);






    //返回键
    connect(ui->btn_return,&QPushButton::clicked,this,&music::return1);
    connect(ui->btn_return,&QPushButton::clicked,this->player,&QMediaPlayer::pause);


    //给播放加设置播放列表
    this->player->setPlaylist(play_list);
    //初始化 音乐播放器
    //设置默认播放模式
    this->play_list->setPlaybackMode(QMediaPlaylist::Loop);//默认列表循环播放
    //设置默认音量为50，范围为0-100
    this->player->setVolume(50);
    ui->volum_adjustment->setRange(0,100);
    ui->volum_adjustment->setValue(50);
    ui->volum_adjustment->setSingleStep(1);


        //按下添加，从本地文件中添加音乐
        connect(ui->btn_add,&QPushButton::clicked,this,[=](){
            //添加我们只添加本地音乐
            QStringList data = QFileDialog::getOpenFileNames(this,"选择音乐","D:\\c++qt\\qt_daima\\音乐", "*.mp3 *.mp4 *.wav *.acc");
            for(int i = 0;i < data.size();i++)
            {

                QFileInfo info(data[i]);
                localMusicMap.insert(info.fileName(), data[i]);


                //1、将音乐路径添加到 音乐播放列表
                if(this->play_list->addMedia(QMediaContent(QUrl::fromLocalFile(data[i]))) )
                {
                    //成功将音乐添加到播放列表之后我才可以添加到 ui 界面
                    ui->btn_paly->setEnabled(true);

                    ui->music_list->addItem(info.fileName());
                }
            }
        });


        //删除音乐
        connect(ui->btn_delete,&QPushButton::clicked,this,[=](){
            //删除指定音乐
            int select_index = ui->music_list->currentRow();
            qDebug()<<select_index << ui->music_list->count() << play_list->mediaCount();
            if(select_index >= 0 && select_index < ui->music_list->count())//删除音乐播放列表的文件
            {
                QString data =  ui->music_list->item(select_index)->text();
                qDebug()<<"hello";
                //删除ui文件
                QListWidgetItem *item= ui->music_list->takeItem(select_index);
                QString musicName = item->text();
                qDebug()<<"hello1"<<musicName;
                // 判断是否是本地添加的音乐并从 QMap 中移除
                if (localMusicMap.contains(musicName))
                {
                    localMusicMap.remove(musicName);
                }
                // 从播放列表中移除音乐
                this->play_list->removeMedia(select_index);
                 qDebug()<<"hello2";

               // QString data =  ui->music_list->item(select_index)->text();
                //找到第一个\t的位置
                int ipos = data.indexOf('\t');
                //找到第二个\t的位置
                QString data2 = data.mid(ipos+1);
                int ipos2 = data2.indexOf('\t');
                 qDebug()<<"hello3";

                QString data3 = data.mid(0,ipos+ipos2+1);
                // 判断是否是在 QMap_song2 中并移除
                if (Map_song2.contains(data3))
                {
                    Map_song2.remove(data3);
                }
            }
            else
            {
                QMessageBox::critical(this,"错误","已没有可删除的文件");


            }
            if(!ui->music_list->count())
            {
                ui->btn_paly->setEnabled(false);
                ui->btn_paly->setText("播放");
            }

        });

    //播放按钮
    connect(ui->btn_paly,&QPushButton::clicked,this,[=](){



        //获取当前选中的歌曲
        int select = ui->music_list->currentRow();



        if(ui->btn_paly->text() == "播放")
        {
            if (this->player->state() == QMediaPlayer::PausedState)
            {
                        // 如果是从暂停状态开始播放，从保存的位置开始播放
                        this->player->setPosition(this->pausedPosition);
            }
            else
            {
                    // 否则设置当前播放索引并开始播放
                   this->play_list->setCurrentIndex(select);
            }


            this->player->play();
            ui->btn_paly->setText("暂停");

        }
        else
        {
            this->player->pause();
            this->pausedPosition=this->player->position();
            ui->btn_paly->setText("播放");

        }
    });

    //上一曲下一曲

    connect(ui->btn_next,&QPushButton::clicked,this,[=](){
        int ipos = this->play_list->currentIndex();//ipos 最大为 this->play_list->mediaCount()-1
        if(ipos == this->play_list->mediaCount()-1)
        {
            ipos = -1;
        }
        this->play_list->setCurrentIndex(ipos+1);
    });

    connect(ui->btn_last,&QPushButton::clicked,this->play_list,[=](){

        int ipos = this->play_list->currentIndex();//ipos 最大为 this->play_list->mediaCount()-1
        if(ipos == 0)
        {
            ipos =  this->play_list->mediaCount();
        }
        this->play_list->setCurrentIndex(ipos-1);
    });

    //音量控制
    connect(ui->volum_adjustment,&QSlider::valueChanged,this->player,&QMediaPlayer::setVolume);


    //播放模式
    connect(ui->comboBox_mode,&QComboBox::currentTextChanged,this,[=](const QString &text){
        if(text == "列表循环")
        {
            this->play_list->setPlaybackMode(QMediaPlaylist::Loop);//默认列表循环播放
        }
        else if(text == "单曲循环")
        {
            this->play_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//单曲循环
        }
        else if(text == "随机播放")
        {
            this->play_list->setPlaybackMode(QMediaPlaylist::Random);//随机播放
        }
    });

    //设置默认 播放第一首
    if(ui->music_list->count() > 0)
    {

        ui->music_list->setCurrentRow(0);

    }
    else
    {
        ui->btn_paly->setEnabled(false);
    }


    //设置 ui 列表 播放歌曲改变的时候，ui 列表随之改变
    connect(this->play_list,&QMediaPlaylist::currentIndexChanged,this,[=](int index){

        ui->music_list->setCurrentRow(index);
    });

    //双击不同的音乐播放，双击相同的音乐 暂停
    connect(ui->music_list,&QListWidget::itemDoubleClicked,this,[=](QListWidgetItem *item){
        int current_index = ui->music_list->row(item);//获取我这个项目在多少行
        int playing_index = this->play_list->currentIndex();

        if(current_index == playing_index)
        {
            emit ui->btn_paly->clicked();
        }
        else
        {
            this->play_list->setCurrentIndex(current_index);//设置我要播放那首歌
            this->player->play();//播放
            ui->btn_paly->setText("暂停");
        }
    });


    //双击搜索列表歌曲，将歌曲添加到播放列表
    connect(ui->reserch_list,&QListWidget::itemDoubleClicked,this,[=](QListWidgetItem *item){


        QString data = item->text();
        //找到第一个\t的位置
        int ipos = data.indexOf('\t');
        //找到第二个\t的位置
        QString data2 = data.mid(ipos+1);
        int ipos2 = data2.indexOf('\t');


        QString data3 = data.mid(0,ipos+ipos2+1);
        qDebug()<<data3;
        //  QJsonObject obj_song  = arry[ui->reserch_list->row(item)].toObject();//获取双击的歌曲所在的行数，等于在歌在arry中的行数

        // Map_song.insert(data3,obj_song["id"].toInt());
        //   qDebug()<< Map_song[data3];
      //  Map_song2.insert(data3,Map_song[data3]);//存储当前播放列表的歌曲

        QString play_path = "http://music.163.com/song/media/outer/url?id=";
        play_path+=QString::number(Map_song[data3])+".mp3";
        //this->play_list->addMedia(QMediaContent(QUrl(play_path)));//将音乐添加到当前播放列表
        // 检查是否存在相同的音乐
        bool exists = false;
        for(int i=0;i<ui->music_list->count();i++)
        {
            if (ui->music_list->item(i)->text() == data)
            {
                exists = true;
                QMessageBox::information(this,"添加音乐","添加失败，音乐已存在播放列表");
                break;
            }

        }
        if(!exists)
        {
              this->play_list->addMedia(QMediaContent(QUrl(play_path)));
             Map_song2.insert(data3,Map_song[data3]);//存储当前播放列表的歌曲
            ui->music_list->addItem(item->text());
            ui->btn_paly->setEnabled(true);
        }

    });


    //http

    this->http = new QNetworkAccessManager(this);

    connect(ui->btn_search,&QPushButton::clicked,this,&music::SendRequest);


    connect(this->http,&QNetworkAccessManager::finished,this,&music::handle_info);
    //http1 歌词

    this->http1=new QNetworkAccessManager(this);
    connect(play_list,&QMediaPlaylist::currentMediaChanged ,this,&music::SendRequest_lyric);

    connect(this->http1,&QNetworkAccessManager::finished,this,&music::handle_lyric);








    // 连接播放器的相应信号和槽
    connect(player, &QMediaPlayer::durationChanged, this, &music::setTotalMusicDuration);
    connect(player, &QMediaPlayer::positionChanged, this, &music::updateProgressBar);

    //滑块与播放器连接
    //connect(ui->Slider_schedule, &QSlider::valueChanged, player, &QMediaPlayer::setPosition);//这个会使播放音乐卡顿
    connect(ui->Slider_schedule, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);






}

music::~music()
{
    delete ui;
}

void music::parseLyrics_fun()
{
    QFile file("D:\\c++qt\\qt_daima\\qt_project\\lyric.txt");
    parseLyrics.clear();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        while (!file.atEnd()) {
            QString line = file.readLine();
            // 解析时间戳和歌词文本
            QStringList parts = line.split("]");
            if (parts.length() == 2) {
                QString timeString = parts[0].mid(1); // 去掉左括号
                QTime time = QTime::fromString(timeString, "mm:ss.zzz");
                qint64 timestamp = time.isValid() ? time.msecsSinceStartOfDay() : 0;
                QString lyricText = parts[1].trimmed(); // 去掉前后空格
                parseLyrics.insert(timestamp, lyricText);
            }
        }
        file.close();
    }

}

void music::SendRequest()
{
    //向ip地址发送请求信息
    //发送请求 -> 网址
    //http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s= + 音乐名 + &type=1&offset=0&total=true&limit=20
    //请求方式 get
    QString ip="http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=" + ui->lineEdit_music->text()+"&type=1&offset=0&total=true&limit=20";
    this->http->get(QNetworkRequest(QUrl(ip)));

}

void music::handle_info(QNetworkReply *reply)
{
    //处理发送过来的信息
    int statu = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statu != 200)
    {
        QMessageBox::critical(this,"错误","网络异常");
        return;
    }
    QByteArray data = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    Map_song.clear();//先清除map容器的元素
    ui->reserch_list->clear();

    if(doc.isObject())
    {
        QJsonObject obj = doc.object();

        obj = obj["result"].toObject();
        QJsonArray  arry = obj["songs"].toArray();//获取歌的信息
        for(int i=0;i<20;i++)
        {
            QJsonObject obj_song = arry[i].toObject();

            int music_id = obj_song["id"].toInt();//找到歌曲id


            //找到歌手
            QJsonArray  arry_artists = obj_song["artists"].toArray();
            QJsonObject obj_artists = arry_artists[0].toObject();
            QString name = obj_artists["name"].toString();





            //歌曲时长
            int song_time= obj_song["duration"].toInt();
            int song_s=song_time/1000;
            int song_min =song_s/60;
            song_s = song_s%60;
            QString duration = QString::asprintf("%02d:%02d",song_min,song_s);

            //这个获取的只是信息，并不是音乐播放路径，我们需要去寻找音乐播放路径
            //路径为："http://music.163.com/song/media/outer/url?id=" + 音乐id + ".mp3";
            QString play_path = "http://music.163.com/song/media/outer/url?id=";

            //play_path += QString::number(music_id) +  ".mp3";//这才是完整的音乐播放路径
            //this->play_list->addMedia(QMediaContent(QUrl(play_path)));
            ui->reserch_list->addItem(obj_song["name"].toString()+'\t'+name+'\t'+duration);
            Map_song.insert(obj_song["name"].toString()+'\t'+name,music_id);
            qDebug()<<song_time;

        }
    }

}

//发送歌词请求
void music::SendRequest_lyric()
{

if(ui->music_list->currentItem()==nullptr)
{
    QMessageBox::information(this,"列表","列表已清空或者未选中歌曲");
    ui->music_lyric->clear();
    return ;
}

    QString data =  ui->music_list->currentItem()->text();//获取当前选中的歌曲
    //找到第一个\t的位置
    int ipos = data.indexOf('\t');
    //找到第二个\t的位置
    QString data2 = data.mid(ipos+1);
    int ipos2 = data2.indexOf('\t');


    QString data3 = data.mid(0,ipos+ipos2+1);
    qDebug()<<data3;
    qDebug()<<Map_song2[data3];
    if(Map_song2.count()<=0)
    {
        QMessageBox::information(this,"歌词","当前已存在歌词歌曲不存在");
        return;
    }

    QString ip2="https://music.163.com/api/song/media?id=";
    ip2+=QString::number(Map_song2[data3]);

    this->http1->get(QNetworkRequest(QUrl(ip2)));


}

//处理歌词
void music::handle_lyric(QNetworkReply *reply1)
{

    //处理发送过来的信息
    int statu = reply1->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statu != 200)
    {
        QMessageBox::critical(this,"错误","网络异常");
        return;
    }
    QByteArray data = reply1->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(doc.isObject())
    {
        QJsonObject obj = doc.object();
        // QJsonArray arry = obj["lyric"].toArray();//获取歌词的信息
        QFile file("D:\\c++qt\\qt_daima\\qt_project\\lyric.txt");
        file.open(QIODevice::WriteOnly| QIODevice::Truncate);
        if(file.isOpen())
        {
            file.write(obj["lyric"].toString().toUtf8());
        }
        else
        {
            qDebug() << "Failed to open file for writing.";
        }

    }

    parseLyrics_fun();

}

void music::setTotalMusicDuration(qint64 duration)
{

    // 设置进度条的范围为音乐总时长
    ui->schedule2->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg(duration/1000%60));
    ui->Slider_schedule->setRange(0, duration);
}

// 更新进度条的槽函数
void music::updateProgressBar(qint64 position)
{
    // 更新进度条的值
    ui->schedule1->setText(QString("%1:%2").arg(position/1000/60,2,10,QChar('0')).arg(position/1000%60));
    ui->Slider_schedule->setValue(position);


    //歌词
    // ui->music_lyric->clear();

    auto it = parseLyrics.upperBound(position);//upper_bound(position);

    if (it != parseLyrics.begin()) {
        // 往前移动一个位置，使得 it 指向不大于 position 的最大键值对
        --it;

        // 清空之前的歌词
        ui->music_lyric->clear();

//        // 添加正在唱的歌词
//        ui->music_lyric->addItem(it.value());
         // 添加正在唱的歌词，并设置特殊效果
           QListWidgetItem *currentItem = new QListWidgetItem(it.value());
           // 设置正在唱的歌词的文本颜色
           currentItem->setForeground(Qt::red); // 设置文本颜色为红色，
           ui->music_lyric->addItem(currentItem);

        // 添加前面的已唱歌词
        int count = 0;
        while (count < 4 && it != parseLyrics.begin()) {
            --it;
            ui->music_lyric->insertItem(0, it.value());
            ++count;
        }

        // 添加后面的未唱歌词
        it = parseLyrics.upperBound(position);
        count = 0;
        while (count < 10 && it != parseLyrics.end()) {
            ui->music_lyric->addItem(it.value());
            ++it;
            ++count;
        }
    } else {
        ui->music_lyric->clear(); // 如果没有找到对应的歌词，清空显示
    }
}

void music::paintEvent(QPaintEvent *)
{
    QPainter pa(this);
    pa.drawPixmap(0,0,975,695,QPixmap(":/new/prefix1/图片/preview3.jpg"));
}


//void music::sliderValueChanged(int value)
//{
//    // 将滑块值转换为对应的音乐播放位置，并设置音乐播放位置
//        player->setPosition(value);
//}

//拖拽进度条时的槽函数
//void music::sliderMoved(int value)
//{
//    // 将滑块值转换为对应的音乐播放位置，并设置音乐播放位置
//    player->setPosition(value);
//}





