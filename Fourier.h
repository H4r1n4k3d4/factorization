//////////////////////////////////////////////////////////////////////
// Fourier.h: interface for the CFourier class.
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#if !defined(FOURIER_H)
#define FOURIER_H

#include "Complex.h"
#include <vector>
#include <list>

//////////////////////////////////////////////////////////////////////
// class CFourier
//////////////////////////////////////////////////////////////////////

class CFourier   
{
	static CComplex A(int, int, int);
	static bool IsAPowerOf2(unsigned int);
	static unsigned int Margin(unsigned int);

public:
	static CComplex Slow(const std::vector<double>&, unsigned int, unsigned int);
	static void Forward(const std::vector<CComplex>&, std::vector<CComplex>&);
	static void Forward(const std::vector<double>&, std::vector<CComplex>&);
	static void Forward(const std::list<char>&, std::vector<CComplex>&, unsigned int);
	static void Inverse(const std::vector<CComplex>&, std::vector<CComplex>&);
	static void Inverse(const std::vector<double>&, std::vector<CComplex>&);
};

std::ostream& operator<<(std::ostream&, const std::vector<CComplex>&);
std::ostream& operator<<(std::ostream&, const std::vector<double>&);

#endif // !defined(FOURIER_H)
