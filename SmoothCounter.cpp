///////////////////////////////////////////////////////////////////////////
// SmoothCounter.cpp : Defines the entry point for the console application.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "intstr.h"
#include "UnitTests.h"
#include "Factorization.h"
#include "Research.h"
#include "Primes.h"

#include "Tmgmnt.h"
#include <utility>
#include <iostream>
#include <assert.h>
#include <conio.h>
#include <math.h>
#include <time.h>
 
int main(int argc, char* argv[])
{
	srand(time(NULL));
	UnitTests();
//	CResearch::Run();

	unsigned int count;
	CFactorization<__int64> factorization;
	assert(factorization.Factorize(6, count)==std::make_pair(__int64(2), __int64(3)));								std::cout << count << std::endl;
	assert(factorization.Factorize(__int64(3077597)*79693, count)==std::make_pair(__int64(79693), __int64(3077597)));		std::cout << count << std::endl;
//	assert(factorization.Factorize(__int64(217121)*14730733, count)==std::make_pair(__int64(217121), __int64(14730733)));	std::cout << count << std::endl;
//	assert(factorization.Factorize(__int64(142453)*13105979, count)==std::make_pair(__int64(142453), __int64(13105979)));	std::cout << count << std::endl;
	assert(factorization.Factorize(6637*9439, count)==std::make_pair(__int64(6637), __int64(9439)));				std::cout << count << std::endl;
	assert(factorization.Factorize(__int64(18077)*9587, count)==std::make_pair(__int64(9587), __int64(18077)));		std::cout << count << std::endl;
	assert(factorization.Factorize(__int64(18149)*10369, count)==std::make_pair(__int64(10369), __int64(18149)));	std::cout << count << std::endl;
	assert(factorization.Factorize(__int64(12907)*27077, count)==std::make_pair(__int64(12907), __int64(27077)));	std::cout << count << std::endl;
	assert(factorization.Factorize(281*631, count)==std::make_pair(__int64(281), __int64(631)));					std::cout << count << std::endl;
	assert(factorization.Factorize(499*1283, count)==std::make_pair(__int64(499), __int64(1283)));					std::cout << count << std::endl;
	assert(factorization.Factorize(25021, count)==std::make_pair(__int64(131), __int64(191)));						std::cout << count << std::endl;
	assert(factorization.Factorize(102757, count)==std::make_pair(__int64(211), __int64(487)));						std::cout << count << std::endl;
	assert(factorization.Factorize(98303, count)==std::make_pair(__int64(197), __int64(499)));						std::cout << count << std::endl;
	assert(factorization.Factorize(194111, count)==std::make_pair(__int64(389), __int64(499)));						std::cout << count << std::endl;
	assert(factorization.Factorize(161197, count)==std::make_pair(__int64(331), __int64(487)));						std::cout << count << std::endl;
	assert(factorization.Factorize(12863, count)==std::make_pair(__int64(19), __int64(677)));						std::cout << count << std::endl;
	assert(factorization.Factorize(1513, count)==std::make_pair(__int64(17), __int64(89)));							std::cout << count << std::endl;
	assert(factorization.Factorize(1943, count)==std::make_pair(__int64(29), __int64(67)));							std::cout << count << std::endl;
	assert(factorization.Factorize(4087, count)==std::make_pair(__int64(61), __int64(67)));							std::cout << count << std::endl;
	assert(factorization.Factorize(10573, count)==std::make_pair(__int64(97), __int64(109)));						std::cout << count << std::endl;
	assert(factorization.Factorize(6557, count)==std::make_pair(__int64(79), __int64(83)));							std::cout << count << std::endl;
	assert(factorization.Factorize(2911, count)==std::make_pair(__int64(41), __int64(71)));							std::cout << count << std::endl;
	assert(factorization.Factorize(979, count)==std::make_pair(__int64(11), __int64(89)));							std::cout << count << std::endl;
	assert(factorization.Factorize(4891, count)==std::make_pair(__int64(67), __int64(73)));							std::cout << count << std::endl;
	assert(factorization.Factorize(__int64(66071)*99991, count)==std::make_pair(__int64(66071), __int64(99991)));	std::cout << count << std::endl;
	assert(factorization.Factorize(__int64(99991)*48649, count)==std::make_pair(__int64(48649), __int64(99991)));	std::cout << count << std::endl;

	unsigned int j=0;
	std::vector<unsigned int> qty;
	std::vector<double> times;
	CPrimes primes;
	unsigned int first=primes.Next(2);
	for(; j<10000; j++)
	{
		for(unsigned int i=0; i<pow(log2(j), 2); ++i)
			first=primes.Next(first);
		unsigned int second=primes.Get(pow(first, 0.7), pow(first, 0.8)+1);
		if(first==second)
			second=primes.Next(second);
		__int64 task=(__int64)first*second;
		
		std::cout << "task: " << first << ' ' << second << ' ';
		 
		unsigned int log_task=log(task)/log(2.0);

		T_START(log_task);
		std::pair<__int64, __int64> factors=factorization.Factorize(task, count);
		T_STOP(log_task);

		if(times.size()<=log_task)
		{
			times.resize(log_task+1, 0);
			qty.resize(log_task+1, 0);
		}

//		times[log_task]*=double(qty[log_task])/(qty[log_task]+1);		// среднее число итераций
		++qty[log_task];
//		times[log_task]+=double(count)/qty[log_task];					

		if(count>times[log_task])
			times[log_task]=count;										// максимальное число разложенний квадратичных вычетов

		if(factors.first==1)
			break;
		assert(factors.first==first || factors.second==first);
		assert(factors.first==second || factors.second==second);

		std::cout << "time: " << (unsigned int) ((double)log(T_GET(log_task)/qty[log_task])/log(2.0)) << '(' << log_task << ") [" << count << "]\n";
		for(i=0; i<times.size(); ++i)
		{
			if(qty[i]==0)
				continue;
			double complexity=exp(1.0*pow(i, 0.4)*pow(log(i), 0.6));	//		pow(64.0/9, 1.0/3)
			std::cout << i << ": " << times[i] << " (" << qty[i] << ") - " << times[i]/complexity;
			std::cout << " time: " << double(T_GET(i))/qty[i] << ' ' << double(T_GET(i))/qty[i]/complexity/complexity << "\n";
		}
	}
	
	unsigned int i=1;
	for(; i<times.size(); ++i)
	{
		if(qty[i]==0)
			continue;
		std::cout << i << ": " << times[i] << " (" << qty[i] << ") - " << times[i]/i/i/i;
		std::cout << " avg_time: " << double(T_GET(i))/qty[i] << ' ' << double(T_GET(i))/qty[i]/i/i/i/i/i/i << "\n";
	}
	T_CLEAR;

	getch();

	std::cout << "*************" << std::endl;
	CFactorization<intstr> factorization_is; 
	assert(factorization_is.Factorize(6637*9439, count)==std::make_pair(intstr(6637), intstr(9439)));						std::cout << count << std::endl;
	assert(factorization_is.Factorize(intstr(18077)*9587, count)==std::make_pair(intstr(9587), intstr(18077)));				std::cout << count << std::endl;
	assert(factorization_is.Factorize(intstr(18149)*10369, count)==std::make_pair(intstr(10369), intstr(18149)));			std::cout << count << std::endl;
	assert(factorization_is.Factorize(intstr(12907)*27077, count)==std::make_pair(intstr(12907), intstr(27077)));			std::cout << count << std::endl;
	assert(factorization_is.Factorize(281*631, count)==std::make_pair(intstr(281), intstr(631)));							std::cout << count << std::endl;
	assert(factorization_is.Factorize(499*1283, count)==std::make_pair(intstr(499), intstr(1283)));							std::cout << count << std::endl;
	assert(factorization_is.Factorize(25021, count)==std::make_pair(intstr(131), intstr(191)));								std::cout << count << std::endl;
	assert(factorization_is.Factorize(102757, count)==std::make_pair(intstr(211), intstr(487)));							std::cout << count << std::endl;
	assert(factorization_is.Factorize(98303, count)==std::make_pair(intstr(197), intstr(499)));								std::cout << count << std::endl;
	assert(factorization_is.Factorize(194111, count)==std::make_pair(intstr(389), intstr(499)));							std::cout << count << std::endl;
	assert(factorization_is.Factorize(161197, count)==std::make_pair(intstr(331), intstr(487)));							std::cout << count << std::endl;
	assert(factorization_is.Factorize(12863, count)==std::make_pair(intstr(19), intstr(677)));								std::cout << count << std::endl;
	assert(factorization_is.Factorize(1513, count)==std::make_pair(intstr(17), intstr(89)));								std::cout << count << std::endl;
	assert(factorization_is.Factorize(1943, count)==std::make_pair(intstr(29), intstr(67)));								std::cout << count << std::endl;
	assert(factorization_is.Factorize(4087, count)==std::make_pair(intstr(61), intstr(67)));								std::cout << count << std::endl;
	assert(factorization_is.Factorize(10573, count)==std::make_pair(intstr(97), intstr(109)));								std::cout << count << std::endl;
	assert(factorization_is.Factorize(6557, count)==std::make_pair(intstr(79), intstr(83)));								std::cout << count << std::endl;
	assert(factorization_is.Factorize(2911, count)==std::make_pair(intstr(41), intstr(71)));								std::cout << count << std::endl;
	assert(factorization_is.Factorize(979, count)==std::make_pair(intstr(11), intstr(89)));									std::cout << count << std::endl;
	assert(factorization_is.Factorize(4891, count)==std::make_pair(intstr(67), intstr(73)));								std::cout << count << std::endl;
	assert(factorization_is.Factorize(intstr(66071)*99991, count)==std::make_pair(intstr(66071), intstr(99991)));			std::cout << count << std::endl;
	assert(factorization_is.Factorize(intstr(99991)*48649, count)==std::make_pair(intstr(48649), intstr(99991)));			std::cout << count << std::endl; 
	assert(factorization_is.Factorize(intstr(91397)*68521, count)==std::make_pair(intstr(68521), intstr(91397)));			std::cout << count << std::endl;
	assert(factorization_is.Factorize(intstr((__int64)35587649)*(__int64)3592033807, count)==std::make_pair(intstr((__int64)35587649), intstr((__int64)3592033807)));			std::cout << count << std::endl;
	
/*	for(j=0; j<2000; j++)
	{
		unsigned int first=primes.Get(300000000+pow(j, 3), 300000000+pow(j, 4)+100);
		unsigned int second=primes.Get(pow(first, 0.4), pow(first, 0.5)+1);
		if(first==second)
			second=primes.Next(second);
		intstr task=(__int64)first*second;
		
		std::cout << "task " << j+1 << ": " << first << ' ' << second;
		 
		unsigned int log_task=log2(task);

		T_START(log_task);
		std::pair<__int64, __int64> factors=factorization_is.Factorize(task, count);
		T_STOP(log_task);

		if(times.size()<=log_task)
		{
			times.resize(log_task+1, 0);
			qty.resize(log_task+1, 0);
		}

//		times[log_task]*=double(qty[log_task])/(qty[log_task]+1);		// среднее число итераций
		++qty[log_task];
//		times[log_task]+=double(count)/qty[log_task];					

		if(count>times[log_task])
			times[log_task]=count;										// максимальное число разложенний квадратичных вычетов

		if(factors.first==1)
			break;
		assert(factors.first==first || factors.second==first);
		assert(factors.first==second || factors.second==second);

		std::cout << "time: " << (unsigned int) ((double)log(T_GET(log_task)/qty[log_task])/log(2.0)) << '(' << log_task << ") [" << count << "]\n";
	}
	
	if(j<100)
		std::cout << "FAIL\n";
	else
		std::cout << "SUCCESS\n";

	for(i=0; i<times.size(); ++i)
		std::cout << i << ": " << times[i] << " (" << qty[i] << ") - " << times[i]/i/i/i/i << "\n";

	getch();
*/
	return 0;
}

