/**
*	@Danger Balls
 *	@file Ascension 2D\Engine\Common\RemoteLogging.h
 *	@brief 
 *	
 *********************************************************************************************/
#ifndef _REMOTELOGGING_H__Included_1521196770
#define _REMOTELOGGING_H__Included_1521196770


namespace ASC
{
	class ASCRemoteLogging
	{
	public:
		ASCRemoteLogging(void);
		~ASCRemoteLogging(void);

		void Begin(ASCString strID);
		void Log(ASCString strLine);
	};

}

#endif //_REMOTELOGGING_H__Included_1521196770

