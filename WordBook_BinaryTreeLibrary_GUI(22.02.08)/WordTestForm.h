//WordTestForm.h
#ifndef _WORDTESTFORM_H
#define _WORDTESTFORM_h
#include <Windows.h>

BOOL CALLBACK WordTestFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CALLBACK WordTestFormTimerProc(UINT uTimerId, UINT message, DWORD dwUserData, DWORD dw1, DWORD dw2);
BOOL WordTestForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordTestForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordtestForm_OnTimerTick(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordTestForm_OnMeaningLostFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordTestForm_OnCategoryLostFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordTestForm_OnExampleLostFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordTestForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordTestForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif //_WORDTESTFORM_H