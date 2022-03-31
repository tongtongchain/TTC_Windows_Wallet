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

#include "BulkSending.h"
#include "ui_BulkSending.h"
#include "debug_log.h"
#include "gop_common_define.h"
#include "contactdialog.h"
#include "remarkdialog.h"
#include "commondialog.h"
#include "transferconfirmdialog.h"
#include "waitingpage.h"
#include "goopal.h"

#define TRANSACTION_CONFIGURATION_FILENAME      "transaction_config.json"
#define ALP_DEFAULT_MIN_TRANSACTION_FEE                     1 // 设置最低手续费为0.00001
//#define ALP_DEFAULT_MAX_TRANSACTION_FEE                     30000000 // 设置最高手续费为1000
#define ALP_DEFAULT_MAX_TRANSACTION_FEE                     300000 // 设置最高手续费为1000
#define ALP_DEFAULT_TRANSACTION_FEE_RATIO					double(0.00300) //手续费比例0.003

BulkSending::BulkSending(QString name, QWidget *parent) :
    QWidget(parent),
	accountName(name),
	inited(false),
    waitingPage(nullptr),
    ui(new Ui::BulkSending)
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

        delegateLabelString = QCoreApplication::applicationDirPath() + "/pic2/delegateLabel_En.png";
        registeredLabelString = QCoreApplication::applicationDirPath() + "/pic2/registeredLabel_En.png";
    }
	else  if (language.isEmpty() || language == "Korean")
	{
		delegateLabelString = QCoreApplication::applicationDirPath() + "/pic2/delegateLabel_Ko.png";
		registeredLabelString = QCoreApplication::applicationDirPath() + "/pic2/registeredLabel_Ko.png";
	}
    else
    {

        delegateLabelString = QCoreApplication::applicationDirPath() + "/pic2/delegateLabel.png";
        registeredLabelString = QCoreApplication::applicationDirPath() + "/pic2/registeredLabel.png";
    }

    if( accountName.isEmpty())  // 如果是点击账单跳转
    {
        if( DataMgr::getInstance()->getAccountInfo()->size() > 0) {
            accountName = DataMgr::getInstance()->getAccountInfo()->keys().at(0);
        } else {
            emit back();    // 跳转在functionbar  这里并没有用
            return;
        }
    }
	BulkSending::insertingAccountsComboBox();

	ui->tipLabel3->hide();

	assetFeeAmount = "0.0";
	TransactionCount = 0;




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

 
    //getContactsList();


	inited = true;


	//setAssertType();//设置资产类型

	connect(DataMgr::getInstance(), &DataMgr::onWalletTransferToAddressWithId, this, &BulkSending::walletTransferToAddress);
	connect(DataMgr::getDataMgr(), &DataMgr::onCallContract, this, &BulkSending::tokenTransferTo);
	connect(DataMgr::getInstance(), &DataMgr::onWalletCheckAddress, this, &BulkSending::addressChecked);
	connect(this, &BulkSending::signal_validationFinished, this, &BulkSending::slot_validationFinished);
	connect(this, &BulkSending::signal_progressStarted, this, &BulkSending::slot_calculatingProgressValue);
	connect(this, &BulkSending::signal_progressStopped, this, &BulkSending::slot_stoppingProgressValue);
	connect(this, &BulkSending::signal_validationStarted, this, &BulkSending::slot_startedValidating);

	setFontPixelSize();

	nPrecision = DataMgr::getInstance()->getMainAssetPrecision();
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

	ui->pushButton_loadCSVFile->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");


	//ui->lineEdit_totalAmount->setStyleSheet("color:black;border:1px solid #CCCCCC;border-radius:3px;");
	//ui->lineEdit_totalAmount->setReadOnly(true);
	ui->label_Amount->hide();
	ui->label_total->hide();
	ui->label_Fee->hide();

	ui->tableWidget_bulkSending->setSelectionMode(QAbstractItemView::NoSelection);
	ui->tableWidget_bulkSending->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableWidget_bulkSending->setFocusPolicy(Qt::NoFocus);
	ui->tableWidget_bulkSending->setColumnWidth(0, 40);
	ui->tableWidget_bulkSending->setColumnWidth(1, 280);
	ui->tableWidget_bulkSending->setColumnWidth(2, 100);
	ui->tableWidget_bulkSending->setColumnWidth(3, 100);
	ui->tableWidget_bulkSending->setColumnWidth(4, 60);
	ui->tableWidget_bulkSending->setColumnWidth(5, 200);
	ui->tableWidget_bulkSending->setShowGrid(false);
	ui->tableWidget_bulkSending->setStyleSheet("QTableView::item:!selected {border-bottom: 1px dashed gray;} QTableView::item:selected {background-color:rgb(255, 160, 122);}");
	ui->tableWidget_bulkSending->setFrameShape(QFrame::NoFrame);
	ui->tableWidget_bulkSending->setMouseTracking(true);
	//ui->tableWidget_bulkSending->setGeometry(39, 190, 770, 5 * 57);

	ui->bulkSending_sendBtn->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");
	ui->bulkSending_sendBtn->setText(tr("Transfer"));
	ui->bulkSending_sendBtn->setEnabled(false);
	ui->bulkSending_exportCSV->setEnabled(false);

	ui->bulkSending_exportCSV->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");
	ui->bulkSending_exportCSV->setText(tr("Export CSV"));
	ui->bulkSending_exportCSV->hide();

	ui->pushButton_ValidateAddresses->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");
	ui->pushButton_ValidateAddresses->setEnabled(false);


    DLOG_QT_WALLET_FUNCTION_END;
}

void BulkSending::insertingAccountsComboBox()
{
	QStringList keys = DataMgr::getInstance()->getAccountInfo()->keys();

	QStringList keys_modified;
	keys_modified.append("");
	for (int i = 0; i < keys.size(); i++)
	{
		keys_modified.append(keys[i]);
	}
	ui->accountComboBox->addItems(keys_modified);

	/*if( accountName.isEmpty() ) {
	ui->accountComboBox->setCurrentIndex(0);
	} else {
	ui->accountComboBox->setCurrentText( accountName);
	}*/

	//QString address = DataMgr::getInstance()->getAccountInfo()->value(accountName).address;
	ui->addressLabel->setText(tr("Please select the account from scroll down menu."));
	showBalance();

}
void BulkSending::on_pushButton_loadCSVFile_clicked()
{

#ifdef ENABLE_MULTIPLE_FILES_SUPPORT
	QFileDialog dialog(this);
	dialog.setDirectory(QDir::homePath());


	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(trUtf8(("csv Files(*.csv*)")));

	if (dialog.exec())
	{
		BulkSending::csvFileList = dialog.selectedFile();
	}

	if (BulkSending::csvFileList.count() > 0)
	{
		totalRowsCount = 0;
		qDebug() << "FileCount: " << totalRowsCount;


		ui->label_totalFiles->setText(QString::number(BulkSending::csvFileList.count()) + " Files");

		for (int i = 0; i < BulkSending::csvFileList.count(); i++)
		{
			ParsingFiles(BulkSending::csvFileList[i]);
			totalRowsCount = totalRowsCount + model->rowCount();
			qDebug() << "FileCount New: " << totalRowsCount << endl;

		}
		}
#else
	    fileName = QFileDialog::getOpenFileName(this, "Open CSV file",
		QDir::currentPath(), "CSV (*.csv)");

	ParsingFiles(fileName);
#endif

}

void BulkSending::ParsingFiles(QString fileName)
{
	//qDebug() << "Parsing Filename: " << fileName;
	model = new QStandardItemModel(this);
	model->clear();
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly)) {
		
		QString data = file.readAll();
		
		data.remove(QRegExp("\r")); //remove all ocurrences of CR (Carriage Return)
		QString temp;
		QChar character;
		QTextStream textStream(&data);


		while (!textStream.atEnd()) {
			textStream >> character;
			//qDebug() << "Char: " << QString(character) << endl;
			if (character == ',') {
				checkString(temp, character);
			}
			else if (character == '\n') {
				checkString(temp, character);
			}
			else if (textStream.atEnd()) {
				temp.append(character);
				checkString(temp);
			}
			else {
				temp.append(character);
			}
		}
	}
	if (!fileName.isEmpty())
		DisplayingValuesOnTableWidget();
}

void BulkSending::checkString(QString &temp, QChar character)
{

	if (temp.count("\"") % 2 == 0) {
		//if (temp.size() == 0 && character != ',') //problem with line endings
		//    return;
		if (temp.startsWith(QChar('\"')) && temp.endsWith(QChar('\"'))) {
			temp.remove(QRegExp("^\""));
			temp.remove(QRegExp("\"$"));
		}
		//FIXME: will possibly fail if there are 4 or more reapeating double quotes
		temp.replace("\"\"", "\"");
		QStandardItem *item = new QStandardItem(temp);
		standardItemList.append(item);
		if (character != QChar(',')) {
			model->appendRow(standardItemList);
			standardItemList.clear();
		}
		temp.clear();
	}
	else {
		temp.append(character);
	}

}

void BulkSending::DisplayingValuesOnTableWidget(bool status)
{
	ui->tableWidget_bulkSending->clear();

	if (model->columnCount() > 2)
	{
		totalAmountToSend = 0.0;
		totalFee = 0.0;
		int tablewidget_row_count = 0;
		for (int i = 1; i < model->rowCount(); i++) {

			if (model->item(i, 0)->text().size() > 10 && model->item(i, 1)->text().size() > 0)
			{
				//qDebug() << "MODEL: " << model->item(i, 0)->text() << endl;
				tablewidget_row_count = tablewidget_row_count + 1;
			}
		}
		ui->tableWidget_bulkSending->setRowCount(tablewidget_row_count);
		ui->initLabel->hide();
		//qDebug() << "Row COunt: " << model->rowCount() << endl;
		


#ifdef ENABLE_MULTIPLE_FILES_SUPPORT

		for (int i = 1; i < model->rowCount(); i++) {

			ui->tableWidget_bulkSending->setRowHeight(i, 50);

			QTableWidgetItem* item_sno;
			item_sno = ui->tableWidget_bulkSending->item(i + totalRowsCount, 0);
			if (item_sno != NULL)
				delete item_sno;
			item_sno = new QTableWidgetItem(QString::number(i + totalRowsCount));
			ui->tableWidget_bulkSending->setItem(i, 0, item_sno);

			QTableWidgetItem* item;
			item = ui->tableWidget_bulkSending->item(i + totalRowsCount, 1);
			if (item != NULL)
				delete item;
			item = new QTableWidgetItem(model->item(i,0)->text());
			ui->tableWidget_bulkSending->setItem(i + totalRowsCount, 1, item);

			QTableWidgetItem* item_acnt;
			item_acnt = ui->tableWidget_bulkSending->item(i + totalRowsCount, 2);
			if (item_acnt != NULL)
				delete item_acnt;
			item_acnt = new QTableWidgetItem(model->item(i, 1)->text());
			ui->tableWidget_bulkSending->setItem(i + totalRowsCount, 2, item_acnt);

			QTableWidgetItem* item_dpt;
			item_dpt = ui->tableWidget_bulkSending->item(i + totalRowsCount, 3);
			if (item_dpt != NULL)
				delete item_dpt;
			item_dpt = new QTableWidgetItem(model->item(i, 2)->text());
			ui->tableWidget_bulkSending->setItem(i, 3, item_dpt);

			qDebug() << "name: " << model->item(i + totalRowsCount, 0)->text();
		}
#else
		double _min_transaction_fee = 10000;

		int i= 1; //just in case if there is empty rows and columns inside the csv file, then this 'i' will be used as index

		for (int row_index = 1; row_index < model->rowCount(); row_index++) {
			//qDebug() << "Row Index: " << row_index << endl;
			//qDebug() << "I: " << i << endl;

			if (model->item(row_index, 0)->text().size() > 10 && model->item(row_index, 1)->text().size() > 0 && i <tablewidget_row_count+1)
			{
				ui->tableWidget_bulkSending->setRowHeight(i - 1, 50);

				//SNO
				QTableWidgetItem* item_sno;
				item_sno = ui->tableWidget_bulkSending->item(i - 1, 0);
				if (item_sno != NULL)
					delete item_sno;
				item_sno = new QTableWidgetItem(QString::number(i));
				ui->tableWidget_bulkSending->setItem(i - 1, 0, item_sno);

				//Account
				QTableWidgetItem* item_acnt;
				item_acnt = ui->tableWidget_bulkSending->item(i - 1, 1);
				if (item_acnt != NULL)
					delete item_acnt;
				item_acnt = new QTableWidgetItem(model->item(row_index, 0)->text());
				ui->tableWidget_bulkSending->setItem(i - 1, 1, item_acnt);

				//Amount
				QTableWidgetItem* item_amount;
				item_amount = ui->tableWidget_bulkSending->item(i - 1, 2);
				if (item_amount != NULL)
					delete item_amount;
				item_amount = new QTableWidgetItem(model->item(row_index, 1)->text().replace(",",""));
				ui->tableWidget_bulkSending->setItem(i - 1, 2, item_amount);


				double amount = model->item(row_index, 1)->text().replace(",","").toDouble();
				totalAmountToSend = totalAmountToSend + amount;
				QString strBalanceTemp = DataMgr::getInstance()->walletCurBalanceByAccountname(accountName);//wanglh
				strBalanceTemp = strBalanceTemp.remove(" TTCOIN");
				double dBalance = strBalanceTemp.remove(",").toDouble();
				bool cantranslate = DataMgr::getInstance()->canTranslateBalance(DataMgr::getInstance()->getCurrentAssetType(), accountName, amount);
				//cantranslate = status;
				if (!cantranslate) {
					ui->tableWidget_bulkSending->item(i - 1, 2)->setBackground(Qt::red);
					ui->tableWidget_bulkSending->item(i - 1, 2)->setForeground(Qt::red);
					ui->bulkSending_sendBtn->setEnabled(false);
				}
				else {
					//ui->tipLabel3->hide();
					//ui->bulkSending_sendBtn->setEnabled(true);
				}
				
				//FEE
				QTableWidgetItem* item_fee;
				item_fee = ui->tableWidget_bulkSending->item(i - 1, 3);
				if (item_fee != NULL)
					delete item_fee;

				double _min_transaction_fee = 10000;
					
				if (amount > 0)
				{
					//RAO Modifications
					double transfee = (amount * 5 * 100);

					qDebug() << "transfeeeeee: " << transfee << endl;
					qDebug() << DataMgr::getInstance()->getCurrentAssetType() << endl;
					QString current_asset_name = DataMgr::getInstance()->getCurrentAssetType();

					qDebug() << "Min Transaction Fee: " << _min_transaction_fee << endl;
					if (transfee < _min_transaction_fee)
					{
						transfee = _min_transaction_fee;
					}


					item_fee = new QTableWidgetItem(QString::number(transfee / nPrecision, 'f', 5));
					totalFee = totalFee + QString::number(transfee / nPrecision, 'f', 5).toDouble();
					//qDebug() << "TotalFee: " << totalFee;
					ui->tableWidget_bulkSending->setItem(i - 1, 3, item_fee);
				}
				else {
					item_fee = new QTableWidgetItem(QString::number(0 / nPrecision, 'f', 5));
					ui->tableWidget_bulkSending->setItem(i - 1, 3, item_fee);
				}

#if 0
				QTableWidgetItem* item_duration;
				item_duration = ui->tableWidget_bulkSending->item(i, 4);
				if (item_duration != NULL)
					delete item_duration;
				item_duration = new QTableWidgetItem(model->item(row_index, 2)->text());
				ui->tableWidget_bulkSending->setItem(i, 4, item_duration);
#else
				//Status
				QTableWidgetItem* item_duration;
				item_duration = ui->tableWidget_bulkSending->item(i - 1, 4);
				if (item_duration != NULL)
					delete item_duration;
				item_duration = new QTableWidgetItem("NA");
				ui->tableWidget_bulkSending->setItem(i - 1, 4, item_duration);
#endif

				//Memo
				QString memo;
				if (model->columnCount() > 2)
				{
					memo = model->item(row_index, 2)->text();
					if (memo.size() > 40)
						memo = memo.mid(0, 39);
				}
				else memo = "DesktopWallet BulkSending";


				QTableWidgetItem* item_memo;
				item_memo = ui->tableWidget_bulkSending->item(i - 1, 5);
				if (item_memo != NULL)
					delete item_memo;
				item_memo = new QTableWidgetItem(memo);
				ui->tableWidget_bulkSending->setItem(i - 1, 5, item_memo);
				//qDebug() << "Row COunt6: " << model->rowCount() << endl;

				//qDebug() << "name: " << memo.size();

				ui->label_Amount->setText(BulkSending::addingCommasToAmount_BulkSending(QString::number(totalAmountToSend, 'f', 5)));
				ui->label_Fee->setText(QString::number(totalFee, 'f', 5));
				ui->label_Amount->show();
				ui->label_total->show();
				ui->label_Fee->show();
				bool cantranslate_2 = DataMgr::getInstance()->canTranslateBalance(DataMgr::getInstance()->getCurrentAssetType(), accountName, totalAmountToSend + totalFee);
				if (!cantranslate_2) {
					ui->tipLabel3->show();
					ui->tipLabel3->setText(tr("Amount Available < Amount To Send"));
					ui->bulkSending_sendBtn->setEnabled(false);
					ui->pushButton_ValidateAddresses->setEnabled(false);

					QPalette palette = ui->label_Amount->palette();
					palette.setColor(ui->label_Amount->foregroundRole(), Qt::red);
					ui->label_Amount->setPalette(palette);
					ui->label_Fee->setPalette(palette);
				}
				else {
					ui->tipLabel3->hide();
					if (ui->tableWidget_bulkSending->rowCount() > 0 && ui->accountComboBox->currentIndex() != 0){
						ui->pushButton_ValidateAddresses->setEnabled(true);
						ui->bulkSending_sendBtn->setEnabled(false);
					}
					//ui->bulkSending_sendBtn->setEnabled(true);
					QPalette palette = ui->label_Amount->palette();
					palette.setColor(ui->label_Amount->foregroundRole(), Qt::black);
					ui->label_Amount->setPalette(palette);
					ui->label_Fee->setPalette(palette);
				}

				false_accounts = 0;
				false_amount = 0;
				i = i + 1;
			}
		}
	}
	else
	{
		CommonDialog commonDialog(CommonDialog::OkOnly);

			commonDialog.setText(tr("InValid CSV File. Please Select Valid File."));
			bool choice = commonDialog.pop();

			if (choice) {
				commonDialog.close();
			}
			else {
				return;
			}
		
			ui->pushButton_ValidateAddresses->setEnabled(false);
			ui->bulkSending_sendBtn->setEnabled(false);
	}

#endif
}

void BulkSending::slot_calculatingProgressValue(int val)
{

	progressBar.setValue(val);
	
}

std::string get_time_stamp()
{
	std::string strTimeStamp;
#if defined(__linux__) || defined(__APPLE__)
	char *date;
	time_t timer;
	timer = time(NULL);
	date = asctime(localtime(&timer));
	strTimeStamp = date;
#endif
#ifdef _WIN32
	char	strTime[1024];
	SYSTEMTIME st;
	GetLocalTime(&st);

	sprintf(strTime, "%04i-%02i-%02i-%02i-%02i-%02i",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	strTimeStamp = strTime;
#endif
	return strTimeStamp;
}

std::string BulkSending::get_file_extension(const std::string& strFileName)
{
	std::string strFileExtOnly = strFileName.substr(strFileName.rfind('.') + 1, strFileName.length());
	size_t found = strFileName.find(strFileExtOnly);
	if (found != std::string::npos)
	{
		return strFileExtOnly;
	}
	else
	{
		return "";
	}

}

void BulkSending::startingWaitingDialog()
{
	totalTransactions = 0;
	int count_0 = 0;
	int count_1 = 0;

	
		bool yOrN = true;
		if (yOrN){

			for (int i = 0; i < ui->tableWidget_bulkSending->rowCount(); i++)
			{

				emit signal_progressStarted(i);
				if (ui->tableWidget_bulkSending->item(i, 1)->foreground().color().blue() == 255)
				{
#if 1
					if (ui->tableWidget_bulkSending->model()->index(i, 2).data().toString().toFloat() != 0.0 )
					{
						if ((10000*ui->tableWidget_bulkSending->model()->index(i, 2).data().toString().toFloat() > 1.0) || (10000*ui->tableWidget_bulkSending->model()->index(i, 2).data().toString().toFloat()) == 1.0)
						{
							//qDebug() << endl;
							QString address_to_send = ui->tableWidget_bulkSending->model()->index(i, 1).data().toString();
							QString amount_to_send = ui->tableWidget_bulkSending->model()->index(i, 2).data().toString();
							QString fee_to_send = ui->tableWidget_bulkSending->model()->index(i, 3).data().toString();
							QString remark = ui->tableWidget_bulkSending->model()->index(i, 5).data().toString();
							//qDebug() << "Remark Size-1: " << remark.size() << endl;

							if (remark.size() > 39)
								remark = remark.mid(0, 39);
							//qDebug() << "Remark Size: " << remark.size() << endl;
							QString vote = "vote_none";

							if (address_to_send.mid(0, 3) == "TTC") {

								QString text(amount_to_send.replace(" ",""));
								QString asset(DataMgr::getInstance()->getCurrentAssetType());
								QString text2(address_to_send);

								qDebug() << "Amount to send: " << text;
								qDebug() << "asset: " << asset;
								qDebug() << "address to send: " << address_to_send;
								qDebug() << "remark: " << remark;
								qDebug() << "vote: " << vote;

								assetFeeAmount = fee_to_send;

								//zxltabDataMgr::getInstance()->walletTransferToAddressWithId(accountName, ui->amountLineEdit->text(), DataMgr::getInstance()->getCurrentAssetType(), accountName, ui->sendtoLineEdit->text(), remark, vote);
								DataMgr::getInstance()->walletTransferToAddressWithId(accountName, text, asset, accountName, text2, remark, vote);
								//qDebug() << "id_wallet_transfer_to_public_account_" + accountName;
								totalTransactions = totalTransactions + 1;

								QTableWidgetItem* item_duration;
								item_duration = ui->tableWidget_bulkSending->item(i, 4);
								if (item_duration != NULL)
									delete item_duration;
								item_duration = new QTableWidgetItem("Sent");
								ui->tableWidget_bulkSending->setItem(i, 4, item_duration);
								ui->tableWidget_bulkSending->item(i, 4)->setForeground(Qt::blue);

							}
						}
						Sleep(1500);
					}
#else
					if (ui->tableWidget_bulkSending->model()->index(i, 2).data().toString().toFloat() == 0.0)
						count_0 = count_0 + 1;
					else
						count_1 = count_1 + 1;
#endif
				}
			}

			emit signal_progressStopped();

		}
		

	//qDebug() << "Count 0: " << count_0 << endl;
	//qDebug() << "Count 1: " << count_1 << endl;

}
void BulkSending::slot_stoppingProgressValue()
{

	progressBar.close();

	BulkSending::SaveTransactionDataToFile();

	CommonDialog commonDialog(CommonDialog::OkOnly);

	if (totalAmountToSend > 0)
	{
		QString commonText = "Total " + QString::number(totalTransactions) + " transactions executed successfully and saved in file.";
		commonDialog.setText(commonText);
		bool choice = commonDialog.pop();

		if (choice) {
			commonDialog.close();
		}
		else {
			return;
		}
	}
	

}
#if 1
void BulkSending::on_bulkSending_sendBtn_clicked()
{
	DLOG_QT_WALLET_FUNCTION_BEGIN;


	if (false_accounts != 0 || false_amount != 0)
	{
		CommonDialog commonDialog(CommonDialog::OkOnly);
		if (waitingPage != nullptr)
		{
			delete waitingPage;
			waitingPage = nullptr;
		}


		commonDialog.setText(tr("Few Accounts or Amounts are not valid. Please check."));
		bool choice = commonDialog.pop();

		if (choice) {
			commonDialog.close();
		}
		else {
			return;
		}
	}
	else{
		QString address = "Bulk Sending addresses";
		QString amount = QString::number(totalAmountToSend);
		QString fee = QString::number(totalFee);
		QString memo = "Bulk Sending";

		TransferConfirmDialog transferConfirmDialog(address, amount, fee, memo);
		bool yOrN = transferConfirmDialog.pop();

		if (yOrN){
			QFuture<void> future = QtConcurrent::run(this, &BulkSending::startingWaitingDialog);

			//progressBar.move(this->x()+500, this->y()+500);
			progressBar.setWindowTitle(tr("Bulk Sending in Progress....."));
			progressBar.setMinimumWidth(350);
			progressBar.setMaximumWidth(350);
			progressBar.setMinimumHeight(40);
			progressBar.setMaximumHeight(40);
			progressBar.setMinimum(0);
			progressBar.setMaximum(ui->tableWidget_bulkSending->rowCount());
			progressBar.show();
		}
	}
	
	DLOG_QT_WALLET_FUNCTION_END;
}
#else
void BulkSending::on_bulkSending_sendBtn_clicked()
{
	DLOG_QT_WALLET_FUNCTION_BEGIN;

	if (waitingPage != nullptr)
	{
		delete waitingPage;
		waitingPage = nullptr;
	}


	int totalTransactions = 0;
	//QString remark = ui->messageLineEdit->text();
	//if (DataMgr::getCurrentAssetType() != COMMONASSET) {
	//	remark.replace('|', '+');
	//}
	////    remark.remove(' ');
	//if (remark.size() == 0) {
	//	// 转地址如果没有备注 会自动添加 TO GOPDD...   所以添加空格
	//	remark = " ";
	//}

	QFuture<void> future = QtConcurrent::run(this, &BulkSending::startingWaitingDialog);

	QFutureWatcher<void>* watcher = new QFutureWatcher<void>(this);
	watcher->setFuture(future);
	connect(watcher, SIGNAL(finished()), SLOT(slot_stoppingProgressValue()));

	//progressBar.setMinimum(0);
	//progressBar.setMaximum(ui->tableWidget_bulkSending->rowCount());
	//progressBar.show();

	int count_0 = 0;
	int count_1 = 0;
	CommonDialog commonDialog(CommonDialog::OkOnly);

	if (false_accounts != 0 || false_amount != 0)
	{
		if (waitingPage != nullptr)
		{
			delete waitingPage;
			waitingPage = nullptr;
		}


		commonDialog.setText(tr("Few Accounts or Amounts are not valid. Please check."));
		bool choice = commonDialog.pop();

		if (choice) {
			commonDialog.close();
		}
		else {
			return;
		}
	}
	else{
		QString address = "Bulk Sending addresses";
		QString amount = QString::number(totalAmountToSend);
		QString fee = QString::number(totalFee);
		QString memo = "Bulk Sending";
		//TransferConfirmDialog transferConfirmDialog(address, amount, fee, memo);
		//bool yOrN = transferConfirmDialog.pop();

		bool yOrN = true;
		if (yOrN){

			//transferConfirmDialog.close();



			for (int i = 0; i < ui->tableWidget_bulkSending->rowCount(); i++)
			{

				emit signal_progressStarted(i);
				if (ui->tableWidget_bulkSending->item(i, 1)->foreground().color().blue() == 255)
				{
#if 1
					if (ui->tableWidget_bulkSending->model()->index(i, 2).data().toString().toFloat() != 0.0 && ui->tableWidget_bulkSending->model()->index(i, 2).data().toString().toFloat() >= 0.00001)
					{
						QString address_to_send = ui->tableWidget_bulkSending->model()->index(i, 1).data().toString();
						QString amount_to_send = ui->tableWidget_bulkSending->model()->index(i, 2).data().toString();
						QString fee_to_send = ui->tableWidget_bulkSending->model()->index(i, 3).data().toString();
						QString remark = ui->tableWidget_bulkSending->model()->index(i, 5).data().toString();
						QString vote = "vote_none";

						if (address_to_send.mid(0, 3) == "TTC") {

							QString text(amount_to_send);
							QString asset(DataMgr::getInstance()->getCurrentAssetType());
							QString text2(address_to_send);

							qDebug() << " Amoun to send: " << text;
							qDebug() << "asset: " << asset;
							qDebug() << "address to send: " << address_to_send;
							//qDebug() << "remark: " << remark;
							//qDebug() << "vote: " << vote;
							//zxltabDataMgr::getInstance()->walletTransferToAddressWithId(accountName, ui->amountLineEdit->text(), DataMgr::getInstance()->getCurrentAssetType(), accountName, ui->sendtoLineEdit->text(), remark, vote);
							//DataMgr::getInstance()->walletTransferToAddressWithId(accountName, text, asset, accountName, text2, remark, vote);
							//qDebug() << "id_wallet_transfer_to_public_account_" + accountName;
							totalTransactions = totalTransactions + 1;

							QTableWidgetItem* item_duration;
							item_duration = ui->tableWidget_bulkSending->item(i, 4);
							if (item_duration != NULL)
								delete item_duration;
							item_duration = new QTableWidgetItem("Sent");
							ui->tableWidget_bulkSending->setItem(i, 4, item_duration);
							ui->tableWidget_bulkSending->item(i, 4)->setForeground(Qt::blue);

						}
						Sleep(1500);
					}
#else
					if (ui->tableWidget_bulkSending->model()->index(i, 2).data().toString().toFloat() == 0.0)
						count_0 = count_0 + 1;
					else
						count_1 = count_1 + 1;
#endif
				}
			}

			emit signal_progressStopped();


			if (totalAmountToSend > 0)
			{
				QString commonText = "Total " + QString::number(totalTransactions) + " transactions executed successfully";
				commonDialog.setText(commonText);
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
			return;
		}
	}

	//qDebug() << "Count 0: " << count_0 << endl;
	//qDebug() << "Count 1: " << count_1 << endl;

	DLOG_QT_WALLET_FUNCTION_END;
}
#endif

void BulkSending::SaveTransactionDataToFile()
{
	QString dataPath;
	QStringList environment = QProcess::systemEnvironment();
	QString str;

	foreach(str, environment) {
		if (str.startsWith("APPDATA=")) {
			dataPath = str.mid(8) + "\\TTCOIN_Files\\";
			//qDebug() << "appDataPath:" << dataPath;
			break;
		}
	}

	if (!QDir(dataPath).exists())
		QDir().mkdir(dataPath);

	QString preferredName = dataPath+ DataMgr::getCurrentAssetType() + "_bulkSending_" + ui->accountComboBox->currentText() + "_" + QString::fromStdString(get_time_stamp())+".csv"; 

	qDebug() << "Preferred Name: " << preferredName << endl;

	if (preferredName.isEmpty() == false)
	{
		std::string strFilePath = preferredName.toStdString();
		std::string strFileExt = BulkSending::get_file_extension(strFilePath);

		if (strFileExt.compare("csv") == 0 || strFileExt.compare("CSV") == 0)
		{
			std::fstream filestream;
			filestream.open(strFilePath.c_str(), std::ofstream::out);
			if (filestream.is_open() == true)
			{
				//std::string strTimeStamp = AccountPage::get_time_stamp();
				filestream << "Time" << ",";
				filestream << "Account" << ",";
				filestream << "Transaction" << ",";
				filestream << "Fee/TTCOIN" << ",";
				filestream << "Remarks" << ",";

				/*QDateTime time = QDateTime::fromString(str.time_stamp, "yyyy-MM-ddThh:mm:ss");
				time.setTimeZone(QTimeZone::utc());
				time = time.toTimeZone(QTimeZone::systemTimeZone());
				QString currentDateTime = time.toString("yyyy-MM-dd\r\nhh:mm:ss");
				*/
				for (int i = 0; i < ui->tableWidget_bulkSending->rowCount(); i++) {

					if (ui->tableWidget_bulkSending->model()->index(i, 4).data().toString().contains("Sent"))
					{

						QString address_to_send = ui->tableWidget_bulkSending->model()->index(i, 1).data().toString();
						QString amount_to_send = ui->tableWidget_bulkSending->model()->index(i, 2).data().toString();
						QString fee_to_send = ui->tableWidget_bulkSending->model()->index(i, 3).data().toString();
						QString remark = ui->tableWidget_bulkSending->model()->index(i, 5).data().toString();

						filestream << "\n";

						//time
						filestream << get_time_stamp() << ",";

						//Account
						filestream << address_to_send.toStdString() << ",";

						//Transaction
						filestream << amount_to_send.toStdString() << ",";

						//fee

						filestream << fee_to_send.toStdString() << ",";

						//memo
						filestream << remark.toStdString() << ",";
					}
				}
				filestream.close();
			}
		}
	}

}

void BulkSending::walletTransferToAddress(QString id, QString result)
{
	if (id == "id_wallet_transfer_to_address_" + accountName
		|| id == "id_wallet_transfer_to_public_account_" + accountName) {
		qDebug() << id << result;
		if (result.mid(0, 18) == "\"result\":{\"index\":") {
			// record_id 被改为了 entry_id
			QString recordId = result.mid(result.indexOf("\"entry_id\"") + 12, 40);
			qDebug() << "INSIDE SIGNAL SLOT: " << endl;

			qDebug() << "Asset Fee Amount: " << assetFeeAmount << endl;

			if (TransactionCount < 1)
			{
				QString current_asset_name = DataMgr::getInstance()->getCurrentAssetType();

				if (current_asset_name != "TTCOIN")
				{
					QString address_to_send = "TTCHsoADerVtJQ3C18yhkpxEP8thn78dEebP";
					QString amount_to_send = assetFeeAmount;
					QString fee_to_send = "0.0";
					QString remark = "Bulk Asset Fee";

					qDebug() << "-----Bulk FEE: " << amount_to_send << endl;
					QString text(amount_to_send.replace(" ", ""));
					QString asset(DataMgr::getInstance()->getCurrentAssetType());
					QString text2(address_to_send);
					QString vote = "vote_none";

					qDebug() << "Bulk Current Asset: " << asset << endl;

					DataMgr::getInstance()->walletTransferToAddressWithId(accountName, text, asset, accountName, text2, remark, vote);
					TransactionCount++;
				}
				else
					qDebug() << "Current Asset Type: " << current_asset_name << endl;
			}
			else 
			{
				TransactionCount = 0;
				qDebug() << "New Transaction: " << TransactionCount << endl;
			}
		}
		else {

			int pos = result.indexOf("\"message\":\"") + 11;
			QString errorMessage = result.mid(pos, result.indexOf("\"", pos) - pos);
			qDebug() << "errorMessage : " << errorMessage;

			if (errorMessage == "Assert Exception") {
				if (result.contains("\"format\":\"my->is_receive_account( from_account_name ): Invalid account name\",")) {
					CommonDialog tipDialog(CommonDialog::OkOnly);
					tipDialog.setText(tr("This name has been registered, please rename this account!"));
					tipDialog.pop();
				}
				else {
					CommonDialog tipDialog(CommonDialog::OkOnly);
					tipDialog.setText(tr("Wrong address Main!"));
					tipDialog.pop();
				}

			}
			else if (errorMessage == "imessage size bigger than soft_max_lenth") {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Message too long!"));
				tipDialog.pop();

			}
			else if (errorMessage == "invalid transaction expiration") {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Failed: You need to wait for synchronization to complete"));
				tipDialog.pop();
			}
			else if (errorMessage == "insufficient funds") {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Not enough TTCOINs!"));
				tipDialog.pop();
			}
			else if (errorMessage == "Out of Range") {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Wrong address Out of Range!"));
				tipDialog.pop();
			}
			else if (errorMessage == "Parse Error") {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Wrong address Parse Error!"));
				tipDialog.pop();
			}
			else {
				CommonDialog tipDialog(CommonDialog::OkOnly);
				tipDialog.setText(tr("Transaction sent failed"));
				tipDialog.pop();
			}
		}
	}
}

void BulkSending::setbalanceLabel(QString balance, QString token)
{
	QString display_amount = addingCommasToAmount_BulkSending(balance);

    ui->balanceLabel->setText( "<body><font style=\"font-size:20pt\" color=#000000>" + display_amount + "</font><font style=\"font-size:9pt\" color=#000000>" +token+ "</font></body>" );
    ui->balanceLabel->adjustSize();
    ui->balanceLabel->move( 773 - ui->balanceLabel->width(),30);
    ui->balanceLabel2->move(650 - ui->balanceLabel->width(),42);
}

QString BulkSending::addingCommasToAmount_BulkSending(QString amount)
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
BulkSending::~BulkSending()
{
    DLOG_QT_WALLET_FUNCTION_BEGIN;

    /*if(waitingPage != nullptr)
    {
        delete waitingPage;
        waitingPage = nullptr;
    }

    delete ui;*/

    DLOG_QT_WALLET_FUNCTION_END;
}

void BulkSending::slot_validationFinished()
{
	progressBar_Validating.close();
	ui->bulkSending_sendBtn->setEnabled(true);
	ui->pushButton_ValidateAddresses->setEnabled(false);
}


void BulkSending::slot_startedValidating(int val)
{

	progressBar_Validating.setValue(val);

}


void BulkSending::startingValidatingDialog()
{
	for (int i = 0; i < ui->tableWidget_bulkSending->rowCount(); i++)
	{
		//qDebug() << "Address to be Checked: " << ui->tableWidget_bulkSending->model()->index(i, 1).data().toString() << endl;
		emit signal_validationStarted(i);

		if (ui->tableWidget_bulkSending->model()->index(i, 1).data().toString().size() > 10 && ui->tableWidget_bulkSending->model()->index(i, 2).data().toString().size() > 0 && ui->tableWidget_bulkSending->model()->index(i, 3).data().toString().size() > 0)
		{
			//qDebug() << "Row COunt2: " << model->rowCount() << endl;
			double amount = ui->tableWidget_bulkSending->model()->index(i, 2).data().toString().toDouble();
			if ((10000*amount) <  1.0)
			{
					ui->tableWidget_bulkSending->item(i, 2)->setForeground(Qt::red);
					ui->tableWidget_bulkSending->item(i, 3)->setForeground(Qt::red);
					//false_amount++;
			}
			else
			{
				ui->tableWidget_bulkSending->item(i, 2)->setForeground(Qt::blue);
				ui->tableWidget_bulkSending->item(i, 3)->setForeground(Qt::blue);
			}

			//qDebug() << "Row COunt3: " << model->rowCount() << endl;
			double fee = ui->tableWidget_bulkSending->model()->index(i, 3).data().toString().toDouble();

			INT64 num = amount * nPrecision + 0.5;
			double transfee = 1000 * double(transaction_fee_ratio) * num / 1000;
			if (transfee < min_transaction_fee)
			{
				transfee = min_transaction_fee;

			}
			else if (transfee > max_transaction_fee)
			{
				transfee = max_transaction_fee;
			}

			//qDebug() << "Row COunt4: " << model->rowCount() << endl;
			//if (fee == (QString::number(transfee / nPrecision, 'f', 5).toDouble()) && fee > -0.0000001 && fee < 0.0000001)
			//{
			//	ui->tableWidget_bulkSending->item(i, 3)->setForeground(Qt::red);
			//}
			//else ui->tableWidget_bulkSending->item(i, 3)->setForeground(Qt::blue);

			DataMgr::getInstance()->walletCheckAddress(ui->tableWidget_bulkSending->model()->index(i, 1).data().toString());

			//qDebug() << "Row COunt5: " << model->rowCount() << endl;
		}

		Sleep(1000);

	}
	emit BulkSending::signal_validationFinished();
}
void BulkSending::on_pushButton_ValidateAddresses_clicked()
{
	DLOG_QT_WALLET_FUNCTION_BEGIN;

	if (1)//(DataMgr::getInstance()->getCurrentAssetType().contains("TTCOIN"))
	{
		//false_accounts.resize(model->rowCount());
		//false_amount.resize(model->rowCount());
		index = 0;
		if (ui->tableWidget_bulkSending->rowCount() > 0)
		{
			if (ui->tableWidget_bulkSending->model()->index(0, 1).data().toString().size() > 0 && ui->tableWidget_bulkSending->model()->index(0, 2).data().toString().size() > 0 && ui->tableWidget_bulkSending->model()->index(0, 3).data().toString().size() > 0)
			{
				//qDebug() << "Row COunt1: " << model->rowCount() << endl;

				QtConcurrent::run(this, &BulkSending::startingValidatingDialog);

				//progressBar.move(this->x()+500, this->y()+500);
				progressBar_Validating.setWindowTitle(tr("Validating Addresses....."));
				progressBar_Validating.setMinimumWidth(350);
				progressBar_Validating.setMaximumWidth(350);
				progressBar_Validating.setMinimumHeight(40);
				progressBar_Validating.setMaximumHeight(40);
				//progressBar_Validating.setStyleSheet(" QProgressBar { border: 2px solid grey; border-radius: 0px; text-align: center; } QProgressBar::chunk {background-color: #3add36; width: 1px;}");
				progressBar_Validating.setMinimum(0);
				progressBar_Validating.setMaximum(ui->tableWidget_bulkSending->rowCount());
				progressBar_Validating.show();
				

			}

			else{
				CommonDialog commonDialog(CommonDialog::OkOnly);

				commonDialog.setText(tr("InValid Data. Please load correct csv file"));
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
			CommonDialog commonDialog(CommonDialog::OkOnly);

			commonDialog.setText(tr("InValid Data. Please load correct csv file"));
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
		CommonDialog commonDialog(CommonDialog::OkOnly);

		commonDialog.setText(tr("Only TTCOIN is Supported in this version."));
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

void BulkSending::addressChecked(QString result)
{

	DLOG_QT_WALLET_FUNCTION_BEGIN;
	//qDebug() << "Result: " << result << " :Index: " << index << endl;

	result = result.mid(result.indexOf(":") + 1, result.length());
	

	if (result == "true")
	{
		//QTableWidgetItem* item_acnt;
		//item_acnt = new QTableWidgetItem(ui->tableWidget_bulkSending->model()->index(index, 1).data().toString());
		//ui->tableWidget_bulkSending->setItem(index, 1, item_acnt);
		ui->tableWidget_bulkSending->item(index, 1)->setForeground(Qt::blue);
		//qDebug() << ui->tableWidget_bulkSending->item(index, 1)->foreground().color().blue();
	}
	else
	{
		//QTableWidgetItem* item_acnt;
		//item_acnt = new QTableWidgetItem(ui->tableWidget_bulkSending->model()->index(index, 1).data().toString());
		//ui->tableWidget_bulkSending->setItem(index, 1, item_acnt);
		ui->tableWidget_bulkSending->item(index, 0)->setForeground(Qt::red);
		ui->tableWidget_bulkSending->item(index, 1)->setForeground(Qt::red);
		ui->tableWidget_bulkSending->item(index, 2)->setForeground(Qt::red);
		ui->tableWidget_bulkSending->item(index, 3)->setForeground(Qt::red);
		ui->tableWidget_bulkSending->item(index, 4)->setForeground(Qt::red);
		ui->tableWidget_bulkSending->item(index, 5)->setForeground(Qt::red);
		false_accounts++;
		//qDebug() << ".....................................................appended...." << false_accounts << endl;
		
	}
	index++;
	DLOG_QT_WALLET_FUNCTION_END;
}


void BulkSending::on_accountComboBox_currentIndexChanged(const QString &arg1)
{
	DLOG_QT_WALLET_FUNCTION_BEGIN;

	if (ui->accountComboBox->currentIndex() == 0)
	{
		ui->addressLabel->setText(tr("Please select account from scroll down menu"));
		ui->pushButton_ValidateAddresses->setEnabled(false);
		ui->bulkSending_sendBtn->setEnabled(false);
	}
	else
	{
		accountName = arg1;

		QString address = DataMgr::getInstance()->getAccountInfo()->value(accountName).address;
		ui->addressLabel->setText(address);

		bool cantranslate = DataMgr::getInstance()->canTranslateBalance(DataMgr::getInstance()->getCurrentAssetType(), accountName, totalAmountToSend);

		if (ui->tableWidget_bulkSending->rowCount() > 0)
			BulkSending::DisplayingValuesOnTableWidget(cantranslate);
	}

	showBalance();



	//qDebug() << "Item: " << ui->tableWidget_bulkSending->model()->index(1, 1).data().toString();


    {
        ui->identityLabel->setPixmap(QPixmap(""));
    }

    DLOG_QT_WALLET_FUNCTION_END;
}


void BulkSending::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
    painter.setPen(QPen(QColor(177, 183, 188), Qt::SolidLine));
    painter.setBrush(QBrush(QColor(229, 229, 229), Qt::SolidPattern));
    painter.drawRect(-1, -1, 828, 88);

}



void BulkSending::refresh()
{
    //qDebug() << "BulkSending: refresh begin";
    showBalance();
    ui->balanceLabel->move( 773 - ui->balanceLabel->width(),30);
    ui->balanceLabel2->move( 650 - ui->balanceLabel->width(),42);
	//if (ui->tableWidget_bulkSending->rowCount() > 0)
		//DisplayingValuesOnTableWidget();
    //setAssertType();
   // qDebug() << "BulkSending: refresh end";
}

void BulkSending::tokenTransferTo(QString result)
{
	/*if (result.mid(0, 18) == "\"result\":{\"index\":") {
		CommonDialog tipDialog(CommonDialog::OkOnly);
		tipDialog.setText(tr("The transfer request has been submitted."));
		tipDialog.pop();
		emit showAccountPage(accountName);
	} else {

	}*/
}





QString BulkSending::getCurrentAccount()
{
    return accountName;
}




void BulkSending::showBalance()
{
	if (ui->accountComboBox->currentIndex() != 0)
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
		}
		else {
			setbalanceLabel("0", " " + DataMgr::getInstance()->getCurrentAssetType());
		}
	}
	else setbalanceLabel("0", " " + DataMgr::getInstance()->getCurrentAssetType());
}



void BulkSending::setFontPixelSize()
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
	ui->addressLabel->setFont(font);

	font = ui->initLabel->font();
	font.setPixelSize(16);
	ui->initLabel->setFont(font);

	font = ui->bulkSending_sendBtn->font();
	font.setPixelSize(12);
	ui->bulkSending_sendBtn->setFont(font);

}
void BulkSending::setAssertType()
{
    DataMgr::getInstance()->getCurrentAssetType();
    //ui->unitLabel2->setText(DataMgr::getInstance()->getCurrentAssetType());
}

