#ifndef ACCOUNTPAGE_H
#define ACCOUNTPAGE_H

#include <QWidget>
#include <qmap.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <Lmcons.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#endif

struct ShowString {
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


namespace Ui {
class AccountPage;
}

#define MODULE_ACCOUNT_PAGE "ACCOUNT_PAGE"

class AccountPage : public QWidget
{
    Q_OBJECT

public:
    explicit AccountPage(QString name,QWidget *parent = 0);
    ~AccountPage();

    void updateTransactionsList();
	QVector <ShowString>::const_iterator showAccountList_iter_CSV;
	QVector <ShowString> showAccountList_real_CSV;
	QVector <ShowString> showAccountList_CSV;
	int size_CSV = 0;
	void SaveTransactionDataToFile(std::string strTemp);

	std::string get_file_extension(const std::string& strFileName);
	std::string get_time_stamp();
	QString addingCommasInAmount(QString amount);

public slots:
    void refresh();
    void retranslator();
	void displayTokenTrxVector();
	void slot_DownloadCSV();


private slots:

    void on_copyBtn_clicked();

    void on_accountComboBox_currentIndexChanged(const QString &arg1);

    void on_transactionTypeComboBox_currentIndexChanged(int index);

    void on_accountTransactionsTableWidget_cellClicked(int row, int column);

    void on_prePageBtn_clicked();

    void on_nextPageBtn_clicked();

    void on_pageLineEdit_editingFinished();

    void on_pageLineEdit_textEdited(const QString &arg1);

	void balance();
	void walletTransactionHistorySpliteWithId(QString, QString);

signals:
    void back();
    void showUpgradePage(QString);
    void accountChanged(QString);
    void showShadowWidget();
    void hideShadowWidget();
    void showApplyDelegatePage(QString);

private:
    Ui::AccountPage *ui;
    QString accountName;
    QString address;
    int transactionType;
    bool inited;
    int currentPageIndex;
    QString delegateLabelString;
    QString registeredLabelString;
	QMap<QString, QString> transactionMap;
    void paintEvent(QPaintEvent*);
    void init();
    void showCommonTransactions(QString, QString);
    void showTokenTransactions(QString, QString);

    void showTransactions(QString);

	void setFontPixelSize();
    void setAssertType();
	QString genCurrentKey();

};

#endif // ACCOUNTPAGE_H
