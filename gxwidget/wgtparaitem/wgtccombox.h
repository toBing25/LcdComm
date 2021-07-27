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

#ifndef WGTCCOMBOX_H
#define WGTCCOMBOX_H

#include <QComboBox>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>

class wgtCCombox : public QComboBox
{
    Q_OBJECT
    enum STATUS{
        status_One,
        status_All
    };
public:
    explicit wgtCCombox(QWidget *parent = 0);
    ~wgtCCombox();

    void addItems(QStringList strItem);
    void cleanItems(void);

    QStringList getItemListId(void);
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void stateChanged(int index);
    void textChanged(const QString &text);

private:
    //是否选择全部
    void selectAllItem(int index);
    QCheckBox *getCheckBox(int index);
private:
    QListWidget  *m_pListWidget;
    QLineEdit    *m_pLineEdit;
    QString       m_strSelectedText;
    bool          m_bSelected;
    STATUS        m_status;
};

#endif // WGTCCOMBOX_H
