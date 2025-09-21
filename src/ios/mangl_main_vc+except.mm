#import "mangl_main_vc+except.h"

#import "mangl_main_vc+timer.h"


@implementation ManglMainViewController(Except)

- (void)onExceptInit {
	//wasException_ = false ;
	//_exceptionTitle = @"Critical Error!" ;
	//_exceptionMessage = @"Unhandled Exception!" ;
}


+ (void)showAlert:(NSString *)message withTitle:(NSString *)title {
	auto alert = [UIAlertController alertControllerWithTitle:title
                                                     message:message
                                              preferredStyle:UIAlertControllerStyleAlert];
    
	auto action = [UIAlertAction actionWithTitle:@"OK"
                                           style:UIAlertActionStyleDefault
                                         handler:^(UIAlertAction *action){}];
    
    [alert addAction:action];
    [ManglMainViewController.instance presentViewController:alert animated:YES completion:nil];
}

+ (void)showAlert:(NSString *)message {
	[self showAlert:message withTitle:@"Alert!"] ;
}

+ (void)showCriticalError:(NSString *)message withTitle:(NSString *)title {
	[self showAlert:message withTitle:title] ;
}

+ (void)showCriticalError:(NSString *)message {
	[self showCriticalError:message withTitle:@"Critical Error"] ;
}

+ (void)showError:(NSError*)error {
    [self showCriticalError:error.localizedDescription withTitle:@"Error"];
}

+ (void)showError:(NSError*)error withTitle:(NSString*)title {
    [self showCriticalError:error.localizedDescription withTitle:title];
}



- (void)handleCriticalExceptionWithTitle: (NSString *)title message: (NSString *)message {
	[self killTimer] ;

	self.exceptionTitle = title ;
	if ( self.exceptionTitle == nil )
		self.exceptionTitle = @"Critical Error!" ;
	
	self.exceptionMessage = message ;
	if ( self.exceptionMessage == nil )
		self.exceptionMessage = @"Unhandled Exception!" ;

    dispatch_async(dispatch_get_main_queue(), ^{
        auto alert = [UIAlertController alertControllerWithTitle:self.exceptionTitle
                                                         message:self.exceptionMessage
                                                  preferredStyle:UIAlertControllerStyleAlert];

        auto defaultAction = [UIAlertAction actionWithTitle:@"OK"
                                                      style:UIAlertActionStyleDefault
                                                    handler:^(UIAlertAction *action) {
            exit(1) ;
        }] ;

        [alert addAction:defaultAction] ;
        [self presentViewController:alert animated:YES completion:nil];
    }) ;
}

- (void)handleCriticalException {
	[self handleCriticalExceptionWithTitle:nil message:nil];
}

- (void)handleCriticalExceptionUnhandled {
	[self handleCriticalExceptionWithTitle:nil message:nil];
}

- (void)handleCriticalExceptionStdExcept:(const std::exception&)e {
	[self handleCriticalExceptionWithTitle:nil message:[NSString stringWithUTF8String:e.what()]];
}

- (void)handleCriticalExceptionNSException:(NSException *)e {
	[self handleCriticalExceptionWithTitle:e.name message:e.reason];
}

@end


