#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>
#include "commondialog.h"

class CommonDialog;

namespace Ui {
class RenameDialog;
}

class RenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDialog(QString name, QWidget *parent = 0);
    ~RenameDialog();

    QString pop();

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

    //void on_nameLineEdit_textChanged(const QString &arg1);

	void slot_chkBtn();

    void on_nameLineEdit_returnPressed();

    void jsonDataUpdated(QString id);

	void accountRenamed(QString result);

private:
    Ui::RenameDialog *ui;
    bool yesOrNO;
    QMovie* gif;
	void setFontPixelSize();
	QString accountName;
};

#endif // RENAMEDIALOG_H
