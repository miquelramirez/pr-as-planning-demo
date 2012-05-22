#include <planning/Single_Queue_FF_BFS.hxx>
#include <util/time.hxx>

namespace aig_tk
{
  
Single_Queue_FF_BFS::Single_Queue_FF_BFS( float w, float z )
	: AT_Best_First_Search< Node, Node_Comparer >(),
	m_closed( 5113 ),
	m_open_hash( 5113 ),
	m_w( w ),
	m_z( z ),
	m_time_budget( 1e7 ),
	m_t0( 0.0f )
{
	m_open_hash.setOwner( false );
	if ( z == 0 )
		Node_Comparer::set_greedy();
	else
		Node_Comparer::set_non_greedy();
}

Single_Queue_FF_BFS::~Single_Queue_FF_BFS()
{
	m_open_hash.clear();
	while(! m_open.empty() ) 
	{	
		Node* n = m_open.pop();
		delete n;
	}
	m_closed.clear();
}

void	Single_Queue_FF_BFS::start()
{
	m_hff.initialize( problem() );
	isEnd = false;
	isIn = true;
 	Hash_Key& h = root()->hash();
	eval(root());
	std::cout << "h(n0)=" << root()->hn() << std::endl;
	m_open.insert(root());
	m_open_hash.add_element(h(), root());
	incGen();
}

bool	Single_Queue_FF_BFS::findSolution( std::vector<Node_Type*>& plan )
{
	m_t0 = time_used();
	Node* end = doSearch();
	if ( end == NULL ) return false;
	extractPlan( root(), end, plan );	
	
	return true;
}

Node*	Single_Queue_FF_BFS::doSearch() 
{
	Node *head = getNode();
	int counter =0;
	while(head) 
	{
		if ( head->gn() >= bound() ) 
		{
			close(head);
			head = getNode();
			continue;
		}

		if(isGoal(head)) 
		{
			close(head);
			setBound( head->gn() );	
			return head;
		}
		if ( (time_used() - m_t0 ) > m_time_budget )
			return NULL;
	
		eval( head );

		process(head);
		close(head);
		counter++;
		head = getNode();
	}
	return NULL;
}

Node* 	Single_Queue_FF_BFS::getNode() 
{
	Node *next = NULL;
	if(! m_open.empty() ) 
	{
		next = m_open.pop();
		m_open_hash.remove_element(next->hash(), next);
	}
	return next;
}

void 	Single_Queue_FF_BFS::openNode(Node *n, Hash_Key& h, Open_List<Node_Comparer,Node*> &o) 
{
	if(n->hn() == infty ) {
		close(n);
	}
	else {
		o.insert(n);
		m_open_hash.add_element(h, n);
	}
	incGen();
}

void	Single_Queue_FF_BFS::process(Node *head) 
{
	for(unsigned i = 0; i < problem().num_actions(); i++) 
	{
		Action* a = problem().actions()[i];
		if( a->can_be_applied_on( *( head->s() ) ) ) 
		{
			Node *suc = head->successor(a);
			Hash_Key& h = suc->hash();
			suc->hn() = head->hn();
			suc->fn() = (m_w * suc->hn()) + (m_z * suc->gn());
			if(previouslyHashed(suc, h)) 
			{
				delete suc;
			}
			else 
				openNode(suc, h, m_open);
		}
	}
	incEval();
}

bool 	Single_Queue_FF_BFS::previouslyHashed(Node *n, Hash_Key& h) 
{
	Node *previous_copy = NULL;


	if((previous_copy = m_closed.get_element(h, n)))
	{
		if(n->gn() < previous_copy->gn())
			return false;

		return true;
	}

	if( (previous_copy = m_open_hash.get_element(h, n)) )
	{
		
		if(n->gn() < previous_copy->gn())
		{
			previous_copy->set_parent(n->parent(), n->op());
			previous_copy->fn() = (m_w * previous_copy->hn()) + (m_z * previous_copy->gn());
		}
		return true;
	}
	return false;
}

void	Single_Queue_FF_BFS::eval( Node* n )
{
	n->hn() = (Cost_Type)m_hff.eval( n->s()->fluent_vec(), problem().goal() );
}

}
