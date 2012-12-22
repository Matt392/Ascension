/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCRandNumGen.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCRANDNUMGEN_H__Included_1827053485
#define _ASCRANDNUMGEN_H__Included_1827053485


namespace ASC
{
	class ASCRandNumGen
	{
	public:
		static void Initialise(UINT32 uMaxRand);

		static void Seed(UINT32 uSeed);
		static void Seed();

		static UINT32 Rand(UINT32 uUpper);
		static SINT32 Rand(SINT32 uLower, SINT32 uUpper);

		static FLOAT32 Rand(FLOAT32 fUpper, FLOAT32 fDecimalSupport = 3.0f);
		static FLOAT32 Rand(FLOAT32 fLower, FLOAT32 fUpper, FLOAT32 fDecimalSupport = 3.0f);

	protected:
		static UINT32 Rand();

		static UINT32 sm_uMaxRand;
		static UINT32 sm_uSeed;
	};

}

#endif //_ASCRANDNUMGEN_H__Included_1827053485

