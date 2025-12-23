#pragma once
#include <afxwin.h>
#include <adslib.h>
#include <aced.h>
#include <acedads.h>
#include <dbobjptr.h> 
#include "resource.h"

class MyDialog : public CDialog
{
private:
    // The Backpack
    AcDbObjectIdArray m_selectedIds;

public:
    // Constructor Promise
    MyDialog(CWnd* pParent = nullptr);

    // Message Map Promise
    DECLARE_MESSAGE_MAP()

    // Function Promises (Prototypes only!)
    afx_msg void OnSelectionModePressed();
    afx_msg void OnObjectProcessPressed();
    afx_msg void OnClosePressed();
};