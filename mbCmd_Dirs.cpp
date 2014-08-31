// ============================================================================
//  mbCmd_Dirs
// ============================================================================

#include "mbPrecompile.h"
#include "mbCmd_Defs.h"
#include "mbException.h"
#include "mbSandbox.h"



// ============================================================================
//	DirEmpty Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(DirEmpty);

void mbCmdDirEmpty::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"));

	PrintExecute(stack, wxT("%s"), id.c_str() );

	if( wxDirExists(id) && (wxDir(id).HasFiles() || wxDir(id).HasSubDirs()) && !optional )
		ThrowError(wxT("Failed: %s"), id.c_str() );

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	DirExists Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(DirExists);

void mbCmdDirExists::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"));

	PrintExecute(stack, wxT("%s"), id.c_str() );

	if( wxDirExists(id) )
	{
		// Execute children
		ExecuteChildren(stack);
	}
	else if( !optional )
	{
		ThrowError(wxT("Failed: %s"), id.c_str() );
	}


	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	DirMake Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(DirMake);

void mbCmdDirMake::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"));

	PrintExecute(stack, wxT("%s"), id.c_str());

	GmbSandbox().Validate(id);

	if( !wxDirExists(id) )
	{
		if( !wxMkdir(id) && !optional )
			ThrowError(wxT("Failed: %s"), id.c_str());
	}

	MB_CMD_UNGUARD_OPT;
}


// ============================================================================
//	DirRemove Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(DirRemove);

void mbCmdDirRemove::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"), false);

	PrintExecute(stack, wxT("%s"), id.c_str() );

	GmbSandbox().Validate(id);

	if( !wxFileName::Rmdir(id) && !optional )
		ThrowError(wxT("Failed: %s"), id.c_str() );

	MB_CMD_UNGUARD_OPT;
}


// ============================================================================
//	DirRename Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(DirRename);

void mbCmdDirRename::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString from = ParseProp(wxT("From"));
	wxString to = ParseProp(wxT("To"));
	wxString file = ParseProp(wxT("File"), true);
	bool overwrite = ParseBool(wxT("Overwrite"), true);

	if( !file.empty() )
	{
		from += file;
		to += file;
	}

	PrintExecute(stack, wxT("%s >> %s"), from.c_str(), to.c_str() );

	GmbSandbox().Validate(to);
	GmbSandbox().Validate(from);

	if( !wxRenameFile(from, to, overwrite) && !optional )
		ThrowError(wxT("Failed: %s >> %s"), from.c_str(), to.c_str() );

	MB_CMD_UNGUARD_OPT;
}


// ============================================================================
//  EOF
// ============================================================================