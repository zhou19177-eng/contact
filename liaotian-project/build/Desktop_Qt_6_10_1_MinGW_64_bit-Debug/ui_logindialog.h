/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "clickedlabel.h"

QT_BEGIN_NAMESPACE

class Ui_loginDialog
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *err_tip;
    QWidget *head_widget;
    QGridLayout *gridLayout;
    QLabel *head_label;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *email_lib;
    QLineEdit *email_edit;
    QSpacerItem *horizontalSpacer_7;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *sercetlabel;
    QLineEdit *pass_edit;
    QSpacerItem *horizontalSpacer_6;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    ClickedLabel *forget_label;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *login_btn;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *reg_btn;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *loginDialog)
    {
        if (loginDialog->objectName().isEmpty())
            loginDialog->setObjectName("loginDialog");
        loginDialog->resize(218, 363);
        loginDialog->setMinimumSize(QSize(200, 200));
        loginDialog->setMaximumSize(QSize(222, 363));
        horizontalLayout_2 = new QHBoxLayout(loginDialog);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(7);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        err_tip = new QLabel(loginDialog);
        err_tip->setObjectName("err_tip");

        verticalLayout->addWidget(err_tip);

        head_widget = new QWidget(loginDialog);
        head_widget->setObjectName("head_widget");
        head_widget->setMinimumSize(QSize(200, 120));
        head_widget->setMaximumSize(QSize(200, 120));
        gridLayout = new QGridLayout(head_widget);
        gridLayout->setObjectName("gridLayout");
        head_label = new QLabel(head_widget);
        head_label->setObjectName("head_label");
        head_label->setPixmap(QPixmap(QString::fromUtf8(":/res/ice.png")));
        head_label->setScaledContents(true);
        head_label->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        head_label->setMargin(5);
        head_label->setIndent(-1);
        head_label->setOpenExternalLinks(false);

        gridLayout->addWidget(head_label, 0, 0, 1, 1);


        verticalLayout->addWidget(head_widget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        email_lib = new QLabel(loginDialog);
        email_lib->setObjectName("email_lib");
        email_lib->setMinimumSize(QSize(0, 25));
        email_lib->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(email_lib);

        email_edit = new QLineEdit(loginDialog);
        email_edit->setObjectName("email_edit");
        email_edit->setMinimumSize(QSize(0, 25));
        email_edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(email_edit);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout);

        widget_2 = new QWidget(loginDialog);
        widget_2->setObjectName("widget_2");
        horizontalLayout_5 = new QHBoxLayout(widget_2);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        sercetlabel = new QLabel(widget_2);
        sercetlabel->setObjectName("sercetlabel");
        sercetlabel->setMinimumSize(QSize(25, 25));
        sercetlabel->setMaximumSize(QSize(25, 25));

        horizontalLayout_5->addWidget(sercetlabel);

        pass_edit = new QLineEdit(widget_2);
        pass_edit->setObjectName("pass_edit");
        pass_edit->setMinimumSize(QSize(0, 25));
        pass_edit->setMaximumSize(QSize(10000, 25));

        horizontalLayout_5->addWidget(pass_edit);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);


        verticalLayout->addWidget(widget_2);

        widget_3 = new QWidget(loginDialog);
        widget_3->setObjectName("widget_3");
        horizontalLayout_6 = new QHBoxLayout(widget_3);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        forget_label = new ClickedLabel(widget_3);
        forget_label->setObjectName("forget_label");
        forget_label->setMinimumSize(QSize(0, 25));
        forget_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_6->addWidget(forget_label);


        verticalLayout->addWidget(widget_3);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        login_btn = new QPushButton(loginDialog);
        login_btn->setObjectName("login_btn");
        login_btn->setMinimumSize(QSize(100, 30));
        login_btn->setMaximumSize(QSize(100, 30));

        horizontalLayout_3->addWidget(login_btn);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        reg_btn = new QPushButton(loginDialog);
        reg_btn->setObjectName("reg_btn");
        reg_btn->setMinimumSize(QSize(100, 30));
        reg_btn->setMaximumSize(QSize(100, 30));

        horizontalLayout_4->addWidget(reg_btn);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(loginDialog);

        QMetaObject::connectSlotsByName(loginDialog);
    } // setupUi

    void retranslateUi(QDialog *loginDialog)
    {
        loginDialog->setWindowTitle(QCoreApplication::translate("loginDialog", "Dialog", nullptr));
        err_tip->setText(QString());
        head_label->setText(QString());
        email_lib->setText(QCoreApplication::translate("loginDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        sercetlabel->setText(QCoreApplication::translate("loginDialog", "\345\257\206\347\240\201:", nullptr));
        forget_label->setText(QCoreApplication::translate("loginDialog", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
        login_btn->setText(QCoreApplication::translate("loginDialog", "\347\231\273\345\275\225", nullptr));
        reg_btn->setText(QCoreApplication::translate("loginDialog", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class loginDialog: public Ui_loginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
