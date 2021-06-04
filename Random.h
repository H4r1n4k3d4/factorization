//////////////////////////////////////////////////////////////////////
// Random.h		: interface for the CRandom class.
//
// Authors		: (a)pavelmalyshkin
// License		: ELEPHANT
//
//////////////////////////////////////////////////////////////////////

#if !defined(RANDOM_H)
#define RANDOM_H 

//////////////////////////////////////////////////////////////////////
// class CInitKey
//////////////////////////////////////////////////////////////////////

class CInitKey
{
public:
	CInitKey();

	virtual ~CInitKey();
};

//////////////////////////////////////////////////////////////////////
// class CRandom
//////////////////////////////////////////////////////////////////////

class CRandom  
{
	static CInitKey myInit;

public:
	static double GetDouble();							// [0, 1]
	static double GetDouble(double, double);			// [min, max]
	static int GetInt(int, int);						// [min, max]
	static __int64 GetLInt();							// [min, max]
	static bool Continue(double);						// defines stop of a cycle to make the average sycle length equal to param
	static bool GetBool(double);						// returns true with defined probability
};

#endif // !defined(RANDOM_H)
