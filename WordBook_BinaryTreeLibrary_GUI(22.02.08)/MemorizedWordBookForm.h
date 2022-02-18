//MemorizedWordBookForm.h
#ifndef _MEMORIZEDWORDBOOKFORM_H
#define _MEMORIZEDWORDBOOKFORM_H
#include <Windows.h>

BOOL CALLBACK MemorizedWordBookFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordBookForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordBookForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordBookForm_OnShowAnswerButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
//BOOL MemorizedWordBookForm_OnWordGroupBoxClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordBookForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordBookForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordBookForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordBookForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM LParam);
BOOL MemorizedWordBookForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordBookForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif //_MEMORIZEDWORDBOOKFORM_H