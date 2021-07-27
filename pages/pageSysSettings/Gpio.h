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

#ifndef GPIO_H
#define GPIO_H

//方向
#define GPIO_IN		0 
#define GPIO_OUT	1

//屏幕保护引脚
#define SCREENSAVER_PIN 18
//蜂鸣器引脚
#define BUZZER_PIN      20

//注册引脚
int gpio_export(int pin);

//取消注册
int gpio_unexport(int pin);

//设置方向
int gpio_direction(int pin, int dir);

//写引脚
int gpio_write(int pin, int value);

//读引脚
int gpio_read(int pin);
int gpio_edge(int pin, int edge);

#endif //GPIO_H
