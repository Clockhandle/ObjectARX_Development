#include "MyDialog.h"
#include "rxregsvc.h"
#include "aced.h"
#include "dbents.h" 
#include "dbsymtb.h"
#include "acdb.h"
#include <dbapserv.h>

static void ShowMyDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); //Tell AutoCAD to use MFC's module state
    MyDialog dlg;
    dlg.DoModal();
}

static void MyHelloWorldCommand() {
    acutPrintf(L"\nHello World!");
}

static void MyAddLineCommand() {
    // Define the geometry (Start Point and End Point)
    AcGePoint3d startPt(0, 0, 0);
    AcGePoint3d endPt(100, 100, 0);

    // Create the Line object in memory (it's not in the drawing yet!)
    AcDbLine* pLine = new AcDbLine(startPt, endPt);

    // Get the current database (the active DWG)
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();

    // Open the "Block Table" (the index of all spaces: Model, Layout1, etc.)
    AcDbBlockTable* pBlockTable;
    pDb->getSymbolTable(pBlockTable, AcDb::kForRead);

    // Get the "Model Space" record (where to draw)
    AcDbBlockTableRecord* pModelSpace;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pModelSpace, AcDb::kForWrite);
    pBlockTable->close();

    // Append new line to Model Space
    AcDbObjectId lineId;
    pModelSpace->appendAcDbEntity(lineId, pLine);

    // CLOSE EVERYTHING
    // AutoCAD will crash or lock the object forever.
    pLine->close();
    pModelSpace->close();

    acutPrintf(L"\nSuccess! I added a line from (0,0) to (100,100).");
}


// Main Entry Point
// AutoCAD calls ONLY this function. It passes "messages" (kInitAppMsg, kUnloadAppMsg)
// to tell you what is happening.
extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
    switch (msg) {

        // CASE 1: Loading (When typing APPLOAD)
    case AcRx::kInitAppMsg:
        // Unlock the app so you can unload it later (Crucial for debugging)
        acrxUnlockApplication(appId);

        // Tell AutoCAD we work with multiple drawings (Prevents SDI mode warning)
        acrxRegisterAppMDIAware(appId);

        // Syntax: GroupName, GlobalName, LocalName, Flags, FunctionPointer
        acedRegCmds->addCommand(
            L"MY_COMMANDS",
            L"HELLO",
            L"HELLO",
            ACRX_CMD_MODAL,
            MyHelloWorldCommand
        );

        acedRegCmds->addCommand(
            L"MY_COMMANDS",
            L"ADDLINE",
            L"ADDLINE",
            ACRX_CMD_MODAL,
            MyAddLineCommand
		);

        acedRegCmds->addCommand(
            L"MY_COMMANDS",
            L"SHOWMYDIALOG",
            L"SHOWMYDIALOG",
            ACRX_CMD_MODAL,
            ShowMyDialog
		);
        break;

        // CASE 2: Unloading
    case AcRx::kUnloadAppMsg:
        // You MUST manually remove your commands.
        // If don't, AutoCAD might crash when trying to run a command 
        // from a plugin that doesn't exist anymore.
        acedRegCmds->removeGroup(L"MY_COMMANDS");

        acutPrintf(L"\nPlugin Unloaded.");
        break;
    }

    return AcRx::kRetOK;
}