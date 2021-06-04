//////////////////////////////////////////////////////////////////////
// Factorization.h: interface for the 
//						- CFactorNode struct and functions,
//						- CFactorization struct and functions
//
// Authors		: (a)pavelmalyshkin
// License		: GNU GPL v3.0
//
//////////////////////////////////////////////////////////////////////

#if !defined(FACTORIZATION_H)
#define FACTORIZATION_H

#include "Primes.h"
#include "Extractor.h"

#include "Tmgmnt.h"
#include <map>
#include <set>
#include <list>
#include <iostream> 

//////////////////////////////////////////////////////////////////////
//	struct CFactorNode - node in the tree of factors
//////////////////////////////////////////////////////////////////////

template<typename T>
struct CFactorNode
{
	typedef std::set<CFactorNode*> TNodeSet;			// набор узлов
	
	std::set<T> myOrigins;	// множители числа для вычисления квадратичного вычета
	std::map<T, unsigned int> myResolution;				// разложение квадратичного вычета на множители

	CFactorNode(std::set<T>&, std::map<T, unsigned int>&);
	std::pair<T, T> Add(std::map<T, CFactorNode*>&, std::map<T, CExtractor<T>::TResolution> const &, T const &);
	CFactorNode* Relocate(std::map<T, CFactorNode*>&, CExtractor<T>::TResolutionMap const &);
};

// -----------------------------------------------------------------------
// non-member CFactorNode functions
// -----------------------------------------------------------------------

std::pair<__int64, __int64> Resolve(CFactorNode<__int64> const & _node, __int64 const & _task);

template<typename T>
std::pair<T, T> Resolve(CFactorNode<T> const & _node, T const & _task)
{
	intstr u=1;						// может быть _node==NULL - тогда _qres должен быть квадратичным вычетом по модулю N
	intstr v=1;
	std::set<T> nonSquares;

	std::set<T> const & origins=_node.myOrigins;
	for(std::set<T>::const_iterator it=origins.begin(); it!=origins.end(); ++it)
	{
		u*=(*it);
		u%=_task;
	}

	std::map<T, unsigned int> const & resolution=_node.myResolution;
	for(std::map<T, unsigned int>::const_iterator itr=resolution.begin(); itr!=resolution.end(); ++itr)
	{
		assert(itr->second%2==0);
		const T& mult=itr->first;
		unsigned int const & pow=itr->second;
		T multiplier=CExtractor<T>::Power(mult, pow/2);
		v*=multiplier;		
		v%=_task;
	}

	if((u+v)%_task==0 || (u-v)%_task==0)
		return std::make_pair(0, 1);								// тривиальное разложение

	T a=CExtractor<T>::GCD(_task, u+v, 0);					// _task>0, u+v!=0
	T b=CExtractor<T>::GCD(_task, u-v, 0);					// _task>0, u-v!=0
	assert(a!=1 && b!=1);

	return a>b ? std::make_pair(b, a) : std::make_pair(a, b);
}

template<typename T>
void Print(std::map<T, CFactorNode<T>*> const & _tree)
{
	for(std::map<T, CFactorNode<T>*>::const_iterator it=_tree.begin(); it!=_tree.end(); ++it)
	{
		std::cout << "\n";
		std::cout << it->first;

		std::map<T, unsigned int>& resolution=it->second->myResolution;
		assert(resolution.find(it->first)!=resolution.end());
		for(std::map<T, unsigned int>::const_iterator itr=resolution.begin(); itr!=resolution.end(); ++itr)
		{
			if(itr->second%2==1 && itr->first!=it->first)
				std::cout << "\n\t" << itr->first;
		}
	}

	std::cout << "\n---------------- (" << _tree.size() << " total)\n";
}

// -----------------------------------------------------------------------
// CFactorNode member functions
// -----------------------------------------------------------------------

template<typename T>
CFactorNode<T>::CFactorNode(std::set<T>& _origins, std::map<T, unsigned int>& _resolution)
{
	myOrigins.swap(_origins);
	myResolution.swap(_resolution);
}

template<typename T>
std::pair<T, T> CFactorNode<T>::Add(std::map<T, CFactorNode*>& _tree, std::map<T, CExtractor<T>::TResolution> const & _resolutions, T const & _task)
{
	for(std::map<T, unsigned int>::reverse_iterator it=myResolution.rbegin(); it!=myResolution.rend(); ++it)
	{
		if(it->second%2==0)
			continue;

		T const & factor=it->first;
		std::map<T, CFactorNode*>::iterator itf=_tree.find(factor);
		if(itf==_tree.end())
		{
			_tree[factor]=this;
			return std::make_pair(1, 1);
		}

		CFactorNode<T>* otherNode=itf->second;
		for(std::set<T>::iterator ito=otherNode->myOrigins.begin(); ito!=otherNode->myOrigins.end(); ++ito)
		{
			std::set<T>::iterator itmo=myOrigins.find(*ito);
			if(itmo==myOrigins.end())
			{
				myOrigins.insert(*ito);
				std::map<T, CExtractor<T>::TResolution>::const_iterator itr=_resolutions.find(*ito);
				assert(itr!=_resolutions.end());
				std::map<T, unsigned int> const & resolution=itr->second;
				for(std::map<T, unsigned int>::const_iterator itm=resolution.begin(); itm!=resolution.end();++itm)
				{
					std::map<T, unsigned int>::iterator itmm=myResolution.find(itm->first);
					if(itmm==myResolution.end())
					{
						myResolution.insert(std::make_pair(itm->first, 0));
						itmm=myResolution.find(itm->first);
					}
					itmm->second+=itm->second;
				}
			}
			else
			{
				std::map<T, CExtractor<T>::TResolution>::const_iterator itr=_resolutions.find(*ito);
				assert(itr!=_resolutions.end());
				std::map<T, unsigned int> const & resolution=itr->second;
				for(std::map<T, unsigned int>::const_iterator itm=resolution.begin(); itm!=resolution.end();++itm)
				{
					std::map<T, unsigned int>::iterator itmm=myResolution.find(itm->first);
					assert(itmm!=myResolution.end());
					assert(itmm->second>=itm->second);
					itmm->second-=itm->second;
					if(itmm->second==0)
						myResolution.erase(itmm);
				}
				myOrigins.erase(itmo);
			}
		}

		it=myResolution.rbegin();
		assert(it!=myResolution.rend());
	}

	assert(myResolution.size()>0);
	return Resolve(*this, _task);
}

template<typename T>
CFactorNode<T>* CFactorNode<T>::Relocate(std::map<T, CFactorNode*>& _tree, CExtractor<T>::TResolutionMap const & _resolutionMap)
{
	std::map<T, unsigned int>::reverse_iterator rit=myResolution.rbegin();
	for( ; rit!=myResolution.rend(); ++rit)
		if(rit->second%2==1)
			break;

	T index=rit->first;
	for(CExtractor<T>::TResolutionMap::const_iterator it=_resolutionMap.begin(); it!=_resolutionMap.end(); ++it)
	{
		std::map<T, unsigned int>::iterator itr=myResolution.find(it->first);
		if(itr==myResolution.end())
			continue;

		unsigned int pow=itr->second;
		CExtractor<T>::TResolution const & resolution=it->second;
		for(CExtractor<T>::TResolution::const_iterator itn=resolution.begin(); itn!=resolution.end(); ++itn)
			myResolution[itn->first]=itn->second*pow;
		myResolution.erase(itr);
	}

	for(rit=myResolution.rbegin(); rit!=myResolution.rend(); ++rit)
		if(rit->second%2==1)
			break;

	assert(index>=rit->first);
	if(index==rit->first)
		return NULL;

	_tree.erase(index);
	if(_tree.find(rit->first)!=_tree.end())
		return this;

	_tree[rit->first]=this;
	return NULL;
}

//////////////////////////////////////////////////////////////////////
//	struct CFactorization - factor tree for a specific number
//////////////////////////////////////////////////////////////////////

template<typename T>
struct CFactorization
{
	typedef std::pair<T, unsigned int> TIndex;		// i и степень, с которым множитель входит в соответствующее число
	typedef std::list<TIndex> TIndexList;			// список i чисел, в которые входит множитель
	typedef std::list<TIndex>::iterator TListIt;

	CExtractor<T>* myFactorBase;
	std::map<T, CFactorNode<T>::TNodeSet> myFactorIndex;	// множители и узлы, в разложение которых эти множители вохдят
	std::map<T, CExtractor<T>::TResolution> myResolutions;	// числа и их разложения
	std::multimap<T, T> myReverseIndex;		// множители и числа, в разложения которых они входят 

	T myGenTask;
	std::map<T, CFactorNode<T>*> myFactorTree;		// узлы, отсортированные по первым факторам их разложений
	
	std::pair<T, T> Add(T const&, T&, CExtractor<T>::TResolution&);
	std::pair<T, T> Factorize(T, unsigned int&);
	void Clear();
};

template<typename T>
std::pair<T, T> CFactorization<T>::Add(T const& _origin, T& _remainder, CExtractor<T>::TResolution& _resolution)
{
	CExtractor<T>::TResolutionMap resolutionMap;
	myFactorBase->ExtractAll(_remainder, _resolution, resolutionMap);

	CFactorNode<T>::TNodeSet relocate;
	std::map<T, std::set<T> > reverseIndex;
	for(CExtractor<T>::TResolutionMap::iterator it=resolutionMap.begin(); it!=resolutionMap.end(); ++it)
	{
		T oldFactor=it->first;
		CFactorNode<T>::TNodeSet& nodeSet=myFactorIndex[oldFactor];
		for(CFactorNode<T>::TNodeSet::iterator itns=nodeSet.begin(); itns!=nodeSet.end(); ++itns)
		{
			CFactorNode<T>* node=*itns;
			relocate.insert(node);
		}
		myFactorIndex.erase(oldFactor);

		std::multimap<T, T>::iterator ubound=myReverseIndex.upper_bound(oldFactor);
		for(std::multimap<T, T>::iterator itri=myReverseIndex.lower_bound(oldFactor); itri!=ubound; )
		{
			reverseIndex[itri->second].insert(oldFactor);
			CExtractor<T>::TResolution& resolution=it->second;
			for(std::map<T, unsigned int>::iterator itr=resolution.begin(); itr!=resolution.end(); ++itr)
				myReverseIndex.insert(std::make_pair(itr->first, itri->second));
			myReverseIndex.erase(itri);
			itri=myReverseIndex.lower_bound(oldFactor);
		}
	}

	CFactorNode<T>::TNodeSet toAdd;
	for(CFactorNode<T>::TNodeSet::iterator itr=relocate.begin(); itr!=relocate.end(); ++itr)
	{
		CFactorNode<T>* node=(*itr)->Relocate(myFactorTree, resolutionMap);
		if(node)
			toAdd.insert(node);
	}

	for(CFactorNode<T>::TNodeSet::iterator ita=toAdd.begin(); ita!=toAdd.end(); ++ita)
	{
		std::pair<T, T> result=(*ita)->Add(myFactorTree, myResolutions, myGenTask);
		assert(result.first<=1);
	}

	for(std::map<T, std::set<T> >::iterator itri=reverseIndex.begin(); itri!=reverseIndex.end(); ++itri)
	{
		CExtractor<T>::TResolution& resolution=myResolutions[itri->first];
		std::set<T>& oldMultipliers=itri->second;
		for(std::set<T>::iterator itom=oldMultipliers.begin(); itom!=oldMultipliers.end(); ++itom)
		{
			T& oldMultiplier=*itom;
			CExtractor<T>::TResolution::iterator itr=resolution.find(oldMultiplier);
			CExtractor<T>::TResolution newResolution=resolutionMap[oldMultiplier];
			for(CExtractor<T>::TResolution::iterator itnr=newResolution.begin(); itnr!=newResolution.end(); ++itnr)
				resolution[itnr->first]=itnr->second*itr->second;
			resolution.erase(itr);
		}
	}

	std::set<T> origins;
	origins.insert(_origin);
	CExtractor<T>::TResolution resolution=_resolution;
	CFactorNode<T>* node=new CFactorNode<T>(origins, resolution);
	std::pair<T, T> result=node->Add(myFactorTree, myResolutions, myGenTask);
	if(result.first==0)
	{
		delete node;
		return std::pair<T, T>(1, 1);
	}
	else if(result.first>1)
		return result;

	for(CExtractor<T>::TResolution::iterator itre=_resolution.begin(); itre!=_resolution.end(); ++itre)
	{
		myFactorIndex[itre->first].insert(node);
		myReverseIndex.insert(std::make_pair(itre->first, _origin));
	}
	return result;
}

template<typename T>
std::pair<T, T> CFactorization<T>::Factorize(T _task, unsigned int& _count)
{
	_count=0;
	std::pair<T, T> result;
	myGenTask=_task; 

	bool square_root;
	T sqrt=CExtractor<T>::SqrtLow(_task, square_root);
	if(square_root)
	{
		result=std::make_pair(sqrt, sqrt);
		return result;
	}

	myFactorBase=new CExtractor<T>(sqrt);

	for(unsigned int smallNumber=1; ; ++smallNumber)
	{
		if(smallNumber!=1 && myGenTask%smallNumber==0)
		{
			result=std::make_pair(smallNumber, myGenTask/smallNumber);
			break;
		}

		T seqNum=myGenTask*smallNumber;
		T root=CExtractor<T>::SqrtLow(seqNum, square_root);

		T origin=root+1; 
		T remainder=origin*origin-seqNum;
		assert(remainder>=0);

		if(!myFactorBase->Check(remainder, smallNumber))
			continue;

		CExtractor<T>::TResolution resolution;
		sqrt=CExtractor<T>::SqrtLow(remainder, square_root);

		if(square_root)
		{
			resolution[sqrt]=2;
			std::set<T> origins;
			origins.insert(origin);
			CFactorNode<T> node(origins, resolution);
			result=Resolve<T>(node, myGenTask);
			break;
		}

		++_count;
		result=Add(origin, remainder, resolution);
		if(result.first>1)
			break;

		if(result.first>0 && myResolutions.find(origin)==myResolutions.end())
			myResolutions[origin].swap(resolution);

//		Print<T>(myFactorTree);
	}

	Clear();
	return result;
}

template<typename T>
void CFactorization<T>::Clear()
{
	for(std::map<T, CFactorNode<T>*>::iterator it=myFactorTree.begin(); it!=myFactorTree.end(); ++it)
		delete it->second;
	myFactorIndex.clear();
	myReverseIndex.clear();
	myResolutions.clear();
	delete myFactorBase;
	myFactorTree.clear();
}

#endif // !defined(FACTORIZATION_H)
