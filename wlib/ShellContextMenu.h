// ShellContextMenu.h: Schnittstelle für die Klasse CShellContextMenu.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "wlib.h"
#include <StringSet.h>

/////////////////////////////////////////////////////////////////////
// class to show shell contextmenu of files/folders/shell objects
// developed by R. Engels 2003
/////////////////////////////////////////////////////////////////////

class WLIB_API CShellContextMenu
{
public:
	CMenu * GetMenu ();
	void SetObjects (IShellFolder * psfFolder, LPITEMIDLIST pidlItem);
	void SetObjects (IShellFolder * psfFolder, LPITEMIDLIST * pidlArray, int nItemCount);
	void SetObjects (LPITEMIDLIST pidl);
	void SetObjects (const std::_tstring & strObject);
	void SetObjects (const StdStringVector & strArray);
    UINT ShowContextMenu(HWND pWnd, CPoint pt);
	CShellContextMenu();
	virtual ~CShellContextMenu();

private:
	int nItems;
	BOOL bDelete;
	CMenu * m_Menu;
	IShellFolder * m_psfFolder;
	LPITEMIDLIST * m_pidlArray;	
	
	void InvokeCommand (LPCONTEXTMENU pContextMenu, UINT idCommand);
	BOOL GetContextMenu (void ** ppContextMenu, int & iMenuType);
	static LRESULT CALLBACK HookWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void FreePIDLArray (LPITEMIDLIST * pidlArray);
	LPITEMIDLIST CopyPIDL (LPCITEMIDLIST pidl, int cb = -1);
	UINT GetPIDLSize (LPCITEMIDLIST pidl);
	LPBYTE GetPIDLPos (LPCITEMIDLIST pidl, int nPos);
	int GetPIDLCount (LPCITEMIDLIST pidl);
};
