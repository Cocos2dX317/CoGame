#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#define WXAppID "wxf92e51752341cf2f"
#define WXAppSecret "0d27e88ce85207d33115613063bf7ffc"

@interface WXView : UIViewController<UITextViewDelegate,UINavigationControllerDelegate>
{
    //
    NSString* OpenID;
    NSTimer * mTimer;
}
- (BOOL)shouldAutorotate;

-(void )sendAuthRequest:(NSString *)scope
                   State:(NSString *)state
                  OpenID:(NSString *)openID;


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
                 scene:(int)scene;
@end
