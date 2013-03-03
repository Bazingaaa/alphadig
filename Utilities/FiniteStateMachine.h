

#ifndef FINITE_STATE_MACHINE_H__
#define FINITE_STATE_MACHINE_H__


namespace AlphaDig
{

class FiniteStateMachine
{

public:
	FiniteStateMachine( int nInitialState );
	~FiniteStateMachine();

	void input( int nEventID );

	int getCurrState() const { return m_nCurrentState; }

protected:

	virtual void _processInputEvent( int nEventID ) = 0;

	void _changeState( int nDestState );

	virtual void _leaveCurrState() = 0;
	virtual void _enterNewState() = 0;

protected:

	int m_nCurrentState;
};





}




#endif

