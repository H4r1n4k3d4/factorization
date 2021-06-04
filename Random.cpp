//////////////////////////////////////////////////////////////////////
// Random.cpp	: implementation of the CRandom class.
//
// Authors		: (a)pavelmalyshkin
// License		: ELEPHANT
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Random.h"

#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// class CInitKey
//////////////////////////////////////////////////////////////////////

CInitKey::CInitKey()
{
	srand((unsigned int)time(NULL));
}

CInitKey::~CInitKey()
{

}

//////////////////////////////////////////////////////////////////////
// class CRandom
//////////////////////////////////////////////////////////////////////

CInitKey CRandom::myInit;

double CRandom::GetDouble()
{
	double result=0.0;
	for(int i=0; i<4; ++i)	// RAND_MAX has 15 bits while double manissa has only 52; 8 extra
	{
		result/=RAND_MAX+1;
		result+=double(rand())/(i==0?RAND_MAX:RAND_MAX+1);
	}
	assert(result>=0.0);
	assert(result<=1.0);
	return result;
}

double CRandom::GetDouble(double _min, double _max)
{
	assert(_min<=_max);
	double result=GetDouble();
	result*=(_max-_min);
	result+=_min;
	assert(result>=_min);
	assert(result<=_max);
	return result;
}

int CRandom::GetInt(int _min, int _max)
{
	if(_min==_max)
		return int(GetDouble()*(__int64((unsigned int)(-1))+1));
	
	unsigned int difference=_max-_min;
	unsigned int random=(unsigned int)(GetDouble()*difference);
	if(random==difference)
		--random;

	return _min+random;
}

__int64 CRandom::GetLInt()
{
	return __int64(GetInt(0, 0))*(__int64((unsigned int)(-1))+1)+GetInt(0, 0);
}

bool CRandom::Continue(double _averageLength)
{
	assert(_averageLength>=0.0);
	double threshold=_averageLength/(_averageLength+1.0);
	double choice=CRandom::GetDouble();
	if(choice>=threshold)
		return false;
	else
		return true;
}

bool CRandom::GetBool(double _probability)
{
	assert(_probability>=0.0 && _probability<=1.0);
	if(_probability==0.0)
		return false;
	if(_probability==1.0)
		return true;

	double choice=CRandom::GetDouble();
	return choice<=_probability;
}