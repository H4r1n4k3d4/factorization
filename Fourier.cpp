//////////////////////////////////////////////////////////////////////
// Fourier.cpp: implementation of the CFourier class.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Fourier.h"

#define _USE_MATH_DEFINES
#include "math.h"

//////////////////////////////////////////////////////////////////////
// class CFourier
//////////////////////////////////////////////////////////////////////

unsigned int CFourier::Margin(unsigned int _size)
{
	unsigned int result=1;
	for(_size-=1; _size>=1; _size/=2)
		result*=2;
	return result;
} 

CComplex CFourier::A(int _m, int _n, int _N)
{
	return exp(-2*PI*CComplex::I*_m*_n/_N);
}

bool CFourier::IsAPowerOf2(unsigned int _size)
{
	if(_size<=1)
		return true;

	bool result=true;
	for(; _size!=1; _size/=2)
	{
		if(_size%2!=0)
		{
			result=false;
			break;
		}
	}
	return result;
}

CComplex CFourier::Slow(const std::vector<double>& _arg, unsigned int _n, unsigned int _N)
{
	CComplex result;
	for(unsigned int m=0; m<_arg.size(); m++)
	{
		CComplex coefficient=A(m, _n, _N);
		CComplex addition=_arg[m]*coefficient;
		result+=addition;
	}
	return result;
}

void CFourier::Forward(const std::vector<CComplex>& _arg, std::vector<CComplex>& _result)
{
	unsigned int N=_arg.size();
	assert(IsAPowerOf2(N));
	
	_result.clear();
	if(N==0)
		return;

	_result.resize(N);

	if(N==1)
	{
		_result[0]=_arg[0];
		return;
	}

	if(N==2)
	{
		CComplex arg0=_arg[0];
		CComplex arg1=_arg[1];
		_result[0]=_arg[0]+_arg[1];
		_result[1]=_arg[0]-_arg[1];
		return;
	}

	unsigned int N2=N/2;
	unsigned int n;

	std::vector<CComplex> even;
	even.resize(N2);
	for(n=0; n<N2; n++)
		even[n]=_arg[2*n];

	std::vector<CComplex> odd;
	odd.resize(N2);
	for(n=0; n<N2; n++)
		odd[n]=_arg[2*n+1];

	std::vector<CComplex> Em;
	Forward(even, Em);

	std::vector<CComplex> Om;
	Forward(odd, Om);

	CComplex multiplier=-2*PI*CComplex::I/N;
	for(unsigned int m=0; m<N2; m++)
	{
		CComplex addition=exp(multiplier*m)*Om[m];
		_result[m]=Em[m]+addition;
		_result[m+N2]=Em[m]-addition;
	}
}

void CFourier::Forward(const std::vector<double>& _arg, std::vector<CComplex>& _result)
{
	unsigned int N=Margin(_arg.size());
	
	std::vector<CComplex> arg;
	arg.resize(N);

	for(unsigned int n=0; n<_arg.size(); n++)
		arg[n]=_arg[n];

	Forward(arg, _result);
}

void CFourier::Forward(const std::list<char>& _arg, std::vector<CComplex>& _result, unsigned int _size)
{
	unsigned int N=Margin(_size);
	
	std::vector<CComplex> arg;
	arg.resize(N, 0);

	std::list<char>::const_iterator it=_arg.begin();
	for(unsigned int n=0; n<_arg.size(); n++)
	{
		if(it==_arg.end())
			break;
		arg[n]=*(it++);
	}

	Forward(arg, _result);
}

void CFourier::Inverse(const std::vector<CComplex>& _arg, std::vector<CComplex>& _result)
{
	std::vector<CComplex> conj;
	CComplex::Conjugate(_arg, conj);
	Forward(conj, _result);
	CComplex::Conjugate(_result, conj);
	for(unsigned int i=0; i<_result.size(); i++)
		_result[i]=conj[i]/_result.size();
}

void CFourier::Inverse(const std::vector<double>& _arg, std::vector<CComplex>& _result)
{
	std::vector<double> reverse;
	reverse.resize(_arg.size());
	for(unsigned int i=0; i<_arg.size(); i++)
		reverse[i]=_arg[(_arg.size()-i)%_arg.size()]/_arg.size();
	Forward(reverse, _result);
}

//////////////////////////////////////////////////////////////////////
// definitions outside class CFourier
//////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& _out, const std::vector<CComplex>& _rhs)
{
	for(unsigned int i=0; i<_rhs.size(); i++)
		_out << _rhs[i] << ' ';
	return _out;
}

std::ostream& operator<<(std::ostream& _out, const std::vector<double>& _rhs)
{
	for(unsigned int i=0; i<_rhs.size(); i++)
		_out << _rhs[i] << ' ';
	return _out;
}
