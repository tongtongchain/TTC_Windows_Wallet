/********************************************************************************
** Form generated from reading UI file 'bottombar.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOTTOMBAR_H
#define UI_BOTTOMBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BottomBar
{
public:
    QToolButton *signalBtn;
    QLabel *syncLabel;
    QLabel *warningLabel;
    QLabel *warningLabel2;
    QToolButton *lockBtn;
    QLabel *promptlabel;
    QLabel *warningLabel2_2;
    QLabel *promptlabel_2;
    QLabel *syncLabel_2;

    void setupUi(QWidget *BottomBar)
    {
        if (BottomBar->objectName().isEmpty())
            BottomBar->setObjectName(QStringLiteral("BottomBar"));
        BottomBar->resize(827, 44);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        BottomBar->setFont(font);
        signalBtn = new QToolButton(BottomBar);
        signalBtn->setObjectName(QStringLiteral("signalBtn"));
        signalBtn->setGeometry(QRect(760, 10, 24, 20));
        signalBtn->setCursor(QCursor(Qt::PointingHandCursor));
        signalBtn->setFocusPolicy(Qt::NoFocus);
        syncLabel = new QLabel(BottomBar);
        syncLabel->setObjectName(QStringLiteral("syncLabel"));
        syncLabel->setGeometry(QRect(180, 13, 131, 16));
        syncLabel->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        warningLabel = new QLabel(BottomBar);
        warningLabel->setObjectName(QStringLiteral("warningLabel"));
        warningLabel->setGeometry(QRect(87, 10, 18, 18));
        warningLabel2 = new QLabel(BottomBar);
        warningLabel2->setObjectName(QStringLiteral("warningLabel2"));
        warningLabel2->setGeometry(QRect(20, 10, 221, 20));
        warningLabel2->setStyleSheet(QStringLiteral(""));
        lockBtn = new QToolButton(BottomBar);
        lockBtn->setObjectName(QStringLiteral("lockBtn"));
        lockBtn->setGeometry(QRect(800, 10, 20, 20));
        lockBtn->setCursor(QCursor(Qt::PointingHandCursor));
        lockBtn->setFocusPolicy(Qt::NoFocus);
        promptlabel = new QLabel(BottomBar);
        promptlabel->setObjectName(QStringLiteral("promptlabel"));
        promptlabel->setGeometry(QRect(240, 10, 161, 20));
        warningLabel2_2 = new QLabel(BottomBar);
        warningLabel2_2->setObjectName(QStringLiteral("warningLabel2_2"));
        warningLabel2_2->setGeometry(QRect(380, 10, 201, 20));
        warningLabel2_2->setStyleSheet(QStringLiteral(""));
        promptlabel_2 = new QLabel(BottomBar);
        promptlabel_2->setObjectName(QStringLiteral("promptlabel_2"));
        promptlabel_2->setGeometry(QRect(640, 10, 111, 20));
        syncLabel_2 = new QLabel(BottomBar);
        syncLabel_2->setObjectName(QStringLiteral("syncLabel_2"));
        syncLabel_2->setGeometry(QRect(600, 10, 131, 20));
        syncLabel_2->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));

        retranslateUi(BottomBar);

        QMetaObject::connectSlotsByName(BottomBar);
    } // setupUi

    void retranslateUi(QWidget *BottomBar)
    {
        BottomBar->setWindowTitle(QApplication::translate("BottomBar", "Form", Q_NULLPTR));
        signalBtn->setText(QString());
        syncLabel->setText(QString());
        warningLabel->setText(QString());
        warningLabel2->setText(QApplication::translate("BottomBar", "Currently there is no sync block source.", Q_NULLPTR));
        lockBtn->setText(QString());
        promptlabel->setText(QApplication::translate("BottomBar", "Synchronizing data", Q_NULLPTR));
        warningLabel2_2->setText(QApplication::translate("BottomBar", "Wallet Synchronization here", Q_NULLPTR));
        promptlabel_2->setText(QApplication::translate("BottomBar", "Synchronizing wallet", Q_NULLPTR));
        syncLabel_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class BottomBar: public Ui_BottomBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOTTOMBAR_H
