#include "MyDialog.h"

BEGIN_MESSAGE_MAP(MyDialog, CDialog)
    ON_BN_CLICKED(IDC_BTN_SELECT_MODE, &MyDialog::OnSelectionModePressed)
    ON_BN_CLICKED(IDC_BTN_PROCESS_OBJ, &MyDialog::OnObjectProcessPressed)
    ON_BN_CLICKED(IDC_BTN_CLOSE, &MyDialog::OnClosePressed)
END_MESSAGE_MAP()

MyDialog::MyDialog(CWnd* pParent /*=nullptr*/)
    : CDialog(IDD_DIALOG1, pParent)
{
    if (pParent == nullptr) {
        m_pParentWnd = CWnd::FromHandle(adsw_acadMainWnd());
    }
}


void MyDialog::OnSelectionModePressed()
{
    // 1. Lock State
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // 2. Get AutoCAD Window DIRECTLY (No more guessing with GetParent)
    CWnd* pAcadWnd = CWnd::FromHandle(adsw_acadMainWnd());

    // 3. Hide the Dialog
    ShowWindow(SW_HIDE);

    // 4. HAND OVER CONTROL (Crucial Step)
    // We manually enable AutoCAD and Force the mouse cursor to it.
    if (pAcadWnd) {
        pAcadWnd->EnableWindow(TRUE);
        pAcadWnd->SetFocus();
    }

    // 5. Run Selection
    m_selectedIds.setLogicalLength(0);
    ads_name selectionSet;
    int res = acedSSGet(NULL, NULL, NULL, NULL, selectionSet);

    // 6. TAKE BACK CONTROL
    // Freeze AutoCAD again so the user can't click it
    if (pAcadWnd) {
        pAcadWnd->EnableWindow(FALSE);
    }

    // 7. Show Dialog & Force Top
    ShowWindow(SW_SHOW);
    SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetActiveWindow();

    // 8. Logic...
    if (res == RTNORM)
    {
        Adesk::Int32 length = 0;
        acedSSLength(selectionSet, &length);

        for (Adesk::Int32 i = 0; i < length; ++i)
        {
            ads_name entityName;
            acedSSName(selectionSet, i, entityName);
            AcDbObjectId objId;
            acdbGetObjectId(objId, entityName);
            m_selectedIds.append(objId);
        }
        acedSSFree(selectionSet);

        CString msg;
        msg.Format(L"Selected %d Objects.", m_selectedIds.length());

        if (GetDlgItem(IDC_STATIC_STATUS)) {
            SetDlgItemText(IDC_STATIC_STATUS, msg);
        }
    }
}

void MyDialog::OnObjectProcessPressed()
{
    AfxMessageBox(L"Processing selected objects test. Logic coming soon!");
}

void MyDialog::OnClosePressed()
{
    CDialog::OnCancel();
}