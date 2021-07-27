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

#ifndef WGTBGSCREEN_H
#define WGTBGSCREEN_H

#include <QWidget>

class WgtBgScreen : public QWidget
{
    Q_OBJECT
public:
    explicit WgtBgScreen(QWidget *parent = nullptr);
    ~WgtBgScreen();

    static void loadImage(QString imageurl)
    {
        if(g_pix == 0)
            g_pix = new QPixmap();
        if(g_pix)
            g_pix->load(imageurl);
    }

protected:
    void paintEvent(QPaintEvent *event);
    bool event(QEvent *e);

private:
    static QPixmap *g_pix;

    static QStringList tabBgList;
};

#endif // WGTBGSCREEN_H
