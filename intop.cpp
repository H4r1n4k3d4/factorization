//////////////////////////////////////////////////////////////////////
// intop.cpp	: implementation of the std::ostream& operator <<
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "intop.h"
#include <list>

std::ostream& operator << (std::ostream& _stream, const __int64& _rhs)
{
	std::list<char> str;
	for(__int64 rhs=_rhs; rhs>0; rhs/=10)
		str.push_front(rhs%10);
	for(std::list<char>::iterator it=str.begin(); it!=str.end(); ++it)
		_stream << char(48+*it);
	return _stream;
}

 