// ============================================================================
//  mbActions
// ============================================================================

#include "mbPrecompile.h"
#include "mbActions.h"
#include "mbException.h"


// ============================================================================
//	mbActions
// ============================================================================
mbActions::mbActions()
{
}

void mbActions::Add( const wxString& name, mbCmd* action )
{
	mbActionsMap::iterator it = m_actions.find(name);
	if( it != m_actions.end() )
		throw mbException( wxT("Action with this name already exists: %s"), name.c_str() );

	m_actions[name] = action;
}

mbCmd* mbActions::Get( const wxString& name )
{
	mbActionsMap::iterator it = m_actions.find(name);
	if( it != m_actions.end() )
		return (*it).second;

	throw mbException( wxT("Unknown Action requested: %s"), name.c_str() );
}

// ============================================================================
//	mbActions - Static
// ============================================================================
mbActions& mbActions::Global()
{
	// Create on first use
	static mbActions p;
	return p;
}


// ============================================================================
//  EOF
// ============================================================================