
#include "wgtlistviewmodel.h"
#include <QDebug>
#include <QFile>

wgtListViewModel::wgtListViewModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

wgtListViewModel::~wgtListViewModel()
{
    foreach (TreeNode* node, m_allNodeList)
    {
        foreach (TreeNode* childNode, node->childList)
        {
            delete childNode;
        }

        delete node;
    }
}

bool wgtListViewModel::addItem(const QString &label, const QString &decorate, const QString &decorate2, int wdgId, quint8 level, quint8 titleId1, quint8 titleId2, quint8 titleId3)
{
    TreeNode* node  = new TreeNode;
    node->label     = label;
    node->decorate  = decorate;
    node->decorate2 = decorate2;
    node->wdgId     = wdgId;
    node->level     = level;
    node->collapse  = true;

    switch (node->level)
    {
    case 1:
    {
        node->parent = NULL;
        node->titleId2 = 0;
        node->titleId3 = 0;

        if(titleId1 > m_allNodeList.count())
        {
            node->titleId1 = m_allNodeList.count() + 1;

            m_allNodeList.insert(m_allNodeList.count(), node);
        }
        else
        {
            node->titleId1 = titleId1;

            m_allNodeList.insert(titleId1 - 1, node);

            for(int i=titleId1; i<m_allNodeList.count(); i++)
            {
                m_allNodeList[i]->titleId1 = i+1;
            }
        }
        break;
    }

    case 2:
    {
        if(titleId1 > m_allNodeList.count())
        {
            return false;
        }
        else
        {
            node->parent = m_allNodeList.at(titleId1-1);
            node->titleId1 = titleId1;
        }

        node->titleId3 = 0;

        if(titleId2 > node->parent->childList.count())
        {
            node->titleId2 = node->parent->childList.count() + 1;

            node->parent->childList.insert(node->parent->childList.count(), node);
        }
        else
        {
            node->titleId2 = titleId2;

            node->parent->childList.insert(titleId2 - 1, node);

            for(int i=titleId2; i< node->parent->childList.count(); i++)
            {
                node->parent->childList[i]->titleId2 = i+1;
            }
        }
        break;
    }

    case 3:
    {
        if(titleId1 > m_allNodeList.count())
        {
            return false;
        }
        else if(titleId2 > m_allNodeList[titleId1-1]->childList.count())
        {
            return false;
        }
        else
        {
            node->parent =  m_allNodeList[titleId1-1]->childList.at(titleId2-1);
            node->titleId1 = titleId1;
            node->titleId2 = titleId2;
        }

        if(titleId3 > node->parent->childList.count())
        {
            node->titleId3 = node->parent->childList.count() + 1;

            node->parent->childList.insert(node->parent->childList.count(), node);
        }
        else
        {
            node->titleId3 = titleId3;

            node->parent->childList.insert(titleId3 - 1, node);

            for(int i=titleId3; i< node->parent->childList.count(); i++)
            {
                node->parent->childList[i]->titleId3 = i+1;
            }
        }
        break;
    }

    default:
        break;
    }


    return true;
}

bool wgtListViewModel::removeChildItem(int titleId1, int  titleId2)
{
    if((titleId1 < 0) || (titleId1 > m_allNodeList.count()))
        return false;

    m_reWidgetList.clear();
    for(int i = 0; i < m_allNodeList.count(); i++)
    {
        //一级节点
        if(m_allNodeList[i]->titleId1 == titleId1)
        {
            for(int j = 0; j < m_allNodeList[i]->childList.count(); j++)
            {
                //二级节点
                if(m_allNodeList[i]->childList[j]->titleId2 == titleId2)
                {
                    //m_reWidgetList.append(m_allNodeList[i]->childList[j]->wdgId);
                    for (int k = 0; k < m_allNodeList[i]->childList[j]->childList.count(); k++)
                    {
                        //三级节点
                        m_reWidgetList.append(m_allNodeList[i]->childList[j]->childList[k]->wdgId);
                    }

                    //删除当前节点下的所有子节点
                    TreeNode* node = m_allNodeList[i]->childList[j];
                    foreach (TreeNode* childNode, node->childList)
                    {
                        delete childNode;
                    }
                    delete node;

                    m_allNodeList[i]->childList.removeAt(j);

                    //emit removeWidgetId(m_reWidgetList);
                    return true;
                }
            }
        }
    }

    return  true;
}

bool wgtListViewModel::removeItem(int titleId1)
{
    if((titleId1 < 0) || (titleId1 > m_allNodeList.count()))
        return false;

    m_reWidgetList.clear();
    for (int i = 0; i < m_allNodeList.count(); i++)
    {
        if(m_allNodeList[i]->titleId1 == titleId1)
        {
            //把子节点对应的界面Id找出来删除
            for (int j = 0; j < m_allNodeList[i]->childList.count(); j++)
            {
                //二级节点
                m_reWidgetList.append(m_allNodeList[i]->childList[j]->wdgId);

                for (int k = 0; k < m_allNodeList[i]->childList[j]->childList.count(); k++)
                {
                    //三级节点
                    m_reWidgetList.append(m_allNodeList[i]->childList[j]->childList[k]->wdgId);
                }
            }

            //删除对应的一级节点
            m_allNodeList.removeAt(i);

            //emit removeWidgetId(m_reWidgetList);

            return true;
        }
    }

    return  true;
}

bool wgtListViewModel::queryItem(int titleId1, int titleId2)
{
    if((titleId1 < 0) || (titleId1 > m_allNodeList.count()))
        return false;

    for(int i = 0; i < m_allNodeList.count(); i++)
    {
        if(m_allNodeList.at(i)->titleId1 == titleId1)
        {
            //如果之前是折叠状态，就设置为展开
            if(m_allNodeList.at(i)->collapse == true)
            {
                m_allNodeList.at(i)->collapse = false;
                refresh();

                break;
            }
        }
    }

    for (int i = 0; i < m_dispNodelist.count(); i++)
    {
        if((m_dispNodelist.at(i)->titleId1 == titleId1) && (m_dispNodelist.at(i)->titleId2 == titleId2))
        {
            //发送菜单点击事件
            QModelIndex index =  QAbstractItemModel::createIndex(i, 0);
            collapse(index);

            return true;
        }
    }

    return true;
}

bool wgtListViewModel::queryChildItem(int titleId1, int titleId2, int titleId3)
{
    if((titleId1 < 0) || (titleId1 > m_allNodeList.count()))
        return false;

    for(int i = 0; i < m_allNodeList.count(); i++)
    {
        if(m_allNodeList.at(i)->titleId1 == titleId1)
        {
            //展开一级节点
            if(m_allNodeList.at(i)->collapse == true)
            {
                m_allNodeList.at(i)->collapse = false;
                refresh();
            }

            for (int j = 0; j < m_allNodeList[i]->childList.count(); j++)
            {
                if(m_allNodeList[i]->childList[j]->titleId2 == titleId2)
                {
                    //展开二级节点
                    if(m_allNodeList[i]->childList[j]->collapse == true)
                    {
                        m_allNodeList[i]->childList[j]->collapse = false;
                        refresh();

                        break;
                    }
                }
                else {
                    if(m_allNodeList[i]->childList[j]->collapse == false)
                    {
                        m_allNodeList[i]->childList[j]->collapse = true;
                        refresh();
                    }
                }
            }
        }
        else {
            if(m_allNodeList.at(i)->collapse == false)
            {
                m_allNodeList.at(i)->collapse = true;
                refresh();
            }
        }
    }

    for (int i = 0; i < m_dispNodelist.count(); i++)
    {
        if((m_dispNodelist.at(i)->titleId1 == titleId1) && (m_dispNodelist.at(i)->titleId2 == titleId2)
                && (m_dispNodelist.at(i)->titleId3 == titleId3))
        {
            //发送菜单点击事件
            QModelIndex index =  QAbstractItemModel::createIndex(i, 0);
            emit clicked(index);

            return true;
        }
    }

    return  true;
}

quint16 wgtListViewModel::getChildDispNum(wgtListViewModel::TreeNode *node)
{
    quint16 childs = 0;
    if(node->level == 1)
    {
        for (int i = 0; i < node->childList.count(); i++)
        {
            childs++;

            //如果一级节点不是折叠的，就把二级节点算进去
            if(node->childList[i]->collapse == false)
            {
                for (int j = 0; j < node->childList[i]->childList.count(); j++)
                {
                    childs++;
                }
            }
        }
    }
    else if(node->level == 2)
    {
        for (int i = 0; i < node->childList.count(); i++)
        {
            childs++;
        }
    }

    return childs;
}

int wgtListViewModel::rowCount( const QModelIndex &parent /* = QModelIndex */ ) const
{
    return m_dispNodelist.count();
}

QVariant wgtListViewModel::data( const QModelIndex &index, int role /* = Qt::DisplayRole */ ) const
{
    if ((!index.isValid()) || (index.row() >= m_dispNodelist.count()) || (index.row() < 0))
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        return m_dispNodelist[index.row()]->label;
    }
    else if (role == Qt::UserRole)
    {
        return (quintptr)(m_dispNodelist[index.row()]);
    }

    return QVariant();
}

void wgtListViewModel::refreshList()
{
    m_dispNodelist.clear();

    for (int i = 0; i < m_allNodeList.count(); i++)
    {
        m_dispNodelist.append(m_allNodeList.at(i));

        //如果一级节点不是折叠的，就显示二级节点
        if(m_allNodeList.at(i)->collapse == false)
        {
            //遍历每个二级节点
            for (int j = 0; j < m_allNodeList[i]->childList.count(); j++)
            {
                m_dispNodelist.append(m_allNodeList[i]->childList.at(j));

                //如果二级节点不是折叠的，就把三级节点显示出来
                if(m_allNodeList[i]->childList[j]->collapse == false)
                {
                    for (int k = 0; k < m_allNodeList[i]->childList[j]->childList.count(); k++)
                    {
                        m_dispNodelist.append(m_allNodeList[i]->childList[j]->childList.at(k));
                    }
                }
            }
        }
    }
}

void wgtListViewModel::collapse( const QModelIndex& index )
{
    TreeNode* currNode = m_dispNodelist.at(index.row());

    emit widgetCharged(currNode->titleId1, currNode->titleId2, currNode->titleId3);

    if(currNode->childList.count() == 0)
    {
        return;
    }

    currNode->collapse = !currNode->collapse;

    {
        for (int i = 0; i < m_allNodeList.count(); i++){
            if(currNode->titleId1 != m_allNodeList.at(i)->titleId1){
                m_allNodeList.at(i)->collapse = true;
            }
        }
    }

    refreshList();

    if ( !currNode->collapse )
    {
        beginInsertRows(QModelIndex(), index.row()+1, index.row()+getChildDispNum(currNode));
        endInsertRows();
    }
    else
    {
        beginRemoveRows(QModelIndex(), index.row()+1, index.row()+getChildDispNum(currNode));
        endRemoveRows();
    }
}

void wgtListViewModel::refresh()
{
    refreshList();
    beginResetModel();
    endResetModel();
}

void wgtListViewModel::setAllNodeCollapse(void)
{
    for (int i = 0; i < m_allNodeList.count(); i++){
        m_allNodeList.at(i)->collapse = true;
        for (int j = 0; j < m_allNodeList[i]->childList.count(); j++){
            m_allNodeList[i]->childList[j]->collapse = true;
        }
    }

    refresh();
}
