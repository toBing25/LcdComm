#include "actparaitem_combobox.h"
#include "ui_actparaitem_combobox.h"
#include "wgtparaitem_ths.h"
#include "wgtparaitem_ac.h"
#include "wgtparaitem_eleak.h"
#include <QComboBox>
#include <QLineEdit>
#include <QListView>
#include <QDebug>

ActParaItem_comboBox::ActParaItem_comboBox(DevTypeEnum devType, int devId, struct Menu menu, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActParaItem_comboBox),
    m_menu(menu),
    m_devtype(devType),
    m_devid(devId)
{
    ui->setupUi(this);
    ui->WgtComBox->setView(new QListView());

    m_name = langN.show[m_menu.show_name];//m_menu.show_name;
    ui->lblParaName->setText(m_name);

    if(!m_menu.Map_type.isEmpty())
    {
        for (int i = 0; i < m_menu.Map_type.length(); i++) {
            m_strlist << langN.show[m_menu.Map_type.at(i).second];//m_menu.Map_type.at(i).second;
        }
    }

    m_value = 0;
    ui->WgtComBox->addItems(m_strlist);
    ui->WgtComBox->setCurrentIndex(m_value);

    //connect(ui->WgtComBox, &QComboBox::currentTextChanged, this, &ActParaItem_comboBox::indexChanged);
    connect(ui->WgtComBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ActParaItem_comboBox::curIndexChanged);
}

ActParaItem_comboBox::~ActParaItem_comboBox()
{
    delete ui;
}

void ActParaItem_comboBox::indexChanged(const QString &str)
{
}

void ActParaItem_comboBox::curIndexChanged(int index)
{
    m_value = index;
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
        ths_val.val   = m_value;

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
        ups_val.val   = m_value;

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
        ac_val.devNo = m_devid;
        ac_val.val   = m_value;

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
        leak_val.val   = m_value;

        emit signal_leak(leak_val);
    }
}

void ActParaItem_comboBox::setValue(QString val)
{
    if(m_value  != val.toInt())
    {
        m_value = val.toInt();
        ui->WgtComBox->setCurrentIndex(m_value);
    }
}

void ActParaItem_comboBox::setUnit(QString unit)
{
    if(unit != ""){
        QString str =m_name+ "("+unit+")";
        ui->lblParaName->setText(str);
    }

}

void ActParaItem_comboBox::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

