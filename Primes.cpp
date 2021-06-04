//////////////////////////////////////////////////////////////////////
// Primes.cpp: implementation of the CPrimes class.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Primes.h"

#include <math.h>
#include "intstr.h" 

//////////////////////////////////////////////////////////////////////
// class CPrimes
//////////////////////////////////////////////////////////////////////

unsigned int CPrimes::RootTop(const unsigned int &_a)
{
	unsigned int root=pow(_a, 0.5);
	if(root*root==_a)
		return root;
	return ++root;
}

void CPrimes::Add(const unsigned int& _to)
{
	for(unsigned int i=myIndexedPrimes.back(); i<=_to; ++i)
	{
		bool factorized=false;
		for(unsigned int j=0; j<myIndexedPrimes.size(); ++j)
		{
			if(i%myIndexedPrimes[j]==0)
			{
				factorized=true;
				break;
			}
			if(myIndexedPrimes[j]>=RootTop(i))
				break;
		}
		if(!factorized)
		{
			myPrimes.insert(std::make_pair(i, myIndexedPrimes.size()));
			myIndexedPrimes.push_back(i);
		}
	}
}
 
const unsigned int& CPrimes::Next(const unsigned int& _from)
{
	std::multimap<unsigned int, unsigned int>::iterator it=myPrimes.upper_bound(_from);
	if(it!=myPrimes.end())
		return it->first;

	Add(_from);
	for(unsigned int i=_from+1; ; i+=1)
	{
		bool factorized=false;
		for(unsigned int j=0; j<myIndexedPrimes.size(); ++j)
		{
			if(i%myIndexedPrimes[j]==0)
			{
				factorized=true;
				break;
			}
			if(myIndexedPrimes[j]>=RootTop(i))
				break;
		}
		if(!factorized && i!=1)
		{
			myPrimes.insert(std::make_pair(i, myIndexedPrimes.size()));
			myIndexedPrimes.push_back(i);
			break;
		}
	}

	return myIndexedPrimes.back();
}

const unsigned int& CPrimes::Get(const unsigned int& _from, const unsigned int& _to)
{
	assert(_to>_from);
	Add(_to);

	std::multimap<unsigned int, unsigned int>::iterator from=myPrimes.lower_bound(_from);
	std::multimap<unsigned int, unsigned int>::iterator to=myPrimes.lower_bound(_to);
	assert(from!=myPrimes.end());
	if(to==myPrimes.end())
		--to;
	return myIndexedPrimes[rand()%(to->second-from->second+1)+from->second];
}