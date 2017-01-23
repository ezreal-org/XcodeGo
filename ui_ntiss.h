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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ntiss
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPlainTextEdit *plainTextEdit;
    QLabel *label;

    void setupUi(QMainWindow *ntiss)
    {
        if (ntiss->objectName().isEmpty())
            ntiss->setObjectName(QStringLiteral("ntiss"));
        ntiss->resize(400, 300);
        centralWidget = new QWidget(ntiss);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(250, 240, 113, 32));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(40, 230, 191, 41));
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 20, 60, 16));
        ntiss->setCentralWidget(centralWidget);

        retranslateUi(ntiss);

        QMetaObject::connectSlotsByName(ntiss);
    } // setupUi

    void retranslateUi(QMainWindow *ntiss)
    {
        ntiss->setWindowTitle(QApplication::translate("ntiss", "ntiss", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ntiss", "Button", Q_NULLPTR));
        label->setText(QApplication::translate("ntiss", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ntiss: public Ui_ntiss {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NTISS_H
