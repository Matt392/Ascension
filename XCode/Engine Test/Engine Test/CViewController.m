//
//  CViewController.m
//  Engine Test
//
//  Created by Matt Dalzell on 9/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "CViewController.h"
#include "Ascension.h"
#include "ASCTouchInput.h"

@implementation CViewController

- (void)dealloc
{
    [super dealloc];
}


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
        [self.view setMultipleTouchEnabled:YES];
        
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)Initialise
{
    [self.view setMultipleTouchEnabled:YES];
    //[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[UIApplication sharedApplication] setStatusBarHidden:YES ];
    
    UIInterfaceOrientation interfaceOrientation = self.interfaceOrientation;
    
    
    if(interfaceOrientation == UIInterfaceOrientationLandscapeRight)
    {
        m_eOrientation = ESO_LANDSCAPE_RIGHT;
    }
    else if(interfaceOrientation == UIInterfaceOrientationLandscapeRight)
    {
        m_eOrientation = ESO_PORTRAIT;
    }
    if(Ascension::Device() == ED_IPAD)
    {
        self.view.bounds = CGRectMake( 0.0, 0.0, 1024, 768 );
    }
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    //return (interfaceOrientation == UIInterfaceOrientationPortrait);
    if ( interfaceOrientation == UIInterfaceOrientationLandscapeRight )
    {
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight];
        
        CGAffineTransform transform = CGAffineTransformMakeRotation(M_PI_2);
        self.view.transform = transform;
        self.view.bounds = CGRectMake( 0.0, 0.0, 480, 320 );
        
        m_eOrientation = ESO_LANDSCAPE_RIGHT;
        
        return YES;
    }
    /*
    else if ( interfaceOrientation == UIInterfaceOrientationPortrait )
    {
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
        
        CGAffineTransform transform = CGAffineTransformMakeRotation(M_PI_2);
        self.view.transform = transform;
        if(Ascension::Device() == ED_IPHONE)
        {
            self.view.bounds = CGRectMake( 0.0, 0.0, 480, 320 );
        }
        else if(Ascension::Device() == ED_IPAD)
        {
            self.view.bounds = CGRectMake( 0.0, 0.0, 1024, 768 );
        }
        
        m_eOrientation = ESO_PORTRAIT;
        
        return ;
    }*/
    /*
    else if ( interfaceOrientation == UIInterfaceOrientationLandscapeLeft )
    {
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft];
        
        CGAffineTransform transform = CGAffineTransformMakeRotation(M_PI_2);
        self.view.transform = transform;
        self.view.bounds = CGRectMake( 0.0, 0.0, 480, 320 );
        
        m_eOrientation = ESO_LANDSCAPE_LEFT;
        
        return YES;
    }
    */
   // m_eOrientation = ESO_COUNT;
     
    return NO;
}
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{    
    [self ProcessTouchEvents:touches ofType:ETS_STARTED];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self ProcessTouchEvents:touches ofType:ETS_MOVED];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self ProcessTouchEvents:touches ofType:ETS_ENDED];
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self ProcessTouchEvents:touches ofType:ETS_ENDED];
}

-(void)ProcessTouchEvents:(NSSet *)touches ofType:(unsigned int)type
{
    
    NSArray *allTouches = [touches allObjects]; 
    UITouch *touch = [touches anyObject];
    CGPoint touchLocation;
    
    int count = [allTouches count];
    
    ASCTouchInput* pTouchInput = reinterpret_cast<ASCTouchInput*>( &Ascension::Touch());
    
    ASCTouchInfo sInfo;
    ASCVector2 v2Pos;
    
    for(UINT32 i = 0; i < count; ++i)
    {
        touch = [allTouches objectAtIndex:i];
        sInfo.State( static_cast<ETouch_State>( type ) );
        
        //Current Position
        touchLocation = [touch locationInView:nil];
        
        //Adjust for orientation
        switch(m_eOrientation)
        {
            case ESO_LANDSCAPE_RIGHT:
            {
                v2Pos.Set(touchLocation.y, touchLocation.x);
                break;
            }
            case ESO_PORTRAIT:
            {
                v2Pos.Set(touchLocation.x, touchLocation.y);
                break;
            }
            default:
            {
                assert_now("Guts, Unsuported resloution");
                break;
            }
        }
        
        
        sInfo.Position( v2Pos );
        
        sInfo.ID( touch );
        
        pTouchInput->AddTouchInfo( sInfo );
    }
}

@end
