#ifndef __AT_BEST_FIRST_SEARCH__
#define __AT_BEST_FIRST_SEARCH__

#include <planning/AT_Search_Engine.hxx>
#include <vector>

namespace aig_tk
{

template < typename Node, typename Node_Cmp >
class	AT_Best_First_Search : public AT_Search_Engine< Node, Node_Cmp >
{
	public:
		typedef Node		Node_Type;
		typedef Node_Cmp	Node_Cmp_Type;

		AT_Best_First_Search() 
		{
			m_problem = NULL;
			m_root = NULL;
		}

		virtual		~AT_Best_First_Search() {}	

		virtual void	init( STRIPS_Problem& p, Node_Type* n0 )
		{
			m_problem = &p;
			m_root = n0;
			setBound( infty );
		}

		virtual bool	findSolution( std::vector<Node_Type*> &plan ) = 0;

	protected:
		STRIPS_Problem&		problem() 			{ return *m_problem; }
		Node_Type*		root()				{ return m_root; }
		Cost_Type		bound() const			{ return m_B; }
		void			setBound( Cost_Type v ) 	{ m_B = v; }
		void			incGen()			{ m_gen_count++; }
		void			incEval()			{ m_exp_count++; }

	protected: // Members
		
		STRIPS_Problem*			m_problem;
		Node_Type*			m_root;
		unsigned			m_exp_count;
		unsigned			m_gen_count;
		Cost_Type			m_B;
};

}


#endif // AT_Best_First_Search.hxx
