//DrawingForm.h
#ifndef _DRAWINGFORM_H
#define _DRAWINGFORM_H
#include <Windows.h>

BOOL CALLBACK DrawingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL DrawingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL DrawingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif //_DRAWINGFORM_H