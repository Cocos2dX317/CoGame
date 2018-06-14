#ifndef _GLOBAL_DEFINE_
#define _GLOBAL_DEFINE_


#define CHECK_FUNCTION_MASK(_MASK_, _FLAG_) ((_MASK_&_FLAG_) > 0)
#define PHONE_CHECK "^((13[0-9])|(15[0-9])|(17[0-9])|(18[0-9]))[0-9]{8}$"
#define EMALI_CHECK1 "^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$"//只允许英文字母、数字、下划线、英文句号、以及中划线组成
#define EMALI_CHECK2 "^[A-Za-z0-9\u4e00-\u9fa5]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$"//名称允许汉字、字母、数字，域名只允许英文域名

//0-63
enum function_switch{
    kfAll = 1<<0,                 //all
    kfWorldChat = 1<<1,           //世界聊天
    kfSafeBox = 1<<2,             //保险箱
    kfTask = 1<<3,                //任务
    kfHuoDong = 1<<4,             //活动
    kfMail = 1<<5,                //邮件
    kfRank = 1<<6,                //排行
};


//LOG


//



#endif
