//
//  ASCRenderView.m
//  Ascension
//
//  Created by Matt Dalzell on 6/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef ASC_IOS

#import "ASCRenderView.h"
#include "Ascension.h"

@implementation ASCRenderView


- (id)initWithFrame:(CGRect)frame
{   
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        [self setupLayer];
        [self setupContext];
        [self setupRenderBuffer];
        [self setupFrameBuffer];
        [self clearScreen];
        [self present];
    }
    return self;
}


+ (Class)layerClass 
{
    return [CAEAGLLayer class];
}


- (void)setupLayer
{
    _eaglLayer = (CAEAGLLayer*) self.layer;
    _eaglLayer.opaque = YES;
    
    //For retina display.
    UIScreen* mainScreen = [UIScreen mainScreen];
    unsigned int uWidth = mainScreen.currentMode.size.width;
    unsigned int uHeight = mainScreen.currentMode.size.height;
    if ( 640 == mainScreen.currentMode.size.width && 960 == mainScreen.currentMode.size.height )
    {
        self.contentScaleFactor = 2.0f;
        _eaglLayer.contentsScale = 2;
        m_fWidth = mainScreen.currentMode.size.width;
        m_fHeight = mainScreen.currentMode.size.height;
        Ascension::Device(ED_IPHONE);
    }
    else if( 768 == mainScreen.currentMode.size.width && 1024 == mainScreen.currentMode.size.height )
    {
        self.contentScaleFactor = 2.0f;
        _eaglLayer.contentsScale = 2;
        m_fWidth = mainScreen.currentMode.size.width;
        m_fHeight = mainScreen.currentMode.size.height;
        Ascension::Device(ED_IPAD);
    }
    else if( 768 == uHeight && 1024 == uWidth )
    {
        self.contentScaleFactor = 2.0f;
        _eaglLayer.contentsScale = 2;
        m_fWidth = uWidth;
        m_fHeight = uHeight;
        Ascension::Device(ED_IPAD);
    }
    else
    {
        Ascension::Device(ED_IPHONE);
        m_fWidth = self.frame.size.width;
        m_fHeight = self.frame.size.height;
    }
}

- (void)setupContext
{
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    if ( !_context )
    {
        NSLog(@"Failed to initialise OpenGLES 2.0 context.");
    }
    
    if ( ![EAGLContext setCurrentContext:(_context)] )
    {
        NSLog(@"Failed to set current OpenGL context.");
        exit( 1 );
    }
}

- (void)setupRenderBuffer
{
    glGenRenderbuffers( 1, &_colorRenderBuffer );
    glBindRenderbuffer( GL_RENDERBUFFER, _colorRenderBuffer );
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
}

- (void)setupFrameBuffer
{
    GLuint frameBuffer;
    glGenFramebuffers( 1, &frameBuffer );
    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer );
}

- (void)clearScreen
{	
    glClear( GL_COLOR_BUFFER_BIT );
    glViewport(0.0f, 0.0f, m_fWidth, m_fHeight);
}

- (void)present
{
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)dealloc
{
    [_context release];
    _context = nil;
    [super dealloc];
}

- (EAGLContext*)getGLContext
{
	return _context;
}


@end

#endif