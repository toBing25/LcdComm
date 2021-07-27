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

#ifndef GETTEXT_H
#define GETTEXT_H

#include <QTranslator>

//language
#define TEXTLANG_INVALID      (0)
#define TEXTLANG_CN           (1) /* 中文    */
#define TEXTLANG_EN           (2) /* 英文    */

#define TEXTLANG_MIN          TEXTLANG_CN
#define TEXTLANG_MAX          TEXTLANG_EN//支持多少语言

#include <QObject>


class GetText
{
public:
    GetText();
    ~GetText();

    static int          init(QTranslator *pTranslator);
    static bool         selectLang(int landId);
    static const char * getString(int id);
    static QString      getIcon(int id);
    static QString      getlandStr(int id);
    static int          getCurrentLangId(void);

    //创建登录配置文件
    static void  createInitFile(void);

    //检测LCD大小
    static int   detectLcdSize(void);
    //获取LCD大小
    static void getLcdSize(int &XSize, int &YSize);

    //设置屏保初始化
    static void  screenSaverInit(void);
private:
    static  int          _currLangId;
    static  QTranslator *_pTranslator;

    static  int  m_XSize;
    static  int  m_YSize;
};

#endif // GETTEXT_H
