#ifndef __SEARCH__ENGINE__
#define __SEARCH__ENGINE__

#include <vector>
#include <algorithm>
#include <planning/STRIPS_Problem.hxx>
#include <cassert>

namespace aig_tk
{

template < typename Node, typename Node_Cmp >
class AT_Search_Engine
{
	public:
		typedef Node		Node_Type;
		typedef Node_Cmp	Node_Cmp_Type;

		virtual void	init( STRIPS_Problem& p, Node_Type* n0 ) = 0;
		virtual bool	findSolution( std::vector<Node_Type*> &plan ) = 0;
		virtual		~AT_Search_Engine() {}

	protected:

		void	extractPlan( Node_Type* s, Node_Type* t, std::vector<Node_Type*>& p );
		
};

template < typename Node, typename Node_Cmp >
void	AT_Search_Engine<Node, Node_Cmp>::extractPlan( Node* s, Node* t, std::vector<Node*>& plan )
{
	Node *tmp = t;
	
	while( tmp != s)//!(*tmp == *s &&  tmp->gn() == s->gn()) ) 
	{
		plan.push_back(tmp);
		tmp = tmp->parent();
	}
	
	std::reverse(plan.begin(), plan.end());
}

}


#endif // Search_Engine.hxx
