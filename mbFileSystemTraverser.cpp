// ============================================================================
//  mbFileSystemTraverser
// ============================================================================
#pragma once

#include "mbPrecompile.h"
#include "mbException.h"
#include "mbFileSystemTraverser.h"


// ============================================================================
//	mbFileSystemTraverser
// ============================================================================

wxDirTraverseResult mbFileSystemTraverser::OnFile(const wxString& WXUNUSED(name))
{
	return wxDIR_CONTINUE;
}

wxDirTraverseResult mbFileSystemTraverser::OnDir(const wxString& WXUNUSED(name))
{
	return wxDIR_CONTINUE;
}

wxDirTraverseResult mbFileSystemTraverser::OnOpenError(const wxString& name)
{
	throw mbException(wxT("OnOpenError: %s"), name.c_str() );
}


// ============================================================================
//	mbDirTraverser
// ============================================================================
mbDirTraverser mbDirTraverser::Init(const wxString& dirName, const wxString& filespec, int flags, bool reverse)
{
	mbDirTraverser iter(reverse);
	wxDir(dirName).Traverse(iter, filespec, flags);
	iter.InitReverse();
	return iter;
}

wxString mbDirTraverser::GetPath() const 
{ 
	return wxFileName::DirName(m_items[m_index]).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR); 
}

wxDirTraverseResult mbDirTraverser::OnDir(const wxString& name)
{
	m_items.Add(name);
	return wxDIR_CONTINUE;
}


// ============================================================================
//	mbFileTraverser
// ============================================================================

mbFileTraverser mbFileTraverser::Init(const wxString& dirName, const wxString& filespec, int flags, bool reverse)
{
	mbFileTraverser iter(reverse);
	wxDir(dirName).Traverse(iter, filespec, flags);
	iter.InitReverse();
	return iter;
}

wxString mbFileTraverser::GetFullPath() const 
{ 
	return wxFileName(m_items[m_index]).GetFullPath(); 
}

wxString mbFileTraverser::GetFullName() const 
{ 
	return wxFileName(m_items[m_index]).GetFullName(); 
}

wxString mbFileTraverser::GetName() const 
{ 
	return wxFileName(m_items[m_index]).GetName(); 
}

wxDirTraverseResult mbFileTraverser::OnFile(const wxString& name)
{
	m_items.Add(name);
	return wxDIR_CONTINUE;
}


// ============================================================================
//  EOF
// ============================================================================