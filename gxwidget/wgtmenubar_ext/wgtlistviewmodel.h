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

#ifndef WGTLISTVIEWMODEL_H
#define WGTLISTVIEWMODEL_H

#include <QAbstractListModel>
#include <vector>

class wgtListViewModel : public QAbstractListModel
{
	Q_OBJECT

public:
	struct TreeNode
	{
            QString label;      //标题
            QString decorate;   //图标
            QString decorate2;   //图标
            quint8 level;       //层次
            quint8 titleId1;    //一级节点
            quint8 titleId2;    //二级节点
            quint8 titleId3;    //三级节点

            int wdgId;          //当前节点对应的窗体ID
            bool collapse;      //当前是否是折叠状态

            TreeNode* parent;
            QList<TreeNode*> childList;  //当前节点的子节点列表
		bool theLast;
	};

    wgtListViewModel(QObject *parent);
    ~wgtListViewModel();

    bool addItem(const QString &label, const QString &decorate, const QString &decorate2, int wdgId, quint8 level, quint8 titleId1, quint8 titleId2 = 0, quint8 titleId3 = 0);

    //删除一级节点
    bool removeItem(int titleId1);
    //跳转到一级节点
    bool queryItem(int titleId1, int titleId2);

    //删除二级节点
    bool removeChildItem(int titleId1, int  titleId2);
    //跳转到二级节点
    bool queryChildItem(int titleId1, int titleId2, int titleId3);

    //设置所有的节点折叠
    void setAllNodeCollapse(void);

    void refresh();

	int rowCount(const QModelIndex &parent /* = QModelIndex */) const;
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;

public slots:
    void collapse(const QModelIndex& index);

signals:
    void widgetCharged(int titleId1, int titleId2, int titleId3);
    void removeWidgetId(QList<int> list);
    void clicked(const QModelIndex &index);

private:
    quint16 getChildDispNum(TreeNode *node);

    void refreshList();

private:
    QList<TreeNode*> m_allNodeList;  //总的链表, 存放一级节点
    QList<TreeNode*> m_dispNodelist; //显示链表，存放显示的节点
    QList<int>       m_reWidgetList; //要删除的页面列表

};

#endif // WGTLISTVIEWMODEL_H
