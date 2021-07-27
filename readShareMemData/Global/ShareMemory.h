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

/*!
 * --------------------------------------------------------------------------------------
 * @file: ShareMemory.h
 * --------------------------------------------------------------------------------------
 * @brief:
 * --------------------------------------------------------------------------------------
 * @author: meiyongpan
 * @version: V1.0
 * @date: 2016.2.18
 * --------------------------------------------------------------------------------------
 * @b:
 *    @li: 2016.2.18
 * --------------------------------------------------------------------------------------
*/

#ifndef SHAREMEMORY_H
#define SHAREMEMORY_H

#include <QObject>

class ShareMemory : public QObject
{
    Q_OBJECT
public:
/*----------------------------Enum----------------------------------------*/
    enum AccessModeEnum
    {
        ReadOnly,
        ReadWrite
    };

public:
    explicit ShareMemory(QObject *parent = 0);
    ~ShareMemory(void);

    bool createShare(int key, int size, AccessModeEnum mode = ReadWrite);
    bool closeShare(void);
    bool releaseShare(void);
    inline void* data(void) { return pBuffer; }

private:
    void* pBuffer;
    int shareSize;
    int fd;
    AccessModeEnum accessMode;

signals:

public slots:

};

#endif // SHAREMEMORY_H
