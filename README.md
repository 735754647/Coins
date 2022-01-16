# Coins
This project is used to learn QT games
1、项目简介

如图所示，将所有的金币都翻转为金色，即可取得胜利。



2、项目基本配置

创建项目（注意不要有中文路径），类名为MainScene
新建Qt Resource File文件，添加图标和音乐等文件到工程项目下
3、主场景

3.1 整个工程项目包括三个场景：

主场景：开始界面（maincene.cpp）
选择关卡场景：进行关卡的选择（chooselevelscene.cpp）
翻金币场景：游戏的主要场景（playscene.cpp）
3.2 在上一步中，我们已经新建了MainScene的类，下面说一下MainScene需要做的工作：

（1）场景的基本配置

设置固定大小（this->setFixedSize(320,588)）
设置应用图标（this->setWindowIcon(QPixmap(":/res/Coin0001.png"))）
设置窗口标题（this->setWindowTitle("翻金币")）
设置背景图片（需要重写MainScene的PaintEvent事件，记得在头文件声明一下）
复制代码
void MainScene::paintEvent(QPaintEvent *)
{
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建QPixmap对象
    QPixmap pix;
    //加载图片
    pix.load(":/res/PlayLevelSceneBg.png");
    //绘制背景图
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");
    //缩放图片
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    //绘制标题
    painter.drawPixmap( 10,30,pix.width(),pix.height(),pix);
}
复制代码
 

（2）创建开始按钮（实现弹跳效果，需要封装出一个按钮控件，来实现这些效果）

新建MyPushButton类，记得修改头文件代码继承于QPushButton
修改MyPushButton的头文件：提供构造的重载版本，可以让MyPushButton提供正常显示的图片及按下后显示的图片；同时需要定义按钮向上和向下跳的特效，需要定义void zoom1( );和void zoom2( );以及需要写一下鼠标按下和释放的事件；
复制代码
//normalImg 代表正常显示的图片
//pressImg  代表按下后显示的图片，默认为空
MyPushButton(QString normalImg,QString pressImg = "");

QString normalImgPath;  //默认显示图片路径
QString pressedImgPath; //按下后显示图片路径
void zoom1();//向下跳
void zoom2();//向上跳  
//重写按下和释放事件
void mousePressEvent(QMouseEvent *);
void mouseReleaseEvent(QMouseEvent *);
复制代码
编写MyPushButton.cpp的代码（就是在头文件中声明的那几个函数）
复制代码
MyPushButton::MyPushButton(QString normalImg,QString pressImg)
{
    //成员变量normalImgPath保存正常显示图片路径
    normalImgPath = normalImg;
    //成员变量pressedImgPath保存按下后显示的图片
    pressedImgPath = pressImg;
    //创建QPixmap对象
    QPixmap pixmap;
    //判断是否能够加载正常显示的图片，若不能提示加载失败
    bool ret = pixmap.load(normalImgPath);
    if(!ret)
    {
        qDebug() << normalImg << "加载图片失败!";
    }
    //设置图片的固定尺寸
    this->setFixedSize( pixmap.width(), pixmap.height() );
    //设置不规则图片的样式表
    this->setStyleSheet("QPushButton{border:0px;}");
    //设置图标
    this->setIcon(pixmap);
    //设置图标大小
    this->setIconSize(QSize(pixmap.width(),pixmap.height()));
}

void MyPushButton::zoom1()
{
    //创建动画对象
    QPropertyAnimation * animation1 = new QPropertyAnimation(this, "geometry");
    //设置时间间隔，单位毫秒
    animation1->setDuration(200);
    //创建起始位置
    animation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    //创建结束位置
    animation1->setEndValue(QRect(this->x(), this->y(), this->width(), this-> height()));
    //设置缓和曲线，QEasingCurve::OutBounce为弹跳效果
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    //开始执行动画
    animation1->start();
}

void MyPushButton::zoom2()
{

    QPropertyAnimation * animation1 = new QPropertyAnimation(this, "geometry");
    animation1->setDuration(200);

    animation1->setStartValue(QRect(this->x(), this->y()+10, this->width(), this->height()));
    animation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    animation1->start();
}

//鼠标按下事件
void MyPushButton::mousePressEvent(QMouseEvent *e)
{
    if(pressedImgPath != "")//选中路径不为空，显示选中图片
    {
        QPixmap pixmap;
        bool ret = pixmap.load(pressedImgPath);
        if(!ret)
        {
            qDebug()<<pressedImgPath<<"加载图片失败";
        }
        
        this->setFixedSize(pixmap.width(), pixmap.height());
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pixmap);
        this->setIconSize(QSize(pixmap.width(), pixmap.height()));
    }
    //交给父类执行按下事件
    return QPushButton::mousePressEvent(e);
}

//鼠标释放事件
void MyPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(normallImgPath != "")//选中路径不为空，显示选中图片
    {
        QPixmap pixmap;
        bool ret = pixmap.load(normallImgPath);
        if(!ret)
        {
            qDebug()<<normallImgPath<<"加载图片失败";
        }
        this->setFixedSize(pixmap.width(), pixmap.height());
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pixmap);
        this->setIconSize(QSize(pixmap.width(), pixmap.height()));
    }
    //交给父类执行，释放事件
    return QPushButton::mouseReleaseEvent(e);
}
复制代码
在MainScene的构造函数中，创建开始按钮，同时监听点击按钮事件，执行特效，进入选择关卡场景
复制代码
MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
startBtn->setParent(this);
startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.7);
//监听点击事件，执行特效
connect(startBtn, &MyPushButton::clicked, [=](){
//播放开始的音效资源
startSound->play(); //开始音效
startBtn->zoom1();//向下跳跃
startBtn->zoom2();//向上跳跃
//进入选择关卡场景
//延时0.5秒后，进入选择场景
QTimer::singleShot(1000, this,[=](){
    this->hide();
    chooseScene->show();
   });
});
复制代码
上述步骤的运行效果如图所示：


4、选择关卡场景

4.1 选择关卡的设置主要包括以下几个方面

场景的基本设置（包括固定的大小、标题之类的）
按钮功能设置（返回按钮和关卡选择按钮）
4.2 详细说一下选择关卡的设置

（1）场景基本设置及背景设置

复制代码
//设置窗口固定大小
this->setFixedSize(320,588);

//设置图标
this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

//设置标题
this->setWindowTitle("选择关卡");

//创建菜单栏
QMenuBar * bar = this->menuBar();
this->setMenuBar(bar);

//创建开始菜单
QMenu * startMenu = bar->addMenu("开始");

//创建按钮菜单项
QAction * quitAction = startMenu->addAction("退出");

//点击退出 退出游戏
connect(quitAction,&QAction::triggered,[=](){this->close();});

void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

     //加载标题
    pix.load(":/res/Title.png");
    painter.drawPixmap( (this->width() - pix.width())*0.5,30,pix.width(),pix.height(),pix);
}
复制代码
（2）返回按钮设置，点击之后返回开始主界面，所有的按钮功能都是调用的MyPushButton中鼠标按下和弹起的功能，由于返回按钮有正常显示图片和点击后显示图片两种模式，所有需要重写MyPushButton.cpp中的MousePressEvent和MouseReleaseEvent（3.2—（2）的那个代码已经是修改过的，可以直接使用），同时音效实现的功能也直接在这里添加了（注意如果要使用音效的话，需要在工程文件代码中第一句加上multimedia，即QT += core gui multimedia，这样就可以在头文件中找到           # include <QSound.h>了）

复制代码
//返回按钮音效
QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);
//返回按钮
MyPushButton * closeBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
closeBtn->setParent(this);
closeBtn->move(this->width()-closeBtn->width(), this->height()-closeBtn->height());

//返回按钮功能实现
connect(closeBtn, &MyPushButton::clicked, [=](){
//播放返回音效
    backSound->play();
    QTimer::singleShot(500, this,[=](){
     this->hide();
      //触发自定义信号，关闭自身，该信号写到signals下做声明
     emit this->chooseSceneBack();
        });
});
复制代码
（3）选择关卡按钮设置（同样加入了音效的功能）

复制代码
//选择关卡按钮音效
QSound *chooseSound = new QSound(":/res/TapButtonSound.wav",this);
//创建选择关卡按钮
    for(int i = 0; i < 20; i++)
    {
        MyPushButton * menuBtn = new 
        MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(130 + (i % 4) * 100, 200 + (i / 4) * 120);

        //监听每个按钮的点击事件
        connect(menuBtn, &MyPushButton::clicked, [=](){
            //播放选择关卡的音效
            chooseSound->play();

            QString str = QString("您选择的是第%1关").arg(i+1);
            qDebug() << str;

            //进入到游戏场景
            this->hide();//将选关场景隐藏掉
            play = new PlayScene(i+1);//创建游戏场景
            play->show();//显示游戏场景

            connect(play, &PlayScene::chooseSceneBack, [=](){
                this->show();
                delete play;
                play = NULL;
            });
        });

        //按钮上显示的文字
        QLabel* label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(), menuBtn->height());
        label->setText(QString::number(i + 1));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置居中
        label->move(130+ (i % 4) * 100, 300 + (i / 4) * 100);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);//鼠标事件穿透
    }
}
复制代码
 

5、翻金币场景

5.1  翻金币场景的设置主要包括以下几个方面

场景及背景的设置（窗口大小、图标、菜单栏）、背景设置、当前关卡文字显示、金币背景显示
返回按钮设置
创建金币类
5.2  详细的内容介绍如下：

（1）场景及背景的设置（窗口大小、图标、菜单栏）、背景设置、当前关卡文字显示、金币背景显示

头文件声明
复制代码
public:
    explicit PlayScene(QWidget *parent = nullptr);

    PlayScene(int index);

    //成员变量 记录关卡索引
    int levelIndex;

    //背景函数
    void paintEvent(QPaintEvent *);

    //声明一个成员变量
    int gameArray[4][4];//二维数组数据   

    //金币按钮数组
    MyCoin * coinBtn[4][4];

    //判断是否胜利
    bool isWin;

    QMediaPlayer *endPlayer;

signals:
    void chooseSceneBack();
复制代码
场景设置
复制代码
PlayScene::PlayScene(int index)
{
    //qDebug() << "当前关卡为"<< index;
    this->levalIndex = index;
    //设置窗口固定大小
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("翻金币");

    //创建菜单栏
    QMenuBar * bar = this->menuBar();
    this->setMenuBar(bar);
    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");
    //创建按钮菜单项
    QAction * quitAction = startMenu->addAction("退出");
    //点击退出 退出游戏
    connect(quitAction,&QAction::triggered,[=](){this->close();});
}
复制代码
背景设置
复制代码
void PlayScene::paintEvent(QPaintEvent *)
{
    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap( 10,30,pix.width(),pix.height(),pix);
}
复制代码
当前关卡显示
复制代码
//当前关卡标题
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    QString str = QString("Leavel: %1").arg(this->levalIndex);
    label->setText(str);
    label->setGeometry(QRect(30, this->height() - 50,120, 50)); //设置大小和位置
复制代码
创建金币背景图片、金币及相关属性（都是在playscene.c中实现的）
复制代码
//绘制背景图片
           QPixmap pix=QPixmap(":/res/BoardNode(1).png");
           QLabel *label=new QLabel;
           label->setGeometry(0,0,50,50);
           label->setPixmap(pix);
           label->setParent(this);
//           label->move(57+i*50,200+j*50);
           label->move(150 + i * 80, 300 + j * 80);

//创建金币
           QString str;
           if(this->gameArray[i][j]==1)
           {
               //显示金币
               str=":/res/Coin0001.png";

           }
           else{
               //显示银币
               str=":/res/Coin0008.png";
           }
           MyCoin *coin=new MyCoin(str);
           coin->setParent(this);
//           coin->move(59+i*50,204+j*50);
           coin->move(150 + i * 80, 300 + j * 80);

           //给金币的属性赋值
           coin->posX=i;
           coin->posY=j;
           coin->flag=this->gameArray[i][j]; //  1正面 0反面

           //将金币放入到金币的二维数组里面 以便于后期的维护
           coinBtn[i][j]=coin;
复制代码
（2）返回按钮设置

复制代码
    //返回按钮
    MyPushButton * closeBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    closeBtn->setParent(this);
    closeBtn->move(this->width()-closeBtn->width(),this->height()-closeBtn->height());

    //返回按钮功能实现
    connect(closeBtn,&MyPushButton::clicked,[=](){
        QTimer::singleShot(500, this,[=](){
            this->hide();
            //触发自定义信号，关闭自身，该信号写到 signals下做声明
            emit this->chooseSceneBack();
             }
        );
    });
复制代码
（3）创建金币类：利用二维数组对金币属性进行维护，且支持点击、翻转特效，把这些功能进行封装

头文件声明
复制代码
public:
//    explicit MyCoin(QWidget *parent = nullptr);

    //图片路径
    MyCoin(QString btnImg);

    //扩展金币类的属性
    int posX;//x坐标
    int posY;//y坐标
    bool flag;//正反标志

    //改变标志，执行翻转效果
    void changeFlag();
    QTimer * timer1;//正面翻反面 定时器
    QTimer * timer2;//正面翻正面 定时器
    int min = 1;//最小图片
    int max = 8;//最大图片

    //翻转动画的标志
    bool isAnimation = false;

    //重写按钮的按下事件
    void mousePressEvent(QMouseEvent *);
    
    //胜利标志
    bool isWin = false;//胜利标志
复制代码
构造函数：创建金币对象—提供一个参数—代表传入的是金币还是银币资源，根据路径创建不同的图案
复制代码
MyCoin::MyCoin(QString butImg)
{

    QPixmap pixmap;
    bool ret = pixmap.load(butImg);
    if(!ret)
    {
        qDebug() << butImg << "加载图片失败!";
    }

    this->setFixedSize( pixmap.width(), pixmap.height() );
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pixmap);
    this->setIconSize(QSize(pixmap.width(),pixmap.height()));

}
复制代码
在当前的工程文件中添加dataconfig.c和dataconfig.h文件
初始化各个关卡
复制代码
//初始化二维数组
    dataConfig config;
    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++)
        {
            gameArray[i][j] = config.mData[this->levalIndex][i][j];
        }
    }
复制代码
翻金币的特效实现（mycoin.c）
复制代码
//初始化定时器对象
    time1=new QTimer(this);
    time2=new QTimer(this);

    //监听正面翻反面的信号， 并且翻硬币
    connect(time1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1").arg(this->min++);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");/*设置不规则图片样式*/
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

        //判断如果翻完了 将min 重置为1
        if(this->min>this->max)
        {
            this->min=1;
            isAnimation=false;//停止做动画，禁用按钮（当一个金币在做翻转动作时，另外一个动画不能动）
            time1->stop();
        }
    });

    //监听反面翻正面的信号，并且翻硬币
    connect(time2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1").arg(this->max--);
        pix.load(str);
        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");/*设置不规则图片样式*/
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

        //判断如果翻完了 将min 重置为1
        if(this->max<this->min)
        {
            this->max=8;
            isAnimation=false;//停止做动画
            time2->stop();
        }
复制代码
复制代码
void MyCoin::changeFlag()
{
    //如果是正面 翻成反面
    if(this->flag)
    {
        time1->start(30);
        isAnimation=true;//开始做动画
        this->flag=false;
    }
    else{//反面翻正面
        time2->start(30);
        isAnimation=true;//开始做动画
        this->flag=true;

    }
复制代码
 翻周围硬币（上下左右四个硬币同时翻动，playscene.c代码）
复制代码
//显示金币背景图案
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            //绘制背景图片
            QPixmap pix = QPixmap(":/res/BoardNode.png");
            QLabel *label = new QLabel;
            label->setGeometry(0, 0, pix.width(), pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57 + i * 50, 200 + j * 50);

            //初始化金币对象
            QString img;
            if (gameArray[i][j] == 1)
            {
                img = ":/res/Coin0001.png";
            }
            else
            {
                img = ":/res/Coin0008.png";
            }
            MyCoin * coin = new MyCoin(img);
            coin->setParent(this);
            coin->move(150 + i * 80, 300 + j * 80);
            coin->posX = i;//记录x坐标
            coin->posY = j;//记录y坐标
            coin->flag = gameArray[i][j];
            
            //记录每个按钮的位置
            coinBtn[i][j] = coin;

            //测试翻转金币的效果
            connect(coin, &MyCoin::clicked, [=](){
                flipSound->play();
               coin->changeFlag();
               //数组内部记录的标志同步修改
               gameArray[i][j] = gameArray[i][j] == 0 ? 1 : 0;

               //翻转周围金币
               QTimer::singleShot(300, this, [=](){
                  if(coin->posX + 1 <= 3)
                  {
                      coinBtn[coin->posX + 1][coin->posY]->changeFlag();
                      gameArray[coin->posX + 1][coin->posY] =
                      gameArray[coin->posX + 1][coin->posY] == 0 ? 1 : 0;
                  }

                  if(coin->posX - 1 >= 0)
                  {
                      coinBtn[coin->posX - 1][coin->posY] -> changeFlag();
                      gameArray[coin->posX - 1][coin->posY] =
                      gameArray[coin->posX -1][coin->posY] == 0 ? 1 : 0;
                  }

                  if(coin->posY + 1 <= 3)
                  {
                      coinBtn[coin->posX][coin->posY + 1]->changeFlag();
                      gameArray[coin->posX][coin->posY + 1] =
                      gameArray[coin->posX][coin->posY + 1] == 0 ? 1 :0;
                  }

                  if(coin->posY + 1 >= 0)
                  {
                      coinBtn[coin->posX][coin->posY - 1]->changeFlag();
                      gameArray[coin->posX][coin->posY - 1] =
                      gameArray[coin->posX][coin->posY - 1] == 0 ? 1 : 0;
                  }
this->isWin = true;
                  for(int i = 0; i < 4; i++)
                  {
                      for(int j = 0; j < 4; j++)
                      {
                          if(coinBtn[i][j]->flag==false)
                          {
                              this->isWin = false;
                              break;
                          }
                      }
                  }

                  if(this->isWin == true)
                  {
                      winSound->play();
                      qDebug() << "游戏胜利";
                      endPlayer->setMedia(QUrl::fromLocalFile("F:/QT/Day4/CoinFip/res/bkmusic.mp3"));
                      endPlayer->setMedia(QUrl("qrc:/res/bkmusic.mp3"));
                      endPlayer->setVolume(100);
                      endPlayer->play();


                      //禁用所有按钮点击事件
                          for(int i = 0 ; i < 4;i++)
                            {
                              for(int j = 0 ; j < 4; j++)
                              {
                                coinBtn[i][j]->isWin = true;
                              }
                             }

                      //将胜利的图片移动下来
                      QPropertyAnimation *animation=new QPropertyAnimation(winLabel,"geometry");
                      //设置时间间隔
                      animation->setDuration(1000);

                      //设置开始位置
                      animation->setStartValue(QRect(QPoint(winLabel->x(),winLabel->y()),QPoint(winLabel->x()+winLabel->width(),winLabel->height())));
                      //设置结束位置
                      animation->setEndValue(QRect(QPoint(winLabel->x(),winLabel->y()+120),QPoint(winLabel->x()+winLabel->width(),winLabel->height()+120)));

                      //设置缓和曲线
                      animation->setEasingCurve(QEasingCurve::OutBounce);
                      //执行动画
                      animation->start();

                  }

               });
            });
        }
    }
}
复制代码
胜利图片显示（playscene.c代码）
复制代码
//胜利图片显示
    QLabel *winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setParent(this);
    winLabel->setGeometry(0, 0, tmpPix.width(), tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->move((this->width() - tmpPix.width()) * 0.5, -tmpPix.height());
复制代码
注意事项：

（1）播放音乐的功能（记得在头文件中声明 #include <QSound>）

复制代码
 endPlayer=new QMediaPlayer(this);
 endPlayer->setMedia(QUrl::fromLocalFile("F:/QT/Config/res/bkmusic.mp3"));
                      
 endPlayer->setMedia(QUrl("qrc:/res/bkmusic.mp3"));
                      
 endPlayer->setVolume(100);
                      
 endPlayer->play();
复制代码
（2）金币就类似按钮的类，点击金币就相当于点击按钮（在其头文件中已经把其父类改成了QPushButton）
