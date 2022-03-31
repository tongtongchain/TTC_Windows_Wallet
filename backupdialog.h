#ifndef BACKUPDIALOG_H
#define BACKUPDIALOG_H

#include <QWidget>
#include <qfiledialog.h>
#include <qmessagebox.h>

#define USE_CLOUD_DB
#undef USE_CLOUD_DB

#ifdef USE_CLOUD_DB
class CrystalDB;
#endif

namespace Ui {
class BackUpDialog;
}

class WaitingPage;

class  BackUpDialog : public QWidget
{
    Q_OBJECT

public:
	explicit  BackUpDialog(QWidget *parent = 0);
	~BackUpDialog();

	void setFontPixelSize_backUp();

	private slots:

	void on_lineEdit_walletName_textChanged(const QString &arg1);
	void on_lineEdit_walletName_bkp_textChanged(const QString &arg1);
	void on_pushButton_saveLocation_clicked();
	void on_pushButton_walletLocation_clicked();
	void on_pushButton_loadFile_bkp_clicked();
	void on_pushButton_saveWallet_bkp_clicked();
	QString getSystemEnvironmentPath();
	void on_pushButton_backup_clicked();
	void BackupCreated(QString result);
	void BackupRestored(QString result);
	void on_pushButton_restore_clicked();

private:
	Ui::BackUpDialog *ui;
	WaitingPage* waitingPage;
	QString delegateLabelString;
	QString registeredLabelString;
	void paintEvent(QPaintEvent*);
	QString _app_data_path;
	QString _app_work_path;
	QString _tool_config_path;
	QString filePath;
#ifdef USE_CLOUD_DB
	CrystalDB* cloudDB;
#endif
	
    
};

#endif // BACKUPDIALOG_H
