//DrawingForm.c
#include "DrawingForm.h"
#include "resource.h"
#include "WordBook.h"
#include <stdio.h>
#pragma warning(disable : 4996)

BOOL CALLBACK DrawingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL result;

	switch (message) {
	case WM_INITDIALOG: result = DrawingForm_OnInitDialog(hWnd, wParam, lParam); break;
	case WM_CLOSE: result = DrawingForm_OnClose(hWnd, wParam, lParam); break;
	default: result = FALSE; break;
	}

	return result;
}

BOOL DrawingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;

	WordBook* wordBook = NULL;
	Word* index = NULL;
	Word word;
	Long number;
	TCHAR numberText[64];
	TCHAR countText[64];

	// 메인윈도우를 찾는다.
	wordBookFormWindow = FindWindow("#32770", "단어장");

	// 메인윈도우의 단어장에서 현재 단어를 빼낸다.
	wordBook = (WordBook*)GetWindowLong(wordBookFormWindow, GWL_USERDATA);
	word = WordBook_Draw(wordBook, wordBook->current);

	// 빼낸 단어를 쓴다.
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)word.spelling);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)word.meaning);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)word.category);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)word.example);

	// 단어장에 단어가 있으면 메인윈도우에 쓴다.
	if (wordBook->current != NULL) {
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, 
			(LPARAM)wordBook->current->spelling);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0,
			(LPARAM)wordBook->current->meaning);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0,
			(LPARAM)wordBook->current->category);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0,
			(LPARAM)wordBook->current->example);
	}
	else {
		// 없으면 메인윈도우의 단어의 내용을 지운다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
	}

	// 메인윈도우의 순번을 정한다.
	number = (Long)GetProp(wordBookFormWindow, "PROP_NUMBER");
	if (number > wordBook->length) {
		number = wordBook->length;
	}

	SetProp(numberText, "%d", number);

	// 메인윈도우의 순번을 쓴다.
	sprintf(numberText, "%d", number);
	SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM) numberText);
	
	// 메인윈도우에 개수를 쓴다.
	sprintf(countText, "%d", wordBook->length);
	SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);

	return TRUE;

}

BOOL DrawingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook* wordBook = NULL;
	Word word;
	Word* index = NULL;
	TCHAR numberText[64];
	TCHAR countText[64];
	int response;

	// 메시지박스를 띄운다.
	response = MessageBox(hWnd, "다시 끼우시겠습니까?", "확인", MB_YESNOCANCEL);
	if (response == IDYES) {
		// 단어를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_GETTEXT, (WPARAM)48, (LPARAM)word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_GETTEXT, (WPARAM)48, (LPARAM)word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_GETTEXT, (WPARAM)48, (LPARAM)word.category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_GETTEXT, (WPARAM)48, (LPARAM)word.example);

		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");
		// 메인윈도우의 단어장에 끼운다.
		wordBook = (WordBook*)GetWindowLong(wordBookFormWindow, GWL_USERDATA);
		index = WordBook_Put(wordBook, word);
		// 메인윈도우에 끼운 단어를 쓴다
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->spelling);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->meaning);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)index->category);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->example);

		// 메인윈도우에서 순번을 정한다.
		SetProp(wordBookFormWindow, "PROP_NUMBER", wordBook->length);

		// 메인윈도우에서 순번을 쓴다.
		sprintf(numberText, "%d", wordBook->length);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// 메인윈도우에서 개수를 쓴다.
		sprintf(countText, "%d", wordBook->length);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);

		EndDialog(hWnd, 0);
	}
	else if (response == IDNO) {
		EndDialog(hWnd, 0);
	}

	return TRUE;
}