/********************************************************************************
** Form generated from reading UI file 'bulkSending.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BULKSENDING_H
#define UI_BULKSENDING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BulkSending
{
public:
    QLabel *balanceLabel;
    QLabel *addressLabel2;
    QLabel *balanceLabel2;
    QLabel *addressLabel;
    QLabel *changeAccountLabel;
    QLabel *delegateLabel2;
    QLabel *identityLabel;
    QComboBox *accountComboBox;
    QTableWidget *tableWidget_bulkSending;
    QLabel *label_11;
    QToolButton *pushButton_loadCSVFile;
    QToolButton *bulkSending_sendBtn;
    QLabel *tipLabel3;
    QToolButton *bulkSending_exportCSV;
    QLabel *initLabel;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_total;
    QLabel *label_Amount;
    QLabel *label_Fee;
    QToolButton *pushButton_ValidateAddresses;

    void setupUi(QWidget *BulkSending)
    {
        if (BulkSending->objectName().isEmpty())
            BulkSending->setObjectName(QStringLiteral("BulkSending"));
        BulkSending->resize(827, 525);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        BulkSending->setFont(font);
        BulkSending->setFocusPolicy(Qt::TabFocus);
        BulkSending->setStyleSheet(QStringLiteral(""));
        balanceLabel = new QLabel(BulkSending);
        balanceLabel->setObjectName(QStringLiteral("balanceLabel"));
        balanceLabel->setGeometry(QRect(692, 30, 271, 32));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(20);
        balanceLabel->setFont(font1);
        addressLabel2 = new QLabel(BulkSending);
        addressLabel2->setObjectName(QStringLiteral("addressLabel2"));
        addressLabel2->setGeometry(QRect(20, 110, 101, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setWeight(50);
        addressLabel2->setFont(font2);
        addressLabel2->setStyleSheet(QStringLiteral(""));
        addressLabel2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        balanceLabel2 = new QLabel(BulkSending);
        balanceLabel2->setObjectName(QStringLiteral("balanceLabel2"));
        balanceLabel2->setGeometry(QRect(540, 30, 110, 30));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font3.setPointSize(13);
        balanceLabel2->setFont(font3);
        balanceLabel2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        addressLabel = new QLabel(BulkSending);
        addressLabel->setObjectName(QStringLiteral("addressLabel"));
        addressLabel->setGeometry(QRect(110, 110, 421, 31));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font4.setPointSize(12);
        addressLabel->setFont(font4);
        addressLabel->setStyleSheet(QStringLiteral(""));
        addressLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        changeAccountLabel = new QLabel(BulkSending);
        changeAccountLabel->setObjectName(QStringLiteral("changeAccountLabel"));
        changeAccountLabel->setGeometry(QRect(180, 40, 61, 24));
        changeAccountLabel->setFont(font3);
        delegateLabel2 = new QLabel(BulkSending);
        delegateLabel2->setObjectName(QStringLiteral("delegateLabel2"));
        delegateLabel2->setGeometry(QRect(20, 18, 161, 31));
        QFont font5;
        font5.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font5.setPointSize(18);
        font5.setBold(true);
        font5.setWeight(75);
        delegateLabel2->setFont(font5);
        delegateLabel2->setStyleSheet(QStringLiteral("color: rgb(40, 40, 40);"));
        identityLabel = new QLabel(BulkSending);
        identityLabel->setObjectName(QStringLiteral("identityLabel"));
        identityLabel->setGeometry(QRect(260, 41, 16, 16));
        identityLabel->setFont(font);
        identityLabel->setStyleSheet(QStringLiteral("background:transparent;"));
        identityLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        accountComboBox = new QComboBox(BulkSending);
        accountComboBox->setObjectName(QStringLiteral("accountComboBox"));
        accountComboBox->setGeometry(QRect(250, 40, 120, 24));
        QFont font6;
        font6.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font6.setBold(false);
        font6.setWeight(50);
        accountComboBox->setFont(font6);
        accountComboBox->setFocusPolicy(Qt::ClickFocus);
        tableWidget_bulkSending = new QTableWidget(BulkSending);
        if (tableWidget_bulkSending->columnCount() < 6)
            tableWidget_bulkSending->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        __qtablewidgetitem->setFont(font);
        tableWidget_bulkSending->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        __qtablewidgetitem1->setFont(font);
        tableWidget_bulkSending->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        __qtablewidgetitem2->setFont(font);
        tableWidget_bulkSending->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        __qtablewidgetitem3->setFont(font);
        tableWidget_bulkSending->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        __qtablewidgetitem4->setFont(font6);
        tableWidget_bulkSending->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        __qtablewidgetitem5->setFont(font);
        tableWidget_bulkSending->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget_bulkSending->setObjectName(QStringLiteral("tableWidget_bulkSending"));
        tableWidget_bulkSending->setGeometry(QRect(19, 190, 781, 261));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget_bulkSending->sizePolicy().hasHeightForWidth());
        tableWidget_bulkSending->setSizePolicy(sizePolicy);
        tableWidget_bulkSending->setMinimumSize(QSize(0, 0));
        tableWidget_bulkSending->setStyleSheet(QStringLiteral(""));
        tableWidget_bulkSending->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tableWidget_bulkSending->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget_bulkSending->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableWidget_bulkSending->setAutoScroll(true);
        tableWidget_bulkSending->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_bulkSending->setTabKeyNavigation(true);
        tableWidget_bulkSending->setProperty("showDropIndicator", QVariant(true));
        tableWidget_bulkSending->setDragDropOverwriteMode(true);
        tableWidget_bulkSending->setShowGrid(true);
        tableWidget_bulkSending->setColumnCount(6);
        tableWidget_bulkSending->horizontalHeader()->setVisible(false);
        tableWidget_bulkSending->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget_bulkSending->horizontalHeader()->setHighlightSections(false);
        tableWidget_bulkSending->verticalHeader()->setVisible(false);
        tableWidget_bulkSending->verticalHeader()->setCascadingSectionResizes(false);
        tableWidget_bulkSending->verticalHeader()->setHighlightSections(false);
        label_11 = new QLabel(BulkSending);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(120, 150, 121, 30));
        label_11->setFont(font);
        label_11->setStyleSheet(QStringLiteral("color: rgb(134, 134, 134);"));
        pushButton_loadCSVFile = new QToolButton(BulkSending);
        pushButton_loadCSVFile->setObjectName(QStringLiteral("pushButton_loadCSVFile"));
        pushButton_loadCSVFile->setGeometry(QRect(570, 110, 231, 31));
        QFont font7;
        font7.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font7.setPointSize(9);
        pushButton_loadCSVFile->setFont(font7);
        pushButton_loadCSVFile->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_loadCSVFile->setFocusPolicy(Qt::NoFocus);
        pushButton_loadCSVFile->setStyleSheet(QStringLiteral(""));
        bulkSending_sendBtn = new QToolButton(BulkSending);
        bulkSending_sendBtn->setObjectName(QStringLiteral("bulkSending_sendBtn"));
        bulkSending_sendBtn->setGeometry(QRect(690, 460, 110, 30));
        bulkSending_sendBtn->setFont(font7);
        bulkSending_sendBtn->setCursor(QCursor(Qt::PointingHandCursor));
        bulkSending_sendBtn->setFocusPolicy(Qt::NoFocus);
        bulkSending_sendBtn->setStyleSheet(QStringLiteral(""));
        tipLabel3 = new QLabel(BulkSending);
        tipLabel3->setObjectName(QStringLiteral("tipLabel3"));
        tipLabel3->setGeometry(QRect(30, 460, 251, 30));
        tipLabel3->setCursor(QCursor(Qt::ArrowCursor));
        tipLabel3->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        tipLabel3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        bulkSending_exportCSV = new QToolButton(BulkSending);
        bulkSending_exportCSV->setObjectName(QStringLiteral("bulkSending_exportCSV"));
        bulkSending_exportCSV->setGeometry(QRect(690, 110, 110, 30));
        bulkSending_exportCSV->setFont(font7);
        bulkSending_exportCSV->setCursor(QCursor(Qt::PointingHandCursor));
        bulkSending_exportCSV->setFocusPolicy(Qt::NoFocus);
        bulkSending_exportCSV->setStyleSheet(QStringLiteral(""));
        initLabel = new QLabel(BulkSending);
        initLabel->setObjectName(QStringLiteral("initLabel"));
        initLabel->setGeometry(QRect(30, 290, 771, 51));
        initLabel->setFont(font4);
        initLabel->setStyleSheet(QStringLiteral("color: rgb(178, 178, 178);"));
        initLabel->setAlignment(Qt::AlignCenter);
        label_14 = new QLabel(BulkSending);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(20, 150, 41, 30));
        label_14->setFont(font);
        label_14->setStyleSheet(QStringLiteral("color: rgb(134, 134, 134);"));
        label_15 = new QLabel(BulkSending);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(450, 150, 81, 30));
        label_15->setFont(font);
        label_15->setStyleSheet(QStringLiteral("color: rgb(134, 134, 134);"));
        label_16 = new QLabel(BulkSending);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(540, 150, 81, 30));
        label_16->setFont(font);
        label_16->setStyleSheet(QStringLiteral("color: rgb(134, 134, 134);"));
        label_17 = new QLabel(BulkSending);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(660, 150, 81, 30));
        label_17->setFont(font);
        label_17->setStyleSheet(QStringLiteral("color: rgb(134, 134, 134);"));
        label_18 = new QLabel(BulkSending);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(350, 150, 81, 30));
        label_18->setFont(font);
        label_18->setStyleSheet(QStringLiteral("color: rgb(134, 134, 134);"));
        label_total = new QLabel(BulkSending);
        label_total->setObjectName(QStringLiteral("label_total"));
        label_total->setGeometry(QRect(280, 460, 41, 30));
        label_Amount = new QLabel(BulkSending);
        label_Amount->setObjectName(QStringLiteral("label_Amount"));
        label_Amount->setGeometry(QRect(320, 460, 111, 30));
        label_Fee = new QLabel(BulkSending);
        label_Fee->setObjectName(QStringLiteral("label_Fee"));
        label_Fee->setGeometry(QRect(445, 460, 101, 30));
        pushButton_ValidateAddresses = new QToolButton(BulkSending);
        pushButton_ValidateAddresses->setObjectName(QStringLiteral("pushButton_ValidateAddresses"));
        pushButton_ValidateAddresses->setGeometry(QRect(560, 460, 121, 31));
        pushButton_ValidateAddresses->setFont(font7);
        pushButton_ValidateAddresses->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_ValidateAddresses->setFocusPolicy(Qt::NoFocus);
        pushButton_ValidateAddresses->setStyleSheet(QStringLiteral(""));
        balanceLabel->raise();
        addressLabel2->raise();
        balanceLabel2->raise();
        addressLabel->raise();
        changeAccountLabel->raise();
        delegateLabel2->raise();
        identityLabel->raise();
        accountComboBox->raise();
        tableWidget_bulkSending->raise();
        label_11->raise();
        bulkSending_sendBtn->raise();
        tipLabel3->raise();
        bulkSending_exportCSV->raise();
        initLabel->raise();
        label_14->raise();
        label_15->raise();
        label_16->raise();
        label_17->raise();
        label_18->raise();
        label_total->raise();
        label_Amount->raise();
        label_Fee->raise();
        pushButton_ValidateAddresses->raise();
        pushButton_loadCSVFile->raise();

        retranslateUi(BulkSending);

        QMetaObject::connectSlotsByName(BulkSending);
    } // setupUi

    void retranslateUi(QWidget *BulkSending)
    {
        BulkSending->setWindowTitle(QApplication::translate("BulkSending", "Form", Q_NULLPTR));
        balanceLabel->setText(QApplication::translate("BulkSending", "<html><head/><body><p><span style=\" font-size:26px;\">0.00</span><span style=\" font-size:12px;\"> TTCOIN</span></p></body></html>", Q_NULLPTR));
        addressLabel2->setText(QApplication::translate("BulkSending", "From", Q_NULLPTR));
        balanceLabel2->setText(QApplication::translate("BulkSending", "Balance", Q_NULLPTR));
        addressLabel->setText(QString());
        changeAccountLabel->setText(QApplication::translate("BulkSending", "Account", Q_NULLPTR));
        delegateLabel2->setText(QApplication::translate("BulkSending", "Bulk Sending", Q_NULLPTR));
        identityLabel->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget_bulkSending->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("BulkSending", "\346\227\266\351\227\264", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_bulkSending->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("BulkSending", "\345\257\271\346\226\271\350\264\246\346\210\267", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_bulkSending->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("BulkSending", "\344\272\244\346\230\223\351\207\221\351\242\235/GOP", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_bulkSending->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("BulkSending", "\346\211\213\347\273\255\350\264\271/GOP", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_bulkSending->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("BulkSending", "\350\264\246\346\210\267\344\275\231\351\242\235/GOP", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_bulkSending->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("BulkSending", "\345\244\207\346\263\250", Q_NULLPTR));
        label_11->setText(QApplication::translate("BulkSending", "Receiver's Account", Q_NULLPTR));
        pushButton_loadCSVFile->setText(QApplication::translate("BulkSending", "Load CSV File", Q_NULLPTR));
        bulkSending_sendBtn->setText(QString());
        tipLabel3->setText(QApplication::translate("BulkSending", "Invalid Balance", Q_NULLPTR));
        bulkSending_exportCSV->setText(QString());
        initLabel->setText(QApplication::translate("BulkSending", "No csv File Selected", Q_NULLPTR));
        label_14->setText(QApplication::translate("BulkSending", "S.No", Q_NULLPTR));
        label_15->setText(QApplication::translate("BulkSending", "Fee", Q_NULLPTR));
        label_16->setText(QApplication::translate("BulkSending", "Status", Q_NULLPTR));
        label_17->setText(QApplication::translate("BulkSending", "Memo", Q_NULLPTR));
        label_18->setText(QApplication::translate("BulkSending", "Amount", Q_NULLPTR));
        label_total->setText(QApplication::translate("BulkSending", "Total:", Q_NULLPTR));
        label_Amount->setText(QString());
        label_Fee->setText(QString());
        pushButton_ValidateAddresses->setText(QApplication::translate("BulkSending", "Validate Data", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BulkSending: public Ui_BulkSending {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BULKSENDING_H
