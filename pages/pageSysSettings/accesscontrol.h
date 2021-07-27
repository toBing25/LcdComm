/****************************************************************************
**
** Copyright (C) 2021 The INVT Company . 
**
** This file is part of LcdComm.
** LcdComm is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** LcdComm is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with LcdComm.  If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H

#include <QWidget>
#include "formmain.h"

namespace Ui {
class AccessControl;
}

//门禁管理
class AccessControl : public QWidget
{
    Q_OBJECT

public:
    explicit AccessControl(QWidget *parent = nullptr);
    ~AccessControl();

private:
    void setupUi(void);

    void setHorizontalHeaderLabels(void);
    void retranslateUi(void);

    void addMode(void);
    void removeMode(void);

    void setTabSize(void);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void showEvent(QShowEvent *event);
    void changeEvent(QEvent* event);

signals:
    void cardChange(QString, QString, QStringList);
    void cardRemove(QString);

private slots:
    void updateTable();

    void cardChangeSlot(CardDb card);
    void cardRemoveSlot(CardDb card);
private:
    Ui::AccessControl *ui;
    QVector<QLineEdit *> lieVector;
    QVector<QCheckBox *> m_vector;
    QVector<int>         m_vectorID;

    QVector<CardDb>* m_cardDb;
    DcCardDevVector  m_dcDev;
    QStringList      m_dclist;

    int              m_rowCount;
};

#endif // ACCESSCONTROL_H
