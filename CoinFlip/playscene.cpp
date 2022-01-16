#pragma execution_character_set("utf-8")
#include "playscene.h"


//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
//{

//}

PlayScene::PlayScene(int levelNum)
{
    //QString str = QString("进入了第 %1 关").arg(levelNum);
    //qDebug() << str;
    this->levelIndex = levelNum;

    //初始化游戏场景
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


    //添加音效资源
    //翻金币音效
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);
    //胜利按钮音效
    QSound *winSound = new QSound(":/res/LevelWinSound.wav",this);
    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);

    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(),this->height() - backBtn->height());

    //点击返回
    connect(backBtn,&MyPushButton::clicked,[=](){
        qDebug() << "翻金币场景中,点击了返回按钮";
        //播放返回按钮音效
        backSound->play();
        //告诉主场景 我返回了，选择场景监听ChooseLevelScene的返回按钮
        //延时返回
        QTimer::singleShot(500,[=](){
            emit this->chooseSceneBack();
        });

    });

    //显示当前关卡数
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    QString str = QString("Leavel: %1").arg(this->levelIndex);
    label->setText(str);
    label->setGeometry(QRect(20, this->height() - 50,150, 50)); //设置大小和位置


    //初始化每个关卡的二维数组
    dataConfig config;
    for(int i = 0; i < 4 ; i++)
    {
        for(int j = 0 ; j < 4 ; j++)
        {
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];

        }
    }

    //胜利图片
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());



    //显示金币的背景图片
    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++)
        {
            //绘制背景图片
            QLabel* label = new QLabel;
            label->setGeometry(0,0,50,50);
            label->setPixmap(QPixmap(":/res/BoardNode.png"));
            label->setParent(this);
            label->move(57 + i*50,200+j*50);


            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                //显示金币
                str = ":/res/Coin0001.png";
            }
            else
            {
                str = ":/res/Coin0008.png";
            }

            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59 + i*50,204+j*50);

            //给金币属性赋值
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j]; //1正面 0反面

            //将金币放入到 金币的二维数组中 以便后期的维护
            coinBtn[i][j] = coin;

            //点击金币 进行翻转
            connect(coin,&MyCoin::clicked,[=](){

                //播放翻金币的音效
                flipSound->play();

                //点击按钮 将所有按钮先都禁用
                for(int i = 0 ; i < 4 ; i++)
                {
                    for(int j = 0 ; j < 4 ; j++)
                    {
                        this->coinBtn[i][j]->isWin = true;
                    }
                }



                coin->changeFlag();
                this->gameArray[i][j] = this->gameArray[i][j] == 0 ? 1 : 0;


                //翻转周围硬币
                //周围的右侧金币翻转的条件，延时翻转
                QTimer::singleShot(300,this,[=](){
                    if(coin->posX + 1 <= 3 )
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY] = this->gameArray[coin->posX+1][coin->posY]  == 0 ? 1 : 0;

                    }
                    //周围的左侧硬币翻转条件
                    if(coin->posX - 1 >= 0)
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY] = this->gameArray[coin->posX-1][coin->posY]  == 0 ? 1 : 0;

                    }
                    //周围的下侧硬币翻转条件
                    if(coin->posY + 1 <= 3)
                    {
                        coinBtn[coin->posX][coin->posY + 1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY + 1] = this->gameArray[coin->posX][coin->posY+1]  == 0 ? 1 : 0;

                    }
                    //周围的上侧硬币翻转条件
                    if(coin->posY - 1 >= 0)
                    {
                        coinBtn[coin->posX][coin->posY - 1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY - 1] = this->gameArray[coin->posX][coin->posY-1]  == 0 ? 1 : 0;

                    }


                    //翻完周围金币后，将所有金币解开禁用
                    for(int i = 0 ; i < 4 ; i++)
                    {
                        for(int j = 0 ; j < 4 ; j++)
                        {
                            this->coinBtn[i][j]->isWin = false;
                        }
                    }

                    //判断是否胜利
                    this->isWin = true;
                    for(int i = 0 ; i < 4 ; i++)
                    {
                        for(int j = 0 ; j < 4 ; j++)
                        {
                            //只要有一个是反面，那就算失败
                            if(coinBtn[i][j]->flag == false)
                            {
                                this->isWin = false;
                                break;

                            }
                        }
                    }
                    if(this->isWin == true)
                    {
                        //添加胜利音效
                        winSound->play();
                        //胜利了！
                        qDebug() << "游戏胜利";
                        //将所有按钮的胜利标志改为true
                        //如果再次点击按钮，直接return，不做响应
                        for(int i = 0 ; i < 4 ; i++)
                        {
                            for(int j = 0 ; j < 4 ; j++)
                            {
                                coinBtn[i][j]->isWin = true;

                            }
                        }

                        //将胜利的图片移动下来
                        QPropertyAnimation * animation = new QPropertyAnimation(winLabel,"geometry");
                        //设置时间间隔
                        animation->setDuration(1000);
                        //设置开始位置
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        //设置结束位置
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                        //设置缓和曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        animation->start();
                    }

                });

            });

         }
    }


}


void PlayScene::paintEvent(QPaintEvent *event)
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

