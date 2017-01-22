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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
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
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QMenuBar *menuBar;
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
        pushButton->setGeometry(QRect(130, 50, 113, 32));
        radioButton = new QRadioButton(centralWidget);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(140, 100, 100, 20));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 50, 113, 32));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(120, 160, 113, 32));
        ntiss->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ntiss);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
        ntiss->setMenuBar(menuBar);
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
        pushButton_2->setText(QApplication::translate("ntiss", "uu", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("ntiss", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ntiss: public Ui_ntiss {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NTISS_H
