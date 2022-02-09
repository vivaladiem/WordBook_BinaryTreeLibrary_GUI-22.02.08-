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

	// ���������츦 ã�´�.
	wordBookFormWindow = FindWindow("#32770", "�ܾ���");

	// ������������ �ܾ��忡�� ���� �ܾ ������.
	wordBook = (WordBook*)GetWindowLong(wordBookFormWindow, GWL_USERDATA);
	word = WordBook_Draw(wordBook, wordBook->current);

	// ���� �ܾ ����.
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)word.spelling);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)word.meaning);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)word.category);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)word.example);

	// �ܾ��忡 �ܾ ������ ���������쿡 ����.
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
		// ������ ������������ �ܾ��� ������ �����.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
	}

	// ������������ ������ ���Ѵ�.
	number = (Long)GetProp(wordBookFormWindow, "PROP_NUMBER");
	if (number > wordBook->length) {
		number = wordBook->length;
	}

	SetProp(numberText, "%d", number);

	// ������������ ������ ����.
	sprintf(numberText, "%d", number);
	SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM) numberText);
	
	// ���������쿡 ������ ����.
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

	// �޽����ڽ��� ����.
	response = MessageBox(hWnd, "�ٽ� ����ðڽ��ϱ�?", "Ȯ��", MB_YESNOCANCEL);
	if (response == IDYES) {
		// �ܾ �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_GETTEXT, (WPARAM)48, (LPARAM)word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_GETTEXT, (WPARAM)48, (LPARAM)word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_GETTEXT, (WPARAM)48, (LPARAM)word.category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_GETTEXT, (WPARAM)48, (LPARAM)word.example);

		// ���������츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");
		// ������������ �ܾ��忡 �����.
		wordBook = (WordBook*)GetWindowLong(wordBookFormWindow, GWL_USERDATA);
		index = WordBook_Put(wordBook, word);
		// ���������쿡 ���� �ܾ ����
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->spelling);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->meaning);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)index->category);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->example);

		// ���������쿡�� ������ ���Ѵ�.
		SetProp(wordBookFormWindow, "PROP_NUMBER", wordBook->length);

		// ���������쿡�� ������ ����.
		sprintf(numberText, "%d", wordBook->length);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// ���������쿡�� ������ ����.
		sprintf(countText, "%d", wordBook->length);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);

		EndDialog(hWnd, 0);
	}
	else if (response == IDNO) {
		EndDialog(hWnd, 0);
	}

	return TRUE;
}