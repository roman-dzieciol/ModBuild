// ============================================================================
//  mbCmdFactory
// ============================================================================

#include "mbPrecompile.h"
#include "mbCmd_Defs.h"
#include "mbCmdFactory.h"
#include "mbException.h"


// ============================================================================
//	mbCmdFactory
// ============================================================================
mbCmdFactory::mbCmdFactory()
{
	MB_CMD_REGISTER(Action);
	MB_CMD_REGISTER(ActionRef);
	MB_CMD_REGISTER(Comment);
	MB_CMD_REGISTER(DirEmpty);
	MB_CMD_REGISTER(DirExists);
	MB_CMD_REGISTER(DirMake);
	MB_CMD_REGISTER(DirRemove);
	MB_CMD_REGISTER(DirRename);
	MB_CMD_REGISTER(FileConcat);
	MB_CMD_REGISTER(FileCopy);
	MB_CMD_REGISTER(FileExists);
	MB_CMD_REGISTER(FileRemove);
	MB_CMD_REGISTER(FileRename);
	MB_CMD_REGISTER(FileWrite);
	MB_CMD_REGISTER(ForEachDir);
	MB_CMD_REGISTER(ForEachFile);
	MB_CMD_REGISTER(GetSVNVersion);
	MB_CMD_REGISTER(Hardlink);
	MB_CMD_REGISTER(Ignore);
	MB_CMD_REGISTER(Launch);
	MB_CMD_REGISTER(MD5File);
	MB_CMD_REGISTER(MD5String);
	MB_CMD_REGISTER(ModBuild);
	MB_CMD_REGISTER(ModBuildActions);
	MB_CMD_REGISTER(Path);
	MB_CMD_REGISTER(RequireVar);
	MB_CMD_REGISTER(Sandbox);
	MB_CMD_REGISTER(Sequence);
	MB_CMD_REGISTER(SetCurrentDir);
	MB_CMD_REGISTER(Var);
	MB_CMD_REGISTER(xmlAttributeSet);
	MB_CMD_REGISTER(xmlAttributeRemove);
	MB_CMD_REGISTER(xmlDocument);
	MB_CMD_REGISTER(xmlElementAdd);
	MB_CMD_REGISTER(xmlElementRemove);
	MB_CMD_REGISTER(xmlForEach);
}

void mbCmdFactory::Add( const wxString& name, mbCmdCreator p )
{
	mbCmdFactoryMap::iterator it = m_commands.find(name);
	if( it != m_commands.end() )
		throw mbException( wxT("Command with this name already exists: %s"), name.c_str() );

	m_commands[name] = p;
}

mbCmd* mbCmdFactory::Get( const wxString& name )
{
	mbCmdFactoryMap::iterator it = m_commands.find(name);
	if( it != m_commands.end() )
		return ((*it).second)();

	throw mbException( wxT("Unknown command requested: <%s>"), name.c_str() );
}

// ============================================================================
//	mbCmdFactory - Static
// ============================================================================
mbCmdFactory& mbCmdFactory::Global()
{
	// Create on first use
	static mbCmdFactory p;
	return p;
}


// ============================================================================
//  EOF
// ============================================================================