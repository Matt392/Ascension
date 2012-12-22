/**
*	@Danger Balls
 *	@file Ascension 2D\Engine\Input\ASCTouchInput.h
 *	@brief 
 *	
 *********************************************************************************************/
#ifndef _ASCTOUCHINPUT_H__Included_936443753
#define _ASCTOUCHINPUT_H__Included_936443753

#ifdef ASC_IOS

#include "ASCInputDevice.h"

namespace ASC
{
    enum ETouch_State
    {
        ETS_STARTED,
        ETS_MOVED,
        ETS_ENDED,
        ETS_COUNT,
    };
    
    class ASCTouchInfo
    {
    public:
        ASCTouchInfo(void);
        ~ASCTouchInfo(void);
        ETouch_State State();
        void State(ETouch_State eState);
        
        ASCVector2& Position();
        void Position(ASCVector2 v2Pos);
        
        ASCVector2& PreviousPosition();
        void PreviousPosition(ASCVector2 v2Pos);
        
        void* ID();
        void ID(void* pID);
        
    protected:
        ETouch_State m_eState;
        ASCVector2 m_v2Pos;
        ASCVector2 m_v2PrevPos;
        
        void* m_pID;
    };
    
	class ASCTouchInput : public ASCInputDevice
	{
	public:
		ASCTouchInput(void);
		~ASCTouchInput(void);
        
        virtual void Update( FLOAT32 fTimeDelta );
        
        virtual bool RegisterInput( UINT32& uEventID, ASCString strID );
        
        virtual void SetFullScreen(bool bVal);
        
        void AddTouchInfo(ASCTouchInfo sInfo);
    
    protected:
        ASCVector<ASCTouchInfo> m_vecTouchInfo;
        
        bool m_bObserved[m_kuTouchInputs];
        UINT32 m_uEventIds[m_kuTouchInputs];
        SInputData m_sTouchData[m_kuTouchInputs];
        
        SInputData* m_pTouches[ASC_MAX_TOUCH_INPUT];
        void* m_pTouchIDs[ASC_MAX_TOUCH_INPUT];
	};

}

#endif

#endif //_ASCTOUCHINPUT_H__Included_936443753