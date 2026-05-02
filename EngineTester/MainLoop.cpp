#include "StdAfx.h"
#include "MainLoop.h"
#include "DisplayManager.h"

int main()
{
	DisplayManager *displayerMgr = new DisplayManager();
	displayerMgr->CreateDisplay();
	displayerMgr->UpdateDisplay();
	displayerMgr->CloseDisplay();
	return 0;
}