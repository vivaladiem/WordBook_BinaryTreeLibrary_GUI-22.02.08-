//FindingForm.c
#include "FindingForm.h"
#include "resource.h"
#include "WordBook.h"
#include <stdio.h>
#pragma warning(disable : 4996)

BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL result;

	switch (message) {
	case WM_INITDIALOG: result = FindingForm_OnInitDialog(hWnd, wParam, lParam); break;
	case WM_COMMAND: result = FindingForm_OnCommand(hWnd, wParam, lParam); break;
	case WM_CLOSE: result = FindingForm_OnClose(hWnd, wParam, lParam); break;
	default: result = FALSE; break;
	}

	return result;
}

BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL result;

	switch (LOWORD(wParam)) {
	case IDC_RADIO_SPELLING: result = FindingForm_OnSpellingRadioButtonClicked(hWnd, wParam, lParam); break;
	case IDC_RADIO_MEANING: result = FindingForm_OnMeaningRadioButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_FIND: result = FindingForm_OnFindButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_SELECT: result = FindingForm_OnSelectButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_FIRST: result = FindingForm_OnFirstButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_PREVIOUS: result = FindingForm_OnPreviousButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_NEXT: result = FindingForm_OnNextButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_LAST: result = FindingForm_OnLastButtonClicked(hWnd, wParam, lParam); break;
	default: result = FALSE; break;
	}

	return result;
}

BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// ö�� ������ư�� üũ�Ѵ�.
	SendMessage(GetDlgItem(hWnd, IDC_RADIO_SPELLING), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);

	// �� ����Ʈ ��Ʈ���� ��Ȱ��ȭ�Ѵ�.
	EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);

	return TRUE;
}

BOOL FindingForm_OnSpellingRadioButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*indexes) = NULL;
	TCHAR numberText[64];
	TCHAR countText[64];

	if (HIWORD(wParam) == BN_CLICKED) {
		// �� ����Ʈ��Ʈ���� �ؽ�Ʈ�� �����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		// �� ����Ʈ ��Ʈ���� ��Ȱ��ȭ�Ѵ�.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);

		// ö�� ����Ʈ ��Ʈ���� Ȱ��ȭ�Ѵ�.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SPELLING), TRUE);

		// ö�� ����Ʈ��Ʈ���� ��Ŀ���Ѵ�.
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_SPELLING));

		// ã�� �ܾ���� ���ش�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		indexes = RemoveProp(hWnd, "PROP_INDEXES");
		if (indexes != NULL) {
			free(indexes);
		}

		// ������ ���Ѵ�.
		SetProp(hWnd, "PROP_NUMBER", 0);

		// ������ ����.
		sprintf(numberText, "%d", 0);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// ������ ����.
		SetProp(hWnd, "PROP_COUNT", 0);

		sprintf(countText, "%d", 0);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);
	}

	return TRUE;
}

BOOL FindingForm_OnMeaningRadioButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*indexes) = NULL;
	TCHAR numberText[64];
	TCHAR countText[64];

	if (HIWORD(wParam) == BN_CLICKED) {
		// ö�� ����Ʈ ��Ʈ���� �ؽ�Ʈ�� �����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// ö�� ����Ʈ ��Ʈ���� ��Ȱ��ȭ�Ѵ�.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SPELLING), FALSE);

		// �� ����Ʈ��Ʈ���� Ȱ��ȭ�Ѵ�.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), TRUE);

		// �� ����Ʈ ��Ʈ���� ��Ŀ���Ѵ�.
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_MEANING));

		// ã�� �ܾ���� ���ش�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		indexes = RemoveProp(hWnd, "PROP_INDEXES");
		if (indexes != NULL) {
			free(indexes);
		}

		// ������ ���Ѵ�.
		SetProp(hWnd, "PROP_NUMBER", 0);

		// ������ ����.
		sprintf(numberText, "%d", 0);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// ������ ����.
		SetProp(hWnd, "PROP_COUNT", 0);

		sprintf(countText, "%d", 0);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);
	}

	return TRUE;
}

BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook = NULL;
	HWND wordBookFormWindow;
	int spellingButtonState;
	int meaningButtonState;
	TCHAR spelling[48];
	TCHAR meaning[64];
	Word* (*indexes) = NULL;
	Long number;
	Long count;
	TCHAR numberText[64];
	TCHAR countText[64];

	if (HIWORD(wParam) == BN_CLICKED) {
		// ö�� ���� ��ư�� �д´�.
		spellingButtonState = SendMessage(GetDlgItem(hWnd, IDC_RADIO_SPELLING), BM_GETCHECK, (WPARAM)0, (LPARAM)0);

		// ö�ڸ� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)48, (LPARAM)spelling);

		// �� ������ư�� �д´�.
		meaningButtonState = SendMessage(GetDlgItem(hWnd, IDC_RADIO_MEANING), BM_GETCHECK, (WPARAM)0, (LPARAM)0);

		// ���� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)64, (LPARAM)meaning);

		// ���������츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");

		// ���ǿ� ���� ������������ �ܾ��忡�� ã�´�.
		wordBook = (WordBook*)GetWindowLong(wordBookFormWindow, GWL_USERDATA);

		indexes = (Word * (*))RemoveProp(hWnd, "PROP_INDEXES");
		if (indexes != NULL) {
			free(indexes);
		}

		if (spellingButtonState == BST_CHECKED) {
			FindBySpelling(wordBook, spelling, &indexes, &count);
		}
		else if (meaningButtonState = BST_CHECKED) {
			FindByMeaning(wordBook, meaning, &indexes, &count);
		}

		SetProp(hWnd, "PROP_INDEXES", indexes);
		SetProp(hWnd, "PROP_COUNT", count);

		// ã���� �ܾ ������ ã�� ù �ܾ ����.
		if (count > 0) {
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->spelling);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->meaning);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->category);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->example);
		}
		else {
			// ã�� �ܾ ������ ������ ����.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		}

		// ������ ���Ѵ�.
		number = 1;
		if (number > count) {
			number = count;
		}
		SetProp(hWnd, "PROP_NUMBER", number);

		// ������ ����.
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// ã�� ������ ����.
		sprintf(countText, "%d", count);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);
		
	}
}

BOOL FindingForm_OnSelectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Long foundNumber;
	Word* (*indexes) = NULL;
	Word* index = NULL;
	HWND wordBookFormWindow;
	WordBook* wordBook = NULL;
	Long number;
	TCHAR numberText[64];
	Word* it = NULL;

	if (HIWORD(wParam) == BN_CLICKED) {
		// �ܾ��� ��ġ�� ã�´�.
		foundNumber = (Long)GetProp(hWnd, "PROP_NUMBER");
		indexes = (Word * (*)) GetProp(hWnd, "PROP_INDEXES");

		// ���������츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");
		// ���������쿡�� ��ġ�� �̵��Ѵ�.
		wordBook = (WordBook*)GetWindowLong(wordBookFormWindow, GWL_USERDATA);
		index = WordBook_Move(wordBook, indexes[foundNumber - 1]);

		// ���������쿡 �ܾ ����.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->spelling);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->meaning);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)index->category);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->example);

		// ��ġ�� ������ ã�´�.
		number = 1;
		it = WordBook_First(wordBook);
		while (it != index) {
			number++;
			it = WordBook_Next(wordBook);
		}

		SetProp(wordBookFormWindow, "PROP_NUMBER", number);

		// ���������쿡�� ������ ����.
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// ã�� �����츦 �ݴ´�.
		RemoveProp(hWnd, "PROP_INDEXES");
		if (indexes != NULL) {
			free(indexes);
		}

		RemoveProp(hWnd, "PROP_NUMBER");
		RemoveProp(hWnd, "PROP_COUNT");

		EndDialog(hWnd, 0);
	}

	return TRUE;
}

BOOL FindingForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*indexes) = NULL;
	TCHAR numberText[64];

	if (HIWORD(wParam) == BN_CLICKED) {

		// ó������ �̵��Ѵ�.
		SetProp(hWnd, "PROP_NUMBER", 1);
		
		// ã�� ù �ܾ ����.
		indexes = (Word * (*)) GetProp(hWnd, "PROP_INDEXES");

		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->example);

		// ������ ����.
		sprintf(numberText, "%d", 1);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
	}

	return TRUE;
}

BOOL FindingForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*indexes) = NULL;
	Long number;
	TCHAR numberText[64];

	if (HIWORD(wParam) == BN_CLICKED) {
		// �������� �̵��Ѵ�.
		number = (Long)GetProp(hWnd, "PROP_NUMBER");
		number--;
		if (number < 1) {
			number = 1;
		}

		SetProp(hWnd, "PROP_NUMBER", number);

		// ���� �ܾ ����.
		indexes = (Word * (*)) GetProp(hWnd, "PROP_INDEXES");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->example);

		// ������ ����.
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
	}

	return TRUE;
}

BOOL FindingForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*indexes) = NULL;
	Long number;
	Long count;
	TCHAR numberText[64];

	if (HIWORD(wParam) == BN_CLICKED) {
		// �������� �̵��Ѵ�.
		number = (Long)GetProp(hWnd, "PROP_NUMBER");
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		number++;
		if (number > count) {
			number = count;
		}

		SetProp(hWnd, "PROP_NUMBER", number);

		// ���� �ܾ ����.
		indexes = (Word * (*)) GetProp(hWnd, "PROP_INDEXES");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->example);

		// ������ ����.
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
	}

	return TRUE;
}

BOOL FindingForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*indexes) = NULL;
	TCHAR numberText[64];

	Long count;

	if (HIWORD(wParam) == BN_CLICKED) {
		// ������ �̵��Ѵ�.
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		SetProp(hWnd, "PROP_NUMBER", count);

		// ������ �ܾ ����.
		indexes = (Word * (*)) GetProp(hWnd, "PROP_INDEXES");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[count - 1]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[count - 1]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[count - 1]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[count - 1]->example);

		// ������ ����
		sprintf(numberText, "%d", count);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
	}

	return TRUE;
}


BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*indexes) = NULL;

	indexes = (Word * (*)) RemoveProp(hWnd, "PROP_INDEXES");
	if (indexes != NULL) {
		free(indexes);
	}

	RemoveProp(hWnd, "PROP_NUMBER");
	RemoveProp(hWnd, "PROP_COUNT");


	EndDialog(hWnd, 0);

	return TRUE;
}