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

#ifndef MASK_WIDGET_H
#define MASK_WIDGET_H

#include <QDialog>

namespace Ui {
class MaskWidget;
}

class MaskWidget : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QStringList names READ names WRITE setNames DESIGNABLE true)
public:    
    explicit MaskWidget(QWidget *parent = 0);

    void setMainWidget(QWidget* pWidget);

    QStringList names() const;
    void setNames(const QStringList& names);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:

    ~MaskWidget();

private:
    Ui::MaskWidget *ui;
    QStringList m_listName{ QStringList() };

    QWidget* m_pMainWidget{ Q_NULLPTR };


};

#endif // MASK_WIDGET_H
