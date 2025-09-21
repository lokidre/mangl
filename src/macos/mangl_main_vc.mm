#import "mangl_main_vc.h"

#import "../apple/mangl_imacos_util.h"

#include "../core/mangl_framework.h"

using namespace mangl;

static ManglMainViewController* mainViewController_{};

@implementation ManglMainViewController

+(instancetype)instance {  return mainViewController_; }


// MARK: Initialization
-(instancetype)initWithNibName:(NSNibName)nibName bundle:(NSBundle *)nibBundle
{
    self = [super initWithNibName:nibName bundle:nibBundle];
    [self ctor];
    return self;
}

- (nullable instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    [self ctor];
    return self;
}

- (void)ctor
{
    mainViewController_ = self;
    
    _runtimeView = [[ManglMainView alloc] initWithFrame:CGRectZero];
    _runtimeView.wantsLayer = YES;
    self.view = _runtimeView;
}


// MARK: View
- (void)loadView
{
    MDebugFunc();
    [super loadView];
    
    //self.view = _runtimeView;
}


- (void)viewDidLoad
{
    MDebugFunc();
    [super viewDidLoad];
    
    //[self.view makeFirs
}


// MARK: Alerts

-(void)showAlert:(NSString *)message withTitle:(NSString *)title
{
    auto alert = [[NSAlert alloc] init];
    [alert setShowsHelp:NO];
    [alert setAlertStyle:NSAlertStyleInformational];
    [alert setMessageText:title];
    [alert setInformativeText:message];
    [alert runModal];
}

-(void)showAlert:(NSString *)message {
    [self showAlert:message withTitle:@"Alert!"];
}

-(void)showCriticalError:(NSString *)message withTitle:(NSString *)title {
    [self showAlert:message withTitle:title];
}

-(void)showCriticalError:(NSString *)message {
    [self showCriticalError:message withTitle:@"Critical Error"];
}



// MARK: Keyboard
- (void)keyDown:(NSEvent *)event
{
}

- (void)keyUp:(NSEvent *)event
{
}


@end

