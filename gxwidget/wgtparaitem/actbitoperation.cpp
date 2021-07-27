#include "actbitoperation.h"
#include "ui_actbitoperation.h"
#include "buzzer.h"
#include <QDesktopWidget>
#include <QSignalMapper>
#include <QCheckBox>
#include <QByteArray>
#include <QDebug>


ActBitOperation::ActBitOperation(QStringList list, QString val, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActBitOperation),
    m_list(list),
    m_bitNum(list.count()),
    m_val(val)
{
    m_valArray = new int [m_bitNum]();
    memset(m_valArray, 0, sizeof(int)*m_bitNum);

    setupUi();

    connect(ui->btnSet, &QPushButton::clicked, this, &ActBitOperation::onBtnClicked);
}

ActBitOperation::~ActBitOperation()
{
    delete ui;
}

void ActBitOperation::setupUi(void)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);

    QGridLayout *gLayout = new QGridLayout;
    gLayout->setSpacing(15);
    gLayout->setObjectName(QStringLiteral("gLayout"));
    gLayout->setContentsMargins(10, 10, 10, 10);

    int row, col;
    int i, j, k;
    col   = 8;
    row   = m_bitNum / col;
    if(m_bitNum % col)
        ++row;

    QSignalMapper *signalMapper = new QSignalMapper(this);
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            k = i * col + j;
            if(k >= m_bitNum)
                break;

            QCheckBox *checkbox = new QCheckBox(ui->wgtBody);
            checkbox->setText(QString("%1").arg(m_list.at(k)));
            checkbox->setStyleSheet("QCheckBox:checked{"
                                    "color:red"
                                    "}"
                                    "QCheckBox{"
                                    "color:rgb(255,255,255);"
                                    "}");

            connect(checkbox, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(checkbox, QString("%1").arg(k));

            QString str = decTobin(m_val);
            //根据值选中具体的位
           if((str.count() - (k + 1) > -1)  && (str.count() - (k + 1) < str.count()))
           {
               if(str.at(str.count() - (k + 1)) == "0")
               {
                   m_valArray[k] = 0;
                   checkbox->setChecked(false);
               }
               else if (str.at(str.count() - (k + 1)) == "1")
               {
                   m_valArray[k] = 1;
                   checkbox->setChecked(true);
               }
           }

            m_vector.append(checkbox);
            gLayout->addWidget(checkbox, i, j);
        }
    }
    //将转发的信号连接到最终的槽函数
    connect(signalMapper, SIGNAL(mapped(QString )), this, SLOT(onComBoBoxClicked(QString)));

    ui->wgtBody->setLayout(gLayout);
    ui->wgtBody->resize(sizeHint());
}

void ActBitOperation::onBtnClicked(bool /*checked*/)
{
    ShortBeep();

    m_result.clear();
    for (int i = m_bitNum - 1; i > -1; i--) {
        m_result.append(QString("%1").arg(m_valArray[i]));
    }

    //释放掉构造函数申请的内存
    free(m_valArray);

    this->done(binToDec(m_result).toInt());
}

void ActBitOperation::onComBoBoxClicked(QString bit)
{
    if(m_vector.at(bit.toInt())->isChecked())
        m_valArray[bit.toInt()] = 1;
    else
        m_valArray[bit.toInt()] = 0;

    m_result.clear();
    for (int i = m_bitNum - 1; i > -1; i--) {
        m_result.append(QString("%1").arg(m_valArray[i]));
    }
}

QString ActBitOperation::binToDec(QString strBin)
{
    QString decimal;
    int nDec = 0;
    int nLen, i, j, k;

    nLen = strBin.length();
    for (i = 0; i < nLen; i++) {
        if(strBin[nLen-i-1] == '0')
            continue;
        else {
            k =1;
            for (j = 0; j < i; j++)
                k = k*2;
            nDec += k;
        }
    }
    decimal = QString::number(nDec);
    return  decimal;
}

QString ActBitOperation::decTobin(QString strDec)
{
    int nDec = strDec.toInt();
    int nYushu, nShang;
    QString strBin, strTemp;
    bool bContinue = true;
    while ( bContinue )
    {
        nYushu  = nDec % 2;
        nShang  = nDec / 2;
        strBin  = QString::number(nYushu)+strBin;
        strTemp = strBin;
        nDec    = nShang;
        if ( nShang == 0 )
            bContinue = false;
    }
    int nTemp = strBin.length() % 4;
    switch(nTemp)
    {
    case 1:
        strTemp = "000"+strBin;
        strBin  = strTemp;
        break;
    case 2:
        strTemp = "00"+strBin;
        strBin  = strTemp;
        break;
    case 3:
        strTemp = "0"+strBin;
        strBin  = strTemp;
        break;
    default:
        break;
    }
    return strBin;
}

void ActBitOperation::showEvent(QShowEvent *)
{
    QRect src;
    if(parent() == 0)
        src = QApplication::desktop()->screenGeometry();
    else {
        src = static_cast<QWidget *>(parent())->rect();
        src.setX(0);
        src.setY(0);
    }
    move(src.center() - rect().center());
}
