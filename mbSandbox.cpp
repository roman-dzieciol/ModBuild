// ============================================================================
//  mbSandbox
// ============================================================================

#include "mbPrecompile.h"
#include "mbSandbox.h"
#include "mbException.h"


// ============================================================================
//	mbSandbox
// ============================================================================
mbSandbox::mbSandbox()
{
}

void mbSandbox::Add( const wxString& filename )
{
	m_paths.push_back(filename);
}

void mbSandbox::Validate( const wxString& filename )
{
	wxFileName fname = wxFileName(filename);
	if( !fname.Normalize() )
		throw mbException( wxT("Failed to normalize path: %s"), fname.GetFullName().c_str() );

	wxString normalized = fname.GetFullPath();
	for( mbSandboxPaths::const_iterator it=m_paths.begin(); it!=m_paths.end(); ++it )
	{
		wxString limit = (*it);
		if( normalized.Left(limit.size()).IsSameAs(limit,false) )
		{
			return;
		}
	}

	// Build error string
	wxString allowed;
	for( mbSandboxPaths::const_iterator it=m_paths.begin(); it!=m_paths.end(); ++it )
	{
		if( it != m_paths.begin() )
			allowed += wxT("\n");
		allowed += *it;
	}

	// Handle error
	throw mbException(wxT("Aborted operation on file: \n%s\n Allowed paths:\n%s"), normalized.c_str(), allowed.c_str());
}


// ============================================================================
//	mbSandbox - Static
// ============================================================================
mbSandbox& mbSandbox::Global()
{
	// Create on first use
	static mbSandbox p;
	return p;
}


// ============================================================================
//  EOF
// ============================================================================