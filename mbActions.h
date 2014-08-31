// ============================================================================
//  mbActions
// ============================================================================
#pragma once

#include "mbCmd.h"


// ============================================================================
//	mbActions 
// ============================================================================
class mbActions
{
private:
	WX_DECLARE_STRING_HASH_MAP( mbCmd*, mbActionsMap );
	mbActionsMap m_actions;

public:
	static mbActions& Global();

public:
	mbActions();

public:
	void Add( const wxString& name, mbCmd* action );
	mbCmd* Get( const wxString& name );

};

// ============================================================================
//	mbActions utilities
// ============================================================================

inline mbActions& GmbActions(){
	return mbActions::Global();
}


// ============================================================================
//  EOF
// ============================================================================