//WordBookForm.h
#ifndef _WORDBOOKFORM_H
#define _WORDBOOKFORM_H
#include <Windows.h>

BOOL CALLBACK WordBookFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnTreeViewItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnDrawButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnTestButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordBookForm_OnMemorizedWordsButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif //_WORDBOOKFORM_H