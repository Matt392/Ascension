//
//  CViewController.h
//  Engine Test
//
//  Created by Matt Dalzell on 9/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

enum EScreenOrentation
{
    ESO_LANDSCAPE_RIGHT,
    ESO_LANDSCAPE_LEFT,
    ESO_PORTRAIT,
    ESO_UPSIDEDOWN,
    ESO_COUNT,
};

@interface CViewController : UIViewController
{
    EScreenOrentation m_eOrientation;
}

- (void)Initialise;

-(void)ProcessTouchEvents:(NSSet *)touches ofType:(unsigned int)type;

@end
