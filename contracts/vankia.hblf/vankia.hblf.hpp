#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/crypto.hpp>
#include <string>

namespace vankia
{

using namespace eosio;
using namespace std;

struct keypair
{

    keypair() {}
    keypair(string str_key, string str_value)
    {
        key = str_key;
        value = str_value;
    }
    string key;
    string value;

    EOSLIB_SERIALIZE(keypair, (key)(value))
};

struct tkeys
{
    keys(){};
    keys(string Key,string Cn,string Value)
    {
        key = Key;
        CN = Cn;
        value = Value;
    };
    string key;
    string CN;
    string value;
    EOSLIB_SERIALIZE(keys,(key)(CN)(value))
};

struct option
{
    option(){}
    option(uint64_t option_id,string content,bool correct)
    {
        optionid = option_id;
        content = content;
        correct = correct;
    }

    uint64_t optionid;
    string content;
    string correct;

    EOSLIB_SERIALIZE(option,(optionid)(content)(correct))
};

/*struct aq
{
    aq(){}
    aq(string Record_id,string Test_question_id,string Test_question_code,string Answer,string Result)
    {
        record_id = Record_id;
        test_question_id = Test_question_id;
        test_question_code = Test_question_code;
        answer = Answer;
        result = Result;
    }
    string record_id;
    string test_question_id;
    string test_question_code;
    string answer;
    string result;
    EOSLIB_SERIALIZE(aq,(record_id)(test_question_id)(test_question_code)(answer)(result))
};*/

struct aserandquest
{
    aserandquest(){}
    aserandquest(string recordId,string test_questionId,string test_questionCode,string answer,string result)
    {
        record_id = recordId;
        test_question_code = test_questionCode;
        test_question_id = test_questionId;
        answer = answer;
        result = result;
    }
    string record_id;
    string test_question_id;
    string test_question_code;
    string answer;
    string result;

    EOSLIB_SERIALIZE(aserandquest,(record_id)(test_question_id)(test_question_code)(answer)(result))

};



class [[eosio::contract("vankia.hblf")]] hblf : public vankia::contract
{
  public:
    using contract::contract;

    [[eosio::action]]  //体质健康ID 学校代码   学校名称           学生ID      姓名     年级          年级名称     学籍号    学年码        是否有效
    void addsecooofir(string tzjkid,string xxdm,string xxmc,string xsid,string xm,string nj,string njmc,string xjh,string xnm,string sfyx,vector<keypair> contentmap); //学生体质健康信息
    [[eosio::action]]
    void modsecooofir(string tzjkid,string xxdm,string xxmc,string xsid,string xm,string nj,string njmc,string xjh,string xnm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delsecooofir(string tzjkid,string xxdm,string xxmc,string xsid,string xm,string nj,string njmc,string xjh,string xnm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]  //任务ID    学籍号          学生ID       学校代码     用户ID    是否有效
    void addsecooosec(string rwid,string xjh,string xsid,string xxdm,string yhid,string sfyx,vector<keypair> contentmap);    //学生体质健康明细信息
    [[eosio::action]]
    void modsecooosec(string rwid,string xjh,string xsid,string xxdm,string yhid,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delsecooosec(string rwid,string xjh,string xsid,string xxdm,string yhid,string sfyx,vector<keypair> contentmap);
    [[eosio::action]] // 教师ID    教师姓名     荣誉编号     荣誉级别名称   学校代码     是否有效
    void addeigooofir(string jsid,string jsxm,string rybh,string rymc,string xxdm,string sfyx,vector<keypair> contentmap);  //教师综合荣誉
    [[eosio::action]]
    void modeigooofir(string jsid,string jsxm,string rybh,string rymc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void deleigooofir(string jsid,string jsxm,string rybh,string rymc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]] //教师ID     职称编号     职称名称           学校代码         是否有效
    void addfifooofir(string jsid,string zcbh,string zcmc,string xxdm,string sfyx,vector<keypair> contentmap);     //教师职称相关
    [[eosio::action]]
    void modfifooofir(string jsid,string zcbh,string zcmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfifooofir(string jsid,string zcbh,string zcmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]] //教师ID    获得历学编号       获得学历名称 学校代码       是否有效
    void addfifooosec(string jsid,string hdxlbh,string hdlxmc,string xxdm,string sfyx,vector<keypair> contentmap);     //教师教育经历相关
    [[eosio::action]]
    void modfifooosec(string jsid,string hdxlbh,string hdlxmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfifooosec(string jsid,string hdxlbh,string hdlxmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]] //教师ID       学校代码           是否有效
    void addfifooothi(string jsid,string xxdm,string sfyx,vector<keypair> contentmap);   //教职工联系方式信息
    [[eosio::action]]
    void modfifooothi(string jsid,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfifooothi(string jsid,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]] //教师ID     政治面貌编号   政治面貌          学校代码        是否有效
    void addfifooofou(string jsid,string zzmmbh,string zzmm,string xxdm,string sfyx,vector<keypair> contentmap);   //教职工政治面貌
    [[eosio::action]]
    void modfifooofou(string jsid,string zzmmbh,string zzmm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfifooofou(string jsid,string zzmmbh,string zzmm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]] //教师ID     聘任单位编号   聘任单位     工作单位编号    工作单位    聘任职务        学校代码            是否有效
    void addfifooofif(string jsid,string prdwbh,string prdw,string gzdwbh,string gzdw,string przw,string xxdm,string sfyx,vector<keypair> contentmap);    //聘任信息
    [[eosio::action]]
    void modfifooofif(string jsid,string prdwbh,string prdw,string gzdwbh,string gzdw,string przw,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfifooofif(string jsid,string prdwbh,string prdw,string gzdwbh,string gzdw,string przw,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]] //教师ID     语种编号     语种           学校代码      是否有效
    void addfifooosix(string jsid,string yzbh,string yz,string xxdm,string sfyx,vector<keypair> contentmap);    //语言能力
    [[eosio::action]]
    void modfifooosix(string jsid,string yzbh,string yz,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfifooosix(string jsid,string yzbh,string yz,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]] //教师ID    教师资格证种类编号  教师资格证种类   教师资格证号码       学校代码         是否有效
    void addfifooosev(string jsid,string jszzzzlbh,string jszzzzl,string jszzzhm,string xxdm,string sfyx,vector<keypair> contentmap);    //教师资格
    [[eosio::action]]
    void modfifooosev(string jsid,string jszzzzlbh,string jszzzzl,string jszzzhm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfifooosev(string jsid,string jszzzzlbh,string jszzzzl,string jszzzhm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]] //教师ID     技能类型编号    技能类型     技能名称       学校代码         是否有效
    void addfifoooeig(string jsid,string jnlxbh,string jnlx,string jnmc,string xxdm,string sfyx,vector<keypair> contentmap);    //其他技能
    [[eosio::action]]
    void modfifoooeig(string jsid,string jnlxbh,string jnlx,string jnmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfifoooeig(string jsid,string jnlxbh,string jnlx,string jnmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void addzerooofir(string orgId,string orgCode,string orgType,string orgName,string areaId,string areaName,vector<keypair> contentmap);     //机构信息
    [[eosio::action]]
    void modzerooofir(string orgId,string orgCode,string orgType,string orgName,string areaId,string areaName,vector<keypair> contentmap);
    [[eosio::action]]
    void delzerooofir(string orgId,string orgCode,string orgType,string orgName,string areaId,string areaName,vector<keypair> contentmap);
    [[eosio::action]]
    void addzerooosev(string orgId,string schoolName,vector<keypair> contentmap);    //学校信息
    [[eosio::action]]
    void modzerooosev(string orgId,string schoolName,vector<keypair> contentmap);
    [[eosio::action]]
    void delzerooosev(string orgId,string schoolName,vector<keypair> contentmap);
    [[eosio::action]]
    void addzerooofif(string userId,string studentCode,string stuName,string orgId,vector<keypair> contentmap);    //学生信息
    [[eosio::action]]
    void modzerooofif(string userId,string studentCode,string stuName,string orgId,vector<keypair> contentmap);
    [[eosio::action]]
    void delzerooofif(string userId,string studentCode,string stuName,string orgId,vector<keypair> contentmap);
    [[eosio::action]]
    void addzerooosix(string orgId,string userId,string teaName,vector<keypair> contentmap);    //教师信息
    [[eosio::action]]
    void modzerooosix(string orgId,string userId,string teaName,vector<keypair> contentmap);
    [[eosio::action]]
    void delzerooosix(string orgId,string userId,string teaName,vector<keypair> contentmap);
    [[eosio::action]]  //学生成绩ID    学校代码    考试ID        学生ID      姓名      科目码     科目名称      分数类考试成绩   等级类考试成绩
    void addfouooothi(string xscjid,string xxdm,string ksid,string xsid,string xm,string kmm,string kmmc,string fslkscj,string djlkscj,vector<keypair> contentmap);  //考试成绩信息表
    [[eosio::action]]
    void modfouooothi(string xscjid,string xxdm,string ksid,string xsid,string xm,string kmm,string kmmc,string fslkscj,string djlkscj,vector<keypair> contentmap);
    [[eosio::action]]
    void delfouooothi(string xscjid,string xxdm,string ksid,string xsid,string xm,string kmm,string kmmc,string fslkscj,string djlkscj,vector<keypair> contentmap);
    [[eosio::action]]  //教师ID       教师姓名     学校代码   是否有效
    void addthiooofir(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);    //师德培训
    [[eosio::action]]
    void modthiooofir(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delthiooofir(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]  //教师ID       教师姓名     学校代码   是否有效
    void addthiooosec(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);    //师德考核
    [[eosio::action]]
    void modthiooosec(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delthiooosec(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]  //教师ID       教师姓名     学校代码   是否有效
    void addthiooothi(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);    //师德奖励
    [[eosio::action]]
    void modthiooothi(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delthiooothi(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]  //教师ID       教师姓名     学校代码   是否有效
    void addthiooofou(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);    //师德惩处
    [[eosio::action]]
    void modthiooofou(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delthiooofou(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]  //学校代码    考试ID      考试名称      用户ID
    void addfouooofir(string xxdm,string ksid,string ksmc,string yhid,vector<keypair> contentmap);    //考试信息表
    [[eosio::action]]
    void modfouooofir(string xxdm,string ksid,string ksmc,string yhid,vector<keypair> contentmap);
    [[eosio::action]]
    void delfouooofir(string xxdm,string ksid,string ksmc,string yhid,vector<keypair> contentmap);

    [[eosio::action]]  //学校代码   考试ID     考试名称     科目码      科目名称      试卷ID      用户ID
    void addfouooosec(string xxdm,string ksid,string ksmc,string kmm,string kmmc,string sjid,string yhid,vector<keypair> contentmap);    //考试班级科目表
    [[eosio::action]]
    void modfouooosec(string xxdm,string ksid,string ksmc,string kmm,string kmmc,string sjid,string yhid,vector<keypair> contentmap);
    [[eosio::action]]
    void delfouooosec(string xxdm,string ksid,string ksmc,string kmm,string kmmc,string sjid,string yhid,vector<keypair> contentmap);

    [[eosio::action]]  //学校代码   试卷ID      用户ID
    void addfouooofou(string xxdm,string sjid,string yhid,vector<keypair> contentmap);    //考试试卷信息表
    [[eosio::action]]
    void modfouooofou(string xxdm,string sjid,string yhid,vector<keypair> contentmap);
    [[eosio::action]]
    void delfouooofou(string xxdm,string sjid,string yhid,vector<keypair> contentmap);

    [[eosio::action]]  //学校代码      知识点ID        试题ID      用户ID      是否有效
    void addfouooofif(string xxdm,string zsdid,string stid,string yhid,string sfyx,vector<keypair> contentmap);    //试题知识点表
    [[eosio::action]]
    void modfouooofif(string xxdm,string zsdid,string stid,string yhid,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfouooofif(string xxdm,string zsdid,string stid,string yhid,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //学校代码    考试ID      考试名称      学生成绩ID     学生成绩明细ID     试卷ID    实际得分    用户ID        是否有效
    void addfouooosix(string xxdm,string ksid,string ksmc,string xscjid,string sxcjmxid,string sjid,string sjdf,string yhid,string sfyx,vector<keypair> contentmap);    //学生考试试题得分表
    [[eosio::action]]
    void modfouooosix(string xxdm,string ksid,string ksmc,string xscjid,string sxcjmxid,string sjid,string sjdf,string yhid,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfouooosix(string xxdm,string ksid,string ksmc,string xscjid,string sxcjmxid,string sjid,string sjdf,string yhid,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID      教师姓名      缺勤类型编号   缺勤类型   学校代码      是否有效
    void addeigooosec(string jsid,string jsxm,string qqlxbh,string qqlx,string xxdm,string sfyx,vector<keypair> contentmap);    //出勤情况
    [[eosio::action]]
    void modeigooosec(string jsid,string jsxm,string qqlxbh,string qqlx,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void deleigooosec(string jsid,string jsxm,string qqlxbh,string qqlx,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]] //教师ID      优质课编号     优质课名称     学校代码     是否有效
    void addninooofir(string jsid,string yzkbh,string yzkmc,string xxdm,string sfyx,vector<keypair> contentmap);    //优质课信息
    [[eosio::action]]
    void modninooofir(string jsid,string yzkbh,string yzkmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delninooofir(string jsid,string yzkbh,string yzkmc,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID     专利号     专利名称      学校代码      是否有效
    void addninooosec(string jsid,string zlh,string zlmc,string xxdm,string sfyx,vector<keypair> contentmap);     //发明专利
    [[eosio::action]]
    void modninooosec(string jsid,string zlh,string zlmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delninooosec(string jsid,string zlh,string zlmc,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID     课题编号     课题名称      学校代码      是否有效
    void addninooothi(string jsid,string ktbh,string ktmc,string xxdm,string sfyx,vector<keypair> contentmap);    //课题信息
    [[eosio::action]]
    void modninooothi(string jsid,string ktbh,string ktmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delninooothi(string jsid,string ktbh,string ktmc,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID     著作编号      著作名称    学校代码    是否有效
    void addninooofou(string jsid,string zzbh,string zzmc,string xxdm,string sfyx,vector<keypair> contentmap);    //著作
    [[eosio::action]]
    void modninooofou(string jsid,string zzbh,string zzmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delninooofou(string jsid,string zzbh,string zzmc,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID    论文编号     论文名称      论文类型编号    论文类型     学校代码    是否有效
    void addninooofif(string jsid,string lwbh,string lwmc,string lwlxbh,string lwlx,string xxdm,string sfyx,vector<keypair> contentmap);    //论文
    [[eosio::action]]
    void modninooofif(string jsid,string lwbh,string lwmc,string lwlxbh,string lwlx,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delninooofif(string jsid,string lwbh,string lwmc,string lwlxbh,string lwlx,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID     荣誉ID     荣誉名称     辅导对象ID      学校代码     是否有效
    void addsixooofir(string jsid,string ryid,string rymc,string fddxid,string xxdm,string sfyx,vector<keypair> contentmap);    //辅导学生获奖
    [[eosio::action]]
    void modsixooofir(string jsid,string ryid,string rymc,string fddxid,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delsixooofir(string jsid,string ryid,string rymc,string fddxid,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]] //教师ID       入学年份ID   入学年份名称    学校代码    是否有效
    void addsixooosec(string jsid,string rxnfid,string rxnfmc,string xxdm,string sfyx,vector<keypair> contentmap);    //管理情况
    [[eosio::action]]
    void modsixooosec(string jsid,string rxnfid,string rxnfmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delsixooosec(string jsid,string rxnfid,string rxnfmc,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID     教师姓名      青年教师ID    青年教师名称     荣誉名称     学校代码     是否有效
    void addsixooothi(string jsid,string jsxm,string qnjsid,string qnjsname,string rymc,string xxdm,string sfyx,vector<keypair> contentmap);    //指导青年教师
    [[eosio::action]]
    void modsixooothi(string jsid,string jsxm,string qnjsid,string qnjsname,string rymc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delsixooothi(string jsid,string jsxm,string qnjsid,string qnjsname,string rymc,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID      教师姓名      进修编号     进修名称    学校代码   是否有效
    void addsevooofir(string jsid,string jsxm,string jxbh,string jxmc,string xxdm,string sfyx,vector<keypair> contentmap);   //进修交流
    [[eosio::action]]
    void modsevooofir(string jsid,string jsxm,string jxbh,string jxmc,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delsevooofir(string jsid,string jsxm,string jxbh,string jxmc,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID        教师姓名   培训项目编号  培训项目       学校代码   是否有效
    void addsevooosec(string jsid,string jsxm,string pxxmbh,string pxxm,string xxdm,string sfyx,vector<keypair> contentmap);    //国内培训
    [[eosio::action]]
    void modsevooosec(string jsid,string jsxm,string pxxmbh,string pxxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delsevooosec(string jsid,string jsxm,string pxxmbh,string pxxm,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID        教师姓名   培训项目编号  培训项目       学校代码   是否有效
    void addsevooothi(string jsid,string jsxm,string pxxmbh,string pxxm,string xxdm,string sfyx,vector<keypair> contentmap);    //海外研修
    [[eosio::action]]
    void modsevooothi(string jsid,string jsxm,string pxxmbh,string pxxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delsevooothi(string jsid,string jsxm,string pxxmbh,string pxxm,string xxdm,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID     教师姓名     学校代码    是否有效
    void addsevooofou(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);    //交流轮岗
    [[eosio::action]]
    void modsevooofou(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delsevooofou(string jsid,string jsxm,string xxdm,string sfyx,vector<keypair> contentmap);


    [[eosio::action]]  //默认教学班ID          班级ID       班级名称       机构ID     机构名称
    void addzerooosec(string teachClassId,string classId,string name,string orgId,string orgName,vector<keypair> contentmap);    //行政班信息
    [[eosio::action]]
    void modzerooosec(string teachClassId,string classId,string name,string orgId,string orgName,vector<keypair> contentmap);
    [[eosio::action]]
    void delzerooosec(string teachClassId,string classId,string name,string orgId,string orgName,vector<keypair> contentmap);

    [[eosio::action]]  //默认教学班ID          班级ID       班级名称       机构ID     机构名称
    void addzerooothi(string teachClassId,string classId,string name,string orgId,string orgName,vector<keypair> contentmap);    //教学班信息
    [[eosio::action]]
    void modzerooothi(string teachClassId,string classId,string name,string orgId,string orgName,vector<keypair> contentmap);
    [[eosio::action]]
    void delzerooothi(string teachClassId,string classId,string name,string orgId,string orgName,vector<keypair> contentmap);

    [[eosio::action]]  //教师ID          教师名称           任教ID           班级ID         班级名称         机构ID       机构名称
    void addzerooofou(string userId,string teacherName,string teachId,string classId,string className,string orgId,string orgName,vector<keypair> contentmap);   //任课信息
    [[eosio::action]]
    void modzerooofou(string userId,string teacherName,string teachId,string classId,string className,string orgId,string orgName,vector<keypair> contentmap);
    [[eosio::action]]
    void delzerooofou(string userId,string teacherName,string teachId,string classId,string className,string orgId,string orgName,vector<keypair> contentmap);

    [[eosio::action]]  //学生ID       学期码     学期名称     活动名称    是否有效
    void addfoufirfir(string xsid,string xqm,string xqmc,string hdmc,string sfyx,vector<keypair> contentmap);    //学生德育信息
    [[eosio::action]]
    void modfoufirfir(string xsid,string xqm,string xqmc,string hdmc,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfoufirfir(string xsid,string xqm,string xqmc,string hdmc,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //学生ID       学期码     学期名称     活动名称    是否有效
    void addfoufirsec(string xsid,string xqm,string xqmc,string hdmc,string sfyx,vector<keypair> contentmap);    //学生美育信息
    [[eosio::action]]
    void modfoufirsec(string xsid,string xqm,string xqmc,string hdmc,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfoufirsec(string xsid,string xqm,string xqmc,string hdmc,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //学生ID       学期码     学期名称     活动名称    是否有效
    void addfoufirthi(string xsid,string xqm,string xqmc,string hdmc,string sfyx,vector<keypair> contentmap);    //学生劳动信息
    [[eosio::action]]
    void modfoufirthi(string xsid,string xqm,string xqmc,string hdmc,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfoufirthi(string xsid,string xqm,string xqmc,string hdmc,string sfyx,vector<keypair> contentmap);



    [[eosio::action]]  //资源ID            资源名称     教师ID              教师名称           上传时间    md5          积分
    void addresource(string resource_id,string name,string teacher_id,string teacher_name,string time,string md5,string score);  //资源确权
    [[eosio::action]]  //  ID                      资源ID    资源名称       使用人ID       使用人名称       时间        使用积分
    void addrscrecord(string resource_record_id,string resource_id,string name,string user_id,string user_name,string time,string score);    //资源使用记录
    [[eosio::action]]  // 积分记录ID        积分类别     积分变化值    使用人ID        使用人名称        时间
    void addscrrecord(string record_id,string type,string value,string user_id,string user_name,string time,string remark);  //积分记录

    [[eosio::action]]       //资源ID            资源名称     教师ID              教师名称           上传时间    md5
    void addresources(string resource_id,string name,string teacher_id,string teacher_name,string time,string md5);    //资源确权
    [[eosio::action]]   //  ID                      资源ID            资源名称    使用人ID          使用人名称        时间
    void addresrecord(string resource_record_id,string resource_id,string name,string user_id,string user_name,string time);    //资源使用记录
    [[eosio::action]]  //  试题ID                   试题编码              题目             分值               难度         解析             老师ID             老师名称           时间       选项
    void addtestques(string test_question_id,string question_code,string question_title,uint32_t score,string level,string analysis,string teacher_id,string teacher_name,string time,vector<option> options);    //试题
    [[eosio::action]]  //  主键           闯关编码              闯关ID            知识点编码          知识点ID      时间          学生ID              学生名称
    void addanswerrec(string record_id,string level_code,string level_id,string knowledge_code,string knowledge_id,string time,string student_id,string student_name);    //答题记录
    [[eosio::action]]   //记录ID                试题ID                   试题编码             选项           是否正确
    void addansquestn(vector<aserandquest> answer_test_questions);    //答题记录、试题关系表







    //新的12个表的合约
    [[eosio::action]]  //用户ID       学校编号      学校名称      考试编号    考试名称    用户ID       机构ID       是否有效
    void addfirfirfir(string yhid,string xxbh,string xxmc,string ksbh,string ksmc,string orgid,string sfyx,vector<keypair> contentmap);         //学生考试信息数据
    [[eosio::action]]
    void modfirfirfir(string yhid,string xxbh,string xxmc,string ksbh,string ksmc,string orgid,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirfir(string yhid,string xxbh,string xxmc,string ksbh,string ksmc,string orgid,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //用户ID      班级编号     班级名称      考试编号     考试名称     年级编号    年级名称      科目编号      科目名称    学校编号    学校名称        机构ID
    void addfirfirsec(string yhid,string bjbh,string bjmc,string ksbh,string ksmc,string njbh,string njmc ,string kmbh,string kmmc,string xxbh,string xxmc,string orgid,vector<keypair> contentmap);    //考试班级科目
    [[eosio::action]]
    void modfirfirsec(string yhid,string bjbh,string bjmc,string ksbh,string ksmc,string njbh,string njmc ,string kmbh,string kmmc,string xxbh,string xxmc,string orgid,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirsec(string yhid,string bjbh,string bjmc,string ksbh,string ksmc,string njbh,string njmc ,string kmbh,string kmmc,string xxbh,string xxmc,string orgid,vector<keypair> contentmap);

    [[eosio::action]]  //机构ID          机构名称     机构类型   机构状态    是否有效
    void addfirfirthi(string orgid,string jgmc,string jglx,string jgzt,string sfyx,vector<keypair> contentmap);    //机构信息
    [[eosio::action]]
    void modfirfirthi(string orgid,string jgmc,string jglx,string jgzt,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirthi(string orgid,string jgmc,string jglx,string jgzt,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //班级名称     学校编号     学校名称     机构ID      是否有效
    void addfirfirfou(string bjmc,string xxbh,string xxmc,string orgid,string sfyx,vector<keypair> contentmap);    //行政班信息
    [[eosio::action]]
    void modfirfirfou(string bjmc,string xxbh,string xxmc,string orgid,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirfou(string bjmc,string xxbh,string xxmc,string orgid,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //学生编号     学生姓名     学校编号    学校名称     科目编号     科目名称     总分       机构ID
    void addfirfirfif(string xsbh,string xsxm,string xxbh,string xxmc,string kmbh,string kmmc,string zf,string orgid,vector<keypair> contentmap);    //单次考试信息
    [[eosio::action]]
    void modfirfirfif(string xsbh,string xsxm,string xxbh,string xxmc,string kmbh,string kmmc,string zf,string orgid,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirfif(string xsbh,string xsxm,string xxbh,string xxmc,string kmbh,string kmmc,string zf,string orgid,vector<keypair> contentmap);

    [[eosio::action]]  //学生编号    学生姓名      科目编号    科目名称    得分          机构ID    是否有效
    void addfirfirsix(string xsbh,string xsxm,string kmbh,string kmmc,string df,string orgid,string sfyx,vector<keypair> contentmap);    //单次考试信息详情
    [[eosio::action]]
    void modfirfirsix(string xsbh,string xsxm,string kmbh,string kmmc,string df,string orgid,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirsix(string xsbh,string xsxm,string kmbh,string kmmc,string df,string orgid,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //机构ID        区域编号    区域名称     学年编号     学年名称  语文学业水平    数学学业水平     科学学业水平     体育学业水平  艺术学业水平     德育学业水平     是否有效
    void addfirfirsev(string orgid,string qybh,string qymc,string xnbh,string xnmc,string ywxysp,string sxxysp,string kxxysp,string tyxysp,string ysxysp,string dyxysp,string sfyx,vector<keypair> contentmap);    //相关科目学生学业水平信息
    [[eosio::action]]
    void modfirfirsev(string orgid,string qybh,string qymc,string xnbh,string xnmc,string ywxysp,string sxxysp,string kxxysp,string tyxysp,string ysxysp,string dyxysp,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirsev(string orgid,string qybh,string qymc,string xnbh,string xnmc,string ywxysp,string sxxysp,string kxxysp,string tyxysp,string ysxysp,string dyxysp,string sfyx,vector<keypair> contentmap);

    [[eosio::action]]  //机构ID       区域编号     区域名称     学年编号    学年名称     学科编号     学科名称     年级编号     年级名称     平均分    是否有效
    void addfirfireig(string orgid,string qybh,string qymc,string xnbh,string xnmc,string xkbh,string xkmc,string njbh,string njmc,string pjf,string sfyx,vector<keypair> contentmap);   //学生总成绩平均分信息
    [[eosio::action]]
    void modfirfireig(string orgid,string qybh,string qymc,string xnbh,string xnmc,string xkbh,string xkmc,string njbh,string njmc,string pjf,string sfyx,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfireig(string orgid,string qybh,string qymc,string xnbh,string xnmc,string xkbh,string xkmc,string njbh,string njmc,string pjf,string sfyx,vector<keypair> contentmap);


    [[eosio::action]]  //机构ID        区域编号     区域名称     学年编号     学年名称    城乡编号     城乡名称    年级编号     年级名称
    void addfirfirnin(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,string njbh,string njmc,vector<keypair> contentmap);    //语文各能力得分信息
    [[eosio::action]]
    void modfirfirnin(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,string njbh,string njmc,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirnin(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,string njbh,string njmc,vector<keypair> contentmap);


    [[eosio::action]]  //机构ID        区域编号     区域名称     学年编号     学年名称    城乡编号     城乡名称    年级编号     年级名称
    void addfirfirten(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,string njbh,string njmc,vector<keypair> contentmap);    //数学各能力得分信息
    [[eosio::action]]
    void modfirfirten(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,string njbh,string njmc,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirten(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,string njbh,string njmc,vector<keypair> contentmap);


    [[eosio::action]] //机构ID        区域编号     区域名称     学年编号     学年名称    城乡编号     城乡名称    年级编号     年级名称
    void addfirfirele(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,string njbh,string njmc,vector<keypair> contentmap);    //英语各能力得分信息
    [[eosio::action]]
    void modfirfirele(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,string njbh,string njmc,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirele(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,string njbh,string njmc,vector<keypair> contentmap);


    [[eosio::action]] //机构ID        区域编号     区域名称     学年编号     学年名称    城乡编号     城乡名称
    void addfirfirtwe(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,vector<keypair> contentmap);    //老师工作状态信息
    [[eosio::action]]
    void modfirfirtwe(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,vector<keypair> contentmap);
    [[eosio::action]]
    void delfirfirtwe(string orgid,string qybh,string qymc,string xnbh,string xnmc,string cxbh,string cxmc,vector<keypair> contentmap);

    [[eosio::action]]  //学生ID                                   创建者
    void addbasicinfo(string stu_id,vector<tkeys> contentmap,string createPerson);    //基本信息
    [[eosio::action]]
    void modbasicinfo(string stu_id,vector<tkeys> contentmap,string createPerson);
    [[eosio::action]]
    void delbasicinfo(string stu_id,vector<tkeys> contentmap,string createPerson);

    [[eosio::action]]  //学生ID                                    创建者
    void addprgrminfo(string stu_id,vector<tkeys> contentmap,string createPerson);    //方案信息
    [[eosio::action]]
    void modprgrminfo(string stu_id,vector<tkeys> contentmap,string createPerson);
    [[eosio::action]]
    void delprgrminfo(string stu_id,vector<tkeys> contentmap,string createPerson);

























private:

};

} // namespace vankia
