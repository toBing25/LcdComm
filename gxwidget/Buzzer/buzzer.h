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

#ifndef BUZZER_H
#define BUZZER_H

#include "Gpio.h"


//蜂鸣器单例对象
class Buzzer
{
public:
    ~Buzzer();
    static Buzzer *Instance(){
        if(_instance == 0)
            _instance = new Buzzer;
        return _instance;
    }
    void beep();
    void setBuzzerEnable(bool enable);
private:
    Buzzer();

private:
    static Buzzer* _instance;
    bool   m_enable;
};

//外面直接调用ShortBeep()就可以让蜂鸣器响
#define	ShortBeep()  Buzzer::Instance()->beep()

#define BuzzerStop()  Buzzer::Instance()->setBuzzerEnable(false)
#define BuzzerStart() Buzzer::Instance()->setBuzzerEnable(true)

#endif // BUZZER_H
