// ============================================================================
//  mbCmd_Variables
// ============================================================================

#include "mbPrecompile.h"
#include "mbCmd_Defs.h"
#include "mbException.h"
#include "mbVariables.h"





// ============================================================================
//	Path Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(Path);

void mbCmdPath::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	bool normalize = ParseBool(wxT("Normalize"), true);
	wxString id = ParseProp(wxT("Id")).Trim().Trim(false);
	wxString dir = ParseProp(wxT("Dir"), true).Trim().Trim(false);
	wxString file = ParseProp(wxT("File"), true).Trim().Trim(false);
	wxString relative = ParseProp(wxT("Relative"), true).Trim().Trim(false);

	if( dir.empty() && file.empty() )
		ThrowError(wxT("Resulting path is empty!"));

	wxFileName filename;
	if( dir.empty() )
		filename = wxFileName(file);
	else if( file.empty() )
		filename = wxFileName::DirName(dir);
	else
		filename = wxFileName(dir, file);

	if( normalize )
	{
		if( !filename.Normalize() && !optional )
			ThrowError(wxT("Failed to normalize path %s"), filename.GetFullName().c_str() );
	}

	wxString path = filename.GetFullPath();

	if( !relative.empty() )
	{
		if( relative.size() < path.size() && relative.IsSameAs(path.Left(relative.size()),false) )
			path = path.Mid(relative.size());
		else
			ThrowError(wxT("attribute Relative does not match the path."));
	}

	path = path.Trim().Trim(false);
	if( path.empty() && !optional )
		ThrowError(wxT("Resulting path is empty!"));

	PrintExecute(stack, wxT("%s = \"%s\""), id.c_str(), path.c_str() );
	GmbVariables().SetGlobal(id, path);

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	RequireVar Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(RequireVar);

void mbCmdRequireVar::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"));
	bool quiet = ParseBool(wxT("Quiet"), true);

	if( !quiet )
		PrintExecute(stack, wxT("Id = \"%s\""),  id.c_str() );

	// Check variable
	wxString value;
	if( !GmbVariables().Get(id, value) )
	{
		ThrowError(wxT("Required variable not found: %s"), id.c_str());
	}

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	Var Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(Var);

void mbCmdVar::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"));
	wxString value = ParseProp(wxT("InnerText"), true);
	bool quiet = ParseBool(wxT("Quiet"), true);

	if( !quiet )
		PrintExecute(stack, wxT("%s = \"%s\""),  id.c_str(), value.c_str() );

	GmbVariables().SetGlobal(id, value);

	MB_CMD_UNGUARD_OPT;
}


// ============================================================================
//  EOF
// ============================================================================