//////////////////////////////////////////////////////////////////////
// intstr.cpp: implementation of the intstr struct.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "intstr.h"
#include "Fourier.h"

#include <map>

//////////////////////////////////////////////////////////////////////
//	struct intstr - string representing an integer
//////////////////////////////////////////////////////////////////////

const __int64 max_num=10000000000000000;		// maximum number used to code digits in the higher-order positions of the divider
intstr::TInit intstr::myMaxNum;					// temporary max_num for testing purposes

intstr::TInit::TInit()
{
	myValue=max_num;
}

intstr::ResetMaxNum()
{
	myMaxNum.myValue=max_num;
}

intstr::intstr()	
{	
	mySign=true;
}

intstr::intstr(__int64 const &_b)	
{	
	mySign=(_b>=0);

	for(__int64 b=(mySign?_b:-_b); b>0; b/=10)
		myData.push_back(b%10);
}

intstr::intstr(int const &_b)	
{	
	mySign=(_b>=0);

	for(__int64 b=(mySign?_b:-_b); b>0; b/=10)
		myData.push_back(b%10);
} 

intstr::intstr(unsigned int const &_b)	
{	
	mySign=true;
	unsigned int b=_b;

	for( ; b>0; b/=10)
		myData.push_back(b%10);
}
 
int intstr::Int()
{
	int result=0;
	for(std::list<char>::reverse_iterator rit=myData.rbegin(); rit!=myData.rend(); ++rit)
	{
		result*=10;
		result+=*rit;
	}
	if(!mySign)
		result=-result;

//	assert(result*result<256*256*256*64);
	return result;
}

void intstr::AddMod(std::list<char>& _result, const std::list<char>& _first, const std::list<char>& _second)
{
	std::list<char> const& longer=(_first.size()>=_second.size())?_first:_second;
	std::list<char> const& shorter=(_first.size()<_second.size())?_first:_second;
	std::list<char>::const_iterator itl=longer.begin();
	for(std::list<char>::const_iterator its=shorter.begin(); its!=shorter.end(); ++its)
	{
		_result.push_back(*itl+*its);
		++itl;
	}
	for( ; itl!=longer.end(); ++itl)
		_result.push_back(*itl);

	unsigned char overflow=0;
	for(std::list<char>::iterator it=_result.begin(); it!=_result.end(); ++it)
	{
		*it+=overflow;
		if(*it>=10)
		{
			*it-=10;
			overflow=1;
		}
		else
			overflow=0;
	}
	if(overflow==1)
		_result.push_back(1);
}

bool intstr::SubtractMod(std::list<char>& _result, const std::list<char>& _first, const std::list<char>& _second)
{
	std::list<char>::const_iterator itf=_first.begin();
	std::list<char>::const_iterator its=_second.begin();
	for( ; its!=_second.end(); ++its)
	{
		if(itf==_first.end())
			break;
		_result.push_back(*itf-*its);
		++itf;
	}
	if(itf==_first.end())
		for( ; its!=_second.end(); ++its)
			_result.push_back(-*its);
	else
		for( ; itf!=_first.end(); ++itf)
			_result.push_back(*itf);

	unsigned char overflow=0;
	for(std::list<char>::iterator it=_result.begin(); it!=_result.end(); ++it)
	{
		*it-=overflow;
		if(*it<0)
		{
			*it+=10;
			overflow=1;
		}
		else
			overflow=0;
	}

	bool sign=true;
	if(overflow==1)
	{
		for(std::list<char>::iterator it=_result.begin(); it!=_result.end(); ++it)
		{
			*it=9-*it+overflow;
			if(*it==10)
				*it=0;
			else
				overflow=0;
		}
		sign=false;
	}

	for(std::list<char>::reverse_iterator rit=_result.rbegin(); rit!=_result.rend(); rit=_result.rbegin())
		if(*rit==0)
			_result.pop_back();
		else
			break;

	return sign;
}

void intstr::Divide(intstr& _result, intstr& _remainder, intstr const& _divident, intstr const& _b)
{
	assert(_b!=0);
	assert(_b.myData.back()>0);
	assert(_b.mySign);

	__int64 divider=0;
	int length=0;
	std::list<char>::const_reverse_iterator ritr;
	for(ritr=_b.myData.rbegin(); ritr!=_b.myData.rend(); ++ritr)
	{
		if(divider>myMaxNum.myValue)
			break;
		divider*=10;
		divider+=*ritr;
		++length;
	}

	std::vector<char> remainder;
	for(std::list<char>::const_iterator it=_divident.myData.begin(); it!=_divident.myData.end(); ++it)
		remainder.push_back(*it);

	char times=0;							// сколько нужно сбросить нулей без их переписывания в result
	for(std::map<char, intstr> subOptions; _b<=remainder; )
	{
		++times;
		__int64 divident=0;
		int ir=remainder.size()-1;
		for(int i=0; i<length; ++i)
		{
			divident*=10;
			divident+=remainder[ir--];
		}

		char digit=divident/divider;
		if(digit==0)
		{
			divident*=10;
			divident+=remainder[ir--];
			digit=divident/divider;
			if(times<2)
			{
				if(!_result.myData.empty())
					_result.myData.push_front(0);
				++times;
			}
		}

		ir-=(_b.myData.size()-length);
		for(char overflow=1; overflow==1; )
		{
			assert(digit!=0);
			intstr& local_mult=subOptions[digit];
			if(local_mult.myData.size()==0)
				Mult(local_mult.myData, _b.myData, digit);

			overflow=0;
			int irc=ir;
			std::vector<char> tmp=remainder;
			for(std::list<char>::const_iterator it=local_mult.myData.begin(); it!=local_mult.myData.end(); ++it)
			{
				tmp[++irc]-=(*it)+overflow;
				if(tmp[irc]<0)
				{
					tmp[irc]+=10;
					overflow=1;
				}
				else
					overflow=0;
			}

			if(overflow==1)
			{
				if(irc==remainder.size()-1)
				{
					assert(overflow==1);
					if(digit>1)
						--digit;
					else
					{
						--ir;
						digit=9;
						++times;
					}
				}
				else
				{
					++irc;
					assert(tmp[irc]==1);
					tmp[irc]=0;
					remainder.swap(tmp);
					overflow=0;
				}
			}
			else
				remainder.swap(tmp);
		}

		_result.myData.push_front(digit);
		for(ir=remainder.size()-1; ir>=0 && remainder[ir]==0; --ir)
		{
			if(remainder.size()>=_b.myData.size() && times==0)
				_result.myData.push_front(0);
			remainder.pop_back();
			if(times>0)
				--times;
		}
	}

	if(remainder.size()==_b.myData.size() && times==0)
		_result.myData.push_front(0);

	for(unsigned int i=0; i<remainder.size(); ++i)
		_remainder.myData.push_back(remainder[i]);

	_remainder.Shrink();
	_result.Shrink();
}

void intstr::Mult(std::list<char>& _result, std::list<char> const & _first, char _second)
{
	int overflow=0;
	for(std::list<char>::const_iterator it=_first.begin(); it!=_first.end(); ++it)
	{
		char result=*it*_second+overflow;
		overflow=result/10;
		_result.push_back(result%10);
	}
	if(overflow>0)
		_result.push_back(overflow);
}

void intstr::Shrink()
{
	for(std::list<char>::reverse_iterator it=myData.rbegin(); it!=myData.rend(); it=myData.rbegin())
		if(*it==0)
			myData.pop_back();
		else
			break;
}

intstr intstr::operator = (intstr const & _b)
{	
	if(this!=&_b)	
	{	
		mySign=_b.mySign;
		myData=_b.myData;	
	}	
	return *this;	
}

bool intstr::operator < (intstr const & _b) const	
{	
	if(!mySign && _b.mySign)
		return true;
	else if(mySign && !_b.mySign)
		return false;

	if(myData.size()<_b.myData.size())
		return mySign;
	else if(myData.size()>_b.myData.size())
		return !mySign;

	std::list<char>::const_reverse_iterator itb=_b.myData.rbegin();
	for(std::list<char>::const_reverse_iterator it=myData.rbegin(); it!=myData.rend(); ++it)
	{
		if(*it<*itb)
			return mySign;
		else if(*it>*itb)
			return !mySign;
		++itb;
	}

	return false;	
}

bool intstr::operator > (intstr const & _b) const	
{	
	if(mySign && !_b.mySign)
		return true;
	else if(!mySign && _b.mySign)
		return false;

	if(myData.size()>_b.myData.size())
		return mySign;
	else if(myData.size()<_b.myData.size())
		return !mySign;

	std::list<char>::const_reverse_iterator itb=_b.myData.rbegin();
	for(std::list<char>::const_reverse_iterator it=myData.rbegin(); it!=myData.rend(); ++it)
	{
		if(*it>*itb)
			return mySign;
		else if(*it<*itb)
			return !mySign;
		++itb;
	}

	return false;	
}

bool intstr::operator <= (intstr const & _b) const	
{	
	if(!mySign && _b.mySign)
		return true;
	else if(mySign && !_b.mySign)
		return false;

	if(myData.size()<_b.myData.size())
		return mySign;
	else if(myData.size()>_b.myData.size())
		return !mySign;

	std::list<char>::const_reverse_iterator itb=_b.myData.rbegin();
	for(std::list<char>::const_reverse_iterator it=myData.rbegin(); it!=myData.rend(); ++it)
	{
		if(*it<*itb)
			return mySign;
		else if(*it>*itb)
			return !mySign;
		++itb;
	}

	return true;	
}

bool intstr::operator <= (std::vector<char> const & _b) const	
{	
	assert(mySign);

	if(myData.size()<_b.size())
		return true;
	else if(myData.size()>_b.size())
		return false;

	int itb=_b.size()-1;
	for(std::list<char>::const_reverse_iterator it=myData.rbegin(); it!=myData.rend(); ++it)
	{
		if(*it<_b[itb])
			return mySign;
		else if(*it>_b[itb])
			return !mySign;
		--itb;
	}

	return true;	
}

bool intstr::operator >= (intstr const & _b) const	
{	
	if(mySign && !_b.mySign)
		return true;
	else if(!mySign && _b.mySign)
		return false;

	if(myData.size()>_b.myData.size())
		return mySign;
	else if(myData.size()<_b.myData.size())
		return !mySign;

	std::list<char>::const_reverse_iterator itb=_b.myData.rbegin();
	for(std::list<char>::const_reverse_iterator it=myData.rbegin(); it!=myData.rend(); ++it)
	{
		if(*it>*itb)
			return mySign;
		else if(*it<*itb)
			return !mySign;
		++itb;
	}

	return true;	
}

bool intstr::operator == (intstr const & _b) const	
{	
	if(mySign!=_b.mySign || myData.size()!=_b.myData.size())
		return false;

	std::list<char>::const_iterator itb=_b.myData.begin();
	for(std::list<char>::const_iterator it=myData.begin(); it!=myData.end(); ++it)
	{
		if(*it!=*itb)
			return false;
		++itb;
	}

	return true;	
}

bool intstr::operator != (intstr const & _b) const	
{	
	if(mySign!=_b.mySign || myData.size()!=_b.myData.size())
		return true;

	std::list<char>::const_iterator itb=_b.myData.begin();
	for(std::list<char>::const_iterator it=myData.begin(); it!=myData.end(); ++it)
	{
		if(*it!=*itb)
			return true;
		++itb;
	}

	return false;	
}

intstr intstr::operator-() const
{
	intstr ret(*this);
	if(myData.size()>0)
		ret.mySign=!mySign;
	return ret;
}

intstr intstr::operator + (intstr const & _b) const
{	
	intstr result(0);
	if(mySign ^ _b.mySign)
	{
		bool sign=SubtractMod(result.myData, myData, _b.myData);
		if(!result.myData.empty())
			result.mySign=(mySign?sign:!sign);
	}
	else
	{
		AddMod(result.myData, myData, _b.myData);
		result.mySign=mySign;
	}

	return result;	
}

intstr intstr::operator - (intstr const & _b) const
{	
	intstr result(0);
	if(mySign ^ _b.mySign)
	{
		AddMod(result.myData, myData, _b.myData);
		result.mySign=mySign;
	}
	else
	{
		bool sign=SubtractMod(result.myData, myData, _b.myData);
		if(!result.myData.empty())
			result.mySign=(mySign?sign:!sign);
	}

	result.Shrink();
	return result;	
}

intstr intstr::operator % (intstr const & _b) const 
{
	intstr result;
	intstr remainder;
	Divide(result, remainder, *this, _b);
	return remainder;
}

intstr intstr::operator / (intstr const & _b) const
{
	intstr result;
	intstr remainder;
	Divide(result, remainder, *this, _b);
	return result;
}

intstr intstr::operator * (intstr const & _b) const
{
	unsigned int size=myData.size()+_b.myData.size();
	std::vector<CComplex> fourier1;
	CFourier::Forward(myData, fourier1, size);

	std::vector<CComplex> fourier2;
	CFourier::Forward(_b.myData, fourier2, size);

	std::vector<CComplex> fourierR;
	fourierR.resize(fourier1.size());
	for(unsigned i=0; i<fourier1.size(); ++i)
		fourierR[i]=fourier1[i]*fourier2[i];
	std::vector<CComplex> product;
	CFourier::Inverse(fourierR, product);

	intstr result;
	int overflow=0;
	for(std::vector<CComplex>::iterator it=product.begin(); it!=product.end(); ++it)
	{
		int pos_value=(*it).Int()+overflow;
		result.myData.push_back(pos_value%10);
		overflow=pos_value/10;
	}

	result.Shrink();
	return result;
}

intstr& intstr::operator += (intstr const & _b)
{
	intstr result(0);
	if(mySign ^ _b.mySign)
	{
		bool sign=SubtractMod(result.myData, myData, _b.myData);
		if(!result.myData.empty())
			result.mySign=(mySign?sign:!sign);
	}
	else
	{
		AddMod(result.myData, myData, _b.myData);
		result.mySign=mySign;
	}

	result.myData.swap(myData);
	return *this;
}

intstr& intstr::operator *= (intstr const & _b)
{
	unsigned int size=myData.size()+_b.myData.size();
	std::vector<CComplex> fourier1;
	CFourier::Forward(myData, fourier1, size);

	std::vector<CComplex> fourier2;
	CFourier::Forward(_b.myData, fourier2, size);

	std::vector<CComplex> fourierR;
	fourierR.resize(fourier1.size());
	for(unsigned i=0; i<fourier1.size(); ++i)
		fourierR[i]=fourier1[i]*fourier2[i];
	std::vector<CComplex> product;
	CFourier::Inverse(fourierR, product);

	intstr result;
	int overflow=0;
	for(std::vector<CComplex>::iterator it=product.begin(); it!=product.end(); ++it)
	{
		int pos_value=(*it).Int()+overflow;
		result.myData.push_back(pos_value%10);
		overflow=pos_value/10;
	}

	result.myData.swap(myData);
	Shrink();
	return *this;
}

intstr& intstr::operator -= (intstr const & _b)
{
	intstr result(0);
	if(mySign ^ _b.mySign)
	{
		AddMod(result.myData, myData, _b.myData);
		result.mySign=mySign;
	}
	else
	{
		bool sign=SubtractMod(result.myData, myData, _b.myData);
		if(!result.myData.empty())
			result.mySign=(mySign?sign:!sign);
	}

	result.myData.swap(myData);
	Shrink();
	return *this;	
}

intstr& intstr::operator /= (intstr const & _b)
{
	intstr result;
	intstr remainder;
	Divide(result, remainder, *this, _b);
	result.myData.swap(myData);
	Shrink();
	return *this;
}

intstr& intstr::operator %= (intstr const & _b)
{
	intstr result;
	intstr remainder;
	Divide(result, remainder, *this, _b);
	remainder.myData.swap(myData);
	Shrink();
	return *this;
}

intstr intstr::sqrt_low(bool& _isSquare) const
{
	assert(mySign);

	std::vector<unsigned int> data;
	for(std::list<char>::const_iterator it=myData.begin(); it!=myData.end(); )
	{
		unsigned int digit=*(it++);
		if(it!=myData.end())
			digit+=*(it++)*10;
		data.push_back(digit);
	}

	intstr result=0;
	intstr remainder=0;
	for( ; data.size()>0; data.pop_back())
	{
		intstr tmp;
		tmp.myData.swap(remainder.myData);	
		tmp.myData.push_front(data.back()/10);
		tmp.myData.push_front(data.back()%10);
		tmp.Shrink();								

		char i=pow(data.back(), 0.5);
		if(result>0)
			i=(tmp/(result*20)).Int();
		for( ; i>=0; i--)
		{
			remainder=tmp-(result*(i*20)+i*i);		
			if(remainder>=0)
				break;
		}
		result.myData.push_front(i);
	}

	_isSquare=false;
	if(remainder==0)
		_isSquare=true;

	return result;
}

void intstr::swap(intstr& _rhs)
{
	bool sign=mySign;
	mySign=_rhs.mySign;
	_rhs.mySign=sign;

	myData.swap(_rhs.myData);
}

void intstr::negate()
{
	mySign=!mySign;
}

bool intstr::operator == (std::pair<intstr, intstr> const & _rhs)
{
	if(*this==_rhs.first || *this==_rhs.second)
		return true;
	return false;
}

intstr& intstr::operator++()
{
    operator += (1);
    return *this;
}

//////////////////////////////////////////////////////////////////////
//	struct intstr non-member operators and functions
//////////////////////////////////////////////////////////////////////

void Abs(intstr& _a)
{
	_a.mySign=true;
}

unsigned int GetPart(intstr const & _a)
{
	intstr result;
	intstr remainder;
	intstr::Divide(result, remainder, _a, 100);
	return remainder.Int();
}

unsigned int log2(intstr const & _a)
{
	intstr a(_a);
	unsigned int i=0;
	for( ; a>0; ++i)
		a/=2;
	return i-1;
}

intstr gcd(intstr const& _a, intstr const& _b)
{
	assert(_a>0);
	assert(_b>0);

	const intstr& larger=_a>=_b?_a:_b;
	const intstr& smaller=_a>=_b?_b:_a;
	intstr remainder=larger%smaller;

	return remainder==0?smaller:gcd(remainder, smaller);
}

std::ostream operator << (std::ostream& stream, intstr const& _b)
{
	if(!_b.mySign)
		stream << '-';
	if(_b.myData.size()==0)
	{
		stream << '0';
		return stream;
	}
	for(std::list<char>::const_reverse_iterator it=_b.myData.rbegin(); it!=_b.myData.rend(); ++it)
		stream << int(*it);
	return stream;
}

std::istream operator >> (std::istream& stream, intstr& _b)
{
	char c;
	_b=0;
	bool first=true;
	while (stream.get(c))
	{
		if(c=='-')
		{
			assert(first);
			_b.mySign=false;
			first=false;
		}
		else if(c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7' || c=='8' || c=='9')
			_b.myData.push_front(c-48);
		else
			break;
	}
	return stream;
}

unsigned int Int(__int64& _val)
{
	return (unsigned int)_val;
}

unsigned int Int(intstr& _val)
{
	return (unsigned int)(_val.Int());
}