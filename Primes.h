//////////////////////////////////////////////////////////////////////
// Primes.h: interface for the CPrimes class.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRIMES_H)
#define AFX_PRIMES_H

#include <map>
#include <vector>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// class CPrimes
//////////////////////////////////////////////////////////////////////

class CPrimes  
{
	std::multimap<unsigned int, unsigned int> myPrimes;		// простое и его индекс в векторе
	std::vector<unsigned int> myIndexedPrimes;				// вектор простых чисел - для рандомного выбора
	unsigned int RootTop(const unsigned int& _a);

public:
	CPrimes()
	{	
		myPrimes.insert(std::make_pair(2, 0));
		myIndexedPrimes.push_back(2);
	}

	void Add(const unsigned int&);
	const unsigned int& Next(const unsigned int&);
	const unsigned int& Get(const unsigned int&, const unsigned int&);

	unsigned int Size()
	{
		return myIndexedPrimes.size();
	}

	void Clear()	
	{	
		myPrimes.clear();
		myIndexedPrimes.clear();
	}
}; 

#endif // !defined(AFX_PRIMES_H)
