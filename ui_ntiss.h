/********************************************************************************
** Form generated from reading UI file 'ntiss.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NTISS_H
#define UI_NTISS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ntiss
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QLabel *label;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_2;
    QWidget *layoutWidget_2;
    QFormLayout *formLayout_2;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ntiss)
    {
        if (ntiss->objectName().isEmpty())
            ntiss->setObjectName(QStringLiteral("ntiss"));
        ntiss->resize(400, 300);
        centralWidget = new QWidget(ntiss);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 100, 113, 32));
        radioButton = new QRadioButton(centralWidget);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(140, 100, 100, 20));
        radioButton_2 = new QRadioButton(centralWidget);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(140, 130, 100, 20));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 270, 60, 16));
        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(0, 270, 113, 32));
        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(20, 270, 113, 32));
        pushButton_6->setAutoDefault(true);
        pushButton_6->setFlat(false);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(130, 160, 111, 90));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, pushButton_3);

        pushButton_4 = new QPushButton(layoutWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        formLayout->setWidget(2, QFormLayout::LabelRole, pushButton_4);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(260, 100, 113, 32));
        layoutWidget_2 = new QWidget(centralWidget);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(130, 10, 111, 90));
        formLayout_2 = new QFormLayout(layoutWidget_2);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_7 = new QPushButton(layoutWidget_2);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, pushButton_7);

        pushButton_8 = new QPushButton(layoutWidget_2);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, pushButton_8);

        ntiss->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(ntiss);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ntiss->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ntiss);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ntiss->setStatusBar(statusBar);

        retranslateUi(ntiss);

        QMetaObject::connectSlotsByName(ntiss);
    } // setupUi

    void retranslateUi(QMainWindow *ntiss)
    {
        ntiss->setWindowTitle(QApplication::translate("ntiss", "ntiss", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ntiss", "PushButton", Q_NULLPTR));
        radioButton->setText(QApplication::translate("ntiss", "RadioButton", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("ntiss", "RadioButton", Q_NULLPTR));
        label->setText(QApplication::translate("ntiss", "status", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("ntiss", "PushButton", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("ntiss", "PushButton", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("ntiss", "PushButton", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("ntiss", "PushButton", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("ntiss", "PushButton", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("ntiss", "PushButton", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("ntiss", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ntiss: public Ui_ntiss {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NTISS_H
