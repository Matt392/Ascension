#include "ASCGenInc.h"
#include "ASCINIFile.h"

#include "ASCTree.h"
#include "ASCFileIO.h"
#include "ASCFinalShutdownDelay.h"

//#define CREATE_FILE

#define TEST_STRING
#define TEST_VECTOR
#define TEST_TREE
#define TEST_INI

int main()
{
	
#ifdef TEST_STRING
#ifdef CREATE_FILE
	ASCFileIO StringFile;
	ASCString strString = "Test";

	if(StringFile.Open("StringTest.ascinib", IO_OUT_BINARY))
	{
		StringFile.Clear();
		strString.Serialize(&StringFile);
		StringFile.Close();
	}
#else
	ASCFileIO StringFile;
	ASCString strString;
	if(StringFile.Open("StringTest.ascinib", IO_IN_BINARY))
	{
		void* pData = StringFile.ReadBinary();
		void* pDataOrig = pData;
		strString.LoadBinary(pData);
		SafeDelete( pDataOrig );
		StringFile.Close();
	}
	
	printf_s(strString.c_str());
	printf_s("\n");

	printf_s("Press Any Key To Continue...");
	getchar();
#endif
#endif

#ifdef TEST_VECTOR
#ifdef CREATE_FILE
	ASCFileIO VectorFile;
	ASCVector<ASCString> vecTest;
	vecTest.push_back("Vec Test");
	vecTest.push_back("Vec TestAgain");

	if(VectorFile.Open("VectorTest.ascinib", IO_OUT_BINARY))
	{
		VectorFile.Clear();
		vecTest.Serialize(&VectorFile);
		VectorFile.Close();
	}
#else
	ASCFileIO VectorFile;
	ASCVector<ASCString> vecTest;
	if(VectorFile.Open("VectorTest.ascinib", IO_IN_BINARY))
	{
		void* pData = VectorFile.ReadBinary();
		void* pDataOrig = pData;
		vecTest.LoadBinary(pData);
		SafeDelete( pDataOrig );
		VectorFile.Close();
	}
	
	printf_s(vecTest[0].c_str());
	printf_s("\n");
	printf_s(vecTest[1].c_str());
	printf_s("\n");

	printf_s("Press Any Key To Continue...");
	getchar();
#endif
#endif

#ifdef TEST_TREE
#ifdef CREATE_FILE
	ASCTree<ASCString> m_tTree;
	ASCFileIO File;
	m_tTree.Add("Test", "Val");
	m_tTree.Add("TestAgain", "Value");

	if(File.Open("TreeTest.ascinib", IO_OUT_BINARY))
	{
		File.Clear();
		m_tTree.Serialize(&File);
		File.Close();
	}
#else
	ASCTree<ASCString> m_tTree;
	ASCFileIO File;
	if(File.Open("TreeTest.ascinib", IO_IN_BINARY))
	{
		void* pData = File.ReadBinary();
		void* pDataOrig = pData;
		m_tTree.LoadBinary(pData);
		SafeDelete( pDataOrig );
		File.Close();
	}
	
	printf_s(m_tTree.At("Test").c_str());
	printf_s("\n");
	printf_s(m_tTree.At("TestAgain").c_str());
	printf_s("\n");

	printf_s("Press Any Key To Continue...");
	getchar();
#endif
#endif

	
#ifdef TEST_INI
	ASCINIFile INIFile;
	if(INIFile.Load("DBInterfaceAtlas.ascatl"))
	{
		printf_s("INI File loaded\n");
	}
	printf_s("Press Any Key To Continue...");
	getchar();
#endif

	ASCFinalShutdownDelay::SetCanEnd( true );
	return 0;
}