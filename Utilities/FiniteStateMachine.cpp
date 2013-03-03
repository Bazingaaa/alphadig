
#include "Utilities/FiniteStateMachine.h"

namespace AlphaDig
{


FiniteStateMachine::FiniteStateMachine( int nInitialState )
: m_nCurrentState( nInitialState )
{

}

FiniteStateMachine::~FiniteStateMachine()
{

}


void FiniteStateMachine::input( int nEventID )
{
	_processInputEvent( nEventID );
}

void FiniteStateMachine::_changeState( int nDestState )
{
	if( m_nCurrentState == nDestState )
		return;

	_leaveCurrState();

	m_nCurrentState = nDestState;

	_enterNewState();
}




}


