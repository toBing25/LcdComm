#include "mask_widget.h"
#include "ui_mask_widget.h"
#include <QDebug>


MaskWidget::MaskWidget(QWidget *parent) : QDialog(parent), ui(new Ui::MaskWidget)
{
    ui->setupUi(this);
    hide();
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool |  Qt::WindowDoesNotAcceptFocus);
    qApp->installEventFilter(this);
}

MaskWidget::~MaskWidget()
{
    delete ui;
}

void MaskWidget::setMainWidget(QWidget *pWidget)
{

    this->setFixedSize(QSize(pWidget->width()*2, pWidget->height()*2));
    this->setParent(pWidget);
    this->move(pWidget->x(), pWidget->y());


}

void MaskWidget::setNames(const QStringList& names)
{
     if(m_listName == names)
     {
         return;
     }
     m_listName = names;
 }

QStringList MaskWidget::names() const
{
 return m_listName;
}

bool MaskWidget::eventFilter(QObject *obj, QEvent *event)
{
 if(event->type() == QEvent::Hide)
 {
     if(m_listName.contains(obj->objectName()))
     {
         hide();
     }
     return QObject::eventFilter(obj, event);
 }

 if (event->type() == QEvent::Show)
 {     
     if (!m_listName.contains(obj->objectName()))
     {
         return QObject::eventFilter(obj, event);
     }

     show();

     auto pWidget = dynamic_cast<QWidget*>(obj);

     if (Q_NULLPTR == pWidget)
     {
         return QObject::eventFilter(obj, event);
     }
     pWidget->activateWindow();
     pWidget->setFocus(Qt::ActiveWindowFocusReason);
     stackUnder(pWidget);


     if(Q_NULLPTR == m_pMainWidget)
     {
         return QObject::eventFilter(obj, event);
     }

     m_pMainWidget->stackUnder(this);

     QRect screenGeometry = m_pMainWidget->geometry();
     int x = screenGeometry.x() + (screenGeometry.width() - pWidget->width()) / 2;
     int y = screenGeometry.y() + (screenGeometry.height() - pWidget->height()) / 2;
     pWidget->move(x, y);
 }

 return QObject::eventFilter(obj, event);
}
