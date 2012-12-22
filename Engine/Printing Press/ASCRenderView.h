//
//  ASCRenderView.h
//  Ascension
//
//  Created by Matt Dalzell on 6/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#ifdef ASC_IOS

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@interface ASCRenderView : UIView {
    CAEAGLLayer*    _eaglLayer;
    EAGLContext*    _context;
    GLuint          _colorRenderBuffer;
    float m_fWidth;
    float m_fHeight;
}

+ (Class) layerClass;
- (void) setupLayer;
- (void) setupContext;
- (void) setupRenderBuffer;
- (void) setupFrameBuffer;
- (void)clearScreen;
- (void)present;
- (EAGLContext*)getGLContext;

@end

#endif