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

#ifndef LOGINMANAGE_H
#define LOGINMANAGE_H

#include <QWidget>
#include <QSettings>
#include <QFile>
#include <QLabel>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QHeaderView>
#include <QTabWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QCloseEvent>
#include <QMap>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <QVBoxLayout>
#include "loginui.h"
#include "language/languagePublic.h"
#include "ACData/ACDataStruct.h"
class WritesqlQueryModel;


struct RoleInfo{
    QString role_account;
    QString role_name;
    QString role_describe;
};

struct UserRole{
    QString user_account;
    QString from_role;
};

struct OneDevInfo
{
    QString parameter_name;
    QString show_name;
};

struct DevList
{
    QString dev_table_name;
    QString table_enable;
};



enum TableName{
    userinfo_table,
    userrole_table,
    roleinfo_table,
    devicelist_table,
    device_table,    
};


//登陆信息
struct Login{
    QString user_account;
    QString user_passwd;
    QString user_name;

    QString  from_role;
    RoleInfo roleInfo;
    QList<QString> devTablelist;
};

//json文件登陆获取信息
struct UserInfo{
    QString usr;
    QString nick;
    int     auth;
};


//菜单信息
struct Menu{
    QString parameter_name;
    langEnum show_name;
    int     parameter_type;
    int     sort_id;
    langEnum parameter_class;
    int     parameter_classID;
    int     show_type;
    int     enumType;


    QList<QPair<int,langEnum>>Map_type;
};

//菜单集合
struct MenuAndClassList{
    QList<Menu>     menuList;
    QList<langEnum>  classList;
};

class LoginManage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginManage(QWidget *parent = 0);

    Login getCurrentLogin(){return m_currentLogin;}
    bool  getLoginState(){return  m_loginState;}
    ~LoginManage();
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void loginVerification(QString account, QString passwd);

public:    
    //根据用户配置文件生产数据库表
    bool initIniToDatabase();
    //打开登陆界面
    void openLoginUI();
    //数据库查询登陆
    Login sqlReadUserVerificationBak(QString account,QString passwd);
    //使用角色编号查询登陆信息
    Login querySqlUserInfo(UserInfo &);
    //登陆
    Login sqlReadUserVerification(QString account,QString passwd);
    //---------------json文件登陆------------------
    bool readJsonUserVerification(QString account,QString passwd,UserInfo &);


    //按账户查询菜单参数
    bool sqlRoleAccessMenu(QString roleuser,QString devtable,MenuAndClassList & menuclasslist/*out*/);

    void openModifydataUI();


public:
    //---------------json文件登陆------------------

    UserInfo userVerification(QString account, QString passwd);
    bool roleAuthorityJson(QString role, AcDevVector &);

private:
    //-------------------acJson--------------------
    bool initJsonData(QString path, QJsonDocument &jsonData);   //json文件转为json文档
    QString openJsonFile(QString filePath); //打开json文件
    QJsonArray findJsonArrayValue(QJsonDocument &jsonDoc, QString objName); //查找数组
    void protoMapInit();    //空调协议初始化
public:
    void convertToMenuList(AcDevVector &/*out*/, MenuAndClassList &/*out*/);


private:
    //---------数据库操作----------
    bool openDatabaseFile(QString dbPath);
    bool initDatabase(QString dbPath);  //创建DB文件，根据配置文件生产数据库表和内容
    bool isTableExist(QSqlQuery &query,QString table);
    bool createTable(QSqlQuery &query,TableName table,QString devName = "");
    //创建设备表
    //bool createDevTable(QSqlQuery &query,QString tableName);
    bool initTable(QSqlQuery &query,TableName table,QSqlDatabase &db,QString devName = "");

    //配置界面
    void initUI(QString dbPath,QList<QString> devTablelist);

    //---------配置文件操作----------
    //读写配置文件
    void writeUserSetting();
    //读ini验证登陆
    Login readUserVerification(QString filePath,QString account,QString passwd);
    //查询角色信息
    RoleInfo getRoleInfo(QString filePath,QString prefix,QString role);
    //查询用户所属角色
    QString getUserRole(QString filePath,QString prefix,QString account);

    //整表查询
    //1 查询用户信息表
    bool queryAllUserInfo(QString filePath,QList<Login> &loginList);
    //2 查询用户-角色关系表
    bool queryAllUserRole(QString filePath,QList<UserRole> &userRoleList);
    //3 查询角色信息表
    bool queryAllRoleInfo(QString filePath,QList<RoleInfo> &roleInfoList);
    //4 查询设备列表
    bool queryAllDevInfo(QString filePath,QList<DevList> &devInfoList);
    //5 根据设备列表查询设备参数表(只包含设备参数名称和显示名称，方便导入到数据库)
    bool queryOneDevInfo(QString filePath,QList<OneDevInfo> &oneDevInfoList);



public:
    //6 读设备参数表，包含权限内容并写入数据库
    bool queryOneDevToDatabase(QString filePath,QSqlQuery &query,QSqlDatabase &db,QString devName);

signals:
    void loginSuccessSignal(bool);
    void closeLoginUI();

public:
    QMap<QString,QString> m_acRoleIndex;

private:    
    Login   m_currentLogin;
    QList<Login>        m_loginList;
    QList<RoleInfo>     m_roleInfoList;
    QList<UserRole>     m_userRoleList;
    QList<OneDevInfo>   m_oneDevInfoList;
    QList<DevList>      m_devList;
    QList<WritesqlQueryModel *> modeList;
    QString m_currentDir;

    QTabWidget *tabWidget;
    QSqlDatabase m_database;
    bool m_loginState;
    QMap<int,QString> m_roleIndex;

    //新空调当前协议map
    QMap<QString,bool> m_AcEn;



};

#endif // LOGINMANAGE_H
