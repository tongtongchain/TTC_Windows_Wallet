#include <QDebug>
#include <QPainter>
#include <QTextCodec>
#include <QDir>

#include <QClipboard>

#ifdef WIN32 
#include <windows.h>
#endif //zxlwin

#include "datamgr.h"
#include "rpcmgr.h"

#include "transferpage.h"
#include "ui_transferpage.h"
#include "debug_log.h"
#include "gop_common_define.h"
#include "contactdialog.h"
#include "remarkdialog.h"
#include "commondialog.h"
#include "transferconfirmdialog.h"
#include "waitingpage.h"
#include "goopal.h"
#define TRANSACTION_CONFIGURATION_FILENAME      "transaction_config.json"
//#define ALP_DEFAULT_MIN_TRANSACTION_FEE                     1 // 设置最低手续费为0.00001
//RAO Modification
#define ALP_DEFAULT_MIN_TRANSACTION_FEE                     10000 // 设置最低手续费为0.1

//#define ALP_DEFAULT_MAX_TRANSACTION_FEE                     30000000 // 设置最高手续费为1000
#define ALP_DEFAULT_MAX_TRANSACTION_FEE                     300000 // 设置最高手续费为1000
#define ALP_DEFAULT_TRANSACTION_FEE_RATIO					double(0.00300) //手续费比例0.003

TransferPage::TransferPage(QString name,QWidget *parent) :
    QWidget(parent),
    accountName(name),
    inited(false),
    transfer_time(0),
    waitingPage(nullptr),
    ui(new Ui::TransferPage)
{
    DLOG_QT_WALLET_FUNCTION_BEGIN;

    ui->setupUi(this);
	   setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255));
    setPalette(palette);

	QString language = DataMgr::getInstance()->getSettings()->value("/settings/language").toString();
    if( language.isEmpty() || language == "English")
    {
        ui->addContactBtn->setStyleSheet(QString("QToolButton{background-image:url(%1pic2/contactBtn2_En.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}"
                                         "QToolButton:hover{background-image:url(%2pic2/contactBtn2_hover_En.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}").arg(QCoreApplication::applicationDirPath() +"/").arg(QCoreApplication::applicationDirPath() +"/"));
        //ui->tipLabel2->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/pic2/feeTip_En.png"));
        //ui->tipLabel2->setGeometry(425,260,229,96);

        delegateLabelString = QCoreApplication::applicationDirPath() + "/pic2/delegateLabel_En.png";
        registeredLabelString = QCoreApplication::applicationDirPath() + "/pic2/registeredLabel_En.png";
    }
	else  if (language.isEmpty() || language == "Korean")
	{
		ui->addContactBtn->setStyleSheet(QString("QToolButton{background-image:url(%1pic2/contactBtn2_Ko.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}"
			"QToolButton:hover{background-image:url(%2pic2/contactBtn2_hover_Ko.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}").arg(QCoreApplication::applicationDirPath() +"/").arg(QCoreApplication::applicationDirPath() +"/"));
		//ui->tipLabel2->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/pic2/feeTip.png"));
		//ui->tipLabel2->setGeometry(425,260,317,77);

		delegateLabelString = QCoreApplication::applicationDirPath() + "/pic2/delegateLabel_Ko.png";
		registeredLabelString = QCoreApplication::applicationDirPath() + "/pic2/registeredLabel_Ko.png";
	}
    else
    {
        ui->addContactBtn->setStyleSheet(QString("QToolButton{background-image:url(%1pic2/contactBtn2.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}"
                                         "QToolButton:hover{background-image:url(%2pic2/contactBtn2_hover.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}").arg(QCoreApplication::applicationDirPath() +"/").arg(QCoreApplication::applicationDirPath() +"/"));
        //ui->tipLabel2->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/pic2/feeTip.png"));
        //ui->tipLabel2->setGeometry(425,260,317,77);

        delegateLabelString = QCoreApplication::applicationDirPath() + "/pic2/delegateLabel.png";
        registeredLabelString = QCoreApplication::applicationDirPath() + "/pic2/registeredLabel.png";
    }

    if( accountName.isEmpty())  // 如果是点击账单跳转
    {
        if( DataMgr::getInstance()->getAccountInfo()->size() > 0) {
            accountName = DataMgr::getInstance()->getAccountInfo()->keys().at(0);
        } else {
			// 如果还没有账户
            emit back();    // 跳转在functionbar  这里并没有用
            return;
        }
    }

    // 账户下拉框按字母顺序排序
	QStringList keys = DataMgr::getInstance()->getAccountInfo()->keys();
    ui->accountComboBox->addItems( keys);
    if( accountName.isEmpty() ) {
        ui->accountComboBox->setCurrentIndex(0);
    } else {
        ui->accountComboBox->setCurrentText( accountName);
    }
	QString address = DataMgr::getInstance()->getAccountInfo()->value(accountName).address;
    ui->addressLabel->setText(address);

//  bool voteOrNot = Goopal::getInstance()->configFile->value("/settings/voteOrNot").toBool();

//  if( Goopal::getInstance()->jsonDataValue("id_wallet_get_all_approved_accounts").contains("\"name\":\"") && voteOrNot)
	if (false) {
        ui->voteCheckBox->setChecked( true);
    } else {
        ui->voteCheckBox->setChecked( false);
    }

    ui->amountLineEdit->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
    ui->amountLineEdit->setTextMargins(8,0,0,0);
    QRegExp rx1("^([0]|[1-9][0-9]{0,10})(?:\\.\\d{1,5})?$|(^\\t?$)");
    QRegExpValidator *pReg1 = new QRegExpValidator(rx1, this);
    ui->amountLineEdit->setValidator(pReg1);
	//QString oIpRange = "^([0]|[1-9][0-9]{0,10})(?:\\.\\d{1,5})?$|(^\\t?$)";
	//QRegExp oIpRegex("^" + oIpRange
	//	+ "\\." + oIpRange
	//	+ "\\." + oIpRange
	//	+ "\\." + oIpRange + "$");
	//ui->amountLineEdit->setValidator(new QRegExpValidator(oIpRegex));
    ui->amountLineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);

    ui->sendtoLineEdit->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
    ui->sendtoLineEdit->setTextMargins(8,0,0,0);
    QRegExp regx("[a-zA-Z0-9\-\.\ \n]+$");
    QValidator *validator = new QRegExpValidator(regx, this);
    ui->sendtoLineEdit->setValidator( validator );
    ui->sendtoLineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);

    ui->feeLineEdit->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
    ui->feeLineEdit->setTextMargins(8,0,0,0);
    ui->feeLineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);

    QRegExp rx("^([0]|[1-9][0-9]{0,5})(?:\\.\\d{1,2})?$|(^\\t?$)");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui->feeLineEdit->setValidator(pReg);

    ui->messageLineEdit->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
    ui->messageLineEdit->setTextMargins(8,0,0,0);

    //ui->tipLabel2->hide();
    ui->tipLabel2->setWordWrap(true);
    ui->tipLabel3->hide();
    ui->tipLabel4->hide();
    ui->tipLabel5->hide();
    ui->tipLabel6->hide();

    ui->tipLabel5->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/pic2/wrong.png"));

    on_amountLineEdit_textChanged(ui->amountLineEdit->text());

    ui->sendBtn->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
                               "QToolButton:hover{background-color:#ffc362;}"
                               "QToolButton:disabled{background-color:#cecece;}");
    ui->sendBtn->setText(tr("Transfer"));

#ifdef WIN32
    ui->accountComboBox->setStyleSheet(QString("QComboBox {border: 1px solid gray;border-radius: 3px;padding: 1px 2px 1px 8px;min-width: 9em;}"
                                           "QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 20px;"
                                                                  "border-left-width: 1px;border-left-color: darkgray;border-left-style: solid;"
                                                                  "border-top-right-radius: 3px;border-bottom-right-radius: 3px;}"
                                           "QComboBox::down-arrow {image: url(%1pic2/comboBoxArrow.png);}").arg(QCoreApplication::applicationDirPath() +"/")
                                           );
#else
    ui->accountComboBox->setStyleSheet(QString(
                                           "QComboBox {"
                                           "background-color: rgba(47, 47, 53, 10);"
                                           "background-repeat: no-repeat; "
                                           "selection-background-color: rgb(85, 85, 255); "
                                           "selection-color: rgb(255, 255, 255); "
                                           "}"
                                           "QComboBox::drop-down {background-color: rgba(47, 47, 53, 10);}"
                                           "QComboBox::down-arrow {image: url(%1pic2/comboBoxArrow.png);}"
                                       ).arg(QCoreApplication::applicationDirPath() +"/")
                                      );
#endif // WIN32 //zxlrun
    ui->voteCheckBox->setStyleSheet(QString("QCheckBox::indicator{ image:url(%1pic2/checkBox_unchecked.png); }"
                                    "QCheckBox::indicator:checked{ image:url(%2pic2/checkBox_checked.png); }").arg(QCoreApplication::applicationDirPath() +"/").arg(QCoreApplication::applicationDirPath() +"/"));

    ui->infoBtn->setStyleSheet(QString("QToolButton{background-image:url(%1pic2/info.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}").arg(QCoreApplication::applicationDirPath() +"/"));
    ui->infoBtn->installEventFilter( this);

    ui->copyBtn->setStyleSheet(QString("QToolButton{background-image:url(%1pic2/copyBtn.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}"
        "QToolButton:hover{background-image:url(%2pic2/copyBtn_hover.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}").arg(QCoreApplication::applicationDirPath() +"/").arg(QCoreApplication::applicationDirPath() +"/"));
#ifdef WIN32
    ui->copyBtn->move(ui->addressLabel->x() + ui->addressLabel->text().length() * 10 + 18, 120);
#else
    ui->copyBtn->move(ui->addressLabel->x() + ui->addressLabel->text().length() * 7.5 + 18, 120);
#endif // WIN32 //zxlrun
    ui->copyBtn->setToolTip(tr("copy to clipboard"));

    getContactsList();

    ui->delegateLabel2->adjustSize();
	//ui->changeAccountLabel->move(ui->delegateLabel2->x() + ui->delegateLabel2->width() + 10, 40);
    //ui->accountComboBox->move(ui->delegateLabel2->x() + ui->delegateLabel2->width() + 55, 35);
    //ui->identityLabel->move( ui->accountComboBox->x() + ui->accountComboBox->width() + 9, 41);

	inited = true;

	showBalance();
	setAssertType();//设置资产类型
    setFeeHint();

	//mod zxj
	//ui->feeLineEdit->setText(QString("%1").arg(DataMgr::getInstance()->getMinTransactionFee()));
	ui->feeLineEdit->setText("");
	ui->infoBtn->hide();
	ui->voteCheckBox->hide();
	ui->addContactBtn->hide();

	connect(DataMgr::getInstance(), &DataMgr::onWalletTransferToAddressWithId, this, &TransferPage::walletTransferToAddress);
	connect(DataMgr::getInstance(), &DataMgr::onWalletTransferToPublicAccountWithId, this, &TransferPage::walletTransferToAddress);
	connect(DataMgr::getDataMgr(), &DataMgr::onCallContract, this, &TransferPage::tokenTransferTo);
	connect(DataMgr::getInstance(), &DataMgr::onWalletCheckAddress, this, &TransferPage::walletCheckAddress);
	setFontPixelSize();

	nPrecision = DataMgr::getInstance()->getMainAssetPrecision();
	TransactionCount = 0;
	min_transaction_fee = ALP_DEFAULT_MIN_TRANSACTION_FEE;
	max_transaction_fee = ALP_DEFAULT_MAX_TRANSACTION_FEE;
	transaction_fee_ratio = ALP_DEFAULT_TRANSACTION_FEE_RATIO;

	QString path = DataMgr::getDataMgr()->getSettings()->value("/settings/ttchainPath").toString();
	path += "\\";
	path += TRANSACTION_CONFIGURATION_FILENAME;
	QFile file(path);
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray bytearray = file.readAll();
		QJsonParseError jsonError;
		QJsonDocument doucment = QJsonDocument::fromJson(bytearray, &jsonError);
		if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
		{
			if (doucment.isObject())
			{
				QJsonObject object = doucment.object();
				if (object.contains("min_transaction_fee")) {
					min_transaction_fee = object.value(QString("min_transaction_fee")).toInt();
				}
				if (object.contains("max_transaction_fee")) {
					max_transaction_fee = object.value(QString("max_transaction_fee")).toInt();
				}
				if (object.contains("transaction_fee_ratio")) {
				   QString strratio = object.value(QString("transaction_fee_ratio")).toString();
				   transaction_fee_ratio = strratio.toDouble();
				}

			}
		}

	}

    DLOG_QT_WALLET_FUNCTION_END;
}

void TransferPage::setbalanceLabel(QString balance, QString token)
{
	QString display_amount = addingCommasToAmount_TransferPage(balance);

    ui->balanceLabel->setText( "<body><font style=\"font-size:20pt\" color=#000000>" + display_amount + "</font><font style=\"font-size:9pt\" color=#000000>" +token+ "</font></body>" );
    ui->balanceLabel->adjustSize();
    ui->balanceLabel->move( 773 - ui->balanceLabel->width(),30);
    ui->balanceLabel2->move(650 - ui->balanceLabel->width(),42);
}

QString TransferPage::addingCommasToAmount_TransferPage(QString amount)
{
	//qDebug() << "Amount: " << amount;

	QStringList amount_list = amount.split(".");
	//qDebug() << "Amount Integer: " << amount_list[0] << endl;
	//if (amount_list.size() > 1)
	//qDebug() << "Amount Decimal: " << amount_list[1] << endl;


	QLocale l = QLocale::English;
	QString s = l.toCurrencyString(amount_list[0].toLongLong());
	//qDebug() << "S:      " << s << endl;
	//qDebug() << "S:      " << s.remove("$").remove(".00") << endl;

	if (amount_list.size() > 1)
		return s.remove("$").remove(".00") + "." + amount_list[1];
	else
		return s.remove("$");
}
TransferPage::~TransferPage()
{
    DLOG_QT_WALLET_FUNCTION_BEGIN;

    if(waitingPage != nullptr)
    {
        delete waitingPage;
        waitingPage = nullptr;
    }

    delete ui;

    DLOG_QT_WALLET_FUNCTION_END;
}

void TransferPage::walletCheckAddress(QString result)
{
    DLOG_QT_WALLET_FUNCTION_BEGIN;

    if(waitingPage != nullptr)
    {
        delete waitingPage;
        waitingPage = nullptr;
    }

	result = result.mid(result.indexOf(":") + 1, result.length());

	if (result == "true")
	{
        if (ui->amountLineEdit->text().toDouble() > -0.0000001 && ui->amountLineEdit->text().toDouble() < 0.0000001) {
			CommonDialog tipDialog(CommonDialog::OkOnly);
			tipDialog.setText(tr("The amount can not be 0"));
			tipDialog.pop();
			return;
		}

		//RAO Modification
		/*
        if (ui->feeLineEdit->text().toDouble() > -0.0000001 && ui->feeLineEdit->text().toDouble() < 0.0000001) {
			CommonDialog tipDialog(CommonDialog::OkOnly);
			tipDialog.setText(tr("The fee can not be 0"));
			tipDialog.pop();
			return;
		}
		*/


		//QString strBalanceTemp = DataMgr::getInstance()->getAccountInfo()->value(accountName).balance;
		QString strBalanceTemp = DataMgr::getInstance()->walletCurBalanceByAccountname(accountName);
		
		//strBalanceTemp = strBalanceTemp.remove(" TTCOIN");
		double dBalance = strBalanceTemp.remove(",").toDouble();
		//bool is_token_transfer = false;//(DataMgr::getCurrentAssetType() != COMMONASSET);
  //      if (is_token_transfer) {
  //          if (dBalance < 0.02) {
  //              CommonDialog tipDialog(CommonDialog::OkOnly);
  //              tipDialog.setText(tr("The TTCOIN amount must be greater than 0.02 ."));
  //              tipDialog.pop();
  //              return;
  //          }
  //      }else 
		{
            if (dBalance < 0.00001) {
                CommonDialog tipDialog(CommonDialog::OkOnly);
                tipDialog.setText(tr("The TTCOIN amount must be greater than 0.00001 ."));
                tipDialog.pop();
                return;
            }
        }

		QString remark = ui->messageLineEdit->text();
		if (DataMgr::getCurrentAssetType() != COMMONASSET) {
			remark.replace('|', '+');
		}
		//    remark.remove(' ');
		if (remark.size() == 0) {
			// 转地址如果没有备注 会自动添加 TO GOPDD...   所以添加空格
			remark = " ";
		}

		QTextCodec* utfCodec = QTextCodec::codecForName("UTF-8");
		QByteArray ba = utfCodec->fromUnicode(remark);
		if (ba.size() > 40) {
			CommonDialog tipDialog(CommonDialog::OkOnly);
			tipDialog.setText(tr("Message length more than 40 bytes!"));
			tipDialog.pop();
			return;
		}

		TransferConfirmDialog transferConfirmDialog(ui->sendtoLineEdit->text(), ui->amountLineEdit->text(), ui->feeLineEdit->text(), remark);
		bool yOrN = transferConfirmDialog.pop();
		if (yOrN) {
			//if (DataMgr::getCurrentAssetType() == COMMONASSET) {
				//QString str = "wallet_set_transaction_fee " + ui->feeLineEdit->text() + '\n';

				//mod zxj
				//Misc::write(str, RpcMgr::getInstance()->currentProcess());
				//QString result = Misc::read(RpcMgr::getInstance()->currentProcess());

				QString vote;
				if (ui->voteCheckBox->isChecked()) {
					vote = "vote_all";
				}
				else if (!ui->voteCheckBox->isChecked()) {
					vote = "vote_none";
				}

				if (ui->sendtoLineEdit->text().mid(0, 3) == "TTC") {

                    QString text(ui->amountLineEdit->text());
                    QString asset(DataMgr::getInstance()->getCurrentAssetType());
                    QString text2(ui->sendtoLineEdit->text());
                    //zxltabDataMgr::getInstance()->walletTransferToAddressWithId(accountName, ui->amountLineEdit->text(), DataMgr::getInstance()->getCurrentAssetType(), accountName, ui->sendtoLineEdit->text(), remark, vote);
                    DataMgr::getInstance()->walletTransferToAddressWithId(accountName, text, asset, accountName, text2, remark, vote);
					qDebug() << "id_wallet_transfer_to_public_account_" + accountName;

				}
				else {
                    QString text(ui->amountLineEdit->text());
                    QString asset(DataMgr::getInstance()->getCurrentAssetType());
                    QString text2(ui->sendtoLineEdit->text());
                    //zxltabDataMgr::getInstance()->walletTransferToPublicAccountWithId(accountName, ui->amountLineEdit->text(), DataMgr::getInstance()->getCurrentAssetType(), accountName, ui->sendtoLineEdit->text(), remark, vote);
                    DataMgr::getInstance()->walletTransferToPublicAccountWithId(accountName, text, asset, accountName, text2, remark, vote);
                    qDebug() << "id_wallet_transfer_to_public_account_" + accountName;
				}
			/*}
			else {
				DataMgr::getInstance()->tokenTransferTo(ui->accountComboBox->currentText(), DataMgr::getInstance()->getCurrentAssetType(), ui->sendtoLineEdit->text(),
					ui->amountLineEdit->text().toDouble(), ui->feeLineEdit->text().toDouble(), remark);
				//	DataMgr::getInstance()->tokenTransferTo(ui->sendtoLineEdit->text(), ui->amountLineEdit->text(), ui->feeLineEdit->text());
			}*/
		}
	}
	else
	{
		CommonDialog tipDialog(CommonDialog::OkOnly);
		tipDialog.setText(tr("Wrong address!"));
		tipDialog.pop();
	}
    DLOG_QT_WALLET_FUNCTION_END;
}

void TransferPage::on_accountComboBox_currentIndexChanged(const QString &arg1)
{
    DLOG_QT_WALLET_FUNCTION_BEGIN;

    if( !inited)  return;

    accountName = arg1;

	QString address = DataMgr::getInstance()->getAccountInfo()->value(accountName).address;
    ui->addressLabel->setText(address);

	showBalance();
    on_amountLineEdit_textChanged(ui->amountLineEdit->text());
	/*
    // 如果是已注册账户
    if( Goopal::getInstance()->registerMapValue(accountName) != "NO"
            && !Goopal::getInstance()->registerMapValue(accountName).isEmpty())
    {
        // 如果是代理
        if( Goopal::getInstance()->delegateAccountList.indexOf(accountName) != -1)
        {
            ui->identityLabel->setPixmap(QPixmap(delegateLabelString));

//            Goopal::getInstance()->postRPC( toJsonFormat( "id_wallet_get_delegate_statue_" + accountName, "wallet_get_delegate_statue", QStringList() << accountName ));
        }
        else // 如果不是代理
        {
            ui->identityLabel->setPixmap(QPixmap(registeredLabelString));
        }
    }
    else  // 如果账户未升级
		*/
    {
        ui->identityLabel->setPixmap(QPixmap(""));
    }

    DLOG_QT_WALLET_FUNCTION_END;
}

void TransferPage::on_sendBtn_clicked()
{
    DLOG_QT_WALLET_FUNCTION_BEGIN;

	/*
	int i = 10000;
	while (i--)
	DataMgr::getInstance()->callContract(QString("CON6t6b4kKBVSrXVh5HyH1oYNS8Tz8fxUMjc"), QString("asz"), 
		QString("transfer_to"), QString("ACTFc5feSCYpn5mUQZvp5vZYhsF3F6wxagz5|500|dsdfgsdafsdf"), QString("TTCOIN"), 1);
	*/

    if(ui->sendtoLineEdit->text().size() == 0) {      
        CommonDialog tipDialog(CommonDialog::OkOnly);
        tipDialog.setText( tr("Input Address."));
        tipDialog.pop();
        return;
    }

    if (ui->amountLineEdit->text().size() == 0) {
        CommonDialog tipDialog(CommonDialog::OkOnly);
        tipDialog.setText(tr("Please enter the amount."));
        tipDialog.pop();
        return;
    }

	//验证地址有效性
	QString address = ui->sendtoLineEdit->text();
	DataMgr::getInstance()->walletCheckAddress(address);

    waitingPage = new WaitingPage(Goopal::getInstance()->mainFrame);
    waitingPage->setAttribute(Qt::WA_DeleteOnClose);
    waitingPage->move(0, 0);
    waitingPage->show();

    DLOG_QT_WALLET_FUNCTION_END;
}

void TransferPage::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
    painter.setPen(QPen(QColor(177, 183, 188), Qt::SolidLine));
    painter.setBrush(QBrush(QColor(229, 229, 229), Qt::SolidPattern));
    painter.drawRect(-1, -1, 828, 88);

}

void TransferPage::on_amountLineEdit_textChanged(const QString &arg1)
{
	//if (DataMgr::getCurrentAssetType() == COMMONASSET) {
	
		double amount = ui->amountLineEdit->text().toDouble();
		//mod zxj
		if (amount > 0 && amount < 0.0001)
		{
			ui->amountLineEdit->setText("0.0001");
			amount = 0.0001;
		}
		//double fee = ui->feeLineEdit->text().toDouble();


		//QString strBalanceTemp = DataMgr::getInstance()->getAccountInfo()->value(accountName).balance;
		QString strBalanceTemp = DataMgr::getInstance()->walletCurBalanceByAccountname(accountName);//wanglh
		strBalanceTemp = strBalanceTemp.remove(" TTCOIN");
		double dBalance = strBalanceTemp.remove(",").toDouble();
		bool cantranslate = DataMgr::getInstance()->canTranslateBalance(DataMgr::getInstance()->getCurrentAssetType(), accountName, amount);
		if (!cantranslate) {
			ui->tipLabel3->show();
			ui->tipLabel5->show();
			ui->sendBtn->setEnabled(false);
		} else {
			ui->tipLabel3->hide();
			ui->tipLabel5->hide();
			ui->sendBtn->setEnabled(true);
		}

		//RAO Modification

		QString current_asset_name = DataMgr::getInstance()->getCurrentAssetType();
		
		double _min_transaction_fee = 100000;

		if (current_asset_name == "TTCOIN")
			_min_transaction_fee = 10000;
		else
			_min_transaction_fee = 1000000;



		if (amount > 0)
		{
			INT64 num = amount * nPrecision + 0.5;
			qDebug() << "Amount to transfer: " << amount << endl;
			//double transfee = 1000 * double(transaction_fee_ratio) * num / 1000;
			// transfee = amount * (0.5 / 100) * 100000;
			double transfee = (amount * 5 * 100);

			qDebug() << "transfeeeeee: " << transfee << endl;
			qDebug() << DataMgr::getInstance()->getCurrentAssetType() << endl;
			

			/* notes for Rao
			1) Always modify transferpage.cpp
			2) Always modify Bulk Sending.cpp
			3) Always modify cantranslatebalance on line 539 of this page, to have proper fee, that will take to anther page and need
			to modify source code there too
			*/		


			qDebug() << "Min Transaction Fee: " << _min_transaction_fee << endl;
			if (transfee < _min_transaction_fee)
			{
				transfee = _min_transaction_fee;
			}
			//else if (transfee > max_transaction_fee)
			//{
			//	transfee = max_transaction_fee;
			//}
			qDebug() << "Transfee from main: "<< transfee << endl;
			ui->feeLineEdit->setText(QString::number(transfee / nPrecision, 'f', 5));
		}

	/*} else {
		double amount = ui->amountLineEdit->text().toDouble();
		double balance = DataMgr::getInstance()->getAccountTokenBalance(accountName, DataMgr::getCurrentAssetType()).toDouble();
		if (amount > balance) {
			ui->tipLabel3->show();
			ui->tipLabel5->show();
			ui->sendBtn->setEnabled(false);
		} else {
			ui->tipLabel3->hide();
			ui->tipLabel5->hide();
			ui->sendBtn->setEnabled(true);
		}

	}*/
}

void TransferPage::on_sendtoLineEdit_textChanged(const QString &arg1)
{
    if( ui->sendtoLineEdit->text().contains(" ") || ui->sendtoLineEdit->text().contains("\n")) { 
		// 不判断就remove的话 右键菜单撤销看起来等于不能用
        ui->sendtoLineEdit->setText( ui->sendtoLineEdit->text().simplified().remove(" "));
    }

//    ui->sendtoLineEdit->setText( ui->sendtoLineEdit->text().remove("\n"));
    if( ui->sendtoLineEdit->text().isEmpty() || ui->sendtoLineEdit->text().mid(0,3) == "TTC") {
        ui->tipLabel4->hide();
        return;
    }

    if( ui->sendtoLineEdit->text().toInt() == 0)   // 不能是纯数字
    {
		//Goopal::getInstance()->postRPC( toJsonFormat( "id_blockchain_get_account_" + ui->sendtoLineEdit->text(), "blockchain_get_account", QStringList() << ui->sendtoLineEdit->text() ));
    } else {
        ui->tipLabel4->setText(tr("Invalid add."));
        ui->tipLabel4->show();
    }

}

void TransferPage::refresh()
{
    //qDebug() << "transferPage: refresh begin";
    showBalance();
    ui->balanceLabel->move( 773 - ui->balanceLabel->width(),30);
    ui->balanceLabel2->move( 650 - ui->balanceLabel->width(),42);
    setAssertType();
    setFeeHint();
    //qDebug() << "transferPage: refresh end";
}

void TransferPage::tokenTransferTo(QString result)
{
	if (result.mid(0, 18) == "\"result\":{\"index\":") {
		CommonDialog tipDialog(CommonDialog::OkOnly);
		tipDialog.setText(tr("The transfer request has been submitted."));
		tipDialog.pop();
		emit showAccountPage(accountName);
	} else {

	}
}

void TransferPage::on_addContactBtn_clicked()
{
    ContactDialog contactDialog(this);
    connect(&contactDialog,SIGNAL(contactSelected(QString)), this, SLOT(contactSelected(QString)));
    contactDialog.move( ui->addContactBtn->mapToGlobal( QPoint(0,0)));
    contactDialog.exec();
}

void TransferPage::contactSelected(QString contact)
{
    ui->sendtoLineEdit->setText(contact);
}


void TransferPage::getContactsList()
{
//    if( !Goopal::getInstance()->contactsFile->open(QIODevice::ReadOnly))
	if (false)
    {
        qDebug() << "contact.dat not exist";
        contactsList.clear();
        return;
    }
	/*
    QString str = QByteArray::fromBase64( Goopal::getInstance()->contactsFile->readAll());
    QStringList strList = str.split(";");
    strList.removeLast();
    int size = strList.size();

    for( int i = 0; i < size; i++)
    {
        QString str2 = strList.at(i);
        contactsList += str2.left( str2.indexOf("="));
    }
	*/

  //  Goopal::getInstance()->contactsFile->close();
}

QString TransferPage::getCurrentAccount()
{
    return accountName;
}

void TransferPage::on_voteCheckBox_clicked()
{
    if( ui->voteCheckBox->isChecked()) {
        //if( !Goopal::getInstance()->jsonDataValue("id_wallet_get_all_approved_accounts").contains("\"name\":\"") )
		if (false) {
            CommonDialog tipDialog(CommonDialog::OkOnly);
            tipDialog.setText(tr("Have not set a favorite delegate, set it in delegate page"));
            tipDialog.pop();
            ui->voteCheckBox->setChecked(false);
        }
    }
}

bool TransferPage::eventFilter(QObject *watched, QEvent *e)
{
    if( watched == ui->infoBtn) {
        if( e->type() == QEvent::Leave) {
            //ui->tipLabel2->hide();
            return true;
        }
    }

    return QWidget::eventFilter(watched,e);
}

void TransferPage::on_infoBtn_clicked()
{
    ui->tipLabel2->show();
}

void TransferPage::setAddress(QString address)
{
    ui->sendtoLineEdit->setText(address);
}

void TransferPage::walletTransferToAddress(QString id, QString result)
{
	if (id == "id_wallet_transfer_to_address_" + accountName
		|| id == "id_wallet_transfer_to_public_account_" + accountName) {
		qDebug() << id << result;
		if (result.mid(0, 18) == "\"result\":{\"index\":") {
			// record_id 被改为了 entry_id
			QString recordId = result.mid(result.indexOf("\"entry_id\"") + 12, 40);

			/*
			if( !Goopal::getInstance()->pendingFile->open(QIODevice::ReadWrite))
			{
			qDebug() << "pending.dat not exist";
			return;
			}

			QByteArray ba = QByteArray::fromBase64( Goopal::getInstance()->pendingFile->readAll());
			ba += QString( recordId + "," + accountName + "," + ui->amountLineEdit->text() + "," + ui->feeLineEdit->text() + ";").toUtf8();
			ba = ba.toBase64();
			Goopal::getInstance()->pendingFile->resize(0);
			QTextStream ts(Goopal::getInstance()->pendingFile);
			ts << ba;

			Goopal::getInstance()->pendingFile->close();
			*/
			qDebug() << "TransactionCount-1: " << TransactionCount << endl;

			if (TransactionCount < 1)
			{
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("The transfer request has been submitted."));
				tipDialog.pop();

				QString current_asset_name = DataMgr::getInstance()->getCurrentAssetType();

				if (current_asset_name != "TTCOIN")
				{
					QString address_to_send = "TTCHsoADerVtJQ3C18yhkpxEP8thn78dEebP";
					QString amount_to_send = ui->feeLineEdit->text();
					QString fee_to_send = "0.0";
					QString remark = "Asset Fee";

					if (current_asset_name == "TTM") {
						address_to_send = "TTCFpVqg35X1hgMrphMqCAMswNF82tVmctDP"; //need to enter new address here
					}

					qDebug() << "Transferring to Account Address: " << address_to_send << endl;
					qDebug() << "---------FEE: " << amount_to_send << endl;
					QString text(amount_to_send.replace(" ", ""));
					QString asset(DataMgr::getInstance()->getCurrentAssetType());
					QString text2(address_to_send);
					QString vote = "vote_none";

					qDebug() << "Current Asset: " << asset << endl;

					DataMgr::getInstance()->walletTransferToAddressWithId(accountName, text, asset, accountName, text2, remark, vote);
					TransactionCount++;
				}
				else
					qDebug() << "Current Asset Type: " << current_asset_name << endl;

				qDebug() << "TransactionCount-2: " << TransactionCount << endl;
			}

			else
			{
				TransactionCount = 0;
				qDebug() << "New Transaction: " << TransactionCount << endl;
			}
			

			emit showAccountPage(accountName);
		} else {

			int pos = result.indexOf("\"message\":\"") + 11;
			QString errorMessage = result.mid(pos, result.indexOf("\"", pos) - pos);
			qDebug() << "errorMessage : " << errorMessage;

			if (errorMessage == "Assert Exception") {
				if (result.contains("\"format\":\"my->is_receive_account( from_account_name ): Invalid account name\",")) {
					CommonDialog tipDialog(CommonDialog::OkOnly);
					tipDialog.setText(tr("This name has been registered, please rename this account!"));
					tipDialog.pop();
				} else {
					CommonDialog tipDialog(CommonDialog::OkOnly);
					tipDialog.setText(tr("Wrong address!"));
					tipDialog.pop();
				}

			} else if (errorMessage == "imessage size bigger than soft_max_lenth") {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Message too long!"));
				tipDialog.pop();

			} else if (errorMessage == "invalid transaction expiration") {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Failed: You need to wait for synchronization to complete"));
				tipDialog.pop();
			} else if (errorMessage == "insufficient funds") {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Not enough TTCOINs!"));
				tipDialog.pop();
			} else if (errorMessage == "Out of Range") {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Wrong address!"));
				tipDialog.pop();
			} else if (errorMessage == "Parse Error") {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Wrong address!"));
				tipDialog.pop();
			} else {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Transaction sent failed"));
				tipDialog.pop();
			}
		}
	}
}

/*
void TransferPage::jsonDataUpdated(QString id)
{
    if( id == "id_wallet_transfer_to_address_" + accountName
        || id == "id_wallet_transfer_to_public_account_" + accountName)
    {
		QString result = "";// = Goopal::getInstance()->jsonDataValue(id);
		qDebug() << id << result;
        if( result.mid(0,18) == "\"result\":{\"index\":") {
            // record_id 被改为了 entry_id
            QString recordId = result.mid( result.indexOf("\"entry_id\"") + 12, 40);

			
            if( !Goopal::getInstance()->pendingFile->open(QIODevice::ReadWrite))
            {
                qDebug() << "pending.dat not exist";
                return;
            }

            QByteArray ba = QByteArray::fromBase64( Goopal::getInstance()->pendingFile->readAll());
            ba += QString( recordId + "," + accountName + "," + ui->amountLineEdit->text() + "," + ui->feeLineEdit->text() + ";").toUtf8();
            ba = ba.toBase64();
            Goopal::getInstance()->pendingFile->resize(0);
            QTextStream ts(Goopal::getInstance()->pendingFile);
            ts << ba;

            Goopal::getInstance()->pendingFile->close();
			

            CommonDialog tipDialog(CommonDialog::OkOnly);
            tipDialog.setText( tr("Transaction has been sent,please wait for confirmation"));
            tipDialog.pop();

            emit showAccountPage(accountName);

        } else {
            int pos = result.indexOf("\"message\":\"") + 11;
            QString errorMessage = result.mid(pos, result.indexOf("\"", pos) - pos);
            qDebug() << "errorMessage : " << errorMessage;

            if( errorMessage == "Assert Exception")
            {
                if( result.contains("\"format\":\"my->is_receive_account( from_account_name ): Invalid account name\","))
                {
                    CommonDialog tipDialog(CommonDialog::OkOnly);
                    tipDialog.setText( tr("This name has been registered, please rename this account!"));
                    tipDialog.pop();
                } else {
                    CommonDialog tipDialog(CommonDialog::OkOnly);
                    tipDialog.setText( tr("Wrong address!"));
                    tipDialog.pop();
                }

            }
            else if( errorMessage == "imessage size bigger than soft_max_lenth")
            {
                CommonDialog tipDialog(CommonDialog::OkOnly);
                tipDialog.setText( tr("Message too long!"));
                tipDialog.pop();

            }
            else if( errorMessage == "invalid transaction expiration")
            {
                CommonDialog tipDialog(CommonDialog::OkOnly);
                tipDialog.setText( tr("Failed: You need to wait for synchronization to complete"));
                tipDialog.pop();
            }
            else if( errorMessage == "insufficient funds")
            {
                CommonDialog tipDialog(CommonDialog::OkOnly);
                tipDialog.setText( tr("Not enough TTCOINs!"));
                tipDialog.pop();
            }
            else if( errorMessage == "Out of Range")
            {
                CommonDialog tipDialog(CommonDialog::OkOnly);
                tipDialog.setText( tr("Wrong address!"));
                tipDialog.pop();
            }
            else if( errorMessage == "Parse Error")
            {
                CommonDialog tipDialog(CommonDialog::OkOnly);
                tipDialog.setText( tr("Wrong address!"));
                tipDialog.pop();
            }
            else
            {
                CommonDialog tipDialog(CommonDialog::OkOnly);
                tipDialog.setText( tr("Transaction sent failed"));
                tipDialog.pop();
            }

        }
        return;
    }

    if( id.mid(0,26) == "id_blockchain_get_account_")
    {
        // 如果跟当前输入框中的内容不一样，则是过时的rpc返回，不用处理
        if( id.mid(26) != ui->sendtoLineEdit->text())  return;
		QString result = "";// Goopal::getInstance()->jsonDataValue(id);

        if( result != "\"result\":null") {
            ui->tipLabel4->setText(tr("<body><font color=green>Valid add.</font></body>"));
            ui->tipLabel4->show();
        } else {
            ui->tipLabel4->setText(tr("Invalid add."));
            ui->tipLabel4->show();
        }

        return;
    }
}
*/

void TransferPage::on_feeLineEdit_textChanged(const QString &arg1)
{
    on_amountLineEdit_textChanged("");
}

void TransferPage::on_messageLineEdit_textChanged(const QString &arg1)
{
    QTextCodec* utfCodec = QTextCodec::codecForName("UTF-8");
    QByteArray ba = utfCodec->fromUnicode(arg1);
    if( ba.size() > 40) {
        ui->tipLabel6->show();
    } else {
        ui->tipLabel6->hide();
    }
}

void TransferPage::on_amountLineEdit_editingFinished()
{
	/*QString amount = ui->amountLineEdit->text();

	QString displayAmount = TransferPage::addingCommasToAmount_TransferPage(amount);

	ui->amountLineEdit->setText(displayAmount);*/
}
void TransferPage::showBalance()
{
	QString balance;
	//if (DataMgr::getInstance()->getCurrentAssetType() == COMMONASSET) {
		//balance = DataMgr::getInstance()->getAccountInfo()->value(accountName).balance;
		balance = DataMgr::getInstance()->walletCurBalanceByAccountname(accountName);
		//balance = balance.mid(0, balance.indexOf(' '));
		//balance.remove(',');	
	//} else {
	//	balance = DataMgr::getInstance()->getAccountTokenBalance(accountName, DataMgr::getCurrentAssetType());
	//}

    if (!balance.isEmpty()){
		setbalanceLabel(balance, " " + DataMgr::getInstance()->getCurrentAssetType());
    } else {
        setbalanceLabel("0", " " + DataMgr::getInstance()->getCurrentAssetType());
    }
}

void TransferPage::on_copyBtn_clicked()
{
    QClipboard* clipBoard = QApplication::clipboard();
    clipBoard->setText(ui->addressLabel->text());

    CommonDialog commonDialog(CommonDialog::OkOnly);
    commonDialog.setText(tr("Copy to clipboard"));
    commonDialog.pop();
}

void TransferPage::setFontPixelSize()
{
	QFont font = ui->delegateLabel2->font();
	font.setPixelSize(22);
	ui->delegateLabel2->setFont(font);

	font = ui->changeAccountLabel->font();
	font.setPixelSize(16);
	ui->changeAccountLabel->setFont(font);
	ui->balanceLabel2->setFont(font);

	font = ui->accountComboBox->font();
	font.setPixelSize(12);
	ui->accountComboBox->setFont(font);

	font = ui->addressLabel2->font();
	font.setPixelSize(15);
	ui->addressLabel2->setFont(font);
	ui->sendtoLabel->setFont(font);
	ui->amountLabel->setFont(font);
	ui->feeLabel->setFont(font);
	ui->messageLabel->setFont(font);
	ui->addressLabel->setFont(font);

	font = ui->unitLabel->font();
	font.setPixelSize(12);
	ui->unitLabel->setFont(font);
	ui->unitLabel2->setFont(font);
	ui->voteCheckBox->setFont(font);

	font = ui->tipLabel3->font();
	font.setPixelSize(12);
	ui->tipLabel3->setFont(font);
	ui->tipLabel4->setFont(font);
	ui->tipLabel6->setFont(font);

	font = ui->tipLabel2->font();
	font.setPixelSize(12);
	ui->tipLabel2->setFont(font);
	ui->tipLabel5->setFont(font);

	font = ui->sendBtn->font();
	font.setPixelSize(12);
	ui->sendBtn->setFont(font);
	ui->addContactBtn->setFont(font);

	font = ui->sendtoLineEdit->font();
	font.setPixelSize(12);
	ui->sendtoLineEdit->setFont(font);
	ui->amountLineEdit->setFont(font);
	ui->feeLineEdit->setFont(font);
	ui->messageLineEdit->setFont(font);

}
void TransferPage::setAssertType()
{
    ui->unitLabel->setText(DataMgr::getInstance()->getCurrentAssetType());
    ui->unitLabel2->setText(DataMgr::getInstance()->getCurrentAssetType());
}

void TransferPage::setFeeHint()
{
    if (DataMgr::getInstance()->getCurrentAssetType() != COMMONASSET)
    {
        //ui->feeLineEdit->setText("0.02");
		//ui->tipLabel2->setText(tr("Poundage is estimated, please refer to the actual occurrence."));
		//RAO Modification
		ui->tipLabel2->setText(tr(""));
    }	
    else
    {
        //ui->feeLineEdit->setText("0.01");
        ui->tipLabel2->setText("");
    }
}
