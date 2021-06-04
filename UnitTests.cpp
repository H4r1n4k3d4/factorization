/////////////////////////////////////////////////////////////////////////
// UnitTests.cpp: implementation of the UnitTests() function.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
/////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Fourier.h"
#include "Complex.h"
#include "Factorization.h"
#include "Extractor.h"
#include "Primes.h"

#include "intstr.h"
#include <windows.h>
#include <conio.h>
#include <fstream>
 
void UnitTests()
{ 
	assert(gcd(9, 10)==1);
	assert(gcd(9, 7)==1);
	assert(gcd(9, 27)==9);
	assert(gcd(16, 4)==4);
	assert(gcd(45, 30)==15);
	assert(gcd(21, 35)==7);

	assert(intstr(0)==0);
	assert(intstr(1)==1);
	assert(intstr(-1)==-1);
	bool sqr;
	assert(intstr(0).sqrt_low(sqr)==0);
	assert(sqr);
	assert(intstr(1).sqrt_low(sqr)==1);
	assert(sqr);

	// --------------------------- Primes -------------------------------
	CPrimes primes;
	primes.Add(23);
	assert(primes.Size()==9);
	assert(primes.Next(0)==2);
	assert(primes.Next(2)==3);
	assert(primes.Next(3)==5);
	assert(primes.Next(5)==7);
	assert(primes.Next(7)==11);
	assert(primes.Next(11)==13);
	assert(primes.Next(13)==17);
	assert(primes.Next(17)==19);
	assert(primes.Next(19)==23);
	assert(primes.Next(23)==29);

	//---------------------------- Multilication using FFT --------------
	{
		std::vector<double> multiplier1;
		multiplier1.resize(4, 0);
		multiplier1[0]=1;				// 11
		multiplier1[1]=1;
		std::vector<CComplex> fourier1;
		CFourier::Forward(multiplier1, fourier1);

		std::vector<double> multiplier2;
		multiplier2.resize(4, 0);		
		multiplier2[0]=5;				// 45
		multiplier2[1]=4;
		std::vector<CComplex> fourier2;
		CFourier::Forward(multiplier2, fourier2);

		std::vector<CComplex> fourierR;
		fourierR.resize(4);
		for(int i=0; i<fourier1.size(); ++i)
			fourierR[i]=fourier1[i]*fourier2[i];
		std::vector<CComplex> result;
		CFourier::Inverse(fourierR, result);
		assert(Int(result)==495);
	}

	//----------------------------- Basic operations on intstr -------------
	{
		for(int i=-20; i<20; ++i)
			for(int j=-20; j<20; ++j)
			{
				assert((intstr(i)+j)==i+j);
				assert((intstr(i)-j)==i-j);
				assert(!((intstr(i)>j) ^ (i>j)));
				assert(!((intstr(i)<j) ^ (i<j)));
				assert(!((intstr(i)>=j) ^ (i>=j)));
				assert(!((intstr(i)<=j) ^ (i<=j)));
				assert(!((intstr(i)==j) ^ (i==j)));
			}
	}

	{
		for(int i=10; i<2000; i+=rand()%20)
		{
			for(int j=1; j<20; ++j)
			{
				assert((intstr(i)/j).Int()==i/j);
				assert((intstr(i)%j).Int()==i%j);
				assert((intstr(i)*j)==i*j);
			}
			bool sqr;
			assert(intstr(i*i).sqrt_low(sqr)==i);
			assert(sqr);
		}
	}

	intstr::SetMaxNum(100);
	assert((intstr(10109)%1011)==10109%1011);
	assert((intstr(10109)/1011)==10109/1011);
	assert((intstr(10119)%1011)==10119%1011);
	assert((intstr(10119)/1011)==10119/1011);
	assert((intstr(10110)%1011)==10110%1011);
	assert((intstr(10110)/1011)==10110/1011);
	assert((intstr(101101)%1011)==101101%1011);
	assert((intstr(101101)/1011)==101101/1011);
	assert((intstr(10111011)%1011)==10111011%1011);
	assert((intstr(10111011)/1011)==10111011/1011);
	intstr::ResetMaxNum();

	//--------------------------------- intstr I/O ------------------------
	intstr odata=__int64(92876523823);
	std::ofstream os;
	os.open ("output.fct", std::fstream::out | std::fstream::trunc);
	os << odata << std::endl;
	os.close();
	intstr idata;
	std::ifstream is("output.fct");
	is >> idata;
	is.close();
	assert(idata==odata);

	//--------------------------------- building factor base ---------------
/*	{
		for(unsigned int j=0; j<1000; ++j)
		{
			__int64 number=1;
			CExtractor<__int64>::TIntSet even_powers;
			CExtractor<__int64>::TIntSet odd_powers;
			{
				__int64 prime=primes.Next(0);
				for(unsigned int i=0; i<5; ++i)
				{
					unsigned int power=rand()%5;
					if(power>0)
					{
						if(power%2==1)
							odd_powers.insert(prime);
						else
							even_powers.insert(prime);

						number*=pow(prime, power);
					}
					prime=primes.Next(prime);
				}
			}

			if(odd_powers.empty() && odd_powers.empty())
				continue;

			__int64 numCopy=number;
			CExtractor<__int64> factorBase(1);
			CExtractor<__int64>::TResolution resolution;
			CExtractor<__int64>::TResolutionMap resolutionMap;
			factorBase.ExtractAll(number, resolution, resolutionMap);
			assert(number==1);
			assert((resolution.size()==2) || (even_powers.find(2)==even_powers.end() && odd_powers.find(2)==odd_powers.end()) || (resolution.size()==1 && resolution.find(2)!=resolution.end()));
			assert((resolution.size()==1) || (even_powers.find(2)!=even_powers.end() || odd_powers.find(2)!=odd_powers.end()));

			__int64 prime=primes.Next(0);
			for(unsigned int i=0; i<5; ++i)
			{
				unsigned int po=0;
				if(even_powers.find(prime)!=even_powers.end() && odd_powers.find(prime)==odd_powers.end())
					po=3;
				if(even_powers.find(prime)==even_powers.end() && odd_powers.find(prime)!=odd_powers.end())
					po=2;
				number*=pow(prime, po);

				prime=primes.Next(prime);
			}

			resolution.clear();
			factorBase.ExtractAll(number, resolution, resolutionMap);

			assert(number==1);
			assert(resolutionMap.size()==0 || resolutionMap.size()==1);
			if(resolutionMap.size()>0)
			{
				CExtractor<__int64>::TResolutionMap::iterator itm=resolutionMap.begin();
				assert(numCopy%itm->first==0);
				for(CExtractor<__int64>::TResolution::iterator itr=itm->second.begin(); itr!=itm->second.end(); ++itr)
				{
					assert(itm->first%__int64(pow(itr->first, itr->second))==0);
					__int64 res=itm->first/pow(itr->first, itr->second);
					assert(res%itr->first!=0);
				}
			}

			for(CExtractor<__int64>::TResolution::iterator it=resolution.begin(); it!=resolution.end(); ++it)
			{
				assert(it->second==2 || odd_powers.find(it->first)==odd_powers.end());
				assert(it->second==3 || even_powers.find(it->first)==even_powers.end());
			}
		}
	}

	{
		for(unsigned int j=0; j<1000; ++j)
		{
			__int64 number=1;
			CExtractor<__int64>::TIntSet even_powers;
			CExtractor<__int64>::TIntSet odd_powers;
			{
				__int64 prime=primes.Next(0);
				for(unsigned int i=0; i<5; ++i)
				{
					unsigned int power=rand()%5;
					if(power>0)
					{
						if(power%2==1)
							odd_powers.insert(prime);
						else
							even_powers.insert(prime);

						number*=pow(prime, power);
					}
					prime=primes.Next(prime);
				}
				number*=13;
			}

			__int64 numCopy=number;
			CExtractor<__int64> factorBase(1);
			CExtractor<__int64>::TResolution resolution;
			CExtractor<__int64>::TResolutionMap resolutionMap;
			factorBase.ExtractAll(number, resolution, resolutionMap);
			assert(number==1);
			assert((resolution.size()==2) || (even_powers.find(2)==even_powers.end() && odd_powers.find(2)==odd_powers.end()) || (resolution.size()==1 && resolution.find(2)!=resolution.end()));
			assert((resolution.size()==1) || (even_powers.find(2)!=even_powers.end() || odd_powers.find(2)!=odd_powers.end()));

			__int64 prime=primes.Next(0);
			for(unsigned int i=0; i<5; ++i)
			{
				unsigned int po=0;
				if(even_powers.find(prime)!=even_powers.end() && odd_powers.find(prime)==odd_powers.end())
					po=3;
				if(even_powers.find(prime)==even_powers.end() && odd_powers.find(prime)!=odd_powers.end())
					po=2;
				number*=pow(prime, po);

				prime=primes.Next(prime);
			}

			resolution.clear();
			factorBase.ExtractAll(number, resolution, resolutionMap);

			assert(number==1);
			assert(resolutionMap.size()==0 || resolutionMap.size()==1);
			if(resolutionMap.size()>0)
			{
				CExtractor<__int64>::TResolutionMap::iterator itm=resolutionMap.begin();
				assert(numCopy%itm->first==0);
				for(CExtractor<__int64>::TResolution::iterator itr=itm->second.begin(); itr!=itm->second.end(); ++itr)
				{
					assert(itm->first%__int64(pow(itr->first, itr->second))==0);
					__int64 res=itm->first/pow(itr->first, itr->second);
					assert(res%itr->first!=0);
				}
			}

			for(CExtractor<__int64>::TResolution::iterator it=resolution.begin(); it!=resolution.end(); ++it)
			{
				assert(it->second==2 || odd_powers.find(it->first)==odd_powers.end() );
				assert(it->second==3 || even_powers.find(it->first)==even_powers.end());
			}
		}
	}

	{
		CExtractor<__int64> factorBase(2);
		CExtractor<__int64>::TResolution resolution;
		CExtractor<__int64>::TResolutionMap resolutionMap;
		__int64 num=2*2*3*5*7;
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==3);
		assert(resolution[2]==2);
		assert(resolution[3]==1);
		assert(resolution[35]==1);

		num=2*25;
		resolution.clear();
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==2);
		assert(resolution[2]==1);
		assert(resolution[5]==2);
		assert(resolutionMap.size()==1);
		assert(resolutionMap[35].size()==2);
		assert(resolutionMap[35][5]==1);
		assert(resolutionMap[35][7]==1);
	}

	{
		CExtractor<__int64> factorBase(2);
		CExtractor<__int64>::TResolution resolution;
		CExtractor<__int64>::TResolutionMap resolutionMap;
		__int64 num=2*2*3*5*49;
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==3);
		assert(resolution[2]==2);
		assert(resolution[3]==1);
		assert(resolution[245]==1);

		num=2*25;
		resolution.clear();
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==2);
		assert(resolution[2]==1);
		assert(resolution[5]==2);
		assert(resolutionMap.size()==1);
		assert(resolutionMap[245].size()==2);
		assert(resolutionMap[245][5]==1);
		assert(resolutionMap[245][7]==2);
	}

	{
		CExtractor<__int64> factorBase(1);
		CExtractor<__int64>::TResolution resolution;
		CExtractor<__int64>::TResolutionMap resolutionMap;
		__int64 num=2*15*49;
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==2);
		assert(resolution[2]==1);
		assert(resolution[735]==1);

		num=15;
		resolution.clear();
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==1);
		assert(resolution[15]==1);
		assert(resolutionMap.size()==1);
		assert(resolutionMap[735].size()==2);
		assert(resolutionMap[735][15]==1);
		assert(resolutionMap[735][7]==2);

		num=35;
		resolution.clear();
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==2);
		assert(resolution[5]==1);
		assert(resolution[7]==1);
		assert(resolutionMap.size()==1);
		assert(resolutionMap[15].size()==2);
		assert(resolutionMap[15][5]==1);
		assert(resolutionMap[15][3]==1);
	}

	{
		CExtractor<__int64> factorBase(2);
		CExtractor<__int64>::TResolution resolution;
		CExtractor<__int64>::TResolutionMap resolutionMap;
		__int64 num=2*3*169*25;
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==3);
		assert(resolution[2]==1);
		assert(resolution[3]==1);
		assert(resolution[13*5]==2);

		num=15;
		resolution.clear();
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==2);
		assert(resolution[3]==1);
		assert(resolution[5]==1);
		assert(resolutionMap.size()==1);
		assert(resolutionMap[65].size()==2);
		assert(resolutionMap[65][13]==1);
		assert(resolutionMap[65][5]==1);

		num=5*121;
		resolution.clear();
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==2);
		assert(resolution[11]==2);
		assert(resolution[5]==1);
		assert(resolutionMap.size()==0);
	}

	{
		CExtractor<__int64> factorBase(1);
		CExtractor<__int64>::TResolution resolution;
		CExtractor<__int64>::TResolutionMap resolutionMap;
		__int64 num=2*169*13*25;
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==2);
		assert(resolution[2]==1);
		assert(resolution[169*13*25]==1);

		num=26;
		resolution.clear();
		factorBase.ExtractAll(num, resolution, resolutionMap);
		assert(num==1);
		assert(resolution.size()==2);
		assert(resolution[2]==1);
		assert(resolution[13]==1);
		assert(resolutionMap.size()==1);
		assert(resolutionMap[169*13*25].size()==2);
		assert(resolutionMap[169*13*25][13]==3);
		assert(resolutionMap[169*13*25][5]==2);
	}
*/
	//--------------------------------- other ------------------------------
	__int64 a=23457898;
	bool square_root;
	assert(CExtractor<__int64>::SqrtLow(a*a, square_root)==a);
	assert(square_root);
}