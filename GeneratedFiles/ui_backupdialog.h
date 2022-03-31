/********************************************************************************
** Form generated from reading UI file 'backupdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BACKUPDIALOG_H
#define UI_BACKUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BackUpDialog
{
public:
    QLabel *label_walletBackUp;
    QLineEdit *lineEdit_walletName;
    QLabel *label_walletName;
    QLineEdit *lineEdit_saveLocation;
    QLabel *label_saveLocation;
    QLabel *label_fileName;
    QLineEdit *lineEdit_fileName;
    QLineEdit *lineEdit_walletLocation;
    QLabel *label_walletLocation;
    QToolButton *pushButton_saveLocation;
    QToolButton *pushButton_backup;
    QToolButton *pushButton_walletLocation;
    QLabel *label_tipWalletName;
    QLabel *label;
    QLineEdit *lineEdit_saveWallet_bkp;
    QLabel *label_walletLocation_2;
    QLineEdit *lineEdit_walletName_bkp;
    QToolButton *pushButton_restore;
    QLineEdit *lineEdit_passWord_bkp;
    QLabel *label_walletName_2;
    QLabel *label_saveLocation_2;
    QToolButton *pushButton_loadFile_bkp;
    QToolButton *pushButton_saveWallet_bkp;
    QLabel *label_fileName_2;
    QLabel *label_tipWalletName_bkp;
    QLineEdit *lineEdit_loadFile_bkp;
    QLabel *label_walletBackUp_2;
    QCheckBox *checkBox_cloudBackup;
    QLabel *label_walletLocation_3;
    QLabel *label_walletLocation_4;
    QCheckBox *checkBox_cloudRestore;

    void setupUi(QWidget *BackUpDialog)
    {
        if (BackUpDialog->objectName().isEmpty())
            BackUpDialog->setObjectName(QStringLiteral("BackUpDialog"));
        BackUpDialog->resize(827, 525);
        QFont font;
        font.setFamily(QStringLiteral("Microsoft YaHei"));
        BackUpDialog->setFont(font);
        label_walletBackUp = new QLabel(BackUpDialog);
        label_walletBackUp->setObjectName(QStringLiteral("label_walletBackUp"));
        label_walletBackUp->setGeometry(QRect(110, 40, 171, 21));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(18);
        font1.setBold(true);
        font1.setWeight(75);
        label_walletBackUp->setFont(font1);
        label_walletBackUp->setStyleSheet(QStringLiteral("color: rgb(40, 40, 40);"));
        lineEdit_walletName = new QLineEdit(BackUpDialog);
        lineEdit_walletName->setObjectName(QStringLiteral("lineEdit_walletName"));
        lineEdit_walletName->setGeometry(QRect(140, 120, 241, 30));
        lineEdit_walletName->setFont(font);
        lineEdit_walletName->setFocusPolicy(Qt::ClickFocus);
        lineEdit_walletName->setContextMenuPolicy(Qt::DefaultContextMenu);
        label_walletName = new QLabel(BackUpDialog);
        label_walletName->setObjectName(QStringLiteral("label_walletName"));
        label_walletName->setGeometry(QRect(10, 118, 151, 30));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(12);
        label_walletName->setFont(font2);
        label_walletName->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEdit_saveLocation = new QLineEdit(BackUpDialog);
        lineEdit_saveLocation->setObjectName(QStringLiteral("lineEdit_saveLocation"));
        lineEdit_saveLocation->setGeometry(QRect(140, 240, 171, 30));
        lineEdit_saveLocation->setFont(font);
        lineEdit_saveLocation->setFocusPolicy(Qt::ClickFocus);
        lineEdit_saveLocation->setContextMenuPolicy(Qt::DefaultContextMenu);
        label_saveLocation = new QLabel(BackUpDialog);
        label_saveLocation->setObjectName(QStringLiteral("label_saveLocation"));
        label_saveLocation->setGeometry(QRect(10, 240, 131, 30));
        label_saveLocation->setFont(font2);
        label_saveLocation->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_fileName = new QLabel(BackUpDialog);
        label_fileName->setObjectName(QStringLiteral("label_fileName"));
        label_fileName->setGeometry(QRect(10, 180, 131, 30));
        label_fileName->setFont(font2);
        label_fileName->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEdit_fileName = new QLineEdit(BackUpDialog);
        lineEdit_fileName->setObjectName(QStringLiteral("lineEdit_fileName"));
        lineEdit_fileName->setGeometry(QRect(140, 180, 241, 30));
        lineEdit_fileName->setFont(font);
        lineEdit_fileName->setFocusPolicy(Qt::ClickFocus);
        lineEdit_fileName->setContextMenuPolicy(Qt::DefaultContextMenu);
        lineEdit_walletLocation = new QLineEdit(BackUpDialog);
        lineEdit_walletLocation->setObjectName(QStringLiteral("lineEdit_walletLocation"));
        lineEdit_walletLocation->setGeometry(QRect(140, 300, 171, 30));
        lineEdit_walletLocation->setFont(font);
        lineEdit_walletLocation->setFocusPolicy(Qt::ClickFocus);
        lineEdit_walletLocation->setContextMenuPolicy(Qt::DefaultContextMenu);
        label_walletLocation = new QLabel(BackUpDialog);
        label_walletLocation->setObjectName(QStringLiteral("label_walletLocation"));
        label_walletLocation->setGeometry(QRect(10, 300, 111, 30));
        label_walletLocation->setFont(font2);
        label_walletLocation->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        pushButton_saveLocation = new QToolButton(BackUpDialog);
        pushButton_saveLocation->setObjectName(QStringLiteral("pushButton_saveLocation"));
        pushButton_saveLocation->setGeometry(QRect(320, 240, 61, 30));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font3.setPointSize(9);
        pushButton_saveLocation->setFont(font3);
        pushButton_saveLocation->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_saveLocation->setFocusPolicy(Qt::NoFocus);
        pushButton_saveLocation->setStyleSheet(QStringLiteral(""));
        pushButton_backup = new QToolButton(BackUpDialog);
        pushButton_backup->setObjectName(QStringLiteral("pushButton_backup"));
        pushButton_backup->setGeometry(QRect(140, 400, 241, 36));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font4.setPointSize(12);
        font4.setBold(true);
        font4.setWeight(75);
        pushButton_backup->setFont(font4);
        pushButton_backup->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_backup->setFocusPolicy(Qt::NoFocus);
        pushButton_backup->setStyleSheet(QStringLiteral(""));
        pushButton_walletLocation = new QToolButton(BackUpDialog);
        pushButton_walletLocation->setObjectName(QStringLiteral("pushButton_walletLocation"));
        pushButton_walletLocation->setGeometry(QRect(320, 300, 61, 30));
        pushButton_walletLocation->setFont(font3);
        pushButton_walletLocation->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_walletLocation->setFocusPolicy(Qt::NoFocus);
        pushButton_walletLocation->setStyleSheet(QStringLiteral(""));
        label_tipWalletName = new QLabel(BackUpDialog);
        label_tipWalletName->setObjectName(QStringLiteral("label_tipWalletName"));
        label_tipWalletName->setGeometry(QRect(300, 120, 81, 30));
        label_tipWalletName->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        label = new QLabel(BackUpDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(410, 100, 2, 400));
        label->setStyleSheet(QStringLiteral("background-color: rgb(150, 150, 150);"));
        lineEdit_saveWallet_bkp = new QLineEdit(BackUpDialog);
        lineEdit_saveWallet_bkp->setObjectName(QStringLiteral("lineEdit_saveWallet_bkp"));
        lineEdit_saveWallet_bkp->setGeometry(QRect(560, 300, 171, 30));
        lineEdit_saveWallet_bkp->setFont(font);
        lineEdit_saveWallet_bkp->setFocusPolicy(Qt::ClickFocus);
        lineEdit_saveWallet_bkp->setContextMenuPolicy(Qt::DefaultContextMenu);
        label_walletLocation_2 = new QLabel(BackUpDialog);
        label_walletLocation_2->setObjectName(QStringLiteral("label_walletLocation_2"));
        label_walletLocation_2->setGeometry(QRect(430, 300, 111, 30));
        label_walletLocation_2->setFont(font2);
        label_walletLocation_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEdit_walletName_bkp = new QLineEdit(BackUpDialog);
        lineEdit_walletName_bkp->setObjectName(QStringLiteral("lineEdit_walletName_bkp"));
        lineEdit_walletName_bkp->setGeometry(QRect(560, 120, 241, 30));
        lineEdit_walletName_bkp->setFont(font);
        lineEdit_walletName_bkp->setFocusPolicy(Qt::ClickFocus);
        lineEdit_walletName_bkp->setContextMenuPolicy(Qt::DefaultContextMenu);
        pushButton_restore = new QToolButton(BackUpDialog);
        pushButton_restore->setObjectName(QStringLiteral("pushButton_restore"));
        pushButton_restore->setGeometry(QRect(560, 400, 241, 36));
        pushButton_restore->setFont(font4);
        pushButton_restore->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_restore->setFocusPolicy(Qt::NoFocus);
        pushButton_restore->setStyleSheet(QStringLiteral(""));
        lineEdit_passWord_bkp = new QLineEdit(BackUpDialog);
        lineEdit_passWord_bkp->setObjectName(QStringLiteral("lineEdit_passWord_bkp"));
        lineEdit_passWord_bkp->setGeometry(QRect(560, 180, 241, 30));
        lineEdit_passWord_bkp->setFont(font);
        lineEdit_passWord_bkp->setFocusPolicy(Qt::ClickFocus);
        lineEdit_passWord_bkp->setContextMenuPolicy(Qt::DefaultContextMenu);
        lineEdit_passWord_bkp->setEchoMode(QLineEdit::Password);
        label_walletName_2 = new QLabel(BackUpDialog);
        label_walletName_2->setObjectName(QStringLiteral("label_walletName_2"));
        label_walletName_2->setGeometry(QRect(430, 118, 101, 30));
        label_walletName_2->setFont(font2);
        label_walletName_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_saveLocation_2 = new QLabel(BackUpDialog);
        label_saveLocation_2->setObjectName(QStringLiteral("label_saveLocation_2"));
        label_saveLocation_2->setGeometry(QRect(430, 240, 131, 30));
        label_saveLocation_2->setFont(font2);
        label_saveLocation_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        pushButton_loadFile_bkp = new QToolButton(BackUpDialog);
        pushButton_loadFile_bkp->setObjectName(QStringLiteral("pushButton_loadFile_bkp"));
        pushButton_loadFile_bkp->setGeometry(QRect(740, 240, 61, 30));
        pushButton_loadFile_bkp->setFont(font3);
        pushButton_loadFile_bkp->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_loadFile_bkp->setFocusPolicy(Qt::NoFocus);
        pushButton_loadFile_bkp->setStyleSheet(QStringLiteral(""));
        pushButton_saveWallet_bkp = new QToolButton(BackUpDialog);
        pushButton_saveWallet_bkp->setObjectName(QStringLiteral("pushButton_saveWallet_bkp"));
        pushButton_saveWallet_bkp->setGeometry(QRect(740, 300, 61, 30));
        pushButton_saveWallet_bkp->setFont(font3);
        pushButton_saveWallet_bkp->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_saveWallet_bkp->setFocusPolicy(Qt::NoFocus);
        pushButton_saveWallet_bkp->setStyleSheet(QStringLiteral(""));
        label_fileName_2 = new QLabel(BackUpDialog);
        label_fileName_2->setObjectName(QStringLiteral("label_fileName_2"));
        label_fileName_2->setGeometry(QRect(430, 180, 131, 30));
        label_fileName_2->setFont(font2);
        label_fileName_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_tipWalletName_bkp = new QLabel(BackUpDialog);
        label_tipWalletName_bkp->setObjectName(QStringLiteral("label_tipWalletName_bkp"));
        label_tipWalletName_bkp->setGeometry(QRect(720, 120, 81, 30));
        label_tipWalletName_bkp->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        lineEdit_loadFile_bkp = new QLineEdit(BackUpDialog);
        lineEdit_loadFile_bkp->setObjectName(QStringLiteral("lineEdit_loadFile_bkp"));
        lineEdit_loadFile_bkp->setGeometry(QRect(560, 240, 171, 30));
        lineEdit_loadFile_bkp->setFont(font);
        lineEdit_loadFile_bkp->setFocusPolicy(Qt::ClickFocus);
        lineEdit_loadFile_bkp->setContextMenuPolicy(Qt::DefaultContextMenu);
        label_walletBackUp_2 = new QLabel(BackUpDialog);
        label_walletBackUp_2->setObjectName(QStringLiteral("label_walletBackUp_2"));
        label_walletBackUp_2->setGeometry(QRect(550, 40, 171, 21));
        label_walletBackUp_2->setFont(font1);
        label_walletBackUp_2->setStyleSheet(QStringLiteral("color: rgb(40, 40, 40);"));
        checkBox_cloudBackup = new QCheckBox(BackUpDialog);
        checkBox_cloudBackup->setObjectName(QStringLiteral("checkBox_cloudBackup"));
        checkBox_cloudBackup->setGeometry(QRect(140, 350, 16, 30));
        checkBox_cloudBackup->setCursor(QCursor(Qt::PointingHandCursor));
        checkBox_cloudBackup->setFocusPolicy(Qt::ClickFocus);
        label_walletLocation_3 = new QLabel(BackUpDialog);
        label_walletLocation_3->setObjectName(QStringLiteral("label_walletLocation_3"));
        label_walletLocation_3->setGeometry(QRect(10, 350, 111, 30));
        label_walletLocation_3->setFont(font2);
        label_walletLocation_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_walletLocation_4 = new QLabel(BackUpDialog);
        label_walletLocation_4->setObjectName(QStringLiteral("label_walletLocation_4"));
        label_walletLocation_4->setGeometry(QRect(430, 350, 111, 30));
        label_walletLocation_4->setFont(font2);
        label_walletLocation_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        checkBox_cloudRestore = new QCheckBox(BackUpDialog);
        checkBox_cloudRestore->setObjectName(QStringLiteral("checkBox_cloudRestore"));
        checkBox_cloudRestore->setGeometry(QRect(560, 350, 16, 30));
        checkBox_cloudRestore->setCursor(QCursor(Qt::PointingHandCursor));
        checkBox_cloudRestore->setFocusPolicy(Qt::ClickFocus);

        retranslateUi(BackUpDialog);

        QMetaObject::connectSlotsByName(BackUpDialog);
    } // setupUi

    void retranslateUi(QWidget *BackUpDialog)
    {
        BackUpDialog->setWindowTitle(QApplication::translate("BackUpDialog", "Form", Q_NULLPTR));
        label_walletBackUp->setText(QApplication::translate("BackUpDialog", "Wallet BackUp", Q_NULLPTR));
        label_walletName->setText(QApplication::translate("BackUpDialog", "Backup FileName", Q_NULLPTR));
        label_saveLocation->setText(QApplication::translate("BackUpDialog", "Backup Location", Q_NULLPTR));
        label_fileName->setText(QApplication::translate("BackUpDialog", " FileName", Q_NULLPTR));
        label_walletLocation->setText(QApplication::translate("BackUpDialog", "Data Location", Q_NULLPTR));
        pushButton_saveLocation->setText(QString());
        pushButton_backup->setText(QString());
        pushButton_walletLocation->setText(QString());
        label_tipWalletName->setText(QApplication::translate("BackUpDialog", "Invalid Name", Q_NULLPTR));
        label->setText(QApplication::translate("BackUpDialog", "TextLabel", Q_NULLPTR));
        label_walletLocation_2->setText(QApplication::translate("BackUpDialog", "Save Path", Q_NULLPTR));
        pushButton_restore->setText(QString());
        label_walletName_2->setText(QApplication::translate("BackUpDialog", "Wallet Name", Q_NULLPTR));
        label_saveLocation_2->setText(QApplication::translate("BackUpDialog", "Load File", Q_NULLPTR));
        pushButton_loadFile_bkp->setText(QString());
        pushButton_saveWallet_bkp->setText(QString());
        label_fileName_2->setText(QApplication::translate("BackUpDialog", "Private Key", Q_NULLPTR));
        label_tipWalletName_bkp->setText(QApplication::translate("BackUpDialog", "Invalid Name", Q_NULLPTR));
        label_walletBackUp_2->setText(QApplication::translate("BackUpDialog", "Wallet Restore", Q_NULLPTR));
        checkBox_cloudBackup->setText(QApplication::translate("BackUpDialog", "Vote", Q_NULLPTR));
        label_walletLocation_3->setText(QApplication::translate("BackUpDialog", "Cloud Backup", Q_NULLPTR));
        label_walletLocation_4->setText(QApplication::translate("BackUpDialog", "Cloud Restore", Q_NULLPTR));
        checkBox_cloudRestore->setText(QApplication::translate("BackUpDialog", "Vote", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BackUpDialog: public Ui_BackUpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BACKUPDIALOG_H
