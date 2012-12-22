//
//  CAppDelegate.m
//  Engine Test
//
//  Created by Matt Dalzell on 8/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#import "CAppDelegate.h"

//Engine Includes
#include "Ascension.h"
#include "ASCWindowTestContext.h"
#include "ASCFinalShutdownDelay.h"
#include "ASCRenderer.h"
#include "ASCClock.h"

@implementation CAppDelegate

@synthesize window = _window;
@synthesize glView = m_glView;

@synthesize viewController = m_pViewController;

- (void)dealloc
{
    [m_displayLink invalidate];
	Ascension::DeInitialise();
	ASCFinalShutdownDelay::SetCanEnd( true );
    [_window release];
    [m_glView release];
    [m_pViewController release];
    [super dealloc];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    ASCFinalShutdownDelay::SetCanEnd( false ); 
    ASCDebuggingManager::OutputWindowLine("Starting Initialisation");
    
    //self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    self.glView = [[[ASCRenderView alloc] initWithFrame:screenBounds] autorelease];
    [self.window addSubview:m_glView];
    //self.window.backgroundColor = [UIColor blueColor];

    
    [self.viewController setView:self.glView];
    [self.viewController Initialise];
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
    
    //[self.window setMultipleTouchEnabled:YES];
    
    
	SInitParameters InitParam;
	InitParam.m_bCreateClock = true;
	InitParam.m_bUseProfiling = true;
	InitParam.m_bCreateMouse = true;
	InitParam.m_bCreateKeyboard = true;
	InitParam.m_pContext = new ASCWindowTestContext();
    InitParam.m_pIOSGLView = self.glView;
    InitParam.m_uScreenHeight = 360;
    InitParam.m_uScreenWidth = 480;
    InitParam.m_uScreenHeight = screenBounds.size.height;
    InitParam.m_uScreenWidth = screenBounds.size.width;
    
	Ascension::Initialise(InitParam);
    
    
    [self startLoop];
    ASCDebuggingManager::OutputWindowLine("Starting App");
    
    return YES;
}

- (void) gameLoop
{
    while ( m_bRunning )
    {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        Ascension::Update();
        [pool release];
    }
}

- (void)startLoop
{
    m_bRunning = YES;
    
    //Spawn gameloop off onto another thread. ##NO LONGER SUPPORTED##
    //[-NSThread detachNewThreadSelector:@selector(gameLoop) toTarget:self withObject:nil];
    
    m_displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(renderFrame)];
    m_displayLink.frameInterval = 1;
    Ascension::Clock().LinkDisplayLink(m_displayLink);
    [m_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)renderFrame
{
    //ASCDebuggingManager::OutputWindowLine("Update then Render");
    
    //float fDelta = displayLink.duration * displayLink.frameInterval;
    Ascension::Update();
}

- (void)stopLoop
{
    m_bRunning = NO;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
    [m_displayLink invalidate];
	Ascension::DeInitialise();
	ASCFinalShutdownDelay::SetCanEnd( true );
}

@end
