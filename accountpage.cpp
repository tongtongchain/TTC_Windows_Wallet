#include <QDebug>
#include <QPainter>
#include <QHelpEvent>
#include <QDateTime>
#include <QTextCodec>
#include <QScrollBar>
#include <QFont>
#include <QTimeZone>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimeZone>

#include "accountpage.h"
#include "ui_accountpage.h"
#include "goopal.h"
#include "debug_log.h"
#include <QClipboard>
#include "commondialog.h"
#include "showcontentdialog.h"
#include "chooseupgradedialog.h"
#include "control/remarkcellwidget.h"
#include "datamgr.h"


AccountPage::AccountPage(QString name, QWidget *parent) :
QWidget(parent),
accountName(name),
transactionType(0),
address(""),
inited(false),
ui(new Ui::AccountPage) {
	DLOG_QT_WALLET_FUNCTION_BEGIN;
	ui->setupUi(this);
	setAutoFillBackground(true);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(255, 255, 255));
	setPalette(palette);
	mutexForAddressMap.lock();

	if (accountName.isEmpty()) { // 如果是点击账单跳转
		{
			mutexForAddressMap.unlock();
			emit back();
			return;
		}
	}

	// 账户下拉框按字母顺序排序
	QStringList keys = DataMgr::getInstance()->getAccountInfo()->keys();
	ui->accountComboBox->addItems(keys);

	if (accountName.isEmpty()) {
		ui->accountComboBox->setCurrentIndex(0);

	}
	else {
		ui->accountComboBox->setCurrentText(accountName);
	}

	mutexForAddressMap.unlock();
	ui->pageLineEdit->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
	ui->pageLineEdit->setText("1");
	QIntValidator *validator = new QIntValidator(1, 9999, this);
	ui->pageLineEdit->setValidator(validator);
	ui->pageLineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
	ui->pageLineEdit->setDisabled(true);
	currentPageIndex = 1;
	ui->pageLineEdit->setText(QString::number(currentPageIndex));
	ui->prePageBtn->setStyleSheet("QToolButton:!hover{border:0px;color:#999999;} QToolButton:hover{border:0px;color:#469cfc;}");
	ui->nextPageBtn->setStyleSheet("QToolButton:!hover{border:0px;color:#999999;} QToolButton:hover{border:0px;color:#469cfc;}");
	ui->downloadCSVBtn->setStyleSheet("QToolButton{background-color:#f49f17;color:#ffffff;border:0px solid rgb(64,153,255);border-radius:3px;}"
		"QToolButton:hover{background-color:#ffc362;}"
		"QToolButton:disabled{background-color:#cecece;}");
	ui->downloadCSVBtn->setEnabled(false);
	ui->transactionTypeComboBox->setEnabled(false);
#ifdef WIN32
	ui->accountComboBox->setStyleSheet(QString("QComboBox{border: 1px solid gray;border-radius: 3px;padding: 1px 2px 1px 8px;min-width: 9em;}"
		"QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 20px;"
		"border-left-width: 1px;border-left-color: darkgray;border-left-style: solid;"
		"border-top-right-radius: 3px;border-bottom-right-radius: 3px;}"
		"QComboBox::down-arrow {image: url(%1pic2/comboBoxArrow.png);}").arg(QCoreApplication::applicationDirPath() + "/")
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
#endif // WIN32//zxlrun
	ui->transactionTypeComboBox->setStyleSheet(QString("QComboBox{border: 1px solid gray;border-radius: 3px;padding: 1px 2px 1px 8px;min-width: 5em;}"
		"QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 20px;"
		"border-left-width: 1px;border-left-color: darkgray;border-left-style: solid;"
		"border-top-right-radius: 3px;border-bottom-right-radius: 3px;}"
		"QComboBox::down-arrow {image: url(%1pic2/comboBoxArrow.png);}").arg(QCoreApplication::applicationDirPath() + "/")
		);
	ui->copyBtn->setStyleSheet(QString("QToolButton{background-image:url(%1pic2/copyBtn.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}"
		"QToolButton:hover{background-image:url(%2pic2/copyBtn_hover.png);background-repeat: repeat-xy;background-position: center;background-attachment: fixed;background-clip: padding;border-style: flat;}").arg(QCoreApplication::applicationDirPath() + "/").arg(QCoreApplication::applicationDirPath() + "/"));
	ui->copyBtn->setToolTip(tr("copy to clipboard"));
	ui->accountTransactionsTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
	ui->accountTransactionsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->accountTransactionsTableWidget->setFocusPolicy(Qt::NoFocus);
	ui->accountTransactionsTableWidget->setColumnWidth(0, 120);
	ui->accountTransactionsTableWidget->setColumnWidth(1, 130);
	ui->accountTransactionsTableWidget->setColumnWidth(2, 130);
	ui->accountTransactionsTableWidget->setColumnWidth(3, 130);
	ui->accountTransactionsTableWidget->setColumnWidth(4, 130);
	ui->accountTransactionsTableWidget->setColumnWidth(5, 120);
	ui->accountTransactionsTableWidget->setShowGrid(false);
	ui->accountTransactionsTableWidget->setStyleSheet("QTableView::item:!selected {border-bottom: 1px dashed gray;} QTableView::item:selected {background-color:rgb(255, 160, 122);}");
	ui->accountTransactionsTableWidget->setFrameShape(QFrame::NoFrame);
	ui->accountTransactionsTableWidget->setMouseTracking(true);
	ui->accountTransactionsTableWidget->setGeometry(39, 190, 770, 5 * 57);
	//ui->initLabel->hide();
	// ui->label_2->hide();
	// ui->transactionTypeComboBox->hide();
	retranslator();
	init();
	updateTransactionsList();
	showTransactions(DataMgr::getInstance()->getCurrentTrx()->value(accountName));
	balance();
	inited = true;
	connect(DataMgr::getInstance(), &DataMgr::onWalletTransactionHistorySpliteWithId, this, &AccountPage::walletTransactionHistorySpliteWithId);
	connect(ui->downloadCSVBtn, SIGNAL(clicked()), this, SLOT(slot_DownloadCSV()));
	setFontPixelSize();
	//  connect(DataMgr::getInstance(), &DataMgr::onBalance, this, &AccountPage::balance);
	//  connect(DataMgr::getInstance(), &DataMgr::onWalletTransactionHistorySplite, this, &AccountPage::walletTransactionHistorySplite);
	DLOG_QT_WALLET_FUNCTION_END;
}

std::string AccountPage::get_file_extension(const std::string& strFileName)
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
void AccountPage::slot_DownloadCSV()
{
	CommonDialog commonDialog(CommonDialog::OkOnly);

	if (size_CSV <= 0)
	{

		commonDialog.setText(tr("There is no transaction to be Saved!"));
		bool choice = commonDialog.pop();

		if (choice) {
			commonDialog.close();
		}
		else {
			return;
		}

	}

	QString currentIndex;

	if (ui->transactionTypeComboBox->currentIndex() == 0)
		currentIndex = "ALL";
	else if (ui->transactionTypeComboBox->currentIndex() == 1)
		currentIndex = "IN";
	else if (ui->transactionTypeComboBox->currentIndex() == 2)
		currentIndex = "OUT";
	QString preferredName = DataMgr::getCurrentAssetType() + "_" + ui->accountComboBox->currentText() + "_" + currentIndex + "_" + QString::fromStdString(get_time_stamp());

	qDebug() << "Preferred Name: " << preferredName << endl;

	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save transactions To File"),
		preferredName,
		tr("CSV Files (*.csv);;"));

	if (fileName.isEmpty() == false)
	{
		std::string strFilePath = fileName.toStdString();
		std::string strFileExt = AccountPage::get_file_extension(strFilePath);

    
		if (strFileExt.compare("csv") == 0 || strFileExt.compare("CSV") == 0)
		{
			SaveTransactionDataToFile(strFilePath);

			commonDialog.setText(tr("Transaction History Saved to csv File!"));
			bool choice = commonDialog.pop();

			if (choice) {
				commonDialog.close();
			}
			else {
				return;
			}

			//showAccountList_real_CSV.clear();
			//showAccountList_CSV.clear();
			//size_CSV = 0;

		}
		else
		{
			commonDialog.setText(tr("Failed to save transactions to file!"));
			bool choice = commonDialog.pop();

			if (choice) {
				commonDialog.close();
			}
			else {
				return;
			}
		}
	}
}

std::string AccountPage::get_time_stamp()
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


void AccountPage::SaveTransactionDataToFile(std::string strFileName)
{

	std::fstream filestream;
	filestream.open(strFileName.c_str(), std::ofstream::out);
	if (filestream.is_open() == true)
	{
		//std::string strTimeStamp = AccountPage::get_time_stamp();
		filestream << "Time" << ",";
		filestream << "Account" << ",";
		filestream << "Transaction" << ",";
		filestream << "Fee/TTCOIN" << ",";
		filestream << "Balance" << ",";
		filestream << "Remarks" << ",";
		filestream << "in/out";

		/*QDateTime time = QDateTime::fromString(str.time_stamp, "yyyy-MM-ddThh:mm:ss");
		time.setTimeZone(QTimeZone::utc());
		time = time.toTimeZone(QTimeZone::systemTimeZone());
		QString currentDateTime = time.toString("yyyy-MM-dd\r\nhh:mm:ss");
		*/
		for (int i = size_CSV - 1; i > -1; i--) {
			filestream << "\n";
			ShowString str;
			str = *(showAccountList_iter_CSV + i);
			QDateTime time = QDateTime::fromString(str.time_stamp, "yyyy-MM-ddThh:mm:ss");
			time.setTimeZone(QTimeZone::utc());
			time = time.toTimeZone(QTimeZone::systemTimeZone());
			QString currentDateTime = time.toString("yyyy-MM-dd-hh:mm:ss");
			qDebug() << "New LIne Date: " << currentDateTime << endl;

			//time
			filestream << currentDateTime.toStdString() << ",";

			//Account
			filestream << str.account.toStdString() << ",";

			//Transaction
			double amount = str.amount.toDouble();
			QString showAmount;
			if (str.reg) {
				showAmount = QString("0");
			}
			else if (str.isOut && !str.self) {
				showAmount = QString("-" + str.amount);
				//showAmount = QString("-" + AccountPage::addingCommasInAmount(str.amount));
				if (showAmount == "-0") {
					showAmount = QString("0");
				}
			}
			else if (!str.self && !str.isOut) {
				showAmount = QString("+" + str.amount);
				//showAmount = QString("+" + AccountPage::addingCommasInAmount(str.amount));
			}
			else {
				showAmount = QString("-/+" + str.amount);
				//showAmount = QString("-/+" + AccountPage::addingCommasInAmount(str.amount));
			}
			//qDebug() << "String Amount------------: " << showAmount << endl;
			filestream << showAmount.toStdString() << ",";
			//filestream << showAmount.toLocal8Bit().constData() << ",";
			//printf(":%s \n", showAmount.toLocal8Bit().constData());

			//fee
			double fee = str.fee.toDouble();
			if (fee > 0.000001) {
				filestream << QString::number(fee).toStdString() << ",";
			}
			else {
				filestream << "----" << ",";
			}

			//balance
			QString balance = str.balance;
			filestream << balance.toStdString() << ",";

			//remarks

			QString remark = str.memo;

			if (remark.isEmpty()) {
				remark = "--";
			}
			filestream << remark.toStdString() << ",";

			if (str.isOut == true)
				filestream << "out";
			else filestream << "in";

		}
		filestream.close();
	}
}
QString discard(const QString &str) {
	int dotPos = str.indexOf(".");

	if (dotPos != -1) {
		return str.left(dotPos + 3);

	}
	else {
		DLOG_QT_WALLET(" no dot!");
		return NULL;
	}
}

AccountPage::~AccountPage() {
	DLOG_QT_WALLET_FUNCTION_BEGIN;
	delete ui;
	DLOG_QT_WALLET_FUNCTION_END;
}

void AccountPage::setFontPixelSize()
{
	QFont font = ui->label_10->font();
	font.setPixelSize(22);
	ui->label_10->setFont(font);

	font = ui->label->font();
	font.setPixelSize(12);
	ui->label->setFont(font);
	ui->addressLabel2->setFont(font);
	ui->label_16->setFont(font);
	ui->label_2->setFont(font);
	ui->label_6->setFont(font);
	ui->label_7->setFont(font);
	ui->label_8->setFont(font);

	font = ui->accountComboBox->font();
	font.setPixelSize(12);
	ui->accountComboBox->setFont(font);

	font = ui->transactionTypeComboBox->font();
	font.setPixelSize(12);
	ui->transactionTypeComboBox->setFont(font);

	font = ui->label_9->font();
	font.setPixelSize(12);
	ui->label_9->setFont(font);
	ui->label_11->setFont(font);
	ui->label_12->setFont(font);
	ui->label_13->setFont(font);
	ui->label_14->setFont(font);
	ui->label_15->setFont(font);

	font = ui->initLabel->font();
	font.setPixelSize(16);
	ui->initLabel->setFont(font);

	font = ui->prePageBtn->font();
	font.setPixelSize(12);
	ui->prePageBtn->setFont(font);
	ui->nextPageBtn->setFont(font);

	font = ui->numberLabel->font();
	font.setPixelSize(12);
	ui->numberLabel->setFont(font);
	ui->pageLabel->setFont(font);

	font = ui->pageLineEdit->font();
	font.setPixelSize(12);
	ui->pageLineEdit->setFont(font);

	font = ui->downloadCSVBtn->font();
	font.setPixelSize(12);
	ui->downloadCSVBtn->setFont(font);

	//font = ui->accountTransactionsTableWidget->font();
	//font.setPixelSize(5);
	//ui->accountTransactionsTableWidget->setFont(font);
}
void AccountPage::init() {
	if (accountName == "") {
		return;
	}

	//ui->label_2->hide();
	//ui->transactionTypeComboBox->hide();
	QString showName = DataMgr::getInstance()->getAccountInfo()->value(accountName).name;
	//showName = showName.left(11) + "...";
	address = DataMgr::getInstance()->getAccountInfo()->value(accountName).address;
	ui->addressLabel->setText(address.left(11) + "...");
	ui->addressLabel->adjustSize();
	ui->copyBtn->move(ui->addressLabel->x() + ui->addressLabel->width() + 9, 45);
	ui->accountComboBox->setCurrentText(accountName);
	/*
	// 如果是已注册账户
	if( Goopal::getInstance()->registerMapValue(accountName) != "NO"
	&& !Goopal::getInstance()->registerMapValue(accountName).isEmpty())
	{
	// 如果是代理
	if( Goopal::getInstance()->delegateAccountList.indexOf(accountName) != -1)
	{
	ui->identityLabel->setPixmap(QPixmap(delegateLabelString));

	//Goopal::getInstance()->postRPC( toJsonFormat( "id_wallet_get_delegate_statue_" + accountName, "wallet_get_delegate_statue", QStringList() << accountName ));
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
}

void AccountPage::updateTransactionsList() {
	DLOG_QT_WALLET_FUNCTION_BEGIN;

	if (accountName.size() > 0) {
		//if (DataMgr::getCurrentAssetType() == COMMONASSET) {
		QString str(DataMgr::getCurrentAssetType());
		//zxltab            DataMgr::getDataMgr()->walletTransactionHistorySpliteWithId(accountName, DataMgr::getCurrentAssetType(), 0, 2);
		DataMgr::getDataMgr()->walletTransactionHistorySpliteWithId(accountName, str, 0, 2);

		//} else {
		//    showTokenTransactions(accountName, DataMgr::getCurrentAssetType());
		//}
	}

	DLOG_QT_WALLET_FUNCTION_END;
}


void AccountPage::refresh() {
	DLOG_QT_WALLET_FUNCTION_BEGIN;
	init();
	updateTransactionsList();
	balance();
	setAssertType();
	DLOG_QT_WALLET_FUNCTION_END;
}

bool isExistInWallet(QString);

void AccountPage::on_copyBtn_clicked() {
	QClipboard* clipBoard = QApplication::clipboard();
	clipBoard->setText(address);
	CommonDialog commonDialog(CommonDialog::OkOnly);
	commonDialog.setText(tr("Copy to clipboard"));
	commonDialog.pop();
}

void AccountPage::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	painter.setPen(QPen(QColor(177, 183, 188), Qt::SolidLine));
	painter.setBrush(QBrush(QColor(229, 229, 229), Qt::SolidPattern));
	painter.drawRect(-1, -1, 828, 92);
	painter.drawLine(QPoint(30, 150), QPoint(796, 150));
}

void AccountPage::on_accountComboBox_currentIndexChanged(const QString &arg1) {
	if (inited) { // 防止accountComboBox当前值改变的时候触发
		accountName = arg1;
		currentPageIndex = 1;
		ui->pageLineEdit->setText(QString::number(currentPageIndex));
		init();
		balance();
		DataMgr::getInstance()->setCurrentAccount(accountName);
		QString key = genCurrentKey();
		if (transactionMap.contains(key))
		{
			qDebug() << "From here------------1" << endl;
			showCommonTransactions(accountName, transactionMap[key]);
		}
		else
			updateTransactionsList();
		//showTransactions(DataMgr::getInstance()->getCurrentTrx()->value(accountName));
	}
}

void AccountPage::retranslator() {
	ui->retranslateUi(this);
	QString language = DataMgr::getInstance()->getSettings()->value("/settings/language").toString();

	if (language.isEmpty() || language == "English") {
		ui->label_3->move(641, 122);
		ui->label_6->move(654, 119);
		ui->label_6->adjustSize();
		ui->label_4->move(706, 122);
		ui->label_7->move(719, 119);
		ui->label_7->adjustSize();
		delegateLabelString = QCoreApplication::applicationDirPath() + "/pic2/delegateLabel_En.png";
		registeredLabelString = QCoreApplication::applicationDirPath() + "/pic2/registeredLabel_En.png";

	}
	else if (language.isEmpty() || language == "Korean")
	{
		ui->label_3->move(601, 122);
		ui->label_6->move(614, 119);
		ui->label_6->adjustSize();
		ui->label_4->move(666, 122);
		ui->label_7->move(679, 119);
		ui->label_7->adjustSize();
		ui->label_5->move(738, 122);
		ui->label_8->move(751, 119);
		ui->label_8->adjustSize();
		delegateLabelString = QCoreApplication::applicationDirPath() + "/pic2/delegateLabel_Ko.png";
		registeredLabelString = QCoreApplication::applicationDirPath() + "/pic2/registeredLabel_Ko.png";
	}
	else {
		ui->label_3->move(651, 122);
		ui->label_6->move(664, 119);
		ui->label_6->adjustSize();
		ui->label_4->move(708, 122);
		ui->label_7->move(721, 119);
		ui->label_7->adjustSize();
		delegateLabelString = QCoreApplication::applicationDirPath() + "/pic2/delegateLabel.png";
		registeredLabelString = QCoreApplication::applicationDirPath() + "/pic2/registeredLabel.png";
	}
}

void AccountPage::balance() {
	if (DataMgr::getInstance()->getAccountInfo()->isEmpty())
	{
		ui->balanceLabel->setText("<body><font style=\"font-size:26px\" color=#000000>0</font><font style=\"font-size:12px\" color=#000000>"
			+ DataMgr::getInstance()->getCurrentAssetType() + "</font></body>");
		ui->balanceLabel->move(772 - ui->balanceLabel->width(), 30);
		ui->label_16->move(706 - ui->balanceLabel->width(), 40);
		return;
	}
	QString amount;

	//if (DataMgr::getInstance()->getCurrentAssetType() == COMMONASSET) {
	//amount = DataMgr::getInstance()->getAccountInfo()->value(accountName).balance;
	amount = DataMgr::getInstance()->walletCurBalanceByAccountname(accountName);
	//amount.chop(4);

	// } else {
	//     amount = DataMgr::getInstance()->getAccountTokenBalance(accountName, DataMgr::getInstance()->getCurrentAssetType());
	//}

	//qDebug() << "Amount............" << amount << endl;
	QString display_amount = AccountPage::addingCommasInAmount(amount);

	ui->balanceLabel->setText("<body><font style=\"font-size:26px\" color=#000000>" + display_amount + "</font><font style=\"font-size:12px\" color=#000000> "
		+ DataMgr::getInstance()->getCurrentAssetType() + "</font></body>");
	ui->balanceLabel->adjustSize();
	ui->balanceLabel->move(782 - ui->balanceLabel->width(), 30);
	ui->label_16->move(716 - ui->balanceLabel->width(), 40);
	return;
}

QString AccountPage::addingCommasInAmount(QString amount)
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

void AccountPage::walletTransactionHistorySpliteWithId(QString id, QString result) {
	qDebug() << "HISTORYYYYYYYYYYYYYYYYYYYYYYYYYYY+++++++++++++++++++++++++++++++" << endl;
	QString commond = "id_wallet_transaction_history_splite_";
	QString account = id.mid(commond.size());

	if (account.size() > 0) {
		DataMgr::getInstance()->getCurrentTrx()->insert(account, result);
		//qDebug() << "Starting From here-----------really" << endl;
		showCommonTransactions(account, DataMgr::getInstance()->getCurrentTrx()->value(account));
	}
}

void AccountPage::on_transactionTypeComboBox_currentIndexChanged(int index) {
	currentPageIndex = 1;
	ui->pageLineEdit->setText(QString::number(currentPageIndex));
	showTransactions(accountName);
}

void AccountPage::showCommonTransactions(QString account, QString result) {
	int type = ui->transactionTypeComboBox->currentIndex();
	qDebug()<< "RESULT: " << result << endl;

	if (result == "\"result\":[]" || result.isEmpty()) {

		//qDebug() << "From here-----------really-1" << endl;
		ui->accountTransactionsTableWidget->setRowCount(0);
		ui->prePageBtn->hide();
		ui->numberLabel->hide();
		ui->pageLineEdit->hide();
		ui->pageLabel->hide();
		ui->nextPageBtn->hide();
		ui->downloadCSVBtn->setEnabled(false);
		ui->transactionTypeComboBox->setEnabled(false);

		if (result == "\"result\":[]") {
			ui->initLabel->show();

		}
		else {
			ui->initLabel->hide();
		}

		return;
	}

	QVector <TrxResult> trxResult;
	DataMgr::getInstance()->parseTrxResultJson(QString("{") + result + QString("}"), trxResult);
	ui->prePageBtn->show();
	ui->numberLabel->show();
	ui->pageLineEdit->show();
	ui->pageLabel->show();
	ui->nextPageBtn->show();
	ui->initLabel->hide();


	/*struct ShowString {
		QString time_stamp;
		QString account;
		QString amount;
		QString fee;
		QString balance;
		QString memo;
		bool isOut;
		bool self;
		bool con;
		bool reg;
		bool isabolished;
		};
		*/
	QVector <ShowString> showAccountList;
	showAccountList_CSV.clear();
	showAccountList_real_CSV.clear();

	//qDebug() << "From here-----------really-21" << endl;
	QString toaccount;
	foreach(TrxResult trx, trxResult) {
		ui->downloadCSVBtn->setEnabled(true);
		ui->transactionTypeComboBox->setEnabled(true);
		//qDebug() << "From here-----------really-2" << endl;
		ShowString showString = { "--", "--", "--", "--", "--", "--", false, false, false, false, false };
		showString.time_stamp = trx.time_stamp;

		if (trx.entries.at(0).memo == "withdraw exec cost") {
			if (trx.entries.at(0).to_account.isEmpty()) {
				showString.con = true;
			}
		}

		if (trx.abolished)
		{
			showString.isabolished = true;
		}

		//add 
		double totalamount = 0;
		foreach(LedgerEntries ledger, trx.entries) {

			toaccount = DataMgr::getInstance()->getAddrAccont(ledger.to_account);
			if (ledger.from_account == account || ledger.to_account == account || toaccount == account) {

				if (ledger.from_account == account) {
					showString.account = toaccount;

					if (showString.account.isEmpty()) {
						showString.account = ledger.to_account;
					}

					//if (showString.con) {
					showString.amount = ledger.amount.amount_div_precision;//QString::number(ledger.amount.amount, 'f', 5);
					//} //else {
					//	showString.amount = QString::number(ledger.amount.amount - 1000, 'f', 5);
					//}
					if (trx.entries.size() > 1 && showString.amount == trx.fee.amount_div_precision)
					{
						showString.amount = "0";
						continue;
					}
					showString.isOut = true;

					//mod zxj
					if (trx.entries.size() > 3)
					{
						continue;
					}
				}
				else {

					//mod zxj
					if (DataMgr::getInstance()->getCurrentAssetTypeID() != ledger.amount.asset_id)
					{
						continue;
					}

					showString.amount = ledger.amount.amount_div_precision; //QString::number(ledger.amount.amount, 'f', 5);

					showString.account = DataMgr::getInstance()->getAddrAccont(ledger.from_account);

					if (showString.account.isEmpty()) {
						showString.account = ledger.from_account;
					}

					showString.isOut = false;
				}

				if (showString.account == account)
				{
					showString.self = true;
				}

				if (trx.entries.size() > 2 && !showString.isOut)
				{
					if (DataMgr::getInstance()->getCurrentAssetTypeID() == ledger.amount.asset_id)
					{
						totalamount += ledger.amount.amount_div_precision.toDouble();
						showString.amount = QString::number(totalamount, 'f', 5);
					}
					continue;
				}

				if (trx.entries.size() > 1 && showString.amount == trx.fee.amount_div_precision)
				{
					showString.self = false;
					continue;
				}

				break;
			}
		}

		if (!showString.isOut && trx.entries.size() == 2 && !trx.entries.at(0).from_account.isEmpty())
			showString.amount = trx.entries.at(1).amount.amount_div_precision;//QString::number(trx.entries.at(1).amount.amount, 'f', 5);
		//mod zxj
		if (trx.entries.size() == 3)
		{
			if (showString.isOut)
			{
				if (DataMgr::getInstance()->getCurrentAssetType() == "TTCOIN" && trx.entries.at(2).memo.startsWith("to Contract"))
				{
					showString.amount = "0";
				}
				else
				{
					if (showString.amount == trx.fee.amount_div_precision)
						showString.amount = trx.entries.at(1).amount.amount_div_precision;
				}
			}
		}



		showString.balance = trx.entries.at(trx.entries.size() - 1).running_balances;
		//mod zxj
		if (trx.entries.size() > 1 && trx.entries.at(trx.entries.size() - 1).amount.asset_id != DataMgr::getInstance()->getCurrentAssetTypeID())
		{
			showString.balance = trx.entries.at(trx.entries.size() - 2).running_balances;
		}

		showString.memo = trx.entries.at(0).memo;
		showString.fee = trx.fee.amount_div_precision;//QString::number(trx.fee.amount, 'f', 5);

		if (showString.con) {
			showString.memo = tr("withdraw exec cost");
			//mod zxj
			if (showString.fee != "0.01")
			{
				showString.fee = "--";
			}
			showString.account = "CON";
		}
		else if (showString.memo.startsWith("register")) {
			if (showString.account == account && trx.entries.at(0).amount.amount == 0) {
				//mod zxj
				//showString.balance = checkZero(showString.balance.toDouble() - 1000);
				showString.reg = true;
			}
		}

		showAccountList.append(showString);
		showAccountList_CSV.append(showString);
	}

	QVector <ShowString> showAccountList_real;
	QVector <ShowString>::const_iterator showAccountList_iter;
	int size = 0;
	size_CSV = 0;

	switch (type) {
	case 0:     // 显示全部交易
		size = showAccountList.size();
		size_CSV = showAccountList_CSV.size();
		showAccountList_iter = showAccountList.constBegin();
		showAccountList_iter_CSV = showAccountList_CSV.constBegin();
		break;

	case 1:     // 显示转入交易
		foreach(ShowString ledger, showAccountList) {
			if (!ledger.isOut || ledger.self)
			{
				showAccountList_real.append(ledger);
				showAccountList_real_CSV.append(ledger);
			}

		}

		size = showAccountList_real.size();
		size_CSV = showAccountList_real_CSV.size();
		showAccountList_iter = showAccountList_real.constBegin();
		showAccountList_iter_CSV = showAccountList_real_CSV.constBegin();
		break;

	case 2:     // 显示转出交易
		foreach(ShowString ledger, showAccountList) {
			if (ledger.isOut || ledger.self)
			{
				showAccountList_real.append(ledger);
				showAccountList_real_CSV.append(ledger);

			}
		}

		size = showAccountList_real.size();
		size_CSV = showAccountList_real_CSV.size();
		showAccountList_iter = showAccountList_real.constBegin();
		showAccountList_iter_CSV = showAccountList_real_CSV.constBegin();
		break;

	default:
		break;
	}

	ui->numberLabel->setText(tr("total ") + QString::number(size) + tr(" ,"));
	ui->pageLabel->setText("/" + QString::number((size - 1) / 10 + 1));

	//qDebug() << "-----------------Size----------------";
	//qDebug() << "Size: " << size_CSV;
	//qDebug() << "Size: " << size;

	int rowCount = size - (currentPageIndex - 1) * 10;

	if (rowCount > 10)
		rowCount = 10;  // 一页最多显示10行

	ui->accountTransactionsTableWidget->setRowCount(rowCount);
	//  ui->accountTransactionsTableWidget->clear();

	for (int i = rowCount - 1; i > -1; i--) {
		ShowString str;
		int next_ledger = size - (i + 1) - (currentPageIndex - 1) * 10;
		str = *(showAccountList_iter + next_ledger);
		ui->accountTransactionsTableWidget->setRowHeight(i, 57);

		QTableWidgetItem* item;
		item = ui->accountTransactionsTableWidget->item(i, 1);
		if (item != NULL)
			delete item;
		item = new QTableWidgetItem(str.account);
		ui->accountTransactionsTableWidget->setItem(i, 1, item);

		item->setText(str.account);
		if (str.account.mid(0, 3) != "TTC") {
			item->setTextColor(QColor(64, 154, 255));
		}

		// 时间
		QDateTime time = QDateTime::fromString(str.time_stamp, "yyyy-MM-ddThh:mm:ss");
		time.setTimeZone(QTimeZone::utc());
		time = time.toTimeZone(QTimeZone::systemTimeZone());
		QString currentDateTime = time.toString("yyyy-MM-dd\r\nhh:mm:ss");
		QTableWidgetItem* time_item;
		time_item = ui->accountTransactionsTableWidget->item(i, 0);
		if (time_item != NULL)
			delete time_item;
		time_item = new QTableWidgetItem(currentDateTime);
		ui->accountTransactionsTableWidget->setItem(i, 0, time_item);

		// 手续费
		double fee = str.fee.toDouble();
		QTableWidgetItem* fee_item;
		fee_item = ui->accountTransactionsTableWidget->item(i, 3);
		if (fee_item != NULL)
			delete fee_item;
		if (fee > 0.000001) {
			fee_item = new QTableWidgetItem(str.fee);
			ui->accountTransactionsTableWidget->setItem(i, 3, fee_item);

		}
		else {
			fee_item = new QTableWidgetItem("--");
			ui->accountTransactionsTableWidget->setItem(i, 3, fee_item);
		}

		// 金额
		double amount = str.amount.toDouble();
		QTableWidgetItem* amount_item;
		QString showAmount;
		amount_item = ui->accountTransactionsTableWidget->item(i, 2);
		if (amount_item != NULL)
			delete amount_item;
		amount_item = new QTableWidgetItem;


		if (str.reg) {
			showAmount = QString("0");
			amount_item->setTextColor(QColor(255, 80, 63));
		}
		else if (str.isOut && !str.self) {
			showAmount = QString("-" + AccountPage::addingCommasInAmount(str.amount));
			if (showAmount == "-0") {
				showAmount = QString("0");
			}
			amount_item->setTextColor(QColor(255, 80, 63));
		}
		else if (!str.self && !str.isOut) {
			showAmount = QString("+" + AccountPage::addingCommasInAmount(str.amount));
			amount_item->setTextColor(QColor(71, 178, 156));
		}
		else {
			showAmount = QString("-/+" + AccountPage::addingCommasInAmount(str.amount));
			amount_item->setTextColor(QColor(255, 80, 63));
		}
		amount_item->setText(showAmount);
		ui->accountTransactionsTableWidget->setItem(i, 2, amount_item);

		QString balance = AccountPage::addingCommasInAmount(str.balance);
		// 如果是自己转自己的 支出部分 余额里面把 交易金额扣除
		//balance = checkZero(balance.toDouble() / 100000);
		QTableWidgetItem* balance_item;
		balance_item = ui->accountTransactionsTableWidget->item(i, 4);
		if (balance_item != NULL)
			delete balance_item;
		balance_item = new QTableWidgetItem(balance);
		if (showAmount.size() > 10) {
			showAmount.insert(showAmount.indexOf('.'), '\n');
		}

		ui->accountTransactionsTableWidget->setItem(i, 4, balance_item);
		// 备注
		QString remark = str.memo;

		if (remark.isEmpty()) {
			remark = "--";
		}
		QTableWidgetItem* remark_item;
		remark_item = ui->accountTransactionsTableWidget->item(i, 5);
		if (remark_item != NULL)
			delete remark_item;
		remark_item = new QTableWidgetItem(remark);
		ui->accountTransactionsTableWidget->setItem(i, 5, remark_item);

		for (int j = 0; j < ui->accountTransactionsTableWidget->columnCount(); j++)
		{
			QTableWidgetItem *item = ui->accountTransactionsTableWidget->item(i, j);
			if (item)
			{
				if (str.isabolished && !str.memo.startsWith("claim"))
				{
					item->setSelected(true);
				}
				else
				{
					item->setSelected(false);
				}
			}
		}
	}
	//缓存结果，以便提高刷新速度
	transactionMap[genCurrentKey()] = result;
}

void AccountPage::showTokenTransactions(QString accountName, QString asset) {
	foreach(ContractFromServer tokenCon, DataMgr::getInstance()->getContractConfig()) {
		if (tokenCon.name == asset) {
			QString straddress = DataMgr::getInstance()->getAccountInfo()->value(accountName).address;
			DataMgr::getInstance()->getTokenTrxConnect()->connectToBlockBrower(straddress, tokenCon.addr, currentPageIndex);
			break;
		}
	}

	connect(DataMgr::getInstance()->getTokenTrxConnect(), &TokenTransaction::tokenTrxRequestEnd, this, &AccountPage::displayTokenTrxVector);
}
void AccountPage::displayTokenTrxVector() {
	int i = 0;
	//  ui-> DataMgr::getInstance()->getTokenTrxConnect()->trxvector.totaRecords;
	int size = DataMgr::getInstance()->getTokenTrxConnect()->trxvector.totaRecords;
	int currentPage = DataMgr::getInstance()->getTokenTrxConnect()->trxvector.currentPage;
	int totalPage = DataMgr::getInstance()->getTokenTrxConnect()->trxvector.totalPage;
	ui->prePageBtn->show();
	ui->numberLabel->show();
	ui->pageLineEdit->show();
	ui->pageLabel->show();
	ui->nextPageBtn->show();
	ui->initLabel->hide();
	ui->numberLabel->setText(tr("total ") + QString::number(size) + tr(" ,"));
	ui->pageLineEdit->setText(QString::number(currentPage));
	ui->accountTransactionsTableWidget->setRowCount(DataMgr::getInstance()->getTokenTrxConnect()->trxvector.trx.size());
	ui->pageLabel->setText("/" + QString::number(totalPage));

	foreach(Transaction t, DataMgr::getInstance()->getTokenTrxConnect()->trxvector.trx) {
		ui->accountTransactionsTableWidget->setRowHeight(i, 57);
		QString fromAddr = t.from_addr;
		QString toAddr = t.to_addr;
		QString fromAccount = DataMgr::getInstance()->getAddrAccont(fromAddr);
		QString toAccount = DataMgr::getInstance()->getAddrAccont(toAddr);
		QString current_addr = DataMgr::getInstance()->getAccountAddr(accountName);
		QString amount = t.amount;
		QDateTime time = QDateTime::fromString(t.trx_time, "yyyy-MM-dd hh:mm:ss");
		time.setTimeZone(QTimeZone::utc());
		time = time.toTimeZone(QTimeZone::systemTimeZone());
		QString currentDateTime = time.toString("yyyy-MM-dd\r\nhh:mm:ss");
		ui->accountTransactionsTableWidget->setItem(i, 0, new QTableWidgetItem(currentDateTime));
		QTableWidgetItem* item;
		bool isOut = false;
		bool isSelf = false;

		if ((fromAddr == toAddr) && (fromAddr == current_addr)) {
			isSelf = true;
		}

		if (fromAddr != current_addr) {
			// 如果 fromaccount 不为本账户，则 为对方账户
			if (fromAccount.isEmpty()) {
				item = new QTableWidgetItem(fromAddr);

			}
			else {
				item = new QTableWidgetItem(fromAccount);
				item->setTextColor(QColor(64, 154, 255));
			}

		}
		else {
			// 如果 fromaccount 为本账户， 则toaccount  为对方账户
			if (toAccount.isEmpty()) {
				item = new QTableWidgetItem(toAddr);

			}
			else {
				item = new QTableWidgetItem(toAccount);
				item->setTextColor(QColor(64, 154, 255));
			}

			isOut = true;
		}

		ui->accountTransactionsTableWidget->setItem(i, 1, item);
		QString showAmount;

		//make a function getTableWidgetItem(QStirng& showStrign, QFont& font, QColor&);
		if (isSelf) {
			//showAmount = QString("-/+" + checkZero(amount.toDouble()));
			showAmount = QString("-/+" + AccountPage::addingCommasInAmount(checkZero(amount.toDouble())));
			QTableWidgetItem* item = new QTableWidgetItem(showAmount);
			item->setTextColor(QColor(255, 80, 63));
			ui->accountTransactionsTableWidget->setItem(i, 2, item);

			//ui->accountTransactionsTableWidget->setItem(i, 4, new QTableWidgetItem(checkZero(t.tobalance.toDouble())));
			ui->accountTransactionsTableWidget->setItem(i, 4, new QTableWidgetItem(AccountPage::addingCommasInAmount(checkZero(t.tobalance.toDouble()))));

		}
		else {
			// amount
			if (isOut) {
				//showAmount = QString("-" + checkZero(amount.toDouble()));
				showAmount = QString("-" + AccountPage::addingCommasInAmount(checkZero(amount.toDouble())));
				QTableWidgetItem* item = new QTableWidgetItem(showAmount);
				item->setTextColor(QColor(255, 80, 63));
				ui->accountTransactionsTableWidget->setItem(i, 2, item);
				//ui->accountTransactionsTableWidget->setItem(i, 4, new QTableWidgetItem(checkZero(t.frombalance.toDouble())));
				ui->accountTransactionsTableWidget->setItem(i, 4, new QTableWidgetItem(AccountPage::addingCommasInAmount(checkZero(t.frombalance.toDouble()))));

			}
			else {
				//showAmount = QString("+" + checkZero(amount.toDouble()));
				showAmount = QString("+" + AccountPage::addingCommasInAmount(checkZero(amount.toDouble())));
				QTableWidgetItem* item = new QTableWidgetItem(showAmount);
				item->setTextColor(QColor(71, 178, 156));
				ui->accountTransactionsTableWidget->setItem(i, 2, item);
				//ui->accountTransactionsTableWidget->setItem(i, 4, new QTableWidgetItem(checkZero(t.tobalance.toDouble())));
				ui->accountTransactionsTableWidget->setItem(i, 4, new QTableWidgetItem(AccountPage::addingCommasInAmount(checkZero(t.tobalance.toDouble()))));

			}
		}

		//ui->accountTransactionsTableWidget->setItem(i,5,new QTableWidgetItem("--"));
		if (isOut) {
			ui->accountTransactionsTableWidget->setItem(i, 3, new QTableWidgetItem(checkZero(t.fee.toDouble()))); //fee

		}
		else {
			ui->accountTransactionsTableWidget->setItem(i, 3, new QTableWidgetItem("--")); //fee
		}

		if (t.memo.isEmpty()) {
			ui->accountTransactionsTableWidget->setItem(i, 5, new QTableWidgetItem("--"));

		}
		else {
			ui->accountTransactionsTableWidget->setItem(i, 5, new QTableWidgetItem(t.memo));
		}

		i++;
	}
}
void AccountPage::showTransactions(QString) {
	//if (DataMgr::getCurrentAssetType() == COMMONASSET) {
	showCommonTransactions(accountName, DataMgr::getInstance()->getCurrentTrx()->value(accountName));

	//} else {
	//    DataMgr::getInstance()->getTokenTrxConnect()->connectToBlockBrower(accountName, DataMgr::getCurrentAssetType(), currentPageIndex);
	//}
}
void AccountPage::on_accountTransactionsTableWidget_cellClicked(int row, int column) {
	/*if( column == 1 ) {
		ShowContentDialog showContentDialog( ui->accountTransactionsTableWidget->item(row, column)->text(),this);
		int scrollBarValue = ui->accountTransactionsTableWidget->verticalScrollBar()->sliderPosition();
		showContentDialog.move( ui->accountTransactionsTableWidget->mapToGlobal( QPoint(120, 57 * (row - scrollBarValue) - 3)));
		showContentDialog.exec();

		return;
		}

		if( column == 5) {

		QString remark = ui->accountTransactionsTableWidget->item(row, column)->text();
		remark.remove(' ');
		if( remark.isEmpty() )  return;
		ShowContentDialog showContentDialog( ui->accountTransactionsTableWidget->item(row, column)->text(),this);
		int scrollBarValue = ui->accountTransactionsTableWidget->verticalScrollBar()->sliderPosition();
		int length = ui->accountTransactionsTableWidget->item(row, column)->text().length();
		if (ui->accountTransactionsTableWidget->item(row, column)->text() == "--")
		{
		showContentDialog.hide();
		}
		else if (ui->accountTransactionsTableWidget->item(row, column)->text().length() > 30)
		{
		showContentDialog.move(ui->accountTransactionsTableWidget->mapToGlobal(QPoint(595 - ui->accountTransactionsTableWidget->item(row, column)->text().length() * 5,
		57 * (row - scrollBarValue) - 3)));
		}
		else
		{
		showContentDialog.move(ui->accountTransactionsTableWidget->mapToGlobal(QPoint(630 - ui->accountTransactionsTableWidget->item(row, column)->text().length() * 5,
		57 * (row - scrollBarValue) - 3)));
		}
		showContentDialog.exec();
		return;
		}*/
}
void AccountPage::on_prePageBtn_clicked() {
	ui->accountTransactionsTableWidget->scrollToTop();

	if (currentPageIndex == 1)
		return;

	currentPageIndex--;
	showTransactions(DataMgr::getInstance()->getCurrentTrx()->value(accountName));
	ui->pageLineEdit->setText(QString::number(currentPageIndex));

	//if (DataMgr::getCurrentAssetType() != COMMONASSET) {
	//    ui->prePageBtn->hide();
	//    ui->numberLabel->hide();
	//    ui->pageLineEdit->hide();
	//    ui->pageLabel->hide();
	//    ui->nextPageBtn->hide();
	//}
}
void AccountPage::on_nextPageBtn_clicked() {
	//    if( currentPageIndex >=  ((searchList.size() - 1)/10 + 1))  return;
	int totalPageNum = ui->pageLabel->text().remove("/").toInt();

	if (currentPageIndex >= totalPageNum)  return;

	currentPageIndex++;
	showTransactions(DataMgr::getInstance()->getCurrentTrx()->value(accountName));
	ui->pageLineEdit->setText(QString::number(currentPageIndex));
	ui->accountTransactionsTableWidget->scrollToTop();

	//if (DataMgr::getCurrentAssetType() != COMMONASSET) {
	//    ui->prePageBtn->hide();
	//    ui->numberLabel->hide();
	//    ui->pageLineEdit->hide();
	//    ui->pageLabel->hide();
	//    ui->nextPageBtn->hide();
	//}
}
void AccountPage::on_pageLineEdit_editingFinished() {
	if (ui->pageLineEdit->text().isEmpty()) {
		ui->pageLineEdit->setText(QString::number(currentPageIndex));
	}

	currentPageIndex = ui->pageLineEdit->text().toInt();
}
void AccountPage::on_pageLineEdit_textEdited(const QString &arg1) {
	if (arg1.isEmpty()) {
		return;
	}

	if (arg1.at(0) == '0') {
		ui->pageLineEdit->setText(arg1.mid(1));
		return;
	}

	int totalPageNum = ui->pageLabel->text().remove("/").toInt();

	if (arg1.toInt() > totalPageNum) {
		ui->pageLineEdit->setText(QString::number(totalPageNum));
		return;
	}
}
void AccountPage::setAssertType() {
	QString curText = ui->label_12->text();
	int sep = curText.indexOf('/');

	if (sep > 0) { //根据资产类表头型设置
		ui->label_12->setText(curText.left(sep + 1) + DataMgr::getInstance()->getCurrentAssetType());
	}

	curText = ui->label_14->text();
	sep = curText.indexOf('/');

	if (sep > 0) { //根据资产类表头型设置
		ui->label_14->setText(curText.left(sep + 1) + DataMgr::getInstance()->getCurrentAssetType());
	}
}

QString AccountPage::genCurrentKey()
{
	return "accountname:" + accountName + "assetid:" + QString("%1").arg(DataMgr::getInstance()->getCurrentAssetTypeID());
}
