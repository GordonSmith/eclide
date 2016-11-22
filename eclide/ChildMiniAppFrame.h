#pragma once

#include "WorkspaceItem.h"
#include <MiniApp.h>

class CMainFrame;
HWND OpenMiniAppMDI(CMainFrame* pFrame, IMiniApp * miniApp, IWorkspaceItem * workspaceItem);
