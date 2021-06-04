// Research.cpp: implementation of the CResearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Research.h"
#include "Primes.h"

#include <math.h>
#include <iostream>

//////////////////////////////////////////////////////////////////////
// class CResearch
//////////////////////////////////////////////////////////////////////

void CResearch::Run()
{
	CPrimes primes;
	std::vector<double> result;
	result.resize(3, 0.0);
	std::vector<unsigned int> counter;
	counter.resize(3, 0);
	std::vector<unsigned int> experience;
	experience.resize(3, 0);

	for(__int64 i=1000; i<=10000000000000000; i*=10)
	{
		unsigned int logI=pow(log(double(i)), 2);
		for(__int64 k=0; k<1000; ++k)
		{
			__int64 m=i+k;
			unsigned int logM=pow(log(double(m)), 2);
			__int64 tempM=m;
			for(unsigned int div=2; div<logM; div=primes.Next(div))
			{
				for( ; tempM%div==0; tempM/=div);
			}

			for(unsigned int j=0; j<result.size(); ++j)
			{
				if(tempM<pow(double(m), pow(0.5, j+1)))
				{
					result[j]*=experience[j];
					result[j]+=counter[j];
					result[j]/=++experience[j];
					counter[j]=0;
				}
				else
					counter[j]++;
			}
		}
 
		for(unsigned int j=0; j<result.size(); ++j)
			std::cout << pow(log(result[j]), 2)/logI << ' '; 
		std::cout << std::endl;
		result.clear();
		result.resize(3, 0.0);
		counter.clear();
		counter.resize(3, 0);
		experience.clear();
		experience.resize(3, 0);
	}
}