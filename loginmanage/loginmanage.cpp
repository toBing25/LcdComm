#include "loginmanage.h"
#include "writesqlquerymodel.h"
#include "Global/RatioStruct.h"
#include "conversiontool.h"
#include <QDateTime>
#include <QDebug>
#include "language/languagePublic.h"
#include "formmain.h"
#define USERINFO_NAME   "user_info"
#define ROLEINFO_NAME   "role_info"
#define USERROLE_NAME   "user_role"
#define SETTING_SUFFIX  ".ini"
#define DEVICELIST_NAME "devices_list"
#define DEVICE_NAME     "ups"
#define DATABASE_FILE   "unify_ui.db"
#define JSONUSRINFO_FILE    "/etc/.eMTR/web/usrInfo.txt"

LoginManage::LoginManage(QWidget *parent) :
    QWidget(parent),
    tabWidget(NULL),
    m_loginState(false),
    m_currentDir(QApplication::applicationDirPath())
{
    //初始化数据库字符到枚举map,用于枚举名称字符串查找枚举
    langMap_Init();
    //空调协议初始化
    protoMapInit();

    //角色索引 方便和web同步账号信息
    m_roleIndex.insert(0,"role003");    //超级管理员
    m_roleIndex.insert(1,"role002");    //管理员
    m_roleIndex.insert(2,"role001");    //普通用户

    m_acRoleIndex.insert("role003", "feserve");    //超级管理员
    m_acRoleIndex.insert("role002", "admin");    //管理员
    m_acRoleIndex.insert("role001", "user");    //普通用户


    //查询表
//    QString userPath = QApplication::applicationDirPath()+"/user_info.ini";
//    bool ok = queryAllUserInfo(userPath,m_loginList);
//    foreach (Login login, m_loginList) {
//        qDebug()<<"user_info start";
//        qDebug()<<login.user_account;
//        qDebug()<<login.user_name;

//    }
    //用户登陆
//    QString user = "a2021001";
//    QString pwd  = "a2021001";
//    Login login = sqlReadUserVerification(user,pwd);
//    if(login.user_account == ""){
//        qDebug()<<"登陆失败";
//    }else {
//        qDebug()<<"登陆成功";
//        qDebug()<<login.user_account<<" "<<login.user_passwd<<" "<<login.user_name<<" "<<login.from_role<<" "<<login.roleInfo.role_name<<" "<<login.roleInfo.role_describe;
//        foreach (QString dev , login.devTablelist) {
//            qDebug()<<dev;
//        }

//        //查询菜单表
//        QList<Menu> menuList;
//        sqlRoleAccessMenu(login.from_role,login.devTablelist.at(0),menuList);

//        foreach (Menu menu, menuList) {
//            qDebug()<<menu.parameter_name<<" "<<menu.show_name<<" "<<menu.parameter_type;
//        }
//    }

    //查询菜单表
//    QList<Menu> menuList;
//    sqlRoleAccessMenu("role001","ups",menuList);

//    foreach (Menu menu, menuList) {
//        qDebug()<<menu.parameter_name<<" "<<menu.show_name<<" "<<menu.parameter_type;
//    }

    //初始化UI界面
    //initUI(m_currentDir+"/"+DATABASE_FILE);


    //初始化ini文件用户信息
    //writeUserSetting();
    //初始化数据库
    //initDatabase(m_currentDir+"/"+DATABASE_FILE);

    //登陆
//    QString userPath = QApplication::applicationDirPath()+"/user_info.ini";
//    Login login;
//    login = readUserVerification(userPath,"c2021001","c2021001");
//    if(login.user_name == ""){
//        qDebug()<<"登陆失败";
//    }else{
//        qDebug()<< login.user_name<<login.user_account<<" 登陆成功";
//        QString rolePath = QApplication::applicationDirPath()+"/user_role.ini";
//        QString roleAccount = getUserRole(rolePath,"user_role",login.user_account);
//        qDebug()<<"所属角色 ="<<roleAccount;

//        //查询角色信息
//        QString roleInfoPath = QApplication::applicationDirPath()+"/role_info.ini";
//        RoleInfo roleinfo = getRoleInfo(roleInfoPath,"role_info",roleAccount);
//        qDebug()<<"角色信息"<<roleAccount<<"|"<<roleinfo.role_name<<"|"<<roleinfo.role_describe;
//    }



}


LoginManage::~LoginManage()
{
    //delete ui;
}

void LoginManage::closeEvent(QCloseEvent *event)
{
    //qDebug()<<"close event";
    this->hide();
    event->ignore();
}

void LoginManage::loginVerification(QString account, QString passwd)
{    
    m_currentLogin = sqlReadUserVerification(account,passwd);
    if(m_currentLogin.user_name.isEmpty()){
        emit loginSuccessSignal(false);
    }else{
        emit loginSuccessSignal(true);
        emit closeLoginUI();
    }
}

bool LoginManage::initIniToDatabase()
{
    return  initDatabase(m_currentDir+"/"+DATABASE_FILE);

}

void LoginManage::openLoginUI()
{
    LoginUI *dialog = new LoginUI();
    dialog->setModal(Qt::ApplicationModal);
    dialog->show();
    connect(dialog,&LoginUI::userInfoSignal,this,&LoginManage::loginVerification);
    connect(this,&LoginManage::loginSuccessSignal,dialog,&LoginUI::loginSuccess);
    connect(this,&LoginManage::closeLoginUI,dialog,&LoginUI::close);
}

Login LoginManage::sqlReadUserVerificationBak(QString account, QString passwd)
{
    Login login;
    QString sql = "";
    QByteArray pwdMD5;
    openDatabaseFile(m_currentDir+"/"+DATABASE_FILE);
    QSqlQuery query(m_database);
    sql = QString("select * from %1 where user_account = '%2'").arg(USERINFO_NAME,account);
    query.exec(sql);

    QString lib_passwd,lib_name,lib_role_account,lib_dev_table_name,lib_table_enable = "";
    pwdMD5 = QCryptographicHash::hash(passwd.toUtf8(), QCryptographicHash::Md5 );
    //qDebug()<<"md5Pwd"<<pwdMD5.toHex();
    //遍历取得的所有结果
    while(query.next()){
        lib_passwd = query.value("user_passwd").toString();
        if(lib_passwd == pwdMD5.toHex()){   //登陆成功

            lib_name = query.value("user_name").toString();
            login.user_account  = account;
            login.user_passwd   = lib_passwd;
            login.user_name     = lib_name;
            //查询用户所属角色
            sql = QString("select * from %1 where user_account = '%2'").arg(USERROLE_NAME,account);
            query.exec(sql);
            while(query.next()){
                lib_role_account = query.value("from_role").toString();
                if(lib_role_account != ""){
                    login.from_role = lib_role_account;
                    //查询角色信息
                    sql = QString("select * from %1 where role_account = '%2'").arg(ROLEINFO_NAME,lib_role_account);
                    query.exec(sql);
                    while(query.next()){
                        login.roleInfo.role_account = query.value("role_account").toString();
                        login.roleInfo.role_name = query.value("role_name").toString();
                        login.roleInfo.role_describe = query.value("role_describe").toString();
                        //查询设备列表
                        sql = QString("select * from %1").arg(DEVICELIST_NAME);
                        query.exec(sql);
                        while(query.next()){
                            lib_table_enable = query.value("table_enable").toString();
                            if(lib_table_enable == "1"){    //设备表可用
                                lib_dev_table_name = query.value("dev_table_name").toString();
                                login.devTablelist.append(lib_dev_table_name);
                            }
                        }

                    }

                }
            }

        }
    }

    m_database.close();
    //登陆成功更新全局当前登陆
    m_loginState = true;
    m_currentLogin = login;
    return login;
}

Login LoginManage::querySqlUserInfo(UserInfo &usrInfo)
{
    Login login;
    QString sql = "";
    openDatabaseFile(m_currentDir+"/"+DATABASE_FILE);
    QSqlQuery query(m_database);
    QString lib_role_account,lib_dev_table_name,lib_table_enable = "";

    login.user_account  = usrInfo.usr;
    login.user_passwd   = "";
    login.user_name     = usrInfo.nick;

    //查询用户所属角色

    lib_role_account = m_roleIndex[usrInfo.auth];
    if(lib_role_account != ""){
        login.from_role = lib_role_account;
        //查询角色信息
        sql = QString("select * from %1 where role_account = '%2'").arg(ROLEINFO_NAME,lib_role_account);
        query.exec(sql);
        while(query.next()){
            login.roleInfo.role_account = query.value("role_account").toString();
            login.roleInfo.role_name = query.value("role_name").toString();
            login.roleInfo.role_describe = query.value("role_describe").toString();
            //查询设备列表
            sql = QString("select * from %1").arg(DEVICELIST_NAME);
            query.exec(sql);
            while(query.next()){
                lib_table_enable = query.value("table_enable").toString();
                if(lib_table_enable == "1"){    //设备表可用
                    lib_dev_table_name = query.value("dev_table_name").toString();
                    login.devTablelist.append(lib_dev_table_name);
                }
            }

        }

    }





    m_database.close();
    //登陆成功更新全局当前登陆
    m_loginState = true;
    m_currentLogin = login;
    return login;
}

Login LoginManage::sqlReadUserVerification(QString account, QString passwd)
{
    Login login;
    UserInfo usrInfo;
    bool ok = readJsonUserVerification(account,passwd,usrInfo);
    if(ok){
        return querySqlUserInfo(usrInfo);
    }else {
        return login;
    }
}

bool LoginManage::sqlRoleAccessMenu(QString roleuser, QString devtable, MenuAndClassList &menuclasslist)
{
    menuclasslist.menuList.clear();
    menuclasslist.classList.clear();
    bool ret = false;
    QString sql = "";
    openDatabaseFile(m_currentDir+"/"+DATABASE_FILE);
    QSqlQuery query(m_database);
    sql = QString("select * from %1 where %2 = '1'").arg(devtable,roleuser);
    query.exec(sql);
    //qDebug()<<sql;
    //遍历取得的所有结果
    while(query.next()){
        Menu menu;
        ret = true;
        menu.parameter_type     = query.value("parameter_type").toInt();
        menu.parameter_name     = query.value("parameter_name").toString();

        menu.show_name          = langMap[query.value("show_name").toString()];     //翻译
        menu.parameter_class    = langMap[query.value("parameter_class").toString()];

        menu.parameter_classID  = query.value("parameter_classID").toInt();        
        menu.show_type          = query.value("show_type").toInt();
        menu.sort_id            = query.value("sort_id").toInt();

        QString set = query.value("type_strings").toString();

        //获取内置设置值        
        if(menu.show_type != 0){            
            if(set.length()>0){
                QStringList setList = set.split("|");

                foreach (QString s, setList) {
                    QPair<int,langEnum> mVal;
                    if(s.length()>0){
                        QStringList valueList = s.split("-");
                        if(valueList.length()== 2){
                            mVal.first = valueList.at(0).trimmed().toInt();
                            mVal.second = langMap[valueList.at(1).trimmed()];   //翻译

                        }
                    }
                    menu.Map_type.append(mVal);
                }

            }
        }



        //添加到列表
        menuclasslist.menuList.append(menu);

    }

    //查询类别消息
    sql = QString("select DISTINCT parameter_class from %1 where %2 = '1'").arg(devtable,roleuser);
    query.exec(sql);
    //qDebug()<<sql;
    //遍历取得的所有结果
    while(query.next()){
        langEnum className = langMap[query.value("parameter_class").toString()]; //翻译
        menuclasslist.classList.append(className);
    }


    m_database.close();

    return ret;
}

void LoginManage::openModifydataUI()
{
    if(m_loginState){
        //获取设备列表

        foreach (QString dev, m_currentLogin.devTablelist) {
            //qDebug()<<"dev list ="<<dev;

        }
        initUI(m_currentDir+"/"+DATABASE_FILE,m_currentLogin.devTablelist);
    }

}

UserInfo LoginManage::userVerification(QString account, QString passwd)
{
    UserInfo usrInfo;
    bool ret = false;
    bool ok = readJsonUserVerification(account,passwd,usrInfo);
    if(ok){
        ret = roleAuthorityJson(m_roleIndex[usrInfo.auth], FormMain::m_acDev); //登陆后读取数据
    }
    if(ret == true)
        return usrInfo;
    else
        return UserInfo{"","",-1};
}

bool LoginManage::roleAuthorityJson(QString role, AcDevVector &acDev)
{
    acDev.acVec.clear();
    QJsonDocument jsonDoc;

    QString path = QString("%1").arg(QCoreApplication::applicationDirPath() + "/acTest.json");
    bool ok = initJsonData(path,jsonDoc);

    if(ok){
        QJsonArray className = findJsonArrayValue(jsonDoc,"className");
        QJsonArray classCout = findJsonArrayValue(jsonDoc,"classCount");

        //第一维 分类
        for(int i=0;i<className.size();++i){
            //第一级名称
            acDev.firtLevelName.append(className.at(i).toString());

            QJsonArray arr = findJsonArrayValue(jsonDoc,className.at(i).toString());   //类的参数数据集合
            QVector<QVector<onePara>> oneList;

            QVector<QString> VsubName;
            //第二维 子分类
            for(int j=0;j<classCout.at(i).toArray().size();++j){
                //第二级名称
                VsubName.append(classCout.at(i).toArray().at(j).toString());
                //第三维 参数
                QVector<onePara> oneVec;

                for(int z=0;z<arr.size();++z){
                    QJsonObject jObj = arr.at(z).toObject();
                    if(j == jObj["subClass"].toInt()){

                        if(jObj[role].toInt() == 1){
                            onePara oneP;

                            oneP.mainClass = i;
                            oneP.subClass = j;
                            oneP.enumType = jObj["enumType"].toInt();
                            oneP.enumName = jObj["enumName"].toString();
                            oneP.showType = jObj["showType"].toInt();
                            oneP.languageName = langMap[jObj["languageName"].toString().trimmed()];
                            //qDebug()<<__LINE__ <<jObj["languageName"].toString() <<oneP.languageName;
                            //选项内容
                            if(oneP.showType >0){
                                oneP.typeStrList.clear();

                                QJsonArray opArr = jObj["optionString"].toArray();
                                for(int i=0;i<opArr.size();++i){
                                    QJsonArray valArr = opArr.at(i).toArray();
                                    if(valArr.size() ==2){
                                        QPair<QString,langEnum> mVal;
                                        mVal.first = valArr.at(0).toString().trimmed();
                                        mVal.second = langMap[valArr.at(1).toString().trimmed()];
                                        //qDebug()<<__FILE__<<__LINE__ <<valArr.at(0).toString().trimmed() <<mVal.first;
                                        oneP.typeStrList.append(mVal);

                                    }

                                }
                            }


                            oneVec.append(oneP);
                        }

                    }

                }
                //----------------
                oneList.append(oneVec);
            }
            acDev.towLevelName.append(VsubName);
            acDev.acVec.append(oneList);
        }

        qDebug()<<"acVec size = "<<acDev.acVec.size();

        if(acDev.acVec.length()>0){
            return true;
        }else {
            return false;
        }

       qDebug()<<111;
    }else{
        qDebug()<<"database file not find";
    }
}

bool LoginManage::initJsonData(QString path, QJsonDocument &jsonData)
{
    QString value = openJsonFile(path);
    QJsonParseError parseJsonErr;
    jsonData = QJsonDocument::fromJson(value.toUtf8(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError)){
        return false;
    }
    return true;
}

QString LoginManage::openJsonFile(QString filePath)
{
    QFile file(filePath);
    bool ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(ok){
       QString value = file.readAll();
       return  value;
    }
    return "";
}

QJsonArray LoginManage::findJsonArrayValue(QJsonDocument &jsonDoc, QString objName)
{
    QJsonObject objList = jsonDoc.object();
    return  objList[objName].toArray();
}

void LoginManage::protoMapInit()
{

    m_AcEn.insert("IR_IT1_x10",true);   //室内回风温度
    m_AcEn.insert("IR_IH_x10",true);   //室内回风湿度
    m_AcEn.insert("IR_OT1_x10",true);   //室内送风温度
    m_AcEn.insert("IR_OH_x10",true);   //室内送风湿度
    m_AcEn.insert("IR_CMPRA_WKTM",true);   //压机A累计时间值
    m_AcEn.insert("IR_HRAT1_WKTM",true);   //加热1累计时间值
    m_AcEn.insert("IR_HUMI_WKTM",true);   //加湿累计运行时间
    m_AcEn.insert("RU_CTL_WAYS",true);      //控制方式
    m_AcEn.insert("IR_SYS_RUN_MOD",true);   //运行模式

    //控制
    m_AcEn.insert("RU_ONOFF_CTL",true);   //开关机控制
    m_AcEn.insert("RU_OTS_x10",true);   //送风温度设定
    m_AcEn.insert("RU_ITS_x10",true);   //回风温度设定
    m_AcEn.insert("RU_HS_x10",true);    //湿度设定
    m_AcEn.insert("RF_IT_OH_x10",true);   //回风高温告警值
    m_AcEn.insert("RF_IT_OL_x10",true);   //回风低温告警值
    m_AcEn.insert("RF_OT_OH_x10",true);   //送风高温告警值
    m_AcEn.insert("RF_OT_OL_x10",true);   //送风低温告警值
    m_AcEn.insert("RF_H_OH_x10",true);   //高湿告警值
    m_AcEn.insert("RF_H_OL_x10",true);   //低湿告警值


}


void LoginManage::convertToMenuList(AcDevVector &acDev, MenuAndClassList &acMenuList)
{
    Menu acMenu;

    for (int i=0; i<acDev.acVec.size(); ++i){
        for (int j=0; j<acDev.acVec.at(i).size(); ++j){
            acMenuList.classList.append(langMap[acDev.towLevelName.at(i).at(j)]);

            for (int k=0; k<acDev.acVec.at(i).at(j).size(); ++k){

                if (m_AcEn[acDev.acVec.at(i).at(j).at(k).enumName]){
                    acMenu.parameter_name = acDev.acVec.at(i).at(j).at(k).enumName;
                    acMenu.enumType = acDev.acVec.at(i).at(j).at(k).enumType;
                    acMenu.show_type = acDev.acVec.at(i).at(j).at(k).showType;
                    acMenu.show_name = acDev.acVec.at(i).at(j).at(k).languageName;
                    acMenu.sort_id = 0;

                    int mainClass = acDev.acVec.at(i).at(j).at(k).mainClass;
                    int subClass = acDev.acVec.at(i).at(j).at(k).subClass;
                    acMenu.parameter_class = langMap[acDev.towLevelName.at(mainClass).at(subClass)];
                    //参数
                    acMenu.Map_type.clear();
                    QPair<int,langEnum> typeString;
                    for(int c=0; c<acDev.acVec.at(i).at(j).at(k).typeStrList.size(); ++c){
                        int16 val = acDev.acVec.at(i).at(j).at(k).typeStrList.at(c).first.toInt(0,16);
                        typeString.first = val;
                        typeString.second =acDev.acVec.at(i).at(j).at(k).typeStrList.at(c).second;
                        acMenu.Map_type.append(typeString);
                    }

                    //除了状态都是设置
                    if(i == 0)
                        acMenu.parameter_type = 0;
                    else
                        acMenu.parameter_type = 2;

                    //添加到列表
                    acMenuList.menuList.append(acMenu);
                }


            }
        }
    }


}

bool LoginManage::openDatabaseFile(QString dbPath)
{
    bool ret = false;
    //判断数据库文件是否存在
    QFile file(dbPath);
    if (!file.exists()){
        QMessageBox::information(0,tr("提示"),tr("数据库文件打开失败！"));
        return false;
    }

    //打开数据库

    if(QSqlDatabase::contains("menu_sqlite"))
      m_database = QSqlDatabase::database("menu_sqlite");
    else
      m_database = QSqlDatabase::addDatabase("QSQLITE","menu_sqlite");

    m_database.setDatabaseName(dbPath);
    if (!m_database.open())
    {
        //qDebug() << "Error: Failed to connect database." << m_database.lastError();
        ret = false;
    }
    else
    {
        //qDebug() << "Succeed to connect database." ;
        ret = true;
    }
    return ret;

}

bool LoginManage::initDatabase(QString dbPath)
{
    //1-设置数据库文件，创建数据库文件
    //判断数据库文件是否存在,提示初始化数据库
    QFile file(dbPath);
    if (file.exists()){
        if(QMessageBox::No == QMessageBox::question(this,tr("警告"),tr("数据库已存在,是否初始化数据库?"),QMessageBox::Yes | QMessageBox:: No))
        {
            return 0;
        }else {
            //删除文件
            file.remove();
        }
    }

    //建立并打开数据库

    QSqlDatabase database;

    if(QSqlDatabase::contains("menu_sqlite"))
      database = QSqlDatabase::database("menu_sqlite");
    else
      database = QSqlDatabase::addDatabase("QSQLITE","menu_sqlite");

        database.setDatabaseName(dbPath);
    if (!database.open())
    {
        //qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        //qDebug() << "Succeed to connect database." ;
    }
    QSqlQuery sql_query;
    QString     sql;

    //2-读取配置表，获取要写入的默认用户名和密码 以及角色名称
    queryAllUserInfo(m_currentDir+"/"+USERINFO_NAME+SETTING_SUFFIX,m_loginList);
    queryAllUserRole(m_currentDir+"/"+USERROLE_NAME+SETTING_SUFFIX,m_userRoleList);
    queryAllRoleInfo(m_currentDir+"/"+ROLEINFO_NAME+SETTING_SUFFIX,m_roleInfoList);
    queryAllDevInfo(m_currentDir+"/"+DEVICELIST_NAME+SETTING_SUFFIX,m_devList);

    //

    //3-创建 “用户信息表”，“角色信息表”，“用户角色关系表”，“设备列表”，
    createTable(sql_query,TableName::userinfo_table);
    createTable(sql_query,TableName::roleinfo_table);
    createTable(sql_query,TableName::userrole_table);
    createTable(sql_query,TableName::devicelist_table);

    //4-插入默认数据
    initTable(sql_query,TableName::userinfo_table,database);
    initTable(sql_query,TableName::roleinfo_table,database);
    initTable(sql_query,TableName::userrole_table,database);
    initTable(sql_query,TableName::devicelist_table,database);


    if(m_devList.length()!= 0){
        foreach (DevList devlist, m_devList) {
               //qDebug()<<devlist.dev_table_name;
            //从配置文件获取设备数据
            queryOneDevInfo(m_currentDir+"/"+devlist.dev_table_name+SETTING_SUFFIX,m_oneDevInfoList);
            if(m_oneDevInfoList.length() == 0)
            {
                //获取数据失败，退出当次循环，查询下一个表
                continue;
            }
            //根据角色信息创建设备表头
            createTable(sql_query,TableName::device_table,devlist.dev_table_name);
            //插入设备数据
            //initTable(sql_query,TableName::device_table,database,devlist.dev_table_name);
            //qDebug()<<devlist.dev_table_name<<m_devList.length();
            //读取参数和权限表.txt并写入数据库
            queryOneDevToDatabase(m_currentDir+"/"+devlist.dev_table_name+".txt",sql_query,database,devlist.dev_table_name);

        }
    }

    //根据 ini文件写入数据库，只包含参数名称和显示名称
    //createTable(sql_query,TableName::device_table);

    database.close();
}

bool LoginManage::isTableExist(QSqlQuery &query, QString table)
{
    query.exec(QString("select count(*) from %1 where type='table' and name='%2'").arg("sqlite_master",table));
    if(query.next()) {
        return query.value(0).toInt();
    }
    else {
        return false;
    }
}

bool LoginManage::createTable(QSqlQuery &query, TableName table, QString devName)
{
    bool ret = false;
    QString sql = "";
    QString tableName = "";
    QString rolestr = "";
    switch (table) {
    case TableName::userinfo_table:
        tableName = USERINFO_NAME;
        sql = QString("create table %1(user_account nvarchar(32) primary key,"
                      " user_passwd nvarchar(32),user_name nvarchar(32))").arg(tableName);
        break;
    case TableName::roleinfo_table:
        tableName = ROLEINFO_NAME;
        sql = QString("create table %1(role_account nvarchar(32) primary key,"
                      " role_name nvarchar(32),role_describe nvarchar(128))").arg(tableName);
        break;
    case TableName::userrole_table:
        tableName = USERROLE_NAME;
        sql = QString("create table %1(user_account nvarchar(32) primary key,"
                      " from_role nvarchar(32))").arg(tableName);
        break;
    case TableName::devicelist_table:
        tableName = DEVICELIST_NAME;
        sql = QString("create table %1(dev_table_name nvarchar(32) primary key,"
                      " table_enable char(1))").arg(tableName);
        break;
    case TableName::device_table:   //设备表按设备名称创表
        //tableName = DEVICE_NAME;
        //根据角色数量动态创建表
        if(m_roleInfoList.length()>0){

            foreach (RoleInfo roleinfo, m_roleInfoList) {
                rolestr+=","+roleinfo.role_account+" char(1) default 0";
            }
        }
        sql = QString("create table %1(parameter_name nvarchar(32) primary key UNIQUE,"
                      " show_name nvarchar(32),parameter_type char(1) default 0,parameter_class nvarchar(32) default '0'%2)").arg(devName,rolestr);
        //qDebug()<<sql;
        break;
    default:
        break;
    }


    //创建表格
    if (!isTableExist(query,tableName)){

        if(!query.exec(sql))
        {
           //qDebug() << "Error: Fail to create table."<< query.lastError();
            ret = false;
        }
        else
        {
            //qDebug() << "Table created!";
            ret = true;

        }
    }
    return ret;
}



bool LoginManage::initTable(QSqlQuery &query, TableName table, QSqlDatabase &db, QString devName)
{
    bool ret = false;
    QString sql = "";
    QString tableName = "";
    QString roleFiller = "";
    //密码加密
    QByteArray pwdMD5;

    int roleCount = 0;
    switch (table) {
    case TableName::userinfo_table:
        tableName = USERINFO_NAME;
        db.transaction();
        foreach (Login login, m_loginList) {
            //md5加密
            pwdMD5 = QCryptographicHash::hash(login.user_passwd.toUtf8(), QCryptographicHash::Md5 );
            //qDebug()<<pwdMD5.toHex();
            sql = QString("INSERT INTO %1 VALUES('%2', '%3', '%4')")
                    .arg(tableName,login.user_account,pwdMD5.toHex(),login.user_name);
            if(!query.exec(sql))
            {
               //qDebug() << query.lastError();
               ret = false;
            }
            else
            {
               ret =true;
            }
        }
        db.commit();

        break;
    case TableName::roleinfo_table:
        tableName = ROLEINFO_NAME;

        db.transaction();
        foreach (RoleInfo roleinfo, m_roleInfoList) {
            sql = QString("INSERT INTO %1 VALUES('%2', '%3', '%4')")
                    .arg(tableName,roleinfo.role_account,roleinfo.role_name,roleinfo.role_describe);
            if(!query.exec(sql))
            {
               //qDebug() << query.lastError();
               ret = false;
            }
            else
            {               
               ret = true;
            }
        }
        db.commit();
        break;
    case TableName::userrole_table:
        tableName = USERROLE_NAME;
        db.transaction();
        foreach (UserRole userrole,m_userRoleList) {
            sql = QString("INSERT INTO %1 VALUES('%2', '%3')")
                    .arg(tableName,userrole.user_account,userrole.from_role);
            if(!query.exec(sql))
            {
               //qDebug() << query.lastError();
               ret = false;
            }
            else
            {
               ret = true;
            }
        }
        db.commit();

        break;
    case TableName::devicelist_table:
        tableName = DEVICELIST_NAME;
        db.transaction();
        foreach (DevList devlist,m_devList) {
            sql = QString("INSERT INTO %1 VALUES('%2', '%3')")
                    .arg(tableName,devlist.dev_table_name,devlist.table_enable);
            if(!query.exec(sql))
            {
               //qDebug() << query.lastError();
               ret = false;
            }
            else
            {
               ret = true;
            }
        }
        db.commit();
        break;
    case TableName::device_table:   //设备表，按传入的名称插入数据
        roleCount = m_roleInfoList.length();
        for(int i=0;i<roleCount;i++){
            roleFiller+=",0";  //
        }
        //设备添加参数数据
        db.transaction();
        foreach (OneDevInfo onedevinfo,m_oneDevInfoList) {
            sql = QString("INSERT OR IGNORE INTO %1 VALUES('%2', '%3','0','0'%4)")
                    .arg(devName,onedevinfo.parameter_name,onedevinfo.show_name,roleFiller);

            if(!query.exec(sql))
            {
               //qDebug() << query.lastError();
               ret = false;
            }
            else
            {               
               ret = true;
            }
        }
        db.commit();
        break;
    default:
        break;
    }

    return ret;
}

void LoginManage::writeUserSetting()
{

    QList<Login> logins_list;
    QList<RoleInfo> roleinfo_list;
    Login log1;
    log1.user_account = "a2021001";
    log1.user_passwd = "a2021001";
    log1.user_name = "普通用户";
    log1.from_role = "role001";
    logins_list.append(log1);

    Login log2;
    log2.user_account = "a2021002";
    log2.user_passwd = "a2021002";
    log2.user_name = "普通用户22";
    log2.from_role = "role001";
    logins_list.append(log2);

    Login log3;
    log3.user_account = "b2021001";
    log3.user_passwd = "b2021001";
    log3.user_name = "厂家";
    log3.from_role = "role002";
    logins_list.append(log3);

    Login log4;
    log4.user_account = "c2021001";
    log4.user_passwd = "c2021001";
    log4.user_name = "管理员";
    log4.from_role = "role003";
    logins_list.append(log4);

    //角色信息
    RoleInfo roleinfo1;
    roleinfo1.role_account = "role001";
    roleinfo1.role_name = "普通用户角色";
    roleinfo1.role_describe = "此角色拥有普通用户权限";
    roleinfo_list.append(roleinfo1);

    RoleInfo roleinfo2;
    roleinfo2.role_account = "role002";
    roleinfo2.role_name = "厂家角色";
    roleinfo2.role_describe = "此角色拥有厂家用户权限";
    roleinfo_list.append(roleinfo2);

    RoleInfo roleinfo3;
    roleinfo3.role_account = "role003";
    roleinfo3.role_name = "管理员角色";
    roleinfo3.role_describe = "此角色拥有管理员最高权限";
    roleinfo_list.append(roleinfo3);


    //写入用户表
    QSettings settings(m_currentDir+"/"+USERINFO_NAME+SETTING_SUFFIX , QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.beginWriteArray(USERINFO_NAME);
    for (int i = 0; i < logins_list.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("account",logins_list.at(i).user_account);
        settings.setValue("password", logins_list.at(i).user_passwd);
        settings.setValue("userName", logins_list.at(i).user_name);

    }
    settings.endArray();

    //创建用户角色关系表
    QSettings settings2(m_currentDir+"/"+USERROLE_NAME+SETTING_SUFFIX, QSettings::IniFormat);
    settings2.setIniCodec("UTF-8");
    for (int i = 0; i < logins_list.size(); i++) {        
        QString keyName = QString(USERROLE_NAME)+"/"+logins_list.at(i).user_account;
        QString keyValue = logins_list.at(i).from_role;
        settings2.setValue(keyName,keyValue);
    }

    //创建角色信息表
    QSettings settings3(m_currentDir +"/"+ROLEINFO_NAME+SETTING_SUFFIX, QSettings::IniFormat);
    settings3.setIniCodec("UTF-8");
    settings3.beginWriteArray(ROLEINFO_NAME);
    for (int i = 0; i < roleinfo_list.size(); i++) {
      settings3.setArrayIndex(i);
      settings3.setValue("role_account",roleinfo_list.at(i).role_account);
      settings3.setValue("role_name",roleinfo_list.at(i).role_name);
      settings3.setValue("role_describe",roleinfo_list.at(i).role_describe);

    }
    settings3.endArray();

    //创建设备列表
//    QSettings settings4(m_currentDir+"/"+DEVICELIST_NAME+SETTING_SUFFIX, QSettings::IniFormat);
//    settings2.setIniCodec("UTF-8");


//    QString keyName = QString(DEVICELIST_NAME)+"/ups";
//    QString keyValue = "1"; //0 表不可用  1 可用

//    QString keyName2 = QString(DEVICELIST_NAME)+"/humiture";
//    QString keyValue2 = "1"; //0 表不可用  1 可用

//    settings4.setValue(keyName,keyValue);
//    settings4.setValue(keyName2,keyValue2);



}

Login LoginManage::readUserVerification(QString filePath, QString account, QString passwd)
{
    Login login;
    //读用户表
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    int size = settings.beginReadArray("user_info");
    QString lib_account,lib_password,lib_userName;
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        lib_account = settings.value("account").toString();
        lib_password = settings.value("password").toString();
        lib_userName = settings.value("userName").toString();

        if(lib_account.simplified() == account){
            if(lib_password.simplified() == passwd){
                //登陆成功

                login.user_account = lib_account;
                login.user_name = lib_userName;
                break;

            }
        }

    }
    settings.endArray();
    return login;
}

RoleInfo LoginManage::getRoleInfo(QString filePath, QString prefix, QString role)
{
    RoleInfo roleinfo;
    //读用户表
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    int size = settings.beginReadArray(prefix);
    QString lib_account,lib_name,lib_describe;
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        lib_account  = settings.value("role_account").toString();
        lib_name     = settings.value("role_name").toString();
        lib_describe = settings.value("role_describe").toString();

        if(lib_account.simplified() == role){
            //已查到信息
            roleinfo.role_account  = lib_account;
            roleinfo.role_name     = lib_name;
            roleinfo.role_describe = lib_describe;
            break;
        }

    }
    settings.endArray();
    return roleinfo;
}

QString LoginManage::getUserRole(QString filePath,QString prefix,QString account)
{
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QString readField = prefix+"/"+account;
    return settings.value(readField).toString();
}

bool LoginManage::queryAllUserInfo(QString filePath, QList<Login> &loginList)
{
   bool ret = false;
   loginList.clear();
   Login login;
    //读用户表
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    int size = settings.beginReadArray(USERINFO_NAME);
    ret = true;
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        login.user_account = settings.value("account").toString();
        login.user_passwd = settings.value("password").toString();
        login.user_name = settings.value("userName").toString();

        loginList.append(login);

    }
    settings.endArray();
    return ret;
}

bool LoginManage::queryAllUserRole(QString filePath, QList<UserRole> &userRoleList)
{
    bool ret = false;
    userRoleList.clear();
    UserRole userrole;
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QStringList groupList = settings.childGroups();
    foreach(QString group,groupList)
    {
        settings.beginGroup(group);
        QStringList keyList = settings.childKeys();
        foreach(QString key,keyList)
        {
            userrole.user_account = key;
            userrole.from_role = settings.value(key).toString();
            userRoleList.append(userrole);
        }
        settings.endGroup();
    }
    return ret;
}

bool LoginManage::queryAllRoleInfo(QString filePath, QList<RoleInfo> &roleInfoList)
{
    bool ret = false;
    roleInfoList.clear();
    RoleInfo roleinfo;
     //
     QSettings settings(filePath, QSettings::IniFormat);
     settings.setIniCodec("UTF-8");
     int size = settings.beginReadArray(ROLEINFO_NAME);
     ret = true;
     for (int i = 0; i < size; i++) {
         settings.setArrayIndex(i);
         roleinfo.role_account   =  settings.value("role_account").toString();
         roleinfo.role_name      =  settings.value("role_name").toString();
         roleinfo.role_describe  =  settings.value("role_describe").toString();

         roleInfoList.append(roleinfo);

     }
     settings.endArray();
     return ret;
}

bool LoginManage::queryAllDevInfo(QString filePath, QList<DevList> &devInfoList)
{
    bool ret = false;
    devInfoList.clear();
    DevList devlist;
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QStringList groupList = settings.childGroups();
    foreach(QString group,groupList)
    {
        settings.beginGroup(group);
        QStringList keyList = settings.childKeys();
        foreach(QString key,keyList)
        {
            devlist.dev_table_name = key;
            devlist.table_enable = settings.value(key).toString();
            devInfoList.append(devlist);
        }
        settings.endGroup();
    }
    return ret;
}

bool LoginManage::queryOneDevInfo(QString filePath, QList<OneDevInfo> &oneDevInfoList)
{
    bool ret = false;
    oneDevInfoList.clear();
    OneDevInfo onedevinfo;
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QStringList groupList = settings.childGroups();
    foreach(QString group,groupList)
    {
        settings.beginGroup(group);
        QStringList keyList = settings.childKeys();
        foreach(QString key,keyList)
        {
            onedevinfo.parameter_name = key;
            onedevinfo.show_name = settings.value(key).toString();
            oneDevInfoList.append(onedevinfo);
        }
        settings.endGroup();
    }
    return ret;
}

bool LoginManage::readJsonUserVerification(QString account, QString passwd, UserInfo &usrInfo)
{
    QJsonDocument   jsonDoc;
    QJsonArray      jsonArr;
    bool     ret = false;
    QString lib_account,lib_password;
    QByteArray pwd;
    QByteArray pwdMD5 = QCryptographicHash::hash(passwd.toUtf8(), QCryptographicHash::Md5 );

    bool ok = ConversionTool::initJsonDataFile(JSONUSRINFO_FILE,jsonDoc);
    if(ok){
        ok = ConversionTool::jsonDocToArray(jsonDoc,jsonArr);
        if(ok){
            for(int i=0;i<jsonArr.size();i++){
                QJsonObject usrObj = jsonArr.at(i).toObject();
                lib_account     = usrObj["usr"].toString();
                lib_password    = usrObj["pwd"].toString();

                if(lib_account == account){     //找到用户名
                    pwd = pwdMD5.toHex();

                    if(lib_password == pwd){
                        //登陆成功
                        usrInfo.usr = account;
                        usrInfo.auth = usrObj["auth"].toInt();
                        usrInfo.nick = usrObj["nick"].toString();
                        ret =true;
                        break;

                    }


                }


            }
        }
    }else{
        qDebug()<<"open usrinfo file fail";
    }
    return ret;
}

bool LoginManage::queryOneDevToDatabase(QString filePath,QSqlQuery &query,QSqlDatabase &db,QString devName)
{
    //qDebug()<<filePath;
    bool ret = false;
    QString sql="";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    QTextStream in(&file);
    in.setCodec("UTF-8"); //请注意这行

    //第一行判断是否和数据库一致
    QString line = in.readLine();
    QStringList dataList = line.split("\t");
    for(int i=0;i<dataList.length();i++){
        dataList[i]=dataList[i].trimmed();
    }
    QStringList dataListInitial = dataList;


    //头信息
    QStringList headStr;
    headStr<<"parameter_name"<<"show_name"<<"parameter_type"<<"parameter_class";
    foreach (RoleInfo roleStr, m_roleInfoList) {
        headStr<<roleStr.role_account;
    }
    //排序
    qSort(dataList.begin(),dataList.end());
    //qDebug()<<dataList;
    qSort(headStr.begin(),headStr.end());
    //qDebug()<<headStr;
    static int valCount,ValOKCount = 0;
    //比较是否一致
    if(headStr == dataList){

        db.transaction();
        valCount = 0;
        ValOKCount = 0;
        while(!in.atEnd())
        {

            valCount++;
            line = in.readLine();
            QStringList dataList = line.split("\t");
            if(dataList.length()>0 and dataList.length() == dataListInitial.length()){
                //插入数据库

                //设备表，按传入的名称插入数据
                //表头
               QString tableInfo = "";
               foreach (QString d , dataListInitial) {
                   tableInfo += d +",";
               }
               tableInfo = tableInfo.left(tableInfo.length()-1);    //去除最后的逗号
               //内容
               QString valInfo = "";
               foreach (QString d , dataList) {
                   valInfo += "'"+ d.trimmed() +"'" + ",";
               }
               valInfo = valInfo.left(valInfo.length()-1);    //去除最后的逗号

                //设备添加参数数据

                sql = QString("INSERT OR IGNORE INTO %1 (%2) VALUES(%3)")
                        .arg(devName,tableInfo,valInfo);


                if(!query.exec(sql))
                {
                   //qDebug() << query.lastError();
                   //qDebug()<<"error value"<<sql;
                   ret = false;
                }
                else
                {
                   ret = true;
                   ValOKCount++;
                }

            }

        }
        db.commit();
    }

    file.close();
    //qDebug()<<"数据条目总行数："<<valCount<<"写入成功数："<<ValOKCount<<"写入失败数："<<valCount-ValOKCount;
    return  ret;

}

void LoginManage::initUI(QString dbPath, QList<QString> devTablelist)
{
    //第一次打开创建tabwidget容器
    if(tabWidget == NULL ){
        tabWidget = new QTabWidget(this);
        QVBoxLayout *vlay = new QVBoxLayout(this);
        vlay->setMargin(0);
        vlay->addWidget(tabWidget);
        this->resize(800,400);
        foreach (QString dev, devTablelist) {
            QWidget *tab = new QWidget();
            tab->setObjectName(dev);
            tabWidget->addTab(tab,dev);

            //--------------------------------
            openDatabaseFile(m_currentDir+"/"+DATABASE_FILE);

            WritesqlQueryModel *model = new WritesqlQueryModel(m_database,dev);
            //保存查询
            modeList.append(model);
            QTableView *viewDev = new QTableView(tab);
            viewDev->setMinimumSize(800,400);
            QVBoxLayout *vlay = new QVBoxLayout(tab);
            vlay->addWidget(viewDev);

            viewDev->setModel(model);
            //viewDev->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            model->setQuery(QString("select * from %1").arg(dev));
            m_database.close();
            }

        }else { //界面已创建，重新设置查询后，打开界面
        foreach (WritesqlQueryModel *model, modeList) {
            openDatabaseFile(m_currentDir+"/"+DATABASE_FILE);
            model->setDataBase(m_database);
            model->setQuery(QString("select * from %1").arg(model->getDevName()));
            m_database.close();
            }
        }
        //显示界面
        this->show();
}
