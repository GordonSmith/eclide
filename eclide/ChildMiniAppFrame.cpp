#include "StdAfx.h"
#include "..\en_us\resource.h"

#include "ChildGraphFrame.h"
#include "WtlMDIChildFrame.h"
#include "ChildFrame.h"
#include "MainFrm.h"
#include "preferencedlg.h"
#include "global.h"
#include "util.h"
#include <HtmlView.h>

class CMiniAppHTMLView : public CHtmlView {

public: 
    CMiniAppHTMLView() : CHtmlView()
    {
    }

    virtual void __stdcall OnEventBeforeNavigate2(IDispatch * pObject, VARIANT * pvarUrl, VARIANT * pvarFlags, VARIANT * pvarTargetFrame, VARIANT * pvarData, VARIANT * pvarHeaders, VARIANT_BOOL * pbCancel)
    {
        CComVariant url(*pvarUrl);
        CString csurl = url;
        if (csurl.Find(_T("eclide")) == 0) {
            CStringArray params;
            params.Add(_T("Key_1"));
            CComVariant result;
            CallJScript(_T("miniApp_get"), params, &result);
            CString msg = _T("Mini App Action:  ") + csurl + _T("\nKey_1 = ") + result;
            MessageBox(msg);
            *pbCancel = VARIANT_TRUE;
        }
    }

    virtual void __stdcall OnEventDocumentComplete(IDispatch* pDisp, VARIANT* URL) {
        CHtmlView::OnEventDocumentComplete(pDisp, URL);
        CStringArray params;
        params.Add(_T("Key_1"));
        params.Add(_T("Val_1"));
        CComVariant result;
        CallJScript(_T("miniApp_set"), params, &result);
    }
};

class CMiniAppFrame :
    public CChildFrame
{
    typedef CMiniAppFrame thisClass;
    typedef CChildFrame baseClass;

public:
    CMiniAppHTMLView m_dlgview;
    CComPtr<IMiniApp> m_miniApp;

public:
    DECLARE_FRAME_WND_CLASS(NULL, IDR_GRAPHWINDOW)
    CMiniAppFrame(IMiniApp * miniApp, IWorkspaceItem * workspaceItem) : baseClass(workspaceItem), m_dlgview()
    {
        m_miniApp = miniApp;
    }

    BOOL PreTranslateMessage(MSG* pMsg)
    {
        if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_F4))
        {
            int d = 0;
        }
        if (pMsg->message == WM_KEYDOWN && (::GetAsyncKeyState(VK_CONTROL) & 0x8000))  // Let frame handle some messages.
        {
            switch (pMsg->wParam)
            {
            case VK_F4:
                return 0;
                break;
            case VK_F6:
                if (::GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    ::GetIMainFrame()->MDIPrev();
                else
                    ::GetIMainFrame()->MDINext();
                return 1;
                break;
            case VK_TAB:
                return 0;
                break;
            }
        }
        return m_dlgview.PreTranslateMessage(pMsg);
    }

    void OnSize(UINT nType, CSize size)
    {
        CRect rectClient;
        GetClientRect(rectClient);

        m_dlgview.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
    }

    void OnSetFocus(HWND /*hWndOther*/)
    {
        if (m_dlgview.IsWindow() && m_dlgview.IsWindowVisible())
            m_dlgview.SetFocus();
    }

    void PostSelectRibbon()
    {
    }

    BEGIN_MSG_MAP(thisClass)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_SIZE(OnSize)
        MSG_WM_SETFOCUS(OnSetFocus)
        MSG_WM_CONTEXTMENU(OnContextMenu)
        MESSAGE_HANDLER(WM_BROWSERDOCUMENTCOMPLETE, OnDocumentComplete)

        CHAIN_MSG_MAP(baseClass)
        REFLECT_NOTIFICATIONS()
        CHAIN_COMMANDS_MEMBER(m_dlgview)
    END_MSG_MAP()

    LRESULT OnCreate(LPCREATESTRUCT lParam)
    {
        m_dlgview.Create(m_hWnd, rcDefault, _T(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
        m_dlgview.CreateHTMLControl();
        m_dlgview.Navigate(m_miniApp->IndexURI());
        return baseClass::OnCreate(lParam);
    }

    virtual void UIUpdateTitle()
    {
        CString title = m_miniApp->Name();
        GetParent().SendMessage(UM_TITLECHANGED, (WPARAM)(const TCHAR *)title);
    }

    virtual bool UIUpdateMenuItems(CCmdUI * cui)
    {
        /*
        if (m_dlgview.UpdateUI(cui))
            return true;
       */
        return false;
    }

    const TCHAR * GetPath()
    {
        return m_miniApp->Name();
    }

    const TCHAR * GetTabTip(std::_tstring & tabTip)
    {
        tabTip = GetPath();
        return tabTip.c_str();
    }

    //  File Access  ---
    virtual void SavePersistInfo(CPersistMap & persistInfo);
    virtual void RestorePersistInfo(const CPersistMap & persistInfo);

    LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
    {
        LPMSG pMsg = (LPMSG)lParam;

        //if(baseClass::PreTranslateMessage(pMsg))
        //	return TRUE;

        return m_dlgview.PreTranslateMessage(pMsg);
    }


    //  IResultSlot override  ---
    void PostStatus(const TCHAR *status, PANE pane = PANE_DEFAULT)
    {
        ::GetIMainFrame()->PostStatus(status, pane);
    }

public:
    void OnContextMenu(HWND phWnd, CPoint pt)
    {
        ATLASSERT(!"Check This");
        /*
        m_dlgview.OnContextMenu(phWnd, pt);
        */
    }

    LRESULT OnDocumentComplete(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        return 0;
    }
};

void CMiniAppFrame::SavePersistInfo(CPersistMap & persistInfo)
{
    baseClass::SavePersistInfo(persistInfo);
    persistInfo.Set(PERSIST_TYPE, PERSISTVAL_MINIAPP);
}

void CMiniAppFrame::RestorePersistInfo(const CPersistMap & persistInfo)
{
    //baseClass::RestorePersistInfo(persistInfo);
}

//  ===========================================================================
class CChildMiniAppFrm : public CWtlMDIChildFrame<StlLinked<CMiniAppFrame> >

{
    typedef CChildMiniAppFrm thisClass;
    typedef CWtlMDIChildFrame<StlLinked<CMiniAppFrame> > baseClass;

public:
    CChildMiniAppFrm(IMiniApp * miniApp, IWorkspaceItem * workspaceItem)
    {
        m_view = new CMiniAppFrame(miniApp, workspaceItem);
    }

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
protected:

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CChildMiniAppFrm, baseClass)
END_MESSAGE_MAP()

// CChildMiniAppFrm diagnostics
#ifdef _DEBUG
void CChildMiniAppFrm::AssertValid() const
{
    CMDIChildWndEx::AssertValid();
}

void CChildMiniAppFrm::Dump(CDumpContext& dc) const
{
    CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG
//  ===========================================================================
HWND OpenMiniAppMDI(CMainFrame* pFrame, IMiniApp * miniApp, IWorkspaceItem * workspaceItem)
{
    CChildMiniAppFrm* pChild = new CChildMiniAppFrm(miniApp, workspaceItem);
    CMDIChildWnd * retVal = CreateNewChild(pFrame, pChild, IDR_GRAPHWINDOW, _T("todo"));
    return retVal->GetSafeHwnd();
}

