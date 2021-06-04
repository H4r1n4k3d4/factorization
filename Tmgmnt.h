//////////////////////////////////////////////////////////////////////
// Tmgmnt.h: interface for the CTmgmnt class.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMGMNT_H)
#define AFX_TMGMNT_H

#include <vector>
#include <ctime>

#define T_START(a) CTmgmnt::Start(a)
#define T_STOP(a) CTmgmnt::Stop(a)
#define T_PRINT CTmgmnt::Print()
#define T_SIZE CTmgmnt::Size()
#define T_GET(a) CTmgmnt::Get(a)
#define T_CLEAR CTmgmnt::Clear()

//////////////////////////////////////////////////////////////////////
// class CTmgmnt
//////////////////////////////////////////////////////////////////////
 
class CTmgmnt  
{
	static std::vector<__int64> myTimes;
	static std::vector<clock_t> myStarts;

public:
	static void Start(unsigned int);
	static void Stop(unsigned int);
	static __int64 Get(unsigned int);
	static unsigned int Size();
	static void Print();
	static void Clear();
};

#endif // !defined(AFX_TMGMNT_H)
