//////////////////////////////////////////////////////////////////////
// Extractor.h: interface for the CExtractor class.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#if !defined(EXTRACTOR_H)
#define EXTRACTOR_H

#include "Primes.h"

#include "intop.h"
#include "Tmgmnt.h"
#include <set>
#include <assert.h>

unsigned int GetPart(__int64 const &);
void Abs(__int64&);
unsigned int log2(__int64 const &);

//////////////////////////////////////////////////////////////////////
//	struct CExtractor - finding common divisors of a number set
//////////////////////////////////////////////////////////////////////

template<typename T>
struct CExtractor  
{	
	typedef std::set<T> TIntSet;
	typedef std::map<T, unsigned int> TResolution;
	typedef std::map<T, TResolution> TResolutionMap;

	CPrimes myPrimes;
	T myDensePrimeLimit;
	T myDensePrimesMargin;
	T mySparsePrimesMargin;

	TIntSet mySparsePrimes;
	TIntSet myOtherFactors;

	CExtractor(const T&);

	static void EvenRoot(T&);
	static T GCD(T, T, T const &);
	static T SqrtLow(T, bool &);
	static unsigned int Extract(T&, T const &);
	static T Power(T const&, unsigned int);

	void FindFactorsByGCD(T const&, T const&, TIntSet&);
	void ExtractAll(T&, TResolution&, TResolutionMap&);
	bool Check(T const &, unsigned int);
};

template<typename T>
CExtractor<T>::CExtractor(const T& _sqrt)
{
	unsigned int limit=pow(2*log2(_sqrt), 1);
	myDensePrimeLimit=limit;
	unsigned int prime=1;
	for(unsigned int i=0; i<limit; ++i)
		prime=myPrimes.Next(prime);
	myDensePrimesMargin=limit>0?prime+1:0; 
	mySparsePrimesMargin=prime*prime+1;
}

template<typename T>
T CExtractor<T>::SqrtLow(T _a, bool &_accuracy)
{
	assert(_a>=0);

	std::vector<unsigned int> data;
	for( ; ; )
	{
		data.push_back(GetPart(_a));
		_a/=100;
		if(_a==0)
			break;
	}

	__int64 result=0;
	__int64 remainder=0;
	for( ; data.size()>0; data.pop_back())
	{
		__int64 tmp=remainder*100+data.back();
		result*=10;
		int i=9;
		for( ; i>=0; i--)
		{
			remainder=tmp-(2*result*i+i*i);
			if(remainder>=0)
				break;
		}
		result+=i;
	}

	_accuracy=(remainder==0);		
	return result;
}

template<typename T>
T CExtractor<T>::GCD(T _a, T _b, T const & _densePrimesMargin)
{
	assert(_a>0);
	if(_b<0)
		Abs(_b);

	if(_densePrimesMargin>0)
	{
		assert(_a>=_densePrimesMargin);
		assert(_b>=_densePrimesMargin);
	}

	T& larger=_a>=_b?_a:_b;
	T& smaller=_a>=_b?_b:_a;
	T remainder=larger%smaller;

	if(_densePrimesMargin>0)
		return remainder==0?smaller:(remainder<_densePrimesMargin?1:GCD(remainder, smaller, _densePrimesMargin));
	else
		return remainder==0?smaller:(remainder==1?1:GCD(remainder, smaller, 0));
}

template<typename T>
T CExtractor<T>::Power(T const & _a, unsigned int _power)
{
	if(_a==0)
		return 0;

	T result=1;
	for(unsigned int i=0; i<_power; ++i)
		result*=_a;
	return result;
}

template<typename T>
void CExtractor<T>::FindFactorsByGCD(T const& _a, T const& _b, TIntSet& _newFactors)
{
	T gcd=GCD(_a, _b, myDensePrimesMargin);
	if(gcd>1)
	{
		T a=_a;
		T b=_b;
		EvenRoot(gcd);
		Extract(a, gcd);
		Extract(b, gcd);
		
		if(gcd<mySparsePrimesMargin)
		{
			_newFactors.insert(gcd);
			if(a>1)
			{
				if(a>=mySparsePrimesMargin)
					EvenRoot(a);
				assert(a>=myDensePrimesMargin);
				_newFactors.insert(a);
			}
			if(b>1)
			{
				if(b>=mySparsePrimesMargin)
					EvenRoot(b);
				assert(b>=myDensePrimesMargin);
				_newFactors.insert(b);
			}
		}
		else if(a>1 || b>1)
		{
			if(a>1)
			{
				TIntSet newFactors;									// разложение для пары a и gcd
				FindFactorsByGCD(a, gcd, newFactors);
				if(newFactors.empty())
					_newFactors.insert(gcd);

				for(TIntSet::iterator itnr=newFactors.begin(); itnr!=newFactors.end(); ++itnr)
				{
					assert(*itnr>1);
					if(a>1)
						Extract(a, *itnr);
					_newFactors.insert(*itnr);
				}

				if(a>1)
				{
					if(a>=mySparsePrimesMargin)
						EvenRoot(a);
					assert(a>=myDensePrimesMargin);
					_newFactors.insert(a);
				}
			}

			if(b>1)
			{
				TIntSet newFactors;									// разложение для пары a и gcd
				FindFactorsByGCD(b, gcd, newFactors);
				if(newFactors.empty())
					_newFactors.insert(gcd);

				for(TIntSet::iterator itnr=newFactors.begin(); itnr!=newFactors.end(); ++itnr)
				{
					assert(*itnr>1);
					if(b>1)
						Extract(b, *itnr);
					_newFactors.insert(*itnr);
				}

				if(b>1)
				{
					if(b>=mySparsePrimesMargin)
						EvenRoot(b);
					assert(b>=myDensePrimesMargin);
					_newFactors.insert(b);
				}
			}
		}
		else
			_newFactors.insert(gcd);
	}
}

template<typename T>
void CExtractor<T>::ExtractAll(T& _a, TResolution& _resolution, TResolutionMap& _newFactors)
{
	_newFactors.clear();
	unsigned int prime=2;
	for(T i=0; i<myDensePrimeLimit; ++i)
	{
		unsigned int po=Extract(_a, prime);
		if(po>0)
			_resolution[prime]=po;
		if(_a<prime*prime)
		{
			if(_a<myDensePrimesMargin)
			{
				if(_a>1)
				{
					_resolution[_a]=1;
					_a=1;
				}
				return;
			}

			break;
		}
		prime=myPrimes.Next(prime);
	}

	if(_a<mySparsePrimesMargin)
	{
		mySparsePrimes.insert(_a);
		_resolution[_a]=1;
	}

	TIntSet allFactors;
	if(_a>=mySparsePrimesMargin)
	{
		for(TIntSet::iterator it=mySparsePrimes.begin(); it!=mySparsePrimes.end(); ++it)
		{
			unsigned int po=Extract(_a, *it);
			if(po>0)
				_resolution[*it]=po;

			if(_a<mySparsePrimesMargin)
			{
				if(_a>1)
				{
					mySparsePrimes.insert(_a);
					allFactors.insert(_a);
				}
				break;
			}
		}
	}

	if(_a>1)
	{
		assert(_a>=myDensePrimesMargin);
		for(TIntSet::iterator ito=myOtherFactors.begin(); ito!=myOtherFactors.end(); ++ito)
		{
			TIntSet newFactors;
			FindFactorsByGCD(_a, *ito, newFactors);
			if(!newFactors.empty())
				_newFactors[*ito];						// набор чисел, для которых будут представлены новые разложения
			for(TIntSet::iterator itn=newFactors.begin(); itn!=newFactors.end(); ++itn)
				allFactors.insert(*itn);				// все найденные факторы
		}
	}
			
	for(TResolutionMap::iterator itrm=_newFactors.begin(); itrm!=_newFactors.end(); )
	{
		TResolution& resolution=itrm->second;			// здесь разложения для всех чисел, записанных в _newFactors
		assert(resolution.empty());
		T b=itrm->first;
		for(TIntSet::iterator ita=allFactors.begin(); ita!=allFactors.end(); ++ita)
		{
			if(b==*ita)
				continue;

			unsigned int po=Extract(b, *ita);
			if(po>0)
			{
				resolution[*ita]=po;
				if(*ita<mySparsePrimesMargin)
					mySparsePrimes.insert(*ita);
				else
					myOtherFactors.insert(*ita);
			}

			if(b<mySparsePrimesMargin)
			{
				if(b>1)
				{
					assert(b>=myDensePrimesMargin);
					assert(allFactors.find(b)!=allFactors.end());

					resolution[b]=1;
					mySparsePrimes.insert(b);
				}

				break;
			}
		}

		if(resolution.size()==0)
		{
			TResolutionMap::iterator itrmn=itrm;
			++itrmn;
			_newFactors.erase(itrm);
			itrm=itrmn;
			continue;
		}

		if(b>=mySparsePrimesMargin)
		{
			resolution[b]=1;
			myOtherFactors.insert(b);
		}

		myOtherFactors.erase(itrm->first);
		++itrm;
	}

	for(TIntSet::iterator ita=allFactors.begin(); ita!=allFactors.end(); ++ita)
	{
		unsigned int po=Extract(_a, *ita);
		if(po>0)
		{
			_resolution[*ita]=po;
			if(*ita<mySparsePrimesMargin)
				mySparsePrimes.insert(*ita);
			else
				myOtherFactors.insert(*ita);
		}

		if(_a<mySparsePrimesMargin)
		{
			if(_a>1)
			{
				assert(_a>=myDensePrimesMargin);

				_resolution[_a]=1;
				mySparsePrimes.insert(_a);
				_a=1;
			}

			return;
		}
	}

	if(_a>1)
	{
		T a=_a; 
		EvenRoot(a);
		_resolution[a]=Extract(_a, a);
		if(a<mySparsePrimesMargin)
			mySparsePrimes.insert(a);
		else
			myOtherFactors.insert(a);
	}
}

template <typename T>
bool CExtractor<T>::Check(T const& _n, unsigned int _s)
{
	T gcd=GCD(_n, _s, 0);
	if(gcd==1)
		return true;

	unsigned int prime=2;
	for(T i=0; i<myDensePrimeLimit; ++i)
	{
		unsigned int po=Extract(gcd, prime);
		if(po>=2)
			return false;
		if(gcd<prime*prime)
			return true;
		prime=myPrimes.Next(prime);
	}

	return true;
}

#endif // !defined(EXTRACTOR_H)
