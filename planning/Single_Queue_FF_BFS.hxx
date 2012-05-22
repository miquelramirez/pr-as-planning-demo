#ifndef __SINGLE_QUEUE_FF_BFS__
#define __SINGLE_QUEUE_FF_BFS__

#include <AT_Best_First_Search.hxx>
#include <search/Node.hxx>
#include <search/Node_Comparer.hxx>
#include <search/Closed_List.hxx>
#include <search/Open_List.hxx>
#include <planning/heuristics/Max_Heuristic.hxx>
#include <planning/heuristics/Relaxed_Plan_Extractor.hxx>

namespace aig_tk
{

class	Single_Queue_FF_BFS : public AT_Best_First_Search<Node,Node_Comparer>
{
	public:
		Single_Queue_FF_BFS( float w = 1, float z = 1 );
		~Single_Queue_FF_BFS( );

		void		start();
		void		set_budget( float v ) { m_time_budget = v; }
		virtual	bool	findSolution( std::vector<Node_Type*>& plan );

	protected:
                void 		close( Node* n ) 	{  m_closed.add_element(n->hash(), n); }
                Node* 		isClosed( Node* n ) 	{ return m_closed.get_element(n->hash() ,n); }
		virtual bool 	isGoal(Node *candidate) { return candidate->s()->entails( problem().goal() ); }
		virtual bool 	isInit(Node *candidate) { return candidate->s()->entails( problem().init() ); }

		void		eval( Node* candidate );
		Node* 		getNode();
		void	 	openNode( Node *n, Hash_Key& h, Open_List<Node_Comparer,Node*> &in_open_list );
		void 		process(  Node *head );
		Node*	 	doSearch();
		bool 		previouslyHashed( Node *s, Hash_Key& h );
		void 		reset();

	protected: // Members
		Closed_List<Node> m_closed, m_open_hash;
                
		bool isEnd;
                bool isIn;

                Open_List<Node_Comparer, Node*> m_open;
    
                float m_w;
                float m_z;
		Relaxed_Plan_Extractor<Max_Heuristic>	m_hff;
		float m_time_budget;
		float m_t0;
};

}

#endif // Single_Queue_FF_BFS.hxx
