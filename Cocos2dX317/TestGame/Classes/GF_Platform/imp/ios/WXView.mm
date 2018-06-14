//
//  TestViewController.m
//  image
//
//  Created by  on 12-7-19.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "WXView.h"
#include "cocos2d.h"
#import "WXApiManager.h"
#import "WXApiRequestHandler.h"

#import "WechatAuthSDK.h"
using namespace cocos2d;
@interface WXView ()<WXApiManagerDelegate,UITextViewDelegate, WechatAuthAPIDelegate>

@end

@implementation WXView

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    NSLog(@"WXView viewDidLoad-------------- ");
    [WXApiManager sharedManager].delegate = self;
  
    

}

- (void)sendAuthRequest:(NSString *)scope
                       State:(NSString *)state
                      OpenID:(NSString *)openID {
    
    [WXApiRequestHandler sendAuthRequestScope: scope
                                        State:state
                                       OpenID:openID
                             InViewController:self];
    NSLog(@"WXView %@,%@,%@",scope,state,openID);

}

/**
 分享连接
 
 @param title @“标题”
 @param desc @“描述”
 @param img @img
 @param linkurl @link
 */
-(void )shareUrl:(NSString *)title
            desc:(NSString *)desc
           image:(NSString *)img
         linkurl:(NSString*)linkurl
           scene:(int)scene{
    
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = title;
    message.description = desc;
    
    NSURL *url = [NSURL URLWithString:img];
    NSData *imageData = [NSData dataWithContentsOfURL:url];
    UIImage *imagelink = [UIImage imageWithData:imageData];
    [message setThumbImage:imagelink];
    
    WXWebpageObject *webpageobj = [WXWebpageObject object];
    webpageobj.webpageUrl = linkurl;
    message.mediaObject = webpageobj;
    
    SendMessageToWXReq* req = [[SendMessageToWXReq alloc] init];
    req.bText = NO;
    req.message = message;
    req.scene = scene;
    
    [WXApi sendReq:req];
    
}


//通过code获取access_token，openid，unionid
- (void)getWeiXinOpenId:(NSString *)code{
    
    //https://api.weixin.qq.com/sns/oauth2/access_token?appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code
    NSString *url =[NSString stringWithFormat:@"https://api.weixin.qq.com/sns/oauth2/access_token?appid=%@&secret=%@&code=%@&grant_type=authorization_code",@WXAppID,@WXAppSecret,code];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSURL *zoneUrl = [NSURL URLWithString:url];
        NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl encoding:NSUTF8StringEncoding error:nil];
        NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
        dispatch_async(dispatch_get_main_queue(), ^{
            if (data){
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
                
                NSLog(@"dic %@ ",dic);
                
                OpenID = dic[@"openid"];
                
                NSString *unionid = dic[@"unionid"];
                NSString * access_token = [dic objectForKey:@"access_token"];
                //NSString * expires_in       = [dic objectForKey:@"expires_in"];
                //NSString * refresh_token    = [dic objectForKey:@"refresh_token"];
                
                [self getWXUserInfo:access_token openid:OpenID];
            }
        });
    });
    
}

//通过access_token得到昵称、unionid等信息
- (void)getWXUserInfo:(NSString *)access_token
                    openid:(NSString *)openid
{
    
    NSString * url = [NSString stringWithFormat:@"https://api.weixin.qq.com/sns/userinfo?access_token=%@&openid=%@",access_token,openid];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSURL *zoneUrl = [NSURL URLWithString:url];
        NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl encoding:NSUTF8StringEncoding error:nil];
        NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
        dispatch_async(dispatch_get_main_queue(), ^{
            if (data){
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
                
                NSLog(@"dic %@ ",dic);

                
                //登录成功
//                GF_ZJHModel::getInstance()->isFirstLoginSucc = true;
//                GF_ZJHModel::getInstance()->ucSid = GF_ZJHModel::getInstance()->g_wx_userinfo.unionid;
//                
//                GF_ZJHModel::getInstance()->g_wx_userinfo.openid = [dic[@"openid"] UTF8String];
//                GF_ZJHModel::getInstance()->g_wx_userinfo.unionid = [dic[@"unionid"] UTF8String];
//                GF_ZJHModel::getInstance()->g_wx_userinfo.city = [dic[@"city"] UTF8String];
//                GF_ZJHModel::getInstance()->g_wx_userinfo.country = [dic[@"country"] UTF8String];
//                GF_ZJHModel::getInstance()->g_wx_userinfo.headimgurl = [dic[@"headimgurl"] UTF8String];
//                GF_ZJHModel::getInstance()->g_wx_userinfo.language = [dic[@"language"] UTF8String];
//                GF_ZJHModel::getInstance()->g_wx_userinfo.nickname = [dic[@"nickname"] UTF8String];
//                GF_ZJHModel::getInstance()->g_wx_userinfo.province = [dic[@"province"] UTF8String];
//                GF_ZJHModel::getInstance()->g_wx_userinfo.sex = [dic[@"sex"] intValue];
//                string avatar = [dic[@"unionid"] UTF8String];
//                GF_ZJHModel::getInstance()->g_wx_userinfo.avatar = avatar+".png";
//                CCHttpAgent::getInstance()->doGetImageByUrl(1000, GF_ZJHModel::getInstance()->g_wx_userinfo.avatar, GF_ZJHModel::getInstance()->g_wx_userinfo.headimgurl.c_str());
                //NSString * nickStr = [dic objectForKey:@"nickname"];
                
                //创建定时器
                mTimer = [NSTimer timerWithTimeInterval:1.0 target:self selector:@selector(updateInfo:) userInfo:nil repeats:YES];
                [[NSRunLoop currentRunLoop] addTimer:mTimer forMode:NSRunLoopCommonModes];
                
//                NSLog(@"nickStr %@,nickStr %@,sex %d ",[NSString stringWithFormat:@"%s", GF_ZJHModel::getInstance()->g_wx_userinfo.nickname.c_str()],
//                      [NSString stringWithFormat:@"%s", GF_ZJHModel::getInstance()->g_wx_userinfo.headimgurl.c_str()],
//                      GF_ZJHModel::getInstance()->g_wx_userinfo.sex);
            }
        });
    });
}


-(void)updateInfo:(int) val
{
//    CCHttpImagePacket &imagePacket = CCHttpAgent::getInstance()->imagePackets[1000];
//    if (imagePacket.status != 0)
//    {
//        if (imagePacket.status == 1) {
//            [mTimer setFireDate:[NSDate distantFuture]];
//            return;
//        }
//        [mTimer setFireDate:[NSDate distantFuture]];
//        GF_ZJHModel::getInstance()->g_wx_userinfo.avatar = imagePacket.path;
//        NSLog(@"updateInfo: %@",[NSString stringWithFormat:@"%s", imagePacket.path.c_str()]);
//
//    }
}


-(void)viewDidAppear:(BOOL)animated{
    [super viewDidAppear:animated];
    
    
}
- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (BOOL)shouldAutorotate{
    return false;
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}


#pragma mark -WechatAuthAPIDelegate
//得到二维码
- (void)onAuthGotQrcode:(UIImage *)image
{
    NSLog(@"onAuthGotQrcode");
}

//二维码被扫描
- (void)onQrcodeScanned
{
    NSLog(@"onQrcodeScanned");
}

//成功登录
- (void)onAuthFinish:(int)errCode AuthCode:(NSString *)authCode
{
    NSLog(@"onAuthFinish");
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"onAuthFinish"
                                                    message:[NSString stringWithFormat:@"authCode:%@ errCode:%d", authCode, errCode]
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
}

#pragma mark - WXApiManagerDelegate
- (void)managerDidRecvGetMessageReq:(GetMessageFromWXReq *)req {
    // 微信请求App提供内容， 需要app提供内容后使用sendRsp返回
    NSString *strTitle = [NSString stringWithFormat:@"微信请求App提供内容"];
    NSString *strMsg = [NSString stringWithFormat:@"openID: %@", req.openID];
    
}

- (void)managerDidRecvShowMessageReq:(ShowMessageFromWXReq *)req {
    WXMediaMessage *msg = req.message;
    
    //显示微信传过来的内容
    NSString *strTitle = [NSString stringWithFormat:@"微信请求App显示内容"];
    NSString *strMsg = nil;
    
    if ([msg.mediaObject isKindOfClass:[WXAppExtendObject class]]) {
        WXAppExtendObject *obj = msg.mediaObject;
        strMsg = [NSString stringWithFormat:@"openID: %@, 标题：%@ \n描述：%@ \n附带信息：%@ \n文件大小:%lu bytes\n附加消息:%@\n", req.openID, msg.title, msg.description, obj.extInfo, (unsigned long)obj.fileData.length, msg.messageExt];
    }
    else if ([msg.mediaObject isKindOfClass:[WXTextObject class]]) {
        WXTextObject *obj = msg.mediaObject;
        strMsg = [NSString stringWithFormat:@"openID: %@, 标题：%@ \n描述：%@ \n内容：%@\n", req.openID, msg.title, msg.description, obj.contentText];
    }
    else if ([msg.mediaObject isKindOfClass:[WXImageObject class]]) {
        WXImageObject *obj = msg.mediaObject;
        strMsg = [NSString stringWithFormat:@"openID: %@, 标题：%@ \n描述：%@ \n图片大小:%lu bytes\n", req.openID, msg.title, msg.description, (unsigned long)obj.imageData.length];
    }
    else if ([msg.mediaObject isKindOfClass:[WXLocationObject class]]) {
        WXLocationObject *obj = msg.mediaObject;
        strMsg = [NSString stringWithFormat:@"openID: %@, 标题：%@ \n描述：%@ \n经纬度：lng:%f_lat:%f\n", req.openID, msg.title, msg.description, obj.lng, obj.lat];
    }
    else if ([msg.mediaObject isKindOfClass:[WXFileObject class]]) {
        WXFileObject *obj = msg.mediaObject;
        strMsg = [NSString stringWithFormat:@"openID: %@, 标题：%@ \n描述：%@ \n文件类型：%@ 文件大小:%lu\n", req.openID, msg.title, msg.description, obj.fileExtension, (unsigned long)obj.fileData.length];
    }
    else if ([msg.mediaObject isKindOfClass:[WXWebpageObject class]]) {
        WXWebpageObject *obj = msg.mediaObject;
        strMsg = [NSString stringWithFormat:@"openID: %@, 标题：%@ \n描述：%@ \n网页地址：%@\n", req.openID, msg.title, msg.description, obj.webpageUrl];
    }
    
}

- (void)managerDidRecvLaunchFromWXReq:(LaunchFromWXReq *)req {
    WXMediaMessage *msg = req.message;
    
    //从微信启动App
    NSString *strTitle = [NSString stringWithFormat:@"从微信启动"];
    NSString *strMsg = [NSString stringWithFormat:@"openID: %@, messageExt:%@", req.openID, msg.messageExt];
    
}

- (void)managerDidRecvMessageResponse:(SendMessageToWXResp *)response {
    NSString *strTitle = [NSString stringWithFormat:@"发送媒体消息结果"];
    NSString *strMsg = [NSString stringWithFormat:@"errcode:%d", response.errCode];
    
}

- (void)managerDidRecvAddCardResponse:(AddCardToWXCardPackageResp *)response {
    NSMutableString* cardStr = [[NSMutableString alloc] init];
    for (WXCardItem* cardItem in response.cardAry) {
        [cardStr appendString:[NSString stringWithFormat:@"code:%@ cardid:%@ cardext:%@ cardstate:%u\n",cardItem.encryptCode,cardItem.cardId,cardItem.extMsg,(unsigned int)cardItem.cardState]];
    }
    
}

- (void)managerDidRecvChooseCardResponse:(WXChooseCardResp *)response {
    NSMutableString* cardStr = [[NSMutableString alloc] init];
    for (WXCardItem* cardItem in response.cardAry) {
        [cardStr appendString:[NSString stringWithFormat:@"cardid:%@, encryptCode:%@, appId:%@\n",cardItem.cardId,cardItem.encryptCode,cardItem.appID]];
    }
    
}

- (void)managerDidRecvChooseInvoiceResponse:(WXChooseInvoiceResp *)response {
    NSMutableString* cardStr = [[NSMutableString alloc] init];
    for (WXInvoiceItem* cardItem in response.cardAry) {
        [cardStr appendString:[NSString stringWithFormat:@"cardid:%@, encryptCode:%@, appId:%@\n",cardItem.cardId,cardItem.encryptCode,cardItem.appID]];
    }
    
}

- (void)managerDidRecvAuthResponse:(SendAuthResp *)response {
    NSString *strTitle = [NSString stringWithFormat:@"Auth结果"];
    NSString *strMsg = [NSString stringWithFormat:@"code:%@,state:%@,errcode:%d", response.code, response.state, response.errCode];
    
    NSLog(@"%@%@", strTitle,strMsg);
    if(response.errCode==0){
        [self getWeiXinOpenId:response.code];
 
    }
}

- (void)managerDidRecvSubscribeMsgResponse:(WXSubscribeMsgResp *)response
{
    NSString *title = [NSString stringWithFormat:@"templateId:%@,scene:%@,action:%@,reserved:%@,openId:%@",response.templateId,[NSNumber numberWithInteger:response.scene],response.action,response.reserved,response.openId];
    
}

- (void)managerDidRecvLaunchMiniProgram:(WXLaunchMiniProgramResp *)response
{
    NSString *strTitle = [NSString stringWithFormat:@"LaunchMiniProgram结果"];
    NSString *strMsg = [NSString stringWithFormat:@"errMsg:%@,errcode:%d", response.extMsg, response.errCode];
    
}

- (void)managerDidRecvInvoiceAuthInsertResponse:(WXInvoiceAuthInsertResp *)response
{
    NSString *strTitle = [NSString stringWithFormat:@"电子发票授权开票"];
    NSString *strMsg = [NSString stringWithFormat:@"errcode:%d,wxorderid:%@", response.errCode, response.wxOrderId];
    
}

- (void)managerDidRecvNonTaxpayResponse:(WXNontaxPayResp *)response
{
    NSString *strTitle = [NSString stringWithFormat:@"非税支付结果"];
    NSString *strMsg = [NSString stringWithFormat:@"errcode:%d,wxorderid:%@", response.errCode, response.wxOrderId];
    
}

- (void)managerDidRecvPayInsuranceResponse:(WXPayInsuranceResp *)response
{
    NSString *strTitle = [NSString stringWithFormat:@"医保支付结果"];
    NSString *strMsg = [NSString stringWithFormat:@"errcode:%d, wxorderid:%@", response.errCode, response.wxOrderId];
    
}


@end
