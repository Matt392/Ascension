/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCCamera.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCCAMERA_H__Included_1644930650
#define _ASCCAMERA_H__Included_1644930650


namespace ASC
{
	class ASCCamera
	{
	public:
		ASCCamera(void);
		~ASCCamera(void);

		ASCVector2 GetPos();
		void SetPos(ASCVector2 v2Pos);

	protected:
		ASCVector2 m_v2Position;
	};

}

#endif //_ASCCAMERA_H__Included_1644930650

