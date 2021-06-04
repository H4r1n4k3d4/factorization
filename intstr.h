//////////////////////////////////////////////////////////////////////
// intstr.h: interface for the intstr struct.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#if !defined(INTSTR_H)
#define INTSTR_H

#include <assert.h>
#include <iostream>
#include <vector>
#include <list>

//////////////////////////////////////////////////////////////////////
//	struct intstr - string representing an integer
//////////////////////////////////////////////////////////////////////

struct intstr
{
	struct TInit
	{
		__int64 myValue;
		TInit();
	};

	static TInit myMaxNum;
	static SetMaxNum(__int64 _value)	{	myMaxNum.myValue=_value;	};
	static ResetMaxNum();

	bool mySign;
	std::list<char> myData;

	intstr();
	intstr(__int64 const &_b);
	intstr(int const &_b);
	intstr(unsigned int const &_b);

	int Int();
	static void AddMod(std::list<char>&, std::list<char> const&, std::list<char> const&);
	static bool SubtractMod(std::list<char>&, std::list<char> const&, std::list<char> const&);
	static void Divide(intstr&, intstr&, intstr const&, intstr const&);
	static void Mult(std::list<char>&, std::list<char> const&, char);

	void Shrink();
	intstr operator-() const;
	intstr operator = (intstr const&);
	bool operator < (intstr const&) const;
	bool operator > (intstr const&) const;
	bool operator <= (intstr const&) const;
	bool operator <= (std::vector<char> const&) const;
	bool operator >= (intstr const&) const;
	bool operator == (intstr const&) const;
	bool operator != (intstr const&) const;
	intstr operator + (intstr const&) const;
	intstr operator - (intstr const&) const;
	intstr operator % (intstr const&) const;
	intstr operator / (intstr const&) const;
	intstr operator * (intstr const&) const;
	intstr& operator += (intstr const&);
	intstr& operator *= (intstr const&);
	intstr& operator -= (intstr const&);
	intstr& operator /= (intstr const&);
	intstr& operator %= (intstr const&);
	intstr& operator++();
 
	intstr sqrt_low(bool&) const; 
	void swap(intstr&);
	void negate();
	bool operator == (std::pair<intstr, intstr> const &);
};

intstr gcd(intstr const&, intstr const&);
std::ostream operator << (std::ostream&, intstr const&);
std::istream operator >> (std::istream&, intstr&);
unsigned int GetPart(intstr const &);
unsigned int log2(intstr const &);
void Abs(intstr&);
unsigned int Int(__int64&);
unsigned int Int(intstr&);
 
#endif // !defined(INTSTR_H)

