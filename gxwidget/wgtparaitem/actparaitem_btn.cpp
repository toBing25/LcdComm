#include "actparaitem_btn.h"
#include "ui_actparaitem_btn.h"
#include "wgtparaitem_ths.h"
#include "wgtparaitem_ac.h"
#include "wgtparaitem_eleak.h"
#include "formmain.h"
#include <QDebug>

ActParaItem_btn::ActParaItem_btn(DevTypeEnum devType, int devId, Menu menu, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActParaItem_btn),
    m_menu(menu),
    m_devtype(devType),
    m_devid(devId)
{
    ui->setupUi(this);

    m_name  = langN.show[m_menu.show_name];//m_menu.show_name;
    ui->lblParaName->setText(m_name);

    QStringList list;
    if(!m_menu.Map_type.isEmpty())
    {
        for(int i=0;i<m_menu.Map_type.length();i++){
            list<< langN.show[m_menu.Map_type.at(i).second];//m_menu.Map_type.at(i).second;
        }
    }

    if(!list.isEmpty() and list.length()>=2)
    {
        ui->btnPara->setText(list.at(0));
        ui->btnPara_2->setText(list.at(1));
    }

    connect(ui->btnPara,  &QPushButton::clicked, this, &ActParaItem_btn::onbtnclicked);
    connect(ui->btnPara_2,  &QPushButton::clicked, this, &ActParaItem_btn::onbtnclicked);
}

ActParaItem_btn::~ActParaItem_btn()
{
    delete ui;
}

void ActParaItem_btn::setValue(QString /*val*/)
{

}

void ActParaItem_btn::setUnit(QString unit)
{
    if(unit != ""){
        QString str =m_name+ "("+unit+")";
        ui->lblParaName->setText(str);
    }

}


void ActParaItem_btn::onbtnclicked(bool checked)
{
    Q_UNUSED(checked);

    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if(!btn)
        return;

    int val = 0;
    if(btn == ui->btnPara)  //
    {
        if(m_menu.Map_type.length()>=2)
            val = m_menu.Map_type.at(0).first;
    }
    else if (btn == ui->btnPara_2)
    {
        if(m_menu.Map_type.length()>=2)
            val = m_menu.Map_type.at(1).first;;
    }

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
        ths_val.val   = val;

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
        ups_val.val   = val;

        emit signal_ups(ups_val);
    }
    else if(m_devtype == DevTypeEnum::Ac)
    {
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

        ac_val.val   = val;



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
        leak_val.val   = val;

        emit signal_leak(leak_val);
    }
}

void ActParaItem_btn::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

