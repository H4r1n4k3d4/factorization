//////////////////////////////////////////////////////////////////////
// Factorization.cpp: implementation of the factorization functions
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Factorization.h"
#include "Primes.h"
#include "intstr.h"

#include <set>
#include <math.h>
#include <assert.h>

bool operator == (__int64 const & _lhs, std::pair<__int64, __int64> const & _rhs)
{
	if(_lhs==_rhs.first || _lhs==_rhs.second)
		return true;
	return false;
}

std::pair<__int64, __int64> Resolve(CFactorNode<__int64> const & _node, __int64 const & _task)
{
	intstr u=1;						// может быть _node==NULL - тогда _qres должен быть квадратичным вычетом по модулю N
	intstr v=1;
	std::set<__int64> nonSquares;

	std::set<__int64> const & origins=_node.myOrigins;
	for(std::set<__int64>::const_iterator it=origins.begin(); it!=origins.end(); ++it)
	{
		u*=(*it);
		u%=_task;
	}

	std::map<__int64, unsigned int> const & resolution=_node.myResolution;
	for(std::map<__int64, unsigned int>::const_iterator itr=resolution.begin(); itr!=resolution.end(); ++itr)
	{
		assert(itr->second%2==0);
		const intstr& mult=itr->first;
		unsigned int const & pow=itr->second;
		intstr multiplier=CExtractor<intstr>::Power(mult, pow/2);
		v*=multiplier;		
		v%=_task;
	}

	if((u+v)%_task==0 || (u-v)%_task==0)
		return std::make_pair(0, 1);								// тривиальное разложение

	intstr a=CExtractor<intstr>::GCD(_task, u+v, 0);				// _task>0, u+v!=0
	intstr b=CExtractor<intstr>::GCD(_task, u-v, 0);				// _task>0, u-v!=0
	assert(a!=1 && b!=1);

	return a>b ? std::make_pair(b.Int(), a.Int()) : std::make_pair(a.Int(), b.Int());
}
 