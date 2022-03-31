#ifdef USE_CLOUD_DB
#include "CloudDB\CloudDB.h"
#endif

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

#include "backupdialog.h"
#include "ui_backupdialog.h"
#include "debug_log.h"
#include "gop_common_define.h"
#include "contactdialog.h"
#include "remarkdialog.h"
#include "commondialog.h"
#include "transferconfirmdialog.h"
#include "waitingpage.h"
#include "goopal.h"


BackUpDialog::BackUpDialog(QWidget *parent) :
    QWidget(parent),
	waitingPage(nullptr),
#ifdef USE_CLOUD_DB
	cloudDB(NULL),
#endif
	ui(new Ui::BackUpDialog)
{
    DLOG_QT_WALLET_FUNCTION_BEGIN;

    ui->setupUi(this);
	   setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255));
    setPalette(palette);

	
#ifndef USE_CLOUD_DB
	ui->label_walletLocation_3->hide();
	ui->checkBox_cloudBackup->hide();
	ui->label_walletLocation_4->hide();
	ui->checkBox_cloudRestore->hide();

#endif

	ui->lineEdit_walletName->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
	ui->lineEdit_walletName->setTextMargins(8, 0, 0, 0);
	QRegExp regx("[a-zA-Z0-9\-\.\ \n]+$");
	QRegExpValidator *pReg1 = new QRegExpValidator(regx, this);
	ui->lineEdit_walletName->setValidator(pReg1);
	ui->lineEdit_walletName->setAttribute(Qt::WA_InputMethodEnabled, false);

	ui->lineEdit_saveLocation->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
	ui->lineEdit_saveLocation->setTextMargins(8, 0, 0, 0);
	ui->lineEdit_saveLocation->setAttribute(Qt::WA_InputMethodEnabled, false);

	ui->lineEdit_fileName->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
	ui->lineEdit_fileName->setTextMargins(8, 0, 0, 0);
	ui->lineEdit_fileName->setAttribute(Qt::WA_InputMethodEnabled, false);

	ui->lineEdit_walletLocation->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
	ui->lineEdit_walletLocation->setTextMargins(8, 0, 0, 0);
	ui->lineEdit_walletLocation->setAttribute(Qt::WA_InputMethodEnabled, false);

	//ui->lineEdit_saveWallet_bkp->setEnabled(false);
	//ui->pushButton_saveWallet_bkp->hide();


	ui->pushButton_saveLocation->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");
	ui->pushButton_saveLocation->setText("Open");

	ui->pushButton_walletLocation->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");
	ui->pushButton_walletLocation->setText("Open");

	ui->pushButton_backup->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");
	ui->pushButton_backup->setText("Backup");
	ui->label_tipWalletName->hide();
	//ui->pushButton_saveLocation->setToolTip()

	ui->lineEdit_walletLocation->setText(BackUpDialog::getSystemEnvironmentPath());
	ui->lineEdit_walletName->setToolTip("JSON File Name");
	ui->lineEdit_fileName->setToolTip(".rar File Name");
	ui->lineEdit_saveLocation->setText(QCoreApplication::applicationDirPath());


	//restore
	ui->lineEdit_walletName_bkp->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
	ui->lineEdit_walletName_bkp->setTextMargins(8, 0, 0, 0);

	ui->lineEdit_walletName_bkp->setValidator(pReg1);
	ui->lineEdit_walletName_bkp->setAttribute(Qt::WA_InputMethodEnabled, false);

	ui->lineEdit_loadFile_bkp->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
	ui->lineEdit_loadFile_bkp->setTextMargins(8, 0, 0, 0);
	ui->lineEdit_loadFile_bkp->setAttribute(Qt::WA_InputMethodEnabled, false);

	ui->lineEdit_passWord_bkp->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
	ui->lineEdit_passWord_bkp->setTextMargins(8, 0, 0, 0);
	ui->lineEdit_passWord_bkp->setAttribute(Qt::WA_InputMethodEnabled, false);

	ui->lineEdit_saveWallet_bkp->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
	ui->lineEdit_saveWallet_bkp->setTextMargins(8, 0, 0, 0);
	ui->lineEdit_saveWallet_bkp->setAttribute(Qt::WA_InputMethodEnabled, false);


	ui->pushButton_loadFile_bkp->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");
	ui->pushButton_loadFile_bkp->setText("Open");

	ui->pushButton_saveWallet_bkp->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");
	ui->pushButton_saveWallet_bkp->setText("Open");

	ui->pushButton_restore->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");
	ui->pushButton_restore->setText("Restore");

	ui->checkBox_cloudBackup->setStyleSheet(QString("QCheckBox::indicator{ image:url(%1pic2/checkBox_unchecked.png); }"
		"QCheckBox::indicator:checked{ image:url(%2pic2/checkBox_checked.png); }").arg(QCoreApplication::applicationDirPath() + "/").arg(QCoreApplication::applicationDirPath() + "/"));

	ui->checkBox_cloudRestore->setStyleSheet(QString("QCheckBox::indicator{ image:url(%1pic2/checkBox_unchecked.png); }"
		"QCheckBox::indicator:checked{ image:url(%2pic2/checkBox_checked.png); }").arg(QCoreApplication::applicationDirPath() + "/").arg(QCoreApplication::applicationDirPath() + "/"));

	setFontPixelSize_backUp();
	ui->label_tipWalletName_bkp->hide();
	//ui->pushButton_saveLocation->setToolTip()

	ui->lineEdit_saveWallet_bkp->setText(BackUpDialog::getSystemEnvironmentPath() + "\\wallets\\");
	ui->lineEdit_walletName_bkp->setToolTip("Wallet Name");
	ui->lineEdit_passWord_bkp->setToolTip("Private Key");
	//ui->lineEdit_loadFile_bkp->setText(QCoreApplication::applicationDirPath());


	connect(DataMgr::getInstance(), &DataMgr::onWalletBackupCreate, this, &BackUpDialog::BackupCreated);
	connect(DataMgr::getInstance(), &DataMgr::onWalletBackupRestore, this, &BackUpDialog::BackupRestored);

    DLOG_QT_WALLET_FUNCTION_END;
}

QString BackUpDialog::getSystemEnvironmentPath() {
	QStringList environment = QProcess::systemEnvironment();
	QString str;

#ifdef WIN32
	foreach(str, environment) {
		if (str.startsWith("APPDATA=")) {
			_app_data_path = str.mid(8) + "\\TTCOIN_V2";
			_tool_config_path = str.mid(8) + "\\TTCOINDevelopmentTool_V2";
			qDebug() << "appDataPath:" << _app_data_path;
			break;
		}
	}

	return _app_data_path;
#elif defined(TARGET_OS_MAC)
	foreach(str, environment) {
		if (str.startsWith("HOME=")) {
			_app_data_path = str.mid(5) + "/Library/Application Support/TTCHAIN";
			_tool_config_path = str.mid(5) + "/Library/Application Support/TTCHAINDevelopmentTool";
			qDebug() << "appDataPath:" << _app_data_path;
			break;
		}
	}
	_app_work_path = QCoreApplication::applicationDirPath() + "/";
	qDebug() << "_app_work_path : " << _app_work_path;
	//QMessageBox::information(NULL, "Title", _app_work_path, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
#else
	foreach(str, environment) {
		if (str.startsWith("HOME=")) {
			_app_data_path = str.mid(5) + "/TTCOIN";
			_tool_config_path = str.mid(5) + "/TTCOINDevelopmentTool";
			qDebug() << "appDataPath:" << _app_data_path;
			break;
		}
	}
#endif
}
void BackUpDialog::on_lineEdit_walletName_textChanged(const QString &arg1)
{
	if (ui->lineEdit_walletName->text().contains(" ") || ui->lineEdit_walletName->text().contains("\n")) {
		ui->lineEdit_walletName->setText(ui->lineEdit_walletName->text().simplified().remove(" "));
	}
	if (ui->lineEdit_walletName->text().isEmpty() || ui->lineEdit_walletName->text().mid(0, 3) == "TTC") {
		ui->label_tipWalletName->hide();
		return;
	}

	if (ui->lineEdit_walletName->text().toInt() == 0)   
	{
		//Goopal::getInstance()->postRPC( toJsonFormat( "id_blockchain_get_account_" + ui->sendtoLineEdit->text(), "blockchain_get_account", QStringList() << ui->sendtoLineEdit->text() ));
	}
	else {
		ui->label_tipWalletName->setText(tr("Invalid Name"));
		ui->label_tipWalletName->show();
	}

}

void BackUpDialog::on_lineEdit_walletName_bkp_textChanged(const QString &arg1)
{
	if (ui->lineEdit_walletName_bkp->text().contains(" ") || ui->lineEdit_walletName_bkp->text().contains("\n")) {
		ui->lineEdit_walletName_bkp->setText(ui->lineEdit_walletName_bkp->text().simplified().remove(" "));
	}

	if (ui->lineEdit_walletName_bkp->text().isEmpty() || ui->lineEdit_walletName_bkp->text().mid(0, 3) == "TTC") {
		ui->label_tipWalletName_bkp->hide();
		return;
	}

	if (ui->lineEdit_walletName->text().toInt() == 0)   
	{
		//Goopal::getInstance()->postRPC( toJsonFormat( "id_blockchain_get_account_" + ui->sendtoLineEdit->text(), "blockchain_get_account", QStringList() << ui->sendtoLineEdit->text() ));
	}
	else {
		ui->label_tipWalletName_bkp->setText(tr("Invalid Name"));
		ui->label_tipWalletName_bkp->show();
	}

	ui->lineEdit_saveWallet_bkp->setText(BackUpDialog::getSystemEnvironmentPath() + "\\wallets\\" + ui->lineEdit_walletName_bkp->text());

}

void BackUpDialog::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setPen(QPen(QColor(177, 183, 188), Qt::SolidLine));
	painter.setBrush(QBrush(QColor(229, 229, 229), Qt::SolidPattern));
	painter.drawRect(-1, -1, 828, 88);

}
void BackUpDialog::on_pushButton_saveLocation_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	//qDebug() << "Directory: " << dir;

	if (dir.isEmpty())
	{
		ui->lineEdit_saveWallet_bkp->setText(BackUpDialog::getSystemEnvironmentPath() + "\\wallets\\");
	}
	else ui->lineEdit_saveLocation->setText(dir);
}

void BackUpDialog::on_pushButton_walletLocation_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	//qDebug() << "Directory: " << dir;
	if (dir.isEmpty())
	{
		ui->lineEdit_walletLocation->setText(BackUpDialog::getSystemEnvironmentPath());
	}
	else ui->lineEdit_walletLocation->setText(dir);
	
}

void BackUpDialog::on_pushButton_loadFile_bkp_clicked()
{

	QFileDialog dialog(this);
	dialog.setDirectory(QDir::homePath());
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(trUtf8(("JSON Files(*.json*)")));
	dialog.exec();
	if (dialog.selectedFiles().size() != 0)
	{
		ui->lineEdit_loadFile_bkp->setText(dialog.selectedFiles().at(0));
	}
}

void BackUpDialog::on_pushButton_saveWallet_bkp_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	//qDebug() << "Directory: " << dir;

	ui->lineEdit_saveWallet_bkp->setText(dir);
}



void BackUpDialog::setFontPixelSize_backUp()
{
	QFont font = ui->label_walletBackUp->font();
	font.setPixelSize(22);
	ui->label_walletBackUp->setFont(font);

	font = ui->label_walletName->font();
	font.setPixelSize(15);
	ui->label_walletName->setFont(font);
	ui->label_saveLocation->setFont(font);
	ui->label_fileName->setFont(font);
	ui->label_walletLocation->setFont(font);


	font = ui->pushButton_saveLocation->font();
	font.setPixelSize(12);
	ui->pushButton_saveLocation->setFont(font);
	ui->pushButton_walletLocation->setFont(font);
	//ui->pushButton_backup->setFont(font);

	font = ui->label_tipWalletName->font();
	font.setPixelSize(12);
	ui->label_tipWalletName->setFont(font);


	font = ui->lineEdit_walletName->font();
	font.setPixelSize(12);
	ui->lineEdit_walletName->setFont(font);
	ui->lineEdit_saveLocation->setFont(font);
	ui->lineEdit_fileName->setFont(font);
	ui->lineEdit_walletLocation->setFont(font);

}

BackUpDialog::~BackUpDialog()
{
	DLOG_QT_WALLET_FUNCTION_BEGIN;

	if (waitingPage != nullptr)
	{
		delete waitingPage;
		waitingPage = nullptr;
	}

	delete ui;

	DLOG_QT_WALLET_FUNCTION_END;
}


void BackUpDialog::on_pushButton_backup_clicked()
{
	DLOG_QT_WALLET_FUNCTION_BEGIN;
	CommonDialog commonDialog(CommonDialog::OkOnly);

	if (ui->lineEdit_walletName->text().isEmpty())
	{
		commonDialog.setText(tr("Please Input Backup File Name!"));
		bool choice = commonDialog.pop();

		if (choice) {
			commonDialog.close();
		}
		else {
			return;
		}
	}
	else
	{

		waitingPage = new WaitingPage(Goopal::getInstance()->mainFrame);
		waitingPage->setAttribute(Qt::WA_DeleteOnClose);
		waitingPage->move(0, 0);
		waitingPage->show();

		BackUpDialog::filePath = ui->lineEdit_saveLocation->text() + "\\" + ui->lineEdit_walletName->text() + ".json";
		qDebug() << "File to be Saved: " << filePath;
		DataMgr::getInstance()->walletBackupCreate(filePath);
	}

	DLOG_QT_WALLET_FUNCTION_END;
}

void BackUpDialog::on_pushButton_restore_clicked()
{
	DLOG_QT_WALLET_FUNCTION_BEGIN;
	CommonDialog commonDialog(CommonDialog::OkOnly);


	if (ui->lineEdit_walletName_bkp->text().isEmpty() || ui->lineEdit_passWord_bkp->text().isEmpty() )
	{
		
		commonDialog.setText(tr("Please Input restore File Name!"));
		bool choice = commonDialog.pop();

		if (choice) {
			commonDialog.close();
		}
		else {
			return;
		}
	}
	else
	{
		if (!ui->lineEdit_loadFile_bkp->text().isEmpty())
		{
			QString walletNameLocation = BackUpDialog::getSystemEnvironmentPath() + "\\wallets\\" + ui->lineEdit_walletName_bkp->text();
			QDir dir(walletNameLocation);

			if (!dir.exists())
			{
				if (walletNameLocation == "localwallet")
				{
					commonDialog.setText(tr("You can' use wallet as restored name. wallet is default name of the wallet used by the TTCOINWallet.\n Please use another name. "));
					bool choice = commonDialog.pop();

					if (choice) {
						commonDialog.close();
					}
					else {
						return;
					}

				}
				else
				{
					waitingPage = new WaitingPage(Goopal::getInstance()->mainFrame);
					waitingPage->setAttribute(Qt::WA_DeleteOnClose);
					waitingPage->move(0, 0);
					waitingPage->show();

					BackUpDialog::filePath = ui->lineEdit_loadFile_bkp->text();
					QString walletName = ui->lineEdit_walletName_bkp->text();
					QString passWord = ui->lineEdit_passWord_bkp->text();
					qDebug() << "File to be Saved: " << filePath;
					DataMgr::getInstance()->walletBackupRestore(filePath, walletName, passWord);
				}
			}
			else{
				commonDialog.setText(tr("Wallet Name Already Exists!"));
				bool choice = commonDialog.pop();

				if (choice) {
					commonDialog.close();
				}
				else {
					return;
				}
			}
		}
		else{
			commonDialog.setText(tr("Please Load Wallet BackUp JSON File."));
			bool choice = commonDialog.pop();

			if (choice) {
				commonDialog.close();
			}
			else {
				return;
			}

		}
	}

	DLOG_QT_WALLET_FUNCTION_END;
}

void BackUpDialog::BackupCreated(QString result)
{
	DLOG_QT_WALLET_FUNCTION_BEGIN;
	CommonDialog commonDialog(CommonDialog::OkOnly);

#ifdef USE_CLOUD_DB
	if (ui->checkBox_cloudBackup->isChecked())
	{
		cloudDB = new CrystalDB();
		cloudDB->DefineDatabases("ttcoinwallet", "walletbackup");
		cloudDB->ConnectToCloudServer();
		cloudDB->CloudDBInfo();
		QString walletFileName = ui->lineEdit_walletName->text();
		QString username = "rao";
		QString filepath = ui->lineEdit_saveLocation->text();
		cloudDB->InsertSingleDocCloud(walletFileName.toStdString().c_str(),username.toStdString().c_str(),filePath.toStdString().c_str());
		cloudDB->DisConnectCloudServer();
		cloudDB->CleanMongod();
	}
#endif
	if (waitingPage != nullptr)
	{
		delete waitingPage;
		waitingPage = nullptr;
	}

	qDebug() << "Result: " << result;
	result = result.mid(result.indexOf(":") + 1, result.length());

	if (result == "null")
	{
		commonDialog.setText(tr("BackUp File Created!"));
		bool choice = commonDialog.pop();

		if (choice) {
			commonDialog.close();
		}
		else {
			return;
		}
	}
	else{
		commonDialog.setText(tr("Wallet Not backed Up."));
		bool choice = commonDialog.pop();

		if (choice) {
			commonDialog.close();
		}
		else {
			return;
		}

	}
	DLOG_QT_WALLET_FUNCTION_END;
}

void BackUpDialog::BackupRestored(QString result)
{
	CommonDialog commonDialog(CommonDialog::OkOnly);
	qDebug() << "Result: " << result;
	DLOG_QT_WALLET_FUNCTION_BEGIN;
	if (waitingPage != nullptr)
	{
		delete waitingPage;
		waitingPage = nullptr;
	}

	QString result_new;
	result_new = result.mid(result.indexOf(":") + 1, result.length());
	qDebug() << "Result New: " << result;

	if (result_new == "null")
	{
		commonDialog.setText(tr("BackUp File Restored!"));
		bool choice = commonDialog.pop();

		if (choice) {
			commonDialog.close();
		}
		else {
			return;
		}
	}
	else
	{

		if (result.contains("invalid_password"))
		{
			commonDialog.setText(tr("Passwrod Incorrect. Please Input Correct Password!"));
			bool choice = commonDialog.pop();

			if (choice) {
				commonDialog.close();
			}
			else {
				return;
			}

		}
		else{
			commonDialog.setText(tr("Wallet Not Restored!"));
			bool choice = commonDialog.pop();

			if (choice) {
				commonDialog.close();
			}
			else {
				return;
			}

		}
	}
	DLOG_QT_WALLET_FUNCTION_END;
}
