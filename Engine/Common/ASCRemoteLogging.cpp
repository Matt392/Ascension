#include "ASCGenInc.h"
#include "ASCRemoteLogging.h"
#ifdef ASC_IOS
#import "TestFlight.h"
#endif

ASCRemoteLogging::ASCRemoteLogging(void)
{
}


ASCRemoteLogging::~ASCRemoteLogging(void)
{
}
#ifdef ASC_IOS
void 
ASCRemoteLogging::Begin(ASCString strID)
{
    NSString* strTeamToken = [[NSString alloc] initWithUTF8String: strID.c_str()];
    
    [TestFlight takeOff:strTeamToken];
    
    [strTeamToken release];
}

void 
ASCRemoteLogging::Log(ASCString strLine)
{
    NSString* strTeamToken = [[NSString alloc] initWithUTF8String: strLine.c_str()];
    
    [TestFlight passCheckpoint:strTeamToken];
    
    [strTeamToken release];
}
#endif
#ifdef ASC_WINDOWS
void 
ASCRemoteLogging::Begin(ASCString strID)
{
	strID;
}

void 
ASCRemoteLogging::Log(ASCString strLine)
{
	strLine;
}
#endif