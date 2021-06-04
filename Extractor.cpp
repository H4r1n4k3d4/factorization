//////////////////////////////////////////////////////////////////////
// Extractor.cpp: implementation of the CExtractor class.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Extractor.h"
#include "intstr.h"

#include <math.h>

unsigned int GetPart(__int64 const& _a)
{
	return _a%100;
}

void Abs(__int64& _a)
{
	if(_a<0)
		_a=-_a;
}

unsigned int log2(__int64 const& _a)
{
	return log(_a)/log(2.0);
}

//////////////////////////////////////////////////////////////////////
// struct CExtractor
//////////////////////////////////////////////////////////////////////

void CExtractor<__int64>::EvenRoot(__int64& _a)
{
	for(bool is_square=true; is_square; )
	{
		__int64 sq_root=SqrtLow(_a, is_square);
		if(is_square)
			_a=sq_root;
	}
}

void CExtractor<intstr>::EvenRoot(intstr& _a)
{
	for(bool is_square=true; is_square; )
	{
		intstr sq_root=SqrtLow(_a, is_square);
		if(is_square)
			_a.swap(sq_root);
	}
}

unsigned int CExtractor<__int64>::Extract(__int64& _number, __int64 const & _multiplier)
{
	assert(_multiplier>0);
	assert(_number>0);

	unsigned int i=0;
	for( ; _number%_multiplier==0; _number=_number/_multiplier)
		++i;

	return i;
}

unsigned int CExtractor<intstr>::Extract(intstr& _number, intstr const & _multiplier)
{
	assert(_multiplier>0);
	assert(_number>0);

	unsigned int i=0;
	for( ; ; )
	{
		intstr result;
		intstr remainder;
		intstr::Divide(result, remainder, _number, _multiplier);

		if(remainder>0)
			break;

		_number.swap(result);
		++i;
	}

	return i;
}