// ============================================================================
//  mbCmdFactory
// ============================================================================
#pragma once

#include "mbCmd.h"


// ============================================================================
//	mbCmdFactory 
// ============================================================================
class mbCmdFactory
{
private:
	typedef mbCmd* (*mbCmdCreator)();
	WX_DECLARE_STRING_HASH_MAP( mbCmdCreator, mbCmdFactoryMap );
	mbCmdFactoryMap m_commands;

public:
	static mbCmdFactory& Global();

public:
	mbCmdFactory();

public:
	void Add( const wxString& name, mbCmdCreator p );
	mbCmd* Get( const wxString& name );

};

// ============================================================================
//	mbCmdFactory utilities
// ============================================================================

inline mbCmdFactory& GmbCmdFactory(){
	return mbCmdFactory::Global();
}


#define MB_CMD_REGISTER(name) GmbCmdFactory().Add( L#name, &mbCmd##name::Create );


// ============================================================================
//  EOF
// ============================================================================