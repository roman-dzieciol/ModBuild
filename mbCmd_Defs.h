// ============================================================================
//  mbCmd_Defs
// ============================================================================
#pragma once

#include "mbCmd.h"
#include "mbCmdXml.h"


// ============================================================================
//	Commands
// ============================================================================

MB_CMD_IMPLEMENT(mbCmd, Action);
MB_CMD_IMPLEMENT(mbCmd, ActionRef);
MB_CMD_IMPLEMENT(mbCmd, Comment);
MB_CMD_IMPLEMENT(mbCmd, DirEmpty);
MB_CMD_IMPLEMENT(mbCmd, DirExists);
MB_CMD_IMPLEMENT(mbCmd, DirMake);
MB_CMD_IMPLEMENT(mbCmd, DirRemove);
MB_CMD_IMPLEMENT(mbCmd, DirRename);
MB_CMD_IMPLEMENT(mbCmd, FileConcat);
MB_CMD_IMPLEMENT(mbCmd, FileCopy);
MB_CMD_IMPLEMENT(mbCmd, FileExists);
MB_CMD_IMPLEMENT(mbCmd, FileRemove);
MB_CMD_IMPLEMENT(mbCmd, FileRename);
MB_CMD_IMPLEMENT(mbCmd, FileWrite);
MB_CMD_IMPLEMENT(mbCmd, ForEachDir);
MB_CMD_IMPLEMENT(mbCmd, ForEachFile);
MB_CMD_IMPLEMENT(mbCmd, GetSVNVersion);
MB_CMD_IMPLEMENT(mbCmd, Hardlink);
MB_CMD_IMPLEMENT(mbCmd, Ignore);
MB_CMD_IMPLEMENT(mbCmd, Launch);
MB_CMD_IMPLEMENT(mbCmd, MD5File);
MB_CMD_IMPLEMENT(mbCmd, MD5String);
MB_CMD_IMPLEMENT(mbCmd, ModBuild);
MB_CMD_IMPLEMENT(mbCmd, ModBuildActions);
MB_CMD_IMPLEMENT(mbCmd, Path);
MB_CMD_IMPLEMENT(mbCmd, RequireVar);
MB_CMD_IMPLEMENT(mbCmd, Sandbox);
MB_CMD_IMPLEMENT(mbCmd, Sequence);
MB_CMD_IMPLEMENT(mbCmd, SetCurrentDir);
MB_CMD_IMPLEMENT(mbCmd, Var);
MB_CMD_IMPLEMENT(mbCmdXml, xmlAttributeSet);
MB_CMD_IMPLEMENT(mbCmdXml, xmlAttributeRemove);
MB_CMD_IMPLEMENT(mbCmdXml, xmlDocument);
MB_CMD_IMPLEMENT(mbCmdXml, xmlElementAdd);
MB_CMD_IMPLEMENT(mbCmdXml, xmlElementRemove);
MB_CMD_IMPLEMENT(mbCmdXml, xmlForEach);


// ============================================================================
//  EOF
// ============================================================================