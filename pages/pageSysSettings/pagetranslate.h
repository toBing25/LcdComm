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

#ifndef PAGETRANSLATE_H
#define PAGETRANSLATE_H

#include <QWidget>

namespace Ui {
class PageTranslate;
}

class QButtonGroup;
class PageTranslate : public QWidget
{
    Q_OBJECT

public:
    explicit PageTranslate(QWidget *parent = nullptr);
    ~PageTranslate();

private:
    void setupUi(void);
    int  setCurrentPage(int);

signals:
    void retranslate();

private slots:
    void language_switch(int id);
    void onPageClicked(bool checked);


private:
    Ui::PageTranslate *ui;
    //QButtonGroup      *m_lanGgroup;

    //int               m_supportLang;
    //int               m_currentPage;
    //int               m_totalPage;
    //int               m_tmpPage;
};

#endif // PAGETRANSLATE_H
