#ifndef BulkSending_H
#define BulkSending_H

#include <QWidget>
#include <qfiledialog.h>
#include <QStandardItemModel>
#include <qvector.h>
#include <qprogressbar.h>
#include <QtConcurrent\qtconcurrentrun.h>
#include <qfuturewatcher.h>
#include <fstream>

#define ENABLE_MULTIPLE_FILES_SUPPORT
#undef ENABLE_MULTIPLE_FILES_SUPPORT

namespace Ui {
class BulkSending;
}

class WaitingPage;
class FunctionBar;

#define MODULE_TRAHSFER_PAGE "TRAHSFER_PAGE"

class BulkSending : public QWidget
{
    Q_OBJECT

public:
	explicit BulkSending(QString name,QWidget *parent = 0);
    ~BulkSending();


    QString getCurrentAccount();
    void setAddress(QString);
	void setbalanceLabel(QString balance, QString token);
	void showBalance();
	QString addingCommasToAmount_BulkSending(QString amount);
	void ParsingFiles(QString fileName);
	void DisplayingValuesOnTableWidget(bool status=false);
	void walletTransferToAddress(QString id, QString result);
	void startingWaitingDialog();
	void startingValidatingDialog();
	void SaveTransactionDataToFile();
	std::string get_file_extension(const std::string& strFileName);
	void insertingAccountsComboBox();

signals:
    void accountChanged(QString);
    void showShadowWidget();
    void hideShadowWidget();
    void showAccountPage(QString);
    void back();
	void signal_validationStarted(int val);
	void signal_validationFinished();
	void signal_progressStarted(int val);
	void signal_progressStopped();

public slots:
    void refresh();
	void tokenTransferTo(QString result);
	void checkString(QString &temp, QChar character = 0 );
	void slot_calculatingProgressValue(int val);
	void slot_stoppingProgressValue();
	void slot_startedValidating(int val);

private slots:
    void on_accountComboBox_currentIndexChanged(const QString &arg1);
	//void on_bulkSending_amountLineEdit_textChanged(const QString &arg1);
	void on_bulkSending_sendBtn_clicked();
	void addressChecked(QString result);
	void on_pushButton_loadCSVFile_clicked();
	void on_pushButton_ValidateAddresses_clicked();
	void slot_validationFinished();

private:
    Ui::BulkSending *ui;
    QString accountName;
    QStringList contactsList;
	QString assetFeeAmount = "0.0";
	int TransactionCount = 0;
    bool inited;
    QString delegateLabelString;
    QString registeredLabelString;
    WaitingPage* waitingPage;
	int nPrecision;
	int min_transaction_fee;
	int max_transaction_fee;
	double transaction_fee_ratio;
	QStandardItemModel *model;
	QList<QStandardItem*> standardItemList;
	QProgressBar progressBar;
	QProgressBar progressBar_Validating;

    void paintEvent(QPaintEvent*);
	int transfer_time;

	void setFontPixelSize();

    void setAssertType();

	QList<QString> csvFileList;
	int totalRowsCount;
	double totalAmountToSend;
	double totalFee;
	int totalAccounts;
	QString fileName;
	int index;
	int false_accounts, false_amount;
	int totalTransactions;
	FunctionBar* functionbar;
};

#endif // BulkSending_H
