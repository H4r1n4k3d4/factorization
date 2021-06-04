//////////////////////////////////////////////////////////////////////
// Tmgmnt.cpp: implementation of the CTmgmnt class.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tmgmnt.h"

#include "intop.h"
#include <iostream>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// class CTmgmnt
//////////////////////////////////////////////////////////////////////

std::vector<__int64> CTmgmnt::myTimes;
std::vector<clock_t> CTmgmnt::myStarts;

void T_START(unsigned int _i)
{
	if(_i>=myTimes.size())
	{
		myTimes.resize(_i+1, 0);
		myStarts.resize(_i+1);
	}
	myStarts[_i]=clock();
}

void T_STOP(unsigned int _i)
{
	assert(_i<myTimes.size());
	assert(_i<myStarts.size());
	myTimes[_i]+=clock()-myStarts[_i];
}

__int64 T_GET(unsigned int _i)
{
	assert(_i<myTimes.size());
	return myTimes[_i];
}

unsigned int T_SIZE
{
	return myTimes.size();
}
 
void T_PRINT
{
	for(unsigned int i=0; i<myTimes.size(); ++i)
		std::cout << i << ' ' << myTimes[i] << std::endl;
}

void T_CLEAR
{
	myTimes.clear();
	myStarts.clear();
}

