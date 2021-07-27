#include "buzzer.h"
#include <QTime>
#include <QCoreApplication>

Buzzer *Buzzer::_instance = 0;

Buzzer::Buzzer()
{
    m_enable = true;
}

void Buzzer::beep()
{
    if(!m_enable)
        return;

    gpio_write(BUZZER_PIN, 1);

    QTime t;
    t.start();
    while(t.elapsed() < 10){
        QCoreApplication::processEvents();
    }

    gpio_write(BUZZER_PIN, 0);
}

void Buzzer::setBuzzerEnable(bool enable)
{
    m_enable  = enable;
}
