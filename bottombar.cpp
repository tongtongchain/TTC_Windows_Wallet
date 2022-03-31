#include "bottombar.h"
#include "ui_bottombar.h"
#include "consoledialog.h"
#include "goopal.h"
#include "debug_log.h"
#include "commontip.h"
#include "peerinfodialog.h"
#include "control/myprogressbar.h"
#include "commondialog.h"
#include "extra/dynamicmove.h"
#include "datamgr.h"

#include <QPainter>
#include <QTimer>
#include <QMovie>
#include <QMouseEvent>

BottomBar::BottomBar(QWidget *parent) :
    QWidget(parent),
    timerForHide(NULL),
    isMoving(false),
    ui(new Ui::BottomBar)
{
    DLOG_QT_WALLET_FUNCTION_BEGIN;
    ui->setupUi(this);

    myProgressBar = new MyProgressBar(this);
	//connect(myProgressBar, SIGNAL(valueChanged(int)), this, SLOT(onProgressBarValueChanged(int)));
    myProgressBar->move(16,16);
    myProgressBar->hide();

	myProgressBar_wallet = new MyProgressBar(this);
	//connect(myProgressBar, SIGNAL(valueChanged(int)), this, SLOT(onProgressBarValueChanged(int)));
	myProgressBar_wallet->move(440, 16);
	myProgressBar_wallet->hide();

    setMouseTracking(true);
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255));
    setPalette(palette);

    mouseInsideOrNot = false;
    timerForHide = new QTimer(this);
    timerForHide->setInterval(2000);
    connect(timerForHide, SIGNAL(timeout()), this, SLOT(ontimerForHideOut()));
    timerForHide->start();

    ui->warningLabel->hide();
    ui->warningLabel2->hide();
	ui->warningLabel2_2->hide();

    ui->warningLabel->setPixmap( QPixmap(QCoreApplication::applicationDirPath() + "/pic2/warning.png") );
	ui->promptlabel->setText(tr("Connecting....."));
	ui->promptlabel_2->setText(tr("Connecting....."));
	
    connectionTip = new CommonTip;

    ui->signalBtn->installEventFilter(this);
    ui->signalBtn->setStyleSheet("background:transparent;");

    ui->lockBtn->setIcon(QPixmap(QCoreApplication::applicationDirPath() + "/pic2/lockBtn.png"));
    ui->lockBtn->setIconSize(QSize(10,13));
    ui->lockBtn->setStyleSheet("background:transparent");

	connect(DataMgr::getInstance(), &DataMgr::onGetInfo, this, &BottomBar::getInfo);
	connect(DataMgr::getInstance(), &DataMgr::onWalletGetInfo, this, &BottomBar::getwalletInfo);
	setFontPixelSize();
    DLOG_QT_WALLET_FUNCTION_END;
}

BottomBar::~BottomBar()
{
    DLOG_QT_WALLET_FUNCTION_BEGIN;
    delete ui;
    DLOG_QT_WALLET_FUNCTION_END;
}

void BottomBar::setFontPixelSize()
{
	QFont font = ui->warningLabel2->font();
	font.setPixelSize(12);
	ui->warningLabel2->setFont(font);
	ui->promptlabel->setFont(font);
	ui->promptlabel_2->setFont(font);
}

void BottomBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QColor(88,93,103));
    painter.setPen(QColor(88,93,103));
    painter.drawRect(QRect(0,0,827,40));
}

void BottomBar::updateNumOfConnections(QString result)
{

    int pos = result.indexOf("\"network_num_connections\"") + 26;
    QString num = result.mid( pos, result.indexOf("," , pos) - pos);
    numOfConnections = num.toInt();

    if( numOfConnections >= 4)
    {
        ui->signalBtn->setIcon( QPixmap(QCoreApplication::applicationDirPath() + "/pic2/signal4.png"));
        ui->signalBtn->setIconSize(QSize(24,20));
    }
    else if( numOfConnections == 3)
    {
        ui->signalBtn->setIcon( QPixmap(QCoreApplication::applicationDirPath() + "/pic2/signal3.png"));
        ui->signalBtn->setIconSize(QSize(24,20));
    }
    else if( numOfConnections == 2)
    {
        ui->signalBtn->setIcon( QPixmap(QCoreApplication::applicationDirPath() + "/pic2/signal2.png"));
        ui->signalBtn->setIconSize(QSize(24,20));
    }
    else if( numOfConnections == 1)
    {
        ui->signalBtn->setIcon( QPixmap(QCoreApplication::applicationDirPath() + "/pic2/signal1.png"));
        ui->signalBtn->setIconSize(QSize(24,20));
    }
    else
    {
        ui->signalBtn->setIcon( QPixmap(QCoreApplication::applicationDirPath() + "/pic2/signal0.png"));
        ui->signalBtn->setIconSize(QSize(24,20));    
    }

    if( numOfConnections > 0)
    {
        myProgressBar->show();
		myProgressBar_wallet->show();
        ui->syncLabel->show();
		//ui->promptlabel->hide();
		//ui->promptlabel_2->hide();
        ui->warningLabel->hide();
        ui->warningLabel2->hide();
		ui->warningLabel2_2->hide();
    } else {
        myProgressBar->hide();
		myProgressBar_wallet->hide();
        ui->syncLabel->hide();
		ui->promptlabel->hide();
		ui->promptlabel_2->hide();
        //ui->warningLabel->show();
        //ui->warningLabel2->show();
    }

}

void BottomBar::on_signalBtn_clicked()
{
//	PeerInfoDialog peerInfoDialog;
//  peerInfoDialog.pop();

}

void BottomBar::retranslator()
{
    ui->retranslateUi(this);
}

void BottomBar::getInfo(QString result)
{
	if (result.isEmpty())  return;
	QString jsonStr = result.right(result.length() - 9);
	QJsonDocument d = QJsonDocument::fromJson(jsonStr.toUtf8());
	QJsonObject info = d.object();

	numOfConnections = info.value(QString("network_num_connections")).toInt();
	updateNumOfConnections(result);

	//display head block
	int last_node_index = info.value(QString("blockchain_head_block_nodeindex")).toInt();
	int last_block = info.value(QString("blockchain_head_block_number")).toInt();
	ui->syncLabel->setText(QString::number(last_node_index) + " : " + QString::number(last_block));

	bool is_synced = info.value(QString("blockchain_is_synced")).toBool();
	if (is_synced == true) {
		myProgressBar->setValue(100);
		BottomBar::progressValueChanged(0);
	}
	else {
		myProgressBar->setValue(0);
	}
}

void BottomBar::progressValueChanged_wallet(int value)
{

	if (value == 100) {
		if (timerForHide) {
			timerForHide->start();
		}
		ui->promptlabel_2->setText(tr("Wallet Sync. Done!"));
	}
	else
	{
		ui->promptlabel_2->setText(tr("Synchronizing Wallet!"));
	}
}

void BottomBar::getwalletInfo(QString result)
	{
		qDebug() << "==========GEt Wallet INFO result: " << result << endl;
		if (result.isEmpty())  return;
		QString jsonStr = result.right(result.length() - 9);
		QJsonDocument d = QJsonDocument::fromJson(jsonStr.toUtf8());
		QJsonObject info = d.object();

		double scan_progress = info.value(QString("scan_progress")).toString().toDouble();

		int scan_progress_int = QString::number(scan_progress,'f',2).toDouble() * 100;
		qDebug() << "---------------SP: " << info.value(QString("scan_progress")) << endl;
		qDebug() << "---------------SCAN PROGRESS: " << scan_progress << endl;
		qDebug() << "---------------SCAN PROGRESS-1: " << scan_progress_int << endl;

		myProgressBar_wallet->setValue(scan_progress_int);

		ui->syncLabel_2->setText(QString::number(scan_progress_int)+ "%");
		progressValueChanged_wallet(scan_progress_int);

}

bool BottomBar::eventFilter(QObject *watched, QEvent *e)
{
    if( watched == ui->signalBtn)
    {
        if( e->type() == QEvent::ToolTip)
        {
            connectionTip->setText( " " + QString::number(numOfConnections) + tr(" nodes connected"));
            connectionTip->move( ui->signalBtn->mapToGlobal( QPoint( -20 ,-20)));
            connectionTip->show();
            return true;
        }
        else if( e->type() == QEvent::Leave)
        {
            connectionTip->hide();
            return true;
        }
    }

    return QWidget::eventFilter(watched,e);
}

void BottomBar::enterEvent(QEvent *e)
{
    mouseInsideOrNot = true;
    timerForHide->stop();
}

void BottomBar::leaveEvent(QEvent *e)
{
    if( myProgressBar->value() == 100)
    {
        mouseInsideOrNot = false;
        if( timerForHide)
        {
            timerForHide->start();
        }
    }
}

void BottomBar::dynamicShow()
{
    if( isMoving)  return;
    isMoving = true;
    DynamicMove* dynamicMove = new DynamicMove( this, QPoint(133,540), 20, 10, this);
    connect( dynamicMove, SIGNAL(moveEnd()), this, SLOT(moveEnd()));
    dynamicMove->start();
}

void BottomBar::dynamicHide()
{
    if( isMoving)  return;
    isMoving = true;
    DynamicMove* dynamicMove = new DynamicMove( this, QPoint(133,580), 20, 10, this);
    connect( dynamicMove, SIGNAL(moveEnd()), this, SLOT(moveEnd()));
    dynamicMove->start();
}

void BottomBar::ontimerForHideOut()
{
    if( !mouseInsideOrNot && myProgressBar->value() == 100 )
    {
        dynamicHide();
    }
}


void BottomBar::progressValueChanged(int value)
{

	if (value == 0) {
		if (timerForHide) {
			timerForHide->start();
		}
		ui->promptlabel->setText(tr("Data Sync. Done!"));
	}
	else
	{
		ui->promptlabel->setText(tr("Synchronizing data!"));
	}
}

void BottomBar::onProgressBarValueChanged(int value)
{

	//qDebug() << "Valud of Bottom Bar: " << value << endl;
    if( value == 100 ) {
        if( timerForHide) {
            timerForHide->start();
        }
		ui->promptlabel->setText(tr("Synchronization Done!"));
    }
	else
	{
		ui->promptlabel->setText(tr("Synchronizing data!"));
	}
}

void BottomBar::on_lockBtn_clicked()
{
    qDebug() << "BottomBar::on_lockBtn_clicked ";
    emit lock();
}

void BottomBar::moveEnd()
{
    isMoving = false;
}


void BottomBar::refresh()
{
	DataMgr::getInstance()->getInfo();
	DataMgr::getInstance()->walletGetInfo();
}
