/********************************************************************************
** Form generated from reading UI file 'userface.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USEFACER_H
#define UI_USEFACER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserFace
{
public:
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *UserFace)
    {
        if (UserFace->objectName().isEmpty())
            UserFace->setObjectName(QString::fromUtf8("UserFace"));
        UserFace->resize(303, 647);
        label = new QLabel(UserFace);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 303, 140));
        label->setFrameShape(QFrame::Shape::Box);
        label_2 = new QLabel(UserFace);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 600, 303, 50));
        label_2->setFrameShape(QFrame::Shape::Box);

        retranslateUi(UserFace);

        QMetaObject::connectSlotsByName(UserFace);
    } // setupUi

    void retranslateUi(QWidget *UserFace)
    {
        UserFace->setWindowTitle(QCoreApplication::translate("UserFace", "Form", nullptr));
        label->setText(QCoreApplication::translate("UserFace", "TextLabel", nullptr));
        label_2->setText(QCoreApplication::translate("UserFace", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserFace: public Ui_UserFace {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USEFACER_H
