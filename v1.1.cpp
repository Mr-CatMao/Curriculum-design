#pragma once

#include<iostream>
#include "fstream"
#include "string"
#include "sstream"
#include "iomanip"
#include <sys/stat.h>

#include <thread>

#include <direct.h>
#include <limits>

#define GAP 12

using namespace std;

const string logFile = "databases/log/logs.txt";

class Staff {
protected:
    //Data order：num,name,gender,age
    string number;    //编号
    string name;    //姓名
    string gender;    //性别
    string age;    //年龄
public:
    Staff()
            : number("None"), name("None"), gender("None"), age("None") {}

    Staff(string nu, string na, string ge, string ag)
            : number(nu), name(na), gender(ge), age(ag) {}

    void set_number(string t) { number = t; }

    void set_name(string t) { name = t; }

    void set_gender(string t) { gender = t; }

    void set_age(string t) { age = t; }

    //输出规定：行宽固定为GAP
    virtual void print();

    string get_number() { return number; }

    string get_name() { return name; }

    string get_gender() { return gender; }

    string get_age() { return age; }
};

//教师
class Teacher : virtual public Staff {
protected:
    //Data order：num,name,gender,age,depart,major
    string depart;    //所在系
    string major;    //专业
    string title;   //职称
public:
    Teacher()
            : Staff(), depart("None"), major("None"), title("None") {}

    Teacher(string nu, string na, string ge, string ag, string de, string ma, string ti)
            : Staff(nu, na, ge, ag), depart(de), major(ma), title(ti) {}

    void set_depart(string t) { depart = t; }

    void set_major(string t) { major = t; }

    void set_title(string t) { title = t; }

    void print();

    string get_depart() { return depart; }

    string get_major() { return major; }

    string get_title() { return title; }
};

//行政人员
class Administrator : virtual public Staff {
protected:
    //Data order：num,name,gender,age,poli_status,job
    string poli_status;    //政治面貌
    string job;    //职务
public:
    Administrator()
            : Staff(), poli_status("None"), job("None") {}

    Administrator(string nu, string na, string ge, string ag, string ps, string jo)
            : Staff(nu, na, ge, ag), poli_status(ps), job(jo) {}

    void set_poli_status(string t) { poli_status = t; }

    void set_job(string t) { job = t; }

    void print();

    string get_poli_status() { return poli_status; }

    string get_job() { return job; }
};

//教师兼行政人员
class Teach_Admr : public Teacher, public Administrator {
public:
    //Data order：num,name,gender,age,depart,major,poli_status,job
    Teach_Admr()
            : Staff(), Teacher(), Administrator() {}

    Teach_Admr(string nu, string na, string ge, string ag, string de, string ma, string ti, string ps, string jo)
            : Staff(nu, na, ge, ag), Teacher(nu, na, ge, ag, de, ma, ti), Administrator(nu, na, ge, ag, ps, jo) {}

    void print();
};

//实验员
class Experimenter : public Staff {
protected:
    //Data order：num,name,gender,age,lab,duty
    string lab;    //所在实验室
    string duty;    //职务
public:
    Experimenter()
            : Staff(), lab("None"), duty("None") {}

    Experimenter(string nu, string na, string ge, string ag, string la, string du)
            : Staff(nu, na, ge, ag), lab(la), duty(du) {}

    void set_lab(string t) { lab = t; }

    void set_duty(string t) { duty = t; }

    void print();

    string get_lab() { return lab; }

    string get_duty() { return duty; }
};

void Staff::print() {
    cout << left << setw(GAP) << number << setw(GAP) << name << setw(GAP) << gender << setw(GAP) << age;
}

void Teacher::print() {
    Staff::print();
    cout << left << setw(GAP) << depart << setw(GAP) << major << setw(GAP) << title << endl;

}

void Administrator::print() {
    Staff::print();
    cout << left << setw(GAP) << poli_status << setw(GAP) << job << endl;
}

void Teach_Admr::print() {
    Staff::print();
    cout << left << setw(GAP) << depart << setw(GAP) << major << setw(GAP) << title << setw(GAP) << poli_status
         << setw(GAP) << job << endl;
}

void Experimenter::print() {
    Staff::print();
    cout << left << setw(GAP) << lab << setw(GAP) << duty << endl;
}

class Log {
public:
    Log(const string &filename);

    ~Log();

    void writeLog(const string &message);
private:
    fstream logfile;
    string getCurrentTime();
};

Log::Log(const std::string &filename) {
    logfile.open(filename,ios::out|ios::app);
    if (!logfile)
        exit(1);
}

Log::~Log() {
    if (logfile.is_open())
        logfile.close();
}

void Log::writeLog(const std::string &message) {
    logfile<<getCurrentTime()<<" "<<message<<endl;
}

string Log::getCurrentTime() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    ostringstream oss;
    oss << 1900 + ltm->tm_year << "-"
        << 1 + ltm->tm_mon << "-"
        << ltm->tm_mday << " "
        << ltm->tm_hour << ":"
        << ltm->tm_min << ":"
        << ltm->tm_sec;
    return oss.str();
}

//链表结点
template<class T>
struct Node {
    T data;
    Node *next = nullptr;
};

//链表类，头结点不空置
class Linklist {
private:
    Node<Teacher> *tea;
    Node<Administrator> *adm;
    Node<Teach_Admr> *bta;
    Node<Experimenter> *exp;

    //对人数等进行统计
    int sum_teacher = 0;
    int sum_administrator = 0;
    int sum_bta = 0;
    int sum_exp = 0;
    int sum_man = 0;
    int sum_woman = 0;

    //由add(int)函数调用，不允许直接使用
    void addTe();    //教师
    void addAd();    //行政人员
    void addTA();    //教师兼行政人员
    void addEx();    //实验员

    //由show()函数调用，用法同上
    void showTe();

    void showAd();

    void showTA();

    void showEx();

    //修改内容的函数，由select(int)函数调用，不允许直接使用。其中修改仅能修改除了编号之外的信息。
    void modifyTe(Node<Teacher> *);

    void modifyAd(Node<Administrator> *);

    void modifyTA(Node<Teach_Admr> *);

    void modifyEx(Node<Experimenter> *);

    //程序初始化各张链表
    friend void initTeacher(Linklist *db);

    friend void initAdministrator(Linklist *db);

    friend void initTeach_Admr(Linklist *db);

    friend void initExperimenter(Linklist *db);

    string login_pw;

public:
    Linklist()
            : tea(nullptr), adm(nullptr), bta(nullptr), exp(nullptr) {}

    //传参顺序为教师，行政，教师兼行政，实验员
    Linklist(Node<Teacher> *te, Node<Administrator> *ad, Node<Teach_Admr> *bt, Node<Experimenter> *ex)
            : tea(te), adm(ad), bta(bt), exp(ex) {}

    void saveTeacherFile();

    void saveAdministratorFile();

    void saveTeach_AdmrFile();

    void saveExperimenterFile();

    //显示
    void show();

    //添加功能
    void add(int);    //总的add，调用此函数实现对添加内容的判断，再由此函数调用add分函数

    //查询功能（每遍历一次，判断target与姓名或编号是否相同）与修改功能（seek结束询问是否修改）
    void select(int);    //按姓名或编号查找	string target
    friend string getSelectNum();

    Node<Teacher> *teacherSelect(string selectNum);

    Node<Administrator> *administratorSelect(string selectNum);

    Node<Teach_Admr> *teach_admrSelect(string selectNum);

    Node<Experimenter> *experimenterSelect(string selectNum);

    //删除功能
    void del();    //仅管理员可以删除数据

    void delTe(const string &);

    void delAd(const string &);

    void delTA(const string &);

    void delEx(const string &);

    //统计功能（统计完直接输出）
    void count();

    //退出功能
    void _exit();

    //程序初始化时遍历数据库时添加数据
    void
    initAddTeacher(string number, string name, string gender, string age, string depart, string major, string title);

    void initAddAdministrator(string number, string name, string gender, string age, string poli_status, string job);

    void
    initAddTeach_Admr(string number, string name, string gender, string age, string depart, string major, string title,
                      string poli_status, string job);

    void initAddExperimenter(string number, string name, string gender, string age, string lab, string duty);

    //判断编号是否存在
    int Exist(const string &);

    //初始化以及修改管理员密码
    void get_pw();

    void modify_pw();

    friend int choose_identity(Linklist *);

    ~Linklist();
};

Linklist::~Linklist() {
    Node<Teacher> *tea_next = tea;
    Node<Teacher> *to_del_tea = nullptr;
    while (tea_next) {
        to_del_tea = tea_next;
        tea_next = tea_next->next;
        delete to_del_tea;
    }

    Node<Administrator> *adm_next = adm;
    Node<Administrator> *to_del_adm = nullptr;
    while (adm_next) {
        to_del_adm = adm_next;
        adm_next = adm_next->next;
        delete to_del_adm;
    }

    Node<Teach_Admr> *bta_next = bta;
    Node<Teach_Admr> *to_del_bta = nullptr;
    while (bta_next) {
        to_del_bta = bta_next;
        bta_next = bta_next->next;
        delete to_del_bta;
    }

    Node<Experimenter> *exp_next = exp;
    Node<Experimenter> *to_del_exp = nullptr;
    while (exp_next) {
        to_del_exp = exp_next;
        exp_next = exp_next->next;
        delete to_del_exp;
    }
}

void Linklist::show() {
    int iden = -1;
    while (iden < 0 or iden > 4) {
        cout << "0：展示全部\n1：教师人员\n2：行政人员\n3：教师兼行政人员\n4：实验员\n";
        cout << "请输入您想展示的人员：";
        cin >> iden;
        if (iden < 0 or iden > 4)
            cout << "输入信息有误，请重新输入\n\n";
        system("cls");
    }

    //添加日志
    string logMessage;
    logMessage.append("show table ");
    if (iden == 0)
        logMessage.append("teacher experimenter administrator teacher_administrator");
    else if (iden == 1)
        logMessage.append("teacher");
    else if (iden == 2)
        logMessage.append("administrator");
    else if (iden == 3)
        logMessage.append("teacher_administrator");
    else if (iden == 4)
        logMessage.append("experimenter");
    Log log(logFile);
    log.writeLog(logMessage);

    switch (iden) {
        case 0://展示全部
            this->showTe();

            this->showEx();

            this->showAd();

            this->showTA();

            break;
        case 1:
            this->showTe();
            break;
        case 2:
            this->showAd();
            break;
        case 3:
            this->showTA();
            break;
        case 4:
            this->showEx();
            break;
    }
}

void Linklist::get_pw() {
    ifstream f("databases/password/pw.txt");
    if (!f)exit(-1);

    string output;

    getline(f, output);
    f.close();

    stringstream ss(output);

    ss >> login_pw;
}

void Linklist::modify_pw() {
    system("cls");
    cout << "管理员您好，您正在修改管理员登录密码！" << endl;
    cout << "请输入您的新密码：";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string _cin, new_pw;
    getline(cin, _cin);
    stringstream ss(_cin);

    ss >> new_pw;
    cout << "输入yes以确认将" << new_pw << "设为新密码：";

    string confirm;
    cin >> confirm;

    if (confirm == "yes") {
        this->login_pw = new_pw;

        fstream f;
        f.open("databases/password/pw.txt", ios::out);
        if (!f)exit(-1);

        f << new_pw;
        f.close();

        system("cls");
        cout << "密码修改成功，请继续您的操作。" << endl << endl;

        //添加日志
        string logMessage;
        logMessage.append("root had modified password.");
        Log log(logFile);
        log.writeLog(logMessage);

    } else {
        system("cls");
        cout << "密码修改操作已舍弃，新密码未保存，请继续您的操作" << endl << endl;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Linklist::saveTeacherFile() {
    fstream teacherFile;
    teacherFile.open("databases/school_staff/teacher.txt", ios::out);
    if (!teacherFile) {
        cout << "无法打开文件!" << endl;
        exit(-1);
    }
    Node<Teacher> *iterator = tea;
    stringstream ssTeacher;
    while (iterator) {
        ssTeacher << left << setw(GAP) << iterator->data.get_number() << setw(GAP) << iterator->data.get_name()
                  << setw(GAP) << iterator->data.get_gender() << setw(GAP) << iterator->data.get_age()
                  << setw(GAP) << iterator->data.get_depart() << setw(GAP) << iterator->data.get_major()
                  << setw(GAP) << iterator->data.get_title() << endl;
        iterator = iterator->next;
    }
    string teacherData = ssTeacher.str();
    teacherFile << teacherData;
    teacherFile.close();

    //添加日志
    string logMessage;
    logMessage.append("save teacher table.");
    Log log(logFile);
    log.writeLog(logMessage);
}

void Linklist::saveAdministratorFile() {
    fstream administratorFile;
    administratorFile.open("databases/school_staff/administrator.txt", ios::out);
    if (!administratorFile) {
        cout << "无法打开文件!" << endl;
        exit(-1);
    }
    Node<Administrator> *iterator = adm;
    stringstream ssAdministrator;
    while (iterator) {
        ssAdministrator << left << setw(GAP) << iterator->data.get_number() << setw(GAP) << iterator->data.get_name()
                        << setw(GAP) << iterator->data.get_gender() << setw(GAP) << iterator->data.get_age()
                        << setw(GAP) << iterator->data.get_poli_status() << setw(GAP) << iterator->data.get_job()
                        << endl;
        iterator = iterator->next;
    }
    string administratorData = ssAdministrator.str();
    administratorFile << administratorData;
    administratorFile.close();

    //添加日志
    string logMessage;
    logMessage.append("save administrator table.");
    Log log(logFile);
    log.writeLog(logMessage);
}

void Linklist::saveTeach_AdmrFile() {
    fstream teach_admrFile;
    teach_admrFile.open("databases/school_staff/teacher_administrator.txt", ios::out);
    if (!teach_admrFile) {
        cout << "无法打开文件!" << endl;
        exit(-1);
    }
    Node<Teach_Admr> *iterator = bta;
    stringstream ssTeach_Admr;
    while (iterator) {
        ssTeach_Admr << left << setw(GAP) << iterator->data.get_number() << setw(GAP) << iterator->data.get_name()
                     << setw(GAP) << iterator->data.get_gender() << setw(GAP) << iterator->data.get_age()
                     << setw(GAP) << iterator->data.get_depart() << setw(GAP) << iterator->data.get_major()
                     << setw(GAP) << iterator->data.get_title() << setw(GAP) << iterator->data.get_poli_status()
                     << setw(GAP) << iterator->data.get_job() << endl;
        iterator = iterator->next;
    }
    string teach_admrData = ssTeach_Admr.str();
    teach_admrFile << teach_admrData;
    teach_admrFile.close();

    //添加日志
    string logMessage;
    logMessage.append("save teacher_administrator table.");
    Log log(logFile);
    log.writeLog(logMessage);
}

void Linklist::saveExperimenterFile() {
    fstream experimenterFile;
    experimenterFile.open("databases/school_staff/experiment.txt", ios::out);
    if (!experimenterFile) {
        cout << "无法打开文件!" << endl;
        exit(-1);
    }
    Node<Experimenter> *iterator = exp;
    stringstream ssExperimenter;
    while (iterator) {
        ssExperimenter << left << setw(GAP) << iterator->data.get_number() << setw(GAP) << iterator->data.get_name()
                       << setw(GAP) << iterator->data.get_gender() << setw(GAP) << iterator->data.get_age()
                       << setw(GAP) << iterator->data.get_lab() << setw(GAP) << iterator->data.get_duty() << endl;
        iterator = iterator->next;
    }
    string experimenterData = ssExperimenter.str();
    experimenterFile << experimenterData;
    experimenterFile.close();

    //添加日志
    string logMessage;
    logMessage.append("save experimenter table.");
    Log log(logFile);
    log.writeLog(logMessage);
}

void Linklist::showTe() {
    cout << "Teacher table:\n------------\n";

    if (tea == nullptr) {
        cout << "教师人员中无数据。" << endl;
        cout << "--------------------------------------\n\n";
        return;
    }

    Node<Teacher> *iterator = tea;
    /*
    cout << left << setw(GAP) << "编号" << setw(GAP) << "姓名" << setw(GAP) << "性别" << setw(GAP) << "年龄"
         << setw(GAP) << "所在系" << setw(GAP) << "专业" << setw(GAP) << "职称" << endl;
    */
    cout << left << setw(GAP) << "Number" << setw(GAP) << "Name" << setw(GAP) << "Gender" << setw(GAP) << "Age"
         << setw(GAP) << "Department" << setw(GAP) << "Major" << setw(GAP) << "Professional title" << endl;
    while (iterator) {
        iterator->data.print();
        iterator = iterator->next;
    }

    cout << "--------------------------------------\n\n";
}

void Linklist::showAd() {
    cout << "Administrator table:\n------------\n";

    if (adm == nullptr) {
        cout << "行政人员中无数据。" << endl;
        cout << "--------------------------------------\n\n";
        return;
    }

    Node<Administrator> *iterator = adm;
    /*
     cout << left << setw(GAP) << "编号" << setw(GAP) << "姓名" << setw(GAP) << "性别" << setw(GAP) << "年龄"
         << setw(GAP) << "政治面貌" << setw(GAP) << "职务" << endl;
    */
    cout << left << setw(GAP) << "Number" << setw(GAP) << "Name" << setw(GAP) << "Gender" << setw(GAP) << "Age"
         << setw(GAP) << "Politics status" << setw(GAP) << "Job" << endl;
    while (iterator) {
        iterator->data.print();
        iterator = iterator->next;
    }

    cout << "--------------------------------------\n\n";
}

void Linklist::showTA() {
    cout << "Administrator and Teacher table:\n------------\n";

    if (bta == nullptr) {
        cout << "教师兼行政人员中无数据。" << endl;
        cout << "--------------------------------------\n\n";
        return;
    }

    Node<Teach_Admr> *iterator = bta;
    /*
     cout << left << setw(GAP) << "编号" << setw(GAP) << "姓名" << setw(GAP) << "性别" << setw(GAP) << "年龄"
         << setw(GAP) << "所在系" << setw(GAP) << "专业" << setw(GAP) << "职称" << setw(GAP) << "政治面貌"
         << setw(GAP) << "职务" << endl;
    */
    cout << left << setw(GAP) << "Number" << setw(GAP) << "Name" << setw(GAP) << "Gender" << setw(GAP) << "Age"
         << setw(GAP) << "Department" << setw(GAP) << "Major" << setw(GAP) << "Professional title" << setw(GAP)
         << "Politics status" << setw(GAP) << "Job" << endl;
    while (iterator) {
        iterator->data.print();
        iterator = iterator->next;
    }

    cout << "--------------------------------------\n\n";
}

void Linklist::showEx() {
    cout << "Experimenter table:\n------------\n";

    if (exp == nullptr) {
        cout << "实验员中无数据。" << endl;
        cout << "--------------------------------------\n\n";
        return;
    }

    Node<Experimenter> *iterator = exp;
    /*
     cout << left << setw(GAP) << "编号" << setw(GAP) << "姓名" << setw(GAP) << "性别" << setw(GAP) << "年龄"
         << setw(GAP) << "所在实验室" << setw(GAP) << "职务" << endl;
    */
    cout << left << setw(GAP) << "Number" << setw(GAP) << "Name" << setw(GAP) << "Gender" << setw(GAP) << "Age"
         << setw(GAP) << "Laboratory" << setw(GAP) << "Duty" << endl;
    while (iterator) {
        iterator->data.print();
        iterator = iterator->next;
    }

    cout << "--------------------------------------\n\n";
}

//传入编号若已存在，则返回存在于第几条链表中，若不存在返回0
int Linklist::Exist(const string &arg) {
    Node<Teacher> *itr1 = tea;
    while (itr1) {
        if (itr1->data.get_number() == arg)
            return 1;
        itr1 = itr1->next;
    }

    Node<Administrator> *itr2 = adm;
    while (itr2) {
        if (itr2->data.get_number() == arg)
            return 2;
        itr2 = itr2->next;
    }

    Node<Teach_Admr> *itr3 = bta;
    while (itr3) {
        if (itr3->data.get_number() == arg)
            return 3;
        itr3 = itr3->next;
    }

    Node<Experimenter> *itr4 = exp;
    while (itr4) {
        if (itr4->data.get_number() == arg)
            return 4;
        itr4 = itr4->next;
    }

    return 0;
}

void Linklist::addTe() {
    Node<Teacher> *temp = new Node<Teacher>;
    string t1;
    string logMessage;

    cout << "请输入您的编号：";
    cin >> t1;
    if (!Exist(t1)) {
        temp->data.set_number(t1);
        logMessage.append("teacher add "+t1);

        cout << "请输入您的姓名：";
        cin >> t1;
        temp->data.set_name(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的性别(m/w)：";
        cin >> t1;
        temp->data.set_gender(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的年龄：";
        cin >> t1;
        temp->data.set_age(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的部门：";
        cin >> t1;
        temp->data.set_depart(t1);
        logMessage.append(" "+t1);

        cout << "请输入您教授的专业：";
        cin >> t1;
        temp->data.set_major(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的职称：";
        cin >> t1;
        temp->data.set_title(t1);
        logMessage.append(" "+t1);

        //添加日志信息
        Log log(logFile);
        log.writeLog(logMessage);

        //头结点为空
        if (this->tea == nullptr) {
            tea = temp;

            system("cls");
            cout << "创建信息成功！请继续您的操作。\n\n";
            return;
        }

        Node<Teacher> *iterator = tea;
        while (iterator->next)
            iterator = iterator->next;
        iterator->next = temp;

        system("cls");
        cout << "创建信息成功！请继续您的操作。\n\n";
    } else {
        system("cls");
        cout << "创建失败，该编号已经存在！\n\n";
        delete temp;
    }
}

void Linklist::addEx() {
    Node<Experimenter> *temp = new Node<Experimenter>;
    string t1;
    string logMessage;

    cout << "请输入您的编号：";
    cin >> t1;
    if (!Exist(t1)) {
        temp->data.set_number(t1);
        logMessage.append("experimenter add "+t1);

        cout << "请输入您的姓名：";
        cin >> t1;
        temp->data.set_name(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的性别(m/w)：";
        cin >> t1;
        temp->data.set_gender(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的年龄：";
        cin >> t1;
        temp->data.set_age(t1);
        logMessage.append(" "+t1);

        cout << "请输入您所在的实验室：";
        cin >> t1;
        temp->data.set_lab(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的职务：";
        cin >> t1;
        temp->data.set_duty(t1);
        logMessage.append(" "+t1);

        //添加日志信息
        Log log(logFile);
        log.writeLog(logMessage);

        if (this->exp == nullptr) {
            exp = temp;

            system("cls");
            cout << "创建信息成功！请继续您的操作。\n\n";
            return;
        }

        Node<Experimenter> *iterator = exp;
        while (iterator->next)
            iterator = iterator->next;
        iterator->next = temp;

        system("cls");
        cout << "创建信息成功！请继续您的操作。\n\n";
    } else {
        system("cls");
        cout << "创建失败，该编号已经存在！\n\n";
        delete temp;
    }
}

void Linklist::addAd() {
    Node<Administrator> *temp = new Node<Administrator>;
    string t1;
    string logMessage;

    cout << "请输入您的编号：";
    cin >> t1;
    if (!Exist(t1)) {
        temp->data.set_number(t1);
        logMessage.append("administrator add "+t1);

        cout << "请输入您的姓名：";
        cin >> t1;
        temp->data.set_name(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的性别(m/w)：";
        cin >> t1;
        temp->data.set_gender(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的年龄：";
        cin >> t1;
        temp->data.set_age(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的政治面貌：";
        cin >> t1;
        temp->data.set_poli_status(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的职务：";
        cin >> t1;
        temp->data.set_job(t1);
        logMessage.append(" "+t1);

        //添加日志信息
        Log log(logFile);
        log.writeLog(logMessage);

        if (this->adm == nullptr) {
            adm = temp;

            system("cls");
            cout << "创建信息成功！请继续您的操作。\n\n";
            return;
        }

        Node<Administrator> *iterator = adm;
        while (iterator->next)
            iterator = iterator->next;
        iterator->next = temp;

        system("cls");
        cout << "创建信息成功！请继续您的操作。\n\n";
    } else {
        system("cls");
        cout << "创建失败，该编号已经存在！\n\n";
        delete temp;
    }
}

void Linklist::addTA() {
    Node<Teach_Admr> *temp = new Node<Teach_Admr>;
    string t1;
    string logMessage;

    cout << "请输入您的编号：";
    cin >> t1;
    if (!Exist(t1)) {
        temp->data.set_number(t1);
        logMessage.append("teacher_administrator add "+t1);

        cout << "请输入您的姓名：";
        cin >> t1;
        temp->data.set_name(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的性别(m/w)：";
        cin >> t1;
        temp->data.set_gender(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的年龄：";
        cin >> t1;
        temp->data.set_age(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的部门：";
        cin >> t1;
        temp->data.set_depart(t1);
        logMessage.append(" "+t1);

        cout << "请输入您教授的专业：";
        cin >> t1;
        temp->data.set_major(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的职称：";
        cin >> t1;
        temp->data.set_title(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的政治面貌：";
        cin >> t1;
        temp->data.set_poli_status(t1);
        logMessage.append(" "+t1);

        cout << "请输入您的职务：";
        cin >> t1;
        temp->data.set_job(t1);
        logMessage.append(" "+t1);

        //添加日志信息
        Log log(logFile);
        log.writeLog(logMessage);

        if (this->bta == nullptr) {
            bta = temp;

            system("cls");
            cout << "创建信息成功！请继续您的操作。\n\n";
            return;
        }

        Node<Teach_Admr> *iterator = bta;
        while (iterator->next)
            iterator = iterator->next;
        iterator->next = temp;

        system("cls");
        cout << "创建信息成功！请继续您的操作。\n\n";
    } else {
        system("cls");
        cout << "创建失败，该编号已经存在！\n\n";
        delete temp;
    }
}

void Linklist::del() {
    cout << "管理员您好，这是用户删除系统。" << endl;

    //判断编号是否存在
    cout << "请输入您想要删除的编号：";
    string num;
    cin >> num;
    int where = Exist(num);
    if (!where) {
        system("cls");
        cout << "您输入的编号不存在！\n\n";
        return;
    }
    if (where == 1)
        delTe(num);
    else if (where == 2)
        delAd(num);
    else if (where == 3)
        delTA(num);
    else if (where == 4)
        delEx(num);

    system("cls");
    cout << "编号为" << num << "的用户已成功删除！\n\n";
}

void Linklist::delTe(const string &num) {
    Node<Teacher> *d = tea;
    if (d->data.get_number() == num) {
        tea = tea->next;
    } else {
        while (d->next->data.get_number() != num)
            d = d->next;
        Node<Teacher> *temp = d;
        d = temp->next;
        temp->next = d->next;
    }

    //添加日志
    string logMessage;
    logMessage.append(
            "delete teacher " + d->data.get_number() + " " + d->data.get_name() + " " + d->data.get_gender() + " "
            + d->data.get_age() + " " + d->data.get_depart() + " " + d->data.get_major() + " " + d->data.get_title());
    Log log(logFile);
    log.writeLog(logMessage);

    delete d;
}

void Linklist::delAd(const string &num) {
    Node<Administrator> *d = adm;
    if (d->data.get_number() == num) {
        adm = adm->next;
    } else {
        while (d->next->data.get_number() != num)
            d = d->next;
        Node<Administrator> *temp = d;
        d = temp->next;
        temp->next = d->next;
    }

    //添加日志
    string logMessage;
    logMessage.append("delete teacher "+d->data.get_number()+" "+d->data.get_name()+" "+d->data.get_gender()+" "
                      +d->data.get_age()+" "+d->data.get_poli_status()+" "+d->data.get_job());
    Log log(logFile);
    log.writeLog(logMessage);

    delete d;
}

void Linklist::delTA(const string &num) {
    Node<Teach_Admr> *d = bta;
    if (d->data.get_number() == num) {
        bta = bta->next;
    } else {
        while (d->next->data.get_number() != num)
            d = d->next;
        Node<Teach_Admr> *temp = d;
        d = temp->next;
        temp->next = d->next;
    }

    //添加日志
    string logMessage;
    logMessage.append(
            "delete teacher " + d->data.get_number() + " " + d->data.get_name() + " " + d->data.get_gender() + " "
            + d->data.get_age() + " " + d->data.get_depart() + " " + d->data.get_major() + " " + d->data.get_title()
            + " " + d->data.get_poli_status() + " " + d->data.get_job());
    Log log(logFile);
    log.writeLog(logMessage);

    delete d;
}

void Linklist::delEx(const string &num) {
    Node<Experimenter> *d = exp;
    if (d->data.get_number() == num) {
        exp = exp->next;
    } else {
        while (d->next->data.get_number() != num)
            d = d->next;
        Node<Experimenter> *temp = d;
        d = temp->next;
        temp->next = d->next;
    }

    //添加日志
    string logMessage;
    logMessage.append("delete teacher "+d->data.get_number()+" "+d->data.get_name()+" "+d->data.get_gender()+" "
                      +d->data.get_age()+" "+d->data.get_lab()+" "+d->data.get_duty());
    Log log(logFile);
    log.writeLog(logMessage);

    delete d;
}

void Linklist::add(int iden) {
    while (iden < 1 or iden > 4) {
        cout << "1.教师\n2.实验员\n3.行政人员\n4.教师兼行政人员\n\n";
        cout << "管理员您好，请输入您想要添加的用户类型：";
        cin >> iden;
        system("cls");
        if (iden < 1 or iden > 4)
            cout << "输入信息有误，请重新输入\n\n";
    }
    string jobs[]{"教师", "实验员", "行政人员", "教师兼行政人员"};
    cout << "这是添加用户系统，您要添加的用户类型为：" << jobs[iden - 1] << endl;

    //分别调用具体的添加人员函数
    if (iden == 1)
        this->addTe();
    if (iden == 2)
        this->addEx();
    if (iden == 3)
        this->addAd();
    if (iden == 4)
        this->addTA();

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string getSelectNum() {
    cout << endl << "请输入要查询的编号：";
    //只提取第一个字符串，剩下的舍弃
    string selectNum;
    cin >> selectNum;
//    string input;
//    getline(cin, input);
//    stringstream ss(input);
//    ss >> selectNum;
    return selectNum;
}

Node<Teacher> *Linklist::teacherSelect(string selectNum) {
    Node<Teacher> *iterator = tea;
    while (iterator) {
        if (iterator->data.get_number() == selectNum)
            return iterator;
        iterator = iterator->next;
    }
    return nullptr;
}

Node<Administrator> *Linklist::administratorSelect(std::string selectNum) {
    Node<Administrator> *iterator = adm;
    while (iterator) {
        if (iterator->data.get_number() == selectNum)
            return iterator;
        iterator = iterator->next;
    }
    return nullptr;
}

Node<Teach_Admr> *Linklist::teach_admrSelect(std::string selectNum) {
    Node<Teach_Admr> *iterator = bta;
    while (iterator) {
        if (iterator->data.get_number() == selectNum)
            return iterator;
        iterator = iterator->next;
    }
    return nullptr;
}

Node<Experimenter> *Linklist::experimenterSelect(std::string selectNum) {
    Node<Experimenter> *iterator = exp;
    while (iterator) {
        if (iterator->data.get_number() == selectNum)
            return iterator;
        iterator = iterator->next;
    }
    return nullptr;
}

void Linklist::select(int temp) {
    //temp为所查找的人员类型编号
    //四个.+Current函数为查找到的人员所在的链表节点

    bool IsFound = false;    //是否成功找到了数据

    //当用户身份为管理员时，需要管理员确认查找的人员类型；若为普通用户则跳过选择人员步骤
    while (temp <= 0 or temp >= 5) {
        cout << "1.教师\n2.实验员\n3.行政人员\n4.教师兼行政人员\n" << endl;
        cout << "请输入要查找的身份：";
        cin >> temp;
        if (temp <= 0 or temp >= 5) {
            system("cls");
            cout << "输入信息有误，请重新输入\n\n";
        }
    }

    string selectNum;
    Node<Teacher> *teacherCurrent = nullptr;
    Node<Administrator> *administratorCurrent = nullptr;
    Node<Teach_Admr> *teach_admrCurrent = nullptr;
    Node<Experimenter> *experimenterCurrent = nullptr;
    switch (temp) {
        case 1:
            if (tea == nullptr) {
                cout << "此表为空" << endl;
                return;
            }
            selectNum = getSelectNum();
            teacherCurrent = teacherSelect(selectNum);

            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "查找结果如下：" << endl << endl;

            if (teacherCurrent == nullptr) {
                cout << "查无此人" << endl;
            } else {
                IsFound = true;

                cout << left << setw(GAP) << "编号" << setw(GAP) << "姓名" << setw(GAP) << "性别" << setw(GAP) << "年龄"
                     << setw(GAP) << "所在系" << setw(GAP) << "专业" << setw(GAP) << "职称" << endl;
                cout << left << setw(GAP) << teacherCurrent->data.get_number() << setw(GAP)
                     << teacherCurrent->data.get_name()
                     << setw(GAP) << teacherCurrent->data.get_gender() << setw(GAP) << teacherCurrent->data.get_age()
                     << setw(GAP) << teacherCurrent->data.get_depart() << setw(GAP) << teacherCurrent->data.get_major()
                     << setw(GAP) << teacherCurrent->data.get_title() << endl;
            }
            break;
        case 2:
            if (exp == nullptr) {
                cout << "此表为空" << endl;
                return;
            }
            selectNum = getSelectNum();
            experimenterCurrent = experimenterSelect(selectNum);

            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "查找结果如下：" << endl << endl;

            if (experimenterCurrent == nullptr)
                cout << "查无此人" << endl;
            else {
                IsFound = true;

                cout << left << setw(GAP) << "编号" << setw(GAP) << "姓名" << setw(GAP) << "性别" << setw(GAP) << "年龄"
                     << setw(GAP) << "所在实验室" << setw(GAP) << "职务" << endl;
                cout << left << setw(GAP) << experimenterCurrent->data.get_number()
                     << setw(GAP) << experimenterCurrent->data.get_name()
                     << setw(GAP) << experimenterCurrent->data.get_gender()
                     << setw(GAP) << experimenterCurrent->data.get_age()
                     << setw(GAP) << experimenterCurrent->data.get_lab()
                     << setw(GAP) << experimenterCurrent->data.get_duty() << endl;
                break;
            }
        case 3:
            if (adm == nullptr) {
                cout << "此表为空" << endl;
                return;
            }
            selectNum = getSelectNum();
            administratorCurrent = administratorSelect(selectNum);

            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "查找结果如下：" << endl << endl;

            if (administratorCurrent == nullptr)
                cout << "查无此人" << endl;
            else {
                IsFound = true;

                cout << left << setw(GAP) << "编号" << setw(GAP) << "姓名" << setw(GAP) << "性别" << setw(GAP) << "年龄"
                     << setw(GAP) << "政治面貌" << setw(GAP) << "职务" << endl;
                cout << left << setw(GAP) << administratorCurrent->data.get_number()
                     << setw(GAP) << administratorCurrent->data.get_name()
                     << setw(GAP) << administratorCurrent->data.get_gender()
                     << setw(GAP) << administratorCurrent->data.get_age()
                     << setw(GAP) << administratorCurrent->data.get_poli_status()
                     << setw(GAP) << administratorCurrent->data.get_job()
                     << endl;
            }
            break;
        case 4:
            if (bta == nullptr) {
                cout << "此表为空" << endl;
                return;
            }
            selectNum = getSelectNum();
            teach_admrCurrent = teach_admrSelect(selectNum);

            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "查找结果如下：" << endl << endl;

            if (teach_admrCurrent == nullptr)
                cout << "查无此人" << endl;
            else {
                IsFound = true;

                cout << left << setw(GAP) << "编号" << setw(GAP) << "姓名" << setw(GAP) << "性别" << setw(GAP) << "年龄"
                     << setw(GAP) << "所在系" << setw(GAP) << "专业" << setw(GAP) << "职称" << setw(GAP) << "政治面貌"
                     << setw(GAP) << "职务" << endl;
                cout << left << setw(GAP) << teach_admrCurrent->data.get_number()
                     << setw(GAP) << teach_admrCurrent->data.get_name()
                     << setw(GAP) << teach_admrCurrent->data.get_gender()
                     << setw(GAP) << teach_admrCurrent->data.get_age()
                     << setw(GAP) << teach_admrCurrent->data.get_depart()
                     << setw(GAP) << teach_admrCurrent->data.get_major()
                     << setw(GAP) << teach_admrCurrent->data.get_title()
                     << setw(GAP) << teach_admrCurrent->data.get_poli_status()
                     << setw(GAP) << teach_admrCurrent->data.get_job() << endl;
            }
            break;
        default:
            cout << "查找不到该编号";
            exit(-1);
    }

    //添加日志
    if (IsFound){
        string logMessage;
        logMessage.append(selectNum);
        Log log(logFile);
        log.writeLog(logMessage);
    }

    cout << "--------------------------------------" << endl << endl;

    if (!IsFound) {
        cout << "查询系统已退出" << endl;
        cout << "--------------------------------------" << endl << endl;
        return;
    }

    cout << "\n操作完成，输入m对此条数据进行修改，输入其他任意字符以退出查询系统：" << endl;
    string command;
    cin >> command;

    if (command != "m") {
        system("cls");
        cout << "查询系统已退出" << endl;
        cout << "--------------------------------------" << endl << endl;
        return;
    }

    system("cls");

    if (temp == 1)
        modifyTe(teacherCurrent);
    if (temp == 2)
        modifyEx(experimenterCurrent);
    if (temp == 3)
        modifyAd(administratorCurrent);
    if (temp == 4)
        modifyTA(teach_admrCurrent);

    system("cls");
    cout << "信息修改完毕，查询与修改系统已退出" << endl;
    cout << "--------------------------------------" << endl << endl;
}

void Linklist::modifyTe(Node<Teacher> *m) {
    cout << "您正在修改编号为" << m->data.get_number() << "的人员信息。" << endl << endl;

    string _new;

    //日志信息
    string logMessage;
    logMessage.append("update teacher "+m->data.get_number());

    cout << "请输入新的姓名：";
    cin >> _new;
    m->data.set_name(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的性别(m/w)：";
    cin >> _new;
    m->data.set_gender(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的年龄：";
    cin >> _new;
    m->data.set_age(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的所在系：";
    cin >> _new;
    m->data.set_depart(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的专业：";
    cin >> _new;
    m->data.set_major(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的职称：";
    cin >> _new;
    m->data.set_title(_new);
    logMessage.append(" "+_new);

    //添加日志
    Log log(logFile);
    log.writeLog(logMessage);
}

void Linklist::modifyEx(Node<Experimenter> *m) {
    cout << "您正在修改编号为" << m->data.get_number() << "的人员信息。" << endl << endl;

    string _new;

    //日志信息
    string logMessage;
    logMessage.append("update experimenter "+m->data.get_number());

    cout << "请输入新的姓名：";
    cin >> _new;
    m->data.set_name(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的性别(m/w)：";
    cin >> _new;
    m->data.set_gender(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的年龄：";
    cin >> _new;
    m->data.set_age(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的所在实验室：";
    cin >> _new;
    m->data.set_lab(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的职务：";
    cin >> _new;
    m->data.set_duty(_new);
    logMessage.append(" "+_new);

    //添加日志
    Log log(logFile);
    log.writeLog(logMessage);
}

void Linklist::modifyAd(Node<Administrator> *m) {
    cout << "您正在修改编号为" << m->data.get_number() << "的人员信息。" << endl << endl;

    string _new;

    //日志信息
    string logMessage;
    logMessage.append("update administrator "+m->data.get_number());

    cout << "请输入新的姓名：";
    cin >> _new;
    m->data.set_name(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的性别(m/w)：";
    cin >> _new;
    m->data.set_gender(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的年龄：";
    cin >> _new;
    m->data.set_age(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的政治面貌：";
    cin >> _new;
    m->data.set_poli_status(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的职务：";
    cin >> _new;
    m->data.set_job(_new);
    logMessage.append(" "+_new);

    //添加日志
    Log log(logFile);
    log.writeLog(logMessage);
}

void Linklist::modifyTA(Node<Teach_Admr> *m) {
    cout << "您正在修改编号为" << m->data.get_number() << "的人员信息。" << endl << endl;

    string _new;

    //日志信息
    string logMessage;
    logMessage.append("update teacher_administrator "+m->data.get_number());

    cout << "请输入新的姓名：";
    cin >> _new;
    m->data.set_name(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的性别(m/w)：";
    cin >> _new;
    m->data.set_gender(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的年龄：";
    cin >> _new;
    m->data.set_age(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的所在系：";
    cin >> _new;
    m->data.set_depart(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的专业：";
    cin >> _new;
    m->data.set_major(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的职称：";
    cin >> _new;
    m->data.set_title(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的政治面貌：";
    cin >> _new;
    m->data.set_poli_status(_new);
    logMessage.append(" "+_new);

    cout << "请输入新的职务：";
    cin >> _new;
    m->data.set_job(_new);
    logMessage.append(" "+_new);

    //添加日志
    Log log(logFile);
    log.writeLog(logMessage);
}

void Linklist::_exit() {
    this->saveTeacherFile();
    this->saveAdministratorFile();
    this->saveTeach_AdmrFile();
    this->saveExperimenterFile();
    cout << "所有数据已成功保存。" << endl;
}

void Linklist::count() {
    //计数器初始化
    sum_teacher = 0;
    sum_administrator = 0;
    sum_bta = 0;
    sum_exp = 0;
    sum_man = 0;
    sum_woman = 0;

    string boy = "m", girl = "w";

    Node<Teacher> *itr1 = tea;
    while (itr1) {
        sum_teacher++;
        if (itr1->data.get_gender() == boy)
            sum_man++;
        if (itr1->data.get_gender() == girl)
            sum_woman++;
        itr1 = itr1->next;
    }

    Node<Administrator> *itr2 = adm;
    while (itr2) {
        sum_administrator++;
        if (itr2->data.get_gender() == boy)
            sum_man++;
        if (itr2->data.get_gender() == girl)
            sum_woman++;
        itr2 = itr2->next;
    }

    Node<Teach_Admr> *itr3 = bta;
    while (itr3) {
        sum_bta++;
        if (itr3->data.get_gender() == boy)
            sum_man++;
        if (itr3->data.get_gender() == girl)
            sum_woman++;
        itr3 = itr3->next;
    }

    Node<Experimenter> *itr4 = exp;
    while (itr4) {
        sum_exp++;
        if (itr4->data.get_gender() == boy)
            sum_man++;
        if (itr4->data.get_gender() == girl)
            sum_woman++;
        itr4 = itr4->next;
    }

    cout << "--------------------------------------" << endl;
    cout << "统计结果如下：" << endl;
    cout << "教师总数为：" << sum_teacher << "人" << endl;
    cout << "行政人员总数为：" << sum_administrator << "人" << endl;
    cout << "教师兼行政人员总数为：" << sum_bta << "人" << endl;
    cout << "实验员总数为：" << sum_exp << "人" << endl;
    cout << "男生总数为：" << sum_man << "人" << endl;
    cout << "女生总数为：" << sum_woman << "人" << endl;
    cout << "\n请继续操作\n";
    cout << "--------------------------------------" << endl;

    string logMessage;
    logMessage.append("count tables\' data.");
    Log log(logFile);
    log.writeLog(logMessage);
}

//选择用户身份，仅管理员需要验证密码
int choose_identity(Linklist *db) {
    int temp = 0;
    while (temp <= 0 or temp >= 6) {
        cout << "--------------------------------------" << endl;
        cout << "欢迎进入高校人员信息管理系统" << endl;
        cout << "1.教师\n2.实验员\n3.行政人员\n4.教师兼行政人员\n5.管理员" << endl;
        cout << "--------------------------------------" << endl;
        cout << "请输入您的身份：";

        if (cin >> temp);
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  //清空缓冲区
        }

        if (temp <= 0 or temp >= 6) {
            system("cls");
            cout << "输入信息有误，请重新输入\n\n";
        }
    }
    system("cls");

    if (temp == 5) {
        cout << "您好管理员，请输入登录密码（初始密码为0000）：";
        string _get_pw;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> _get_pw;
        if (_get_pw == db->login_pw) {
            system("cls");
            cout << "密码正确，请继续您的操作" << endl << endl;
        } else {
            system("cls");
            cout << "密码错误，登录失败！\n程序自动退出！" << endl;
            exit(418);
        }
    }

    return temp;
}

void Linklist::initAddTeacher(string number, string name, string gender, string age, string depart, string major,
                              string title) {
    Node<Teacher> *temp = new Node<Teacher>;

    temp->data.set_number(number);
    temp->data.set_name(name);
    temp->data.set_gender(gender);
    temp->data.set_age(age);
    temp->data.set_depart(depart);
    temp->data.set_major(major);
    temp->data.set_title(title);

    if (this->tea == nullptr)
        tea = temp;
    else {
        Node<Teacher> *iterator = tea;
        while (iterator->next)
            iterator = iterator->next;
        iterator->next = temp;
    }
}

//程序内初始化Teacher，放入链表中
void initTeacher(Linklist *db) {
    fstream openTeacherFile;
    openTeacherFile.open("databases/school_staff/teacher.txt", ios::in);
    if (!openTeacherFile) {
        cout << "文件不存在!";
        exit(-1);
    }

    string line;                          //读取一整行再分块读取内容
    string num, name, gender, age, depart, major, title;

    while (getline(openTeacherFile, line)) {
        stringstream ss(line);
        ss >> num >> name >> gender >> age >> depart >> major >> title;
        db->initAddTeacher(num, name, gender, age, depart, major, title);
    }
}

void Linklist::initAddAdministrator(string number, string name, string gender, string age,
                                    string poli_status, string job) {
    Node<Administrator> *temp = new Node<Administrator>;
    temp->data.set_number(number);
    temp->data.set_name(name);
    temp->data.set_gender(gender);
    temp->data.set_age(age);
    temp->data.set_poli_status(poli_status);
    temp->data.set_job(job);

    if (this->adm == nullptr)
        adm = temp;
    else {
        Node<Administrator> *iterator = adm;
        while (iterator->next)
            iterator = iterator->next;
        iterator->next = temp;
    }
}

void initAdministrator(Linklist *db) {
    fstream openAdministratorFile;
    openAdministratorFile.open("databases/school_staff/administrator.txt", ios::in);
    if (!openAdministratorFile) {
        cout << "文件不存在!";
        exit(-1);
    }

    string line;                          //读取一整行再分块读取内容
    string num, name, gender, age, poli_status, job;

    //Linklist teacherList;
    while (getline(openAdministratorFile, line)) {
        stringstream ss(line);
        ss >> num >> name >> gender >> age >> poli_status >> job;
        db->initAddAdministrator(num, name, gender, age, poli_status, job);
    }
}

void
Linklist::initAddTeach_Admr(string number, string name, string gender, string age, string depart,
                            string major, string title, string poli_status, string job) {
    Node<Teach_Admr> *temp = new Node<Teach_Admr>;
    temp->data.set_number(number);
    temp->data.set_name(name);
    temp->data.set_gender(gender);
    temp->data.set_age(age);
    temp->data.set_depart(depart);
    temp->data.set_major(major);
    temp->data.set_title(title);
    temp->data.set_poli_status(poli_status);
    temp->data.set_job(job);

    if (this->bta == nullptr)
        bta = temp;
    else {
        Node<Teach_Admr> *iterator = bta;
        while (iterator->next)
            iterator = iterator->next;
        iterator->next = temp;
    }
}

void initTeach_Admr(Linklist *db) {
    fstream openTeach_AdmrFile;
    openTeach_AdmrFile.open("databases/school_staff/teacher_administrator.txt", ios::in);
    if (!openTeach_AdmrFile) {
        cout << "文件不存在!";
        exit(-1);
    }

    string line;                          //读取一整行再分块读取内容
    string num, name, gender, age, depart, major, title, poli_status, job;

    //Linklist teacherList;
    while (getline(openTeach_AdmrFile, line)) {
        stringstream ss(line);
        ss >> num >> name >> gender >> age >> depart >> major >> title >> poli_status >> job;
        db->initAddTeach_Admr(num, name, gender, age, depart, major, title, poli_status, job);
    }
}

void
Linklist::initAddExperimenter(std::string number, std::string name, std::string gender, string age, std::string lab,
                              std::string duty) {
    Node<Experimenter> *temp = new Node<Experimenter>;
    temp->data.set_number(number);
    temp->data.set_name(name);
    temp->data.set_gender(gender);
    temp->data.set_age(age);
    temp->data.set_lab(lab);
    temp->data.set_duty(duty);

    if (this->exp == nullptr)
        exp = temp;
    else {
        Node<Experimenter> *iterator = exp;
        while (iterator->next)
            iterator = iterator->next;
        iterator->next = temp;
    }
}

void initExperimenter(Linklist *db) {
    fstream openExperimentFile;
    openExperimentFile.open("databases/school_staff/experiment.txt", ios::in);
    if (!openExperimentFile) {
        cout << "文件不存在!";
        exit(-1);
    }

    string line;                          //读取一整行再分块读取内容
    string num, name, gender, age, lab, duty;

    //Linklist teacherList;
    while (getline(openExperimentFile, line)) {
        stringstream ss(line);
        ss >> num >> name >> gender >> age >> lab >> duty;
        db->initAddExperimenter(num, name, gender, age, lab, duty);
    }
}

//输出提示，并得到用户所需要的操作
int print_and_get_intro(int iden) {

    bool is_scanf_right = false;    //判断用户输入数字是否在范围以内
    int sign = -1;

    string jobs[]{"教师", "实验员", "行政人员", "教师兼行政人员", "管理员"};
    cout << "欢迎您，" << jobs[iden - 1] << "!" << endl << endl;

    while (!is_scanf_right) {
        cout << "--------------------------------------" << endl;
        if (iden == 5) {
            cout << "1.添加数据" << endl;
            cout << "2.查询、修改信息" << endl;
            cout << "3.显示数据" << endl;
            cout << "4.删除数据" << endl;
            cout << "5.统计数据" << endl;
            cout << "6.修改管理员登录密码" << endl;
            cout << "7.退出系统" << endl;
            cout << "--------------------------------------" << endl;
            cout << "请输入数字以继续操作：";

            if (cin >> sign);
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  //清空缓冲区
            }

            if (sign >= 1 and sign <= 7)
                is_scanf_right = true;
            else {
                system("cls");
                cout << "输入信息有误，请重新输入\n\n";
            }
        } else {
            cout << "1.添加数据" << endl;
            cout << "2.查询、修改信息" << endl;
            cout << "3.退出系统" << endl;
            cout << "--------------------------------------" << endl;
            cout << "请输入数字以继续操作：";

            if (cin >> sign);
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  //清空缓冲区
            }

            if (sign >= 1 and sign <= 3)
                is_scanf_right = true;
            else {
                system("cls");
                cout << "输入信息有误，请重新输入\n\n";
            }
        }
    }
    return sign;
}

//判断路径下的文件是否存在
bool fileExists(const std::string &filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

//初始化，包含创建文件夹以及.txt文件
void file_init() {
    string path01 = "databases";
    string path02 = "databases/school_staff";

    if (!fileExists(path01)) {
        try {
            _mkdir(path01.c_str());
        } catch (const exception &e) {
            exit(-1);
        }
    }

    if (!fileExists(path02)) {
        try {
            _mkdir(path02.c_str());
        } catch (const exception &e) {
            exit(-1);
        }
        string path1 = "databases/school_staff/experiment.txt";
        string path2 = "databases/school_staff/teacher_administrator.txt";
        string path3 = "databases/school_staff/administrator.txt";
        string path4 = "databases/school_staff/teacher.txt";
        string paths[4]{path1, path2, path3, path4};
        for (const auto &item: paths) {
            ofstream outfile(item);
            outfile.close();
        }
    }

    string path_pw = "databases/password";
    if (!fileExists(path_pw)) {
        try {
            _mkdir(path_pw.c_str());
        } catch (const exception &e) {
            exit(-1);
        }
        ofstream outfile("databases/password/pw.txt");
        outfile << "0000";
        outfile.close();
    }

    string path_log = "databases/log";
    if (!fileExists(path_log)) {
        try {
            _mkdir(path_log.c_str());
        } catch (const exception &e) {
            exit(-1);
        }
        ofstream outfile(logFile);
        outfile.close();
    }
}

//统一初始化链表
void init(Linklist *db) {
    initTeacher(db);
    initExperimenter(db);
    initAdministrator(db);
    initTeach_Admr(db);
    db->get_pw();
}

void func() {
    Linklist DB;    //总数据库

    //链表内容的初始化
    init(&DB);

    int IDEN = choose_identity(&DB);    //用户身份

    bool RUNNING = true;

    while (RUNNING) {
        int sign = print_and_get_intro(IDEN);    //用户的操作

        system("cls");

        if (IDEN == 5) {
            switch (sign) {
                case 1:
                    DB.add(IDEN);
                    break;
                case 2:
                    DB.select(IDEN);
                    break;
                case 3:
                    DB.show();
                    break;
                case 4:
                    DB.del();
                    break;
                case 5:
                    DB.count();
                    break;
                case 6:
                    DB.modify_pw();
                    break;
                case 7:
                    DB._exit();
                    RUNNING = false;
                    break;
            }
        } else {
            switch (sign) {
                case 1:
                    DB.add(IDEN);
                    break;
                case 2:
                    DB.select(IDEN);
                    break;
                case 3:
                    DB._exit();
                    RUNNING = false;
                    break;
            }
        }

        //system("cls");	//需要适当修改确认是否清屏。
    }
    cout << "系统已退出！" << endl;
}

int main() {
    file_init();    //.txt文档初始化
    func();    //主函数
    return 0;
}