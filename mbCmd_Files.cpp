// ============================================================================
//  mbCmd_Files
// ============================================================================

#include "mbPrecompile.h"
#include "mbCmd_Defs.h"
#include "mbException.h"
#include "mbSandbox.h"
#include "mbPlatform.h"


// ============================================================================
//	FileConcat Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(FileConcat);

void mbCmdFileConcat::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"), false);
	wxString first = ParseProp(wxT("First"), false);
	wxString second = ParseProp(wxT("Second"), false);

	PrintExecute(stack, wxT("%s + %s = %s"), first.c_str(), second.c_str(), id.c_str());

	GmbSandbox().Validate(id);

	if( !wxConcatFiles(first, second, id) && !optional )
		ThrowError(wxT("%s + %s = %s"), first.c_str(), second.c_str(), id.c_str());

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	FileCopy Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(FileCopy);

void mbCmdFileCopy::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString from = ParseProp(wxT("From"), false);
	wxString to = ParseProp(wxT("To"), false);
	wxString file = ParseProp(wxT("File"), true);
	bool overwrite = ParseBool(wxT("Overwrite"), true);

	if( !file.empty() )
	{
		from += file;
		to += file;
	}

	PrintExecute(stack, wxT("%s >> %s"), from.c_str(), to.c_str());

	GmbSandbox().Validate(to);

	if( !wxCopyFile(from, to, overwrite) && !optional )
		ThrowError(wxT("%s >> %s"), from.c_str(), to.c_str());

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	FileExists Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(FileExists);

void mbCmdFileExists::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"));

	PrintExecute(stack, wxT("%s"), id.c_str() );

	if( wxFileExists(id) )
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
//	FileRemove Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(FileRemove);

void mbCmdFileRemove::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"), false);

	PrintExecute(stack, wxT("%s"), id.c_str());

	GmbSandbox().Validate(id);

	if( !wxRemoveFile(id) && !optional )
		ThrowError(wxT("%s"), id.c_str());

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	FileRename Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(FileRename);

void mbCmdFileRename::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString from = ParseProp(wxT("From"), false);
	wxString to = ParseProp(wxT("To"), false);
	wxString file = ParseProp(wxT("File"), true);
	bool overwrite = ParseBool(wxT("Overwrite"), true);

	if( !file.empty() )
	{
		from += file;
		to += file;
	}

	PrintExecute(stack, wxT("%s >> %s"), from.c_str(), to.c_str());

	GmbSandbox().Validate(from);
	GmbSandbox().Validate(to);

	if( !wxRenameFile(from, to, overwrite) && !optional )
		ThrowError(wxT("%s >> %s"), from.c_str(), to.c_str());

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	FileWrite Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(FileWrite);

enum EFileWriteMode {
	FWM_Overwrite = 0,
	FWM_Prepend,
	FWM_Append
};

void mbCmdFileWrite::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"), false);
	wxString encoding = ParseProp(wxT("Encoding"), true);
	wxString text = ParseProp(wxT("InnerText"), false);
	wxString mode = ParseProp(wxT("Mode"), true);

	// Get write mode enum
	int writemode = FWM_Overwrite;
	if( wxString(wxT("Append")).IsSameAs(mode, false) )
		writemode = FWM_Append;
	else if( wxString(wxT("Prepend")).IsSameAs(mode, false) )
		writemode = FWM_Prepend;

	PrintExecute(stack, wxT("%s"), PrintProps().c_str());

	GmbSandbox().Validate(id);

	// Fix newline chars
	text.Replace(wxT("\\n"), wxTextFile::GetEOL(), true);

	// Pick conversion
	wxMBConv& conv = wxConvUTF8;
	if( wxString(wxT("ANSI")).IsSameAs(encoding, false) )
		conv = wxConvLocal;


	wxFile file;
	if( writemode == FWM_Overwrite || !wxFileExists(id) )
	{
		if( !file.Open(id, wxFile::write) )
			ThrowError(wxT("Failed to open: %s"), id.c_str() );

		if( !file.Write(text, conv) )
			ThrowError(wxT("Failed to write to: %s"), id.c_str() );
	}
	else if( writemode == FWM_Append )
	{
		if( !file.Open(id, wxFile::write_append) )
			ThrowError(wxT("Failed to open: %s"), id.c_str() );

		if( !file.Write(text, conv) )
			ThrowError(wxT("Failed to write to: %s"), id.c_str() );
	}
	else if( writemode == FWM_Prepend )
	{
		wxTempFile out;
		if( !out.Open(id) )
			ThrowError(wxT("Failed to open temp: %s"), id.c_str() );

		if( !out.Write(text, conv) )
			ThrowError(wxT("Failed to write temp: %s"), id.c_str() );

		if( !file.Open(id, wxFile::read) )
			ThrowError(wxT("Failed to open: %s"), id.c_str() );

		ssize_t ofs;
		unsigned char buf[1024];

		do{
			ofs = file.Read(buf,WXSIZEOF(buf));
			if( ofs == wxInvalidOffset ) 
				ThrowError(wxT("Failed to read: %s"), id.c_str() );

			if( ofs > 0 && !out.Write(buf,ofs))
				ThrowError(wxT("Failed to write temp: %s"), id.c_str() );
					
		} while ( ofs == static_cast<ssize_t>(WXSIZEOF(buf)) );


		if( !file.Close() )
			ThrowError(wxT("Failed to close: %s"), id.c_str() );

		if( !out.Commit() )
			ThrowError(wxT("Failed to close temp: %s"), id.c_str() );
	}

	MB_CMD_UNGUARD_OPT;
}


// ============================================================================
//	Hardlink Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(Hardlink);

void mbCmdHardlink::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString from = ParseProp(wxT("From"), false);
	wxString to = ParseProp(wxT("To"), false);
	wxString file = ParseProp(wxT("File"), true);
	bool overwrite = ParseBool(wxT("Overwrite"), true);

	if( !file.empty() )
	{
		from += file;
		to += file;
	}

	PrintExecute(stack, wxT("%s >> %s"), from.c_str(), to.c_str() );

	GmbSandbox().Validate(to);

	// Handle overwriting
	if( wxFileExists(to) )
	{
		if( overwrite )
		{
			if( !wxRemoveFile(to) )
				ThrowError(wxT("Could not remove: %s"), to.c_str() );
		}
		else
			ThrowError(wxT("File already exists: %s"), to.c_str() );
	}
	
	if( !mbCreateHardLink(from, to) && !optional )
		ThrowError(wxT("Failed: %s >> %s"), from.c_str(), to.c_str() );

	MB_CMD_UNGUARD_OPT;
}


// ============================================================================
//  EOF
// ============================================================================