# no-factor-base (NFB) factorization
Factorization C/C++ NFB lib, the no-factor-base sub-exponential general purpose algorithm

In the first version, the lib implements a modification to the Dixon's factorization algorithm.
The modification uses no factor base. Instead, all the common factors of the quadratic residues 
modulo n are searched by GCD. As a result:
- no optimization by the size of the factor base is necessary
- the complexity improves to L(2/5, 2) in L-notation (heuristically)

The concept of a factor base is obsolete. It was introduced in 1975 probably to run factorization 
under severe RAM limitations.

Surprisingly (or not?), finding common factors of quadratic residues by GCD instead of using 
factor base does not improve the asymptotic complexity of the Quadratic sieve algorithm.

Checking divisions by small factors is used in Extractor.h, but elimination of this part does not 
influence the asymptotic complexity of the algorithm.

### author
(a) pavelmalyshkin
