// ============================================================================
//  mbCmd_FileSystem
// ============================================================================

#include "mbPrecompile.h"
#include "mbCmd_Defs.h"
#include "mbException.h"
#include "mbVariables.h"
#include "mbSandbox.h"
#include "mbFileSystemTraverser.h"


// ============================================================================
//	ForEachFile Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(ForEachFile);

void mbCmdForEachFile::Execute(mbCommands& stack)
{
	mbFileTraverser traverser;
	wxString idref;

	MB_CMD_GUARD_OPT;
	const bool hidden = ParseBool(wxT("Hidden"), true, wxT("yes"));
	const bool recurse = ParseBool(wxT("Recurse"), true);
	const bool reverse = ParseBool(wxT("Reverse"), true);
	const wxString path = ParseProp(wxT("Path"), false);
	const wxString id = ParseProp(wxT("Id"), true, wxT("ForEach"));

	wxString flagstr;
	int flags = wxDIR_FILES;
	if( hidden )
	{
		flags |= wxDIR_HIDDEN;
		flagstr += wxT("Hidden ");
	}
	if( recurse )
	{
		flags |= wxDIR_DIRS;
		flagstr += wxT("Recurse ");
	}
	if( reverse )
	{
		flagstr += wxT("Reverse ");
	}

	wxFileName fname = wxFileName(path);
	wxString filespec = fname.GetFullName();


	PrintExecute(stack, wxT("%s %s %s"), fname.GetPathWithSep().c_str(), filespec.c_str(), flagstr.c_str() );

	traverser = mbFileTraverser::Init( fname.GetPathWithSep(), filespec, flags, reverse );
	idref = id;

	// Optional parameter does not affect ForEachFile child elements so stop here
	MB_CMD_UNGUARD_OPT;

	for( mbFileTraverser ft = traverser; ft; ++ft )
	{
		GmbVariables().PushLocal(GetDebugId(&stack), idref, ft.GetFullPath() );
		GmbVariables().PushLocal(GetDebugId(&stack), idref + wxT("?Name"), ft.GetName() );
		GmbVariables().PushLocal(GetDebugId(&stack), idref + wxT("?FullName"), ft.GetFullName() );

		for( mbCommands::iterator it=m_commands.begin(); it!=m_commands.end(); ++it )
		{
			// Special Ignore elements
			if( wxString(wxT("Ignore")).IsSameAs((*it)->GetName()) )
			{
				// Parse ignore mask, if matches current file then skip to next file
				if( wxMatchWild((*it)->ParseProp(wxT("Id"), false), ft.GetFullPath(), false) )
					break;
			}
			else
			{
				// Execute other children elements normally
				mbCmd::StaticExecute(stack, *it);
			}
		}

		GmbVariables().PopLocal(GetDebugId(&stack));
	}
}

// ============================================================================
//	ForEachDir Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(ForEachDir);

void mbCmdForEachDir::Execute(mbCommands& stack)
{
	mbDirTraverser traverser;
	wxString idref;

	MB_CMD_GUARD_OPT;
	bool hidden = ParseBool(wxT("Hidden"), true, wxT("yes"));
	bool recurse = ParseBool(wxT("Recurse"), true);
	bool reverse = ParseBool(wxT("Reverse"), true);
	wxString id = ParseProp(wxT("Id"), true, wxT("ForEach"));
	wxString path = ParseProp(wxT("Path"), false);

	wxString flagstr;
	int flags = 0;
	if( hidden )
	{
		flags |= wxDIR_HIDDEN;
		flagstr += wxT("Hidden ");
	}
	if( recurse )
	{
		flags |= wxDIR_DIRS;
		flagstr += wxT("Recurse ");
	}
	if( reverse )
	{
		flagstr += wxT("Reverse ");
	}

	wxFileName fname = wxFileName(path);
	wxString filespec = fname.GetFullName();


	PrintExecute(stack, wxT("%s %s %s"), fname.GetPathWithSep().c_str(), filespec.c_str(), flagstr.c_str() );

	traverser = mbDirTraverser::Init( fname.GetPathWithSep(), filespec, flags, reverse );
	idref = id;

	// Optional parameter does not affect ForEachFile child elements so stop here
	MB_CMD_UNGUARD_OPT;

	for( mbDirTraverser ft=traverser; ft; ++ft )
	{
		GmbVariables().PushLocal(GetDebugId(&stack), idref, ft.GetPath() );

		for( mbCommands::iterator it=m_commands.begin(); it!=m_commands.end(); ++it )
		{
			// Special Ignore elements
			if( wxString(wxT("Ignore")).IsSameAs((*it)->GetName()) )
			{
				// Parse ignore mask, if matches current file then skip to next file
				if( wxMatchWild((*it)->ParseProp(wxT("Id"), false), ft.GetPath(), false) )
					break;
			}
			else
			{
				// Execute other children elements normally
				mbCmd::StaticExecute(stack, *it);
			}
		}

		GmbVariables().PopLocal(GetDebugId(&stack));
	}
}



// ============================================================================
//	Ignore Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(Ignore);
MB_CMD_DEFAULT_EXECUTE(Ignore);


// ============================================================================
//	Sandbox Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(Sandbox);

void mbCmdSandbox::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"));

	PrintExecute(stack, wxT("%s"), id.c_str() );

	GmbSandbox().Add(id);

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	SetCurrentDir Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(SetCurrentDir);

void mbCmdSetCurrentDir::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"), false);

	PrintExecute(stack, wxT("%s"), id.c_str() );

	if( !wxSetWorkingDirectory(id) )
		ThrowError(wxT("Failed to set CWD to: %s"), id.c_str() );

	MB_CMD_UNGUARD_OPT;
}



// ============================================================================
//  EOF
// ============================================================================