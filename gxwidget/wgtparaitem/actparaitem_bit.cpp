#include "actparaitem_bit.h"
#include "ui_actparaitem_bit.h"
#include "buzzer.h"
#include "wgtparaitem_ths.h"
#include "wgtparaitem_ac.h"
#include "wgtparaitem_eleak.h"
#include <QDesktopWidget>
#include <QDebug>
#include "formmain.h"

ActParaItem_bit::ActParaItem_bit(DevTypeEnum devType, int devId, Menu menu, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActParaItem_bit),
    m_menu(menu),
    m_devtype(devType),
    m_devid(devId)
{
    ui->setupUi(this);

    m_name = langN.show[m_menu.show_name];//m_menu.show_name;
    ui->lblParaName->setText(m_name);

    m_value = QString::number(7);
    ui->btnPara->setText(m_value);

    if(!m_menu.Map_type.isEmpty())
    {
        for (int i = 0; i < m_menu.Map_type.length(); i++) {
            m_list << langN.show[m_menu.Map_type.at(i).second];//m_menu.Map_type.at(i).second;
        }
    }

    connect(ui->btnPara,  &QPushButton::clicked,  this, &ActParaItem_bit::onbtnclicked);
}

ActParaItem_bit::~ActParaItem_bit()
{
    delete ui;
}

void ActParaItem_bit::onbtnclicked(bool /*checked*/)
{
    ShortBeep();

    ActBitOperation  m_bitOper(m_list, m_value);
    int  val = m_bitOper.exec();

    m_value = QString::number(val);
    ui->btnPara->setText(m_value);

    if(m_devtype == DevTypeEnum::Ths)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ths::ThsParaEnum>();
        //把参数对应的值转换成枚举值
        int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
        if((-1 == index) && (index >= eThsParaNum))
            return;

        ths_set ths_val;
        ths_val.devNo = m_devid;
        ths_val.para  = (ThsParaEnum)index;
        ths_val.val   = m_value.toDouble();

        emit signal_ths(ths_val);
    }
    else if(m_devtype == DevTypeEnum::Ups)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ups::UpsCtlEnum>();
        //把参数对应的值转换成枚举值
        int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
        if((-1 == index) && (index >= eUpsCtlNum))
            return;

        ups_set ups_val;
        ups_val.devNo = m_devid;
        ups_val.para  = (UpsCtlEnum)index;
        ups_val.val   = m_value.toInt();

        emit signal_ups(ups_val);
    }
    else if(m_devtype == DevTypeEnum::Ac)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ac::AcRunParaEnum>();
        //把参数对应的值转换成枚举值
        int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
        if((-1 == index) && (index >= eAcParaNum))
            return;

        ac_set ac_val;
        ac_val.devNo = m_devid;
        if(m_menu.parameter_type == TYPE_PARA_SETTING){
            QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ac::AcRunParaEnum>();
            //把参数对应的值转换成枚举值
            int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
            if((-1 == index) && (index >= eAcParaNum))
                return;
            ac_val.union_ac.para  = (AcRunParaEnum)index;
            ac_val.paraType = TYPE_PARA_SETTING;
        }else if(m_menu.parameter_type == TYPE_STAT_SETTING) {
            QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ac::AcRunStatusEnum>();
            //把参数对应的值转换成枚举值
            int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
            if((-1 == index) && (index >= eAcStsNum))
                return;
            ac_val.union_ac.sta_para = (AcRunStatusEnum)index;
            ac_val.paraType = TYPE_STAT_SETTING;
        }
        ac_val.val   = m_value.toInt();

        emit signal_ac(ac_val);
    }
    else if(m_devtype == DevTypeEnum::Leak)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
        //把参数对应的值转换成枚举值
        int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
        if((-1 == index) && (index >= eLeakParaNum))
            return;

        leak_set leak_val;
        leak_val.devNo = m_devid;
        leak_val.para  = (LeakParaEnum)index;
        leak_val.val   = m_value.toInt();

        emit signal_leak(leak_val);
    }
}

void ActParaItem_bit::setValue(QString val)
{
    m_value = val;
    ui->btnPara->setText(m_value);
}

void ActParaItem_bit::setUnit(QString unit)
{
    if(unit != ""){
        QString str =m_name+ "("+unit+")";
        ui->lblParaName->setText(str);
    }

}

void ActParaItem_bit::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}




