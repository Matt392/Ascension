//
//  CAppDelegate.h
//  Engine Test
//
//  Created by Matt Dalzell on 8/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ASCRenderView.h"
#import "CViewController.h"

@interface CAppDelegate : UIResponder <UIApplicationDelegate>{
    ASCRenderView* m_glView;
    CViewController* m_pViewController;
    bool m_bRunning;
    CADisplayLink* m_displayLink;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet CViewController* viewController;
@property (nonatomic, retain) IBOutlet ASCRenderView* glView;

- (void)gameLoop;
- (void)startLoop;
- (void)stopLoop;
- (void)renderFrame;

@end
