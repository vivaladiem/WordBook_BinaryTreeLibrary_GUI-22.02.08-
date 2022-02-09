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
	// 철자 라디오버튼을 체크한다.
	SendMessage(GetDlgItem(hWnd, IDC_RADIO_SPELLING), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);

	// 뜻 에디트 컨트롤을 비활성화한다.
	EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);

	return TRUE;
}

BOOL FindingForm_OnSpellingRadioButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*indexes) = NULL;
	TCHAR numberText[64];
	TCHAR countText[64];

	if (HIWORD(wParam) == BN_CLICKED) {
		// 뜻 에디트컨트롤의 텍스트를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		// 뜻 에디트 컨트롤을 비활성화한다.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);

		// 철자 에디트 컨트롤을 활성화한다.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SPELLING), TRUE);

		// 철자 에디트컨트롤을 포커스한다.
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_SPELLING));

		// 찾은 단어들을 없앤다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		indexes = RemoveProp(hWnd, "PROP_INDEXES");
		if (indexes != NULL) {
			free(indexes);
		}

		// 순번을 정한다.
		SetProp(hWnd, "PROP_NUMBER", 0);

		// 순번을 쓴다.
		sprintf(numberText, "%d", 0);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// 개수를 쓴다.
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
		// 철자 에디트 컨트롤의 텍스트를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// 철자 에디트 컨트롤을 비활성화한다.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SPELLING), FALSE);

		// 뜻 에디트컨트롤을 활성화한다.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), TRUE);

		// 뜻 에디트 컨트롤을 포커스한다.
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_MEANING));

		// 찾은 단어들을 없앤다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		indexes = RemoveProp(hWnd, "PROP_INDEXES");
		if (indexes != NULL) {
			free(indexes);
		}

		// 순번을 정한다.
		SetProp(hWnd, "PROP_NUMBER", 0);

		// 순번을 쓴다.
		sprintf(numberText, "%d", 0);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// 개수를 쓴다.
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
		// 철자 라디오 버튼을 읽는다.
		spellingButtonState = SendMessage(GetDlgItem(hWnd, IDC_RADIO_SPELLING), BM_GETCHECK, (WPARAM)0, (LPARAM)0);

		// 철자를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)48, (LPARAM)spelling);

		// 뜻 라디오버튼을 읽는다.
		meaningButtonState = SendMessage(GetDlgItem(hWnd, IDC_RADIO_MEANING), BM_GETCHECK, (WPARAM)0, (LPARAM)0);

		// 뜻을 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)64, (LPARAM)meaning);

		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");

		// 조건에 따라 메인윈도우의 단어장에서 찾는다.
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

		// 찾아진 단어가 있으면 찾은 첫 단어를 쓴다.
		if (count > 0) {
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->spelling);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->meaning);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->category);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->example);
		}
		else {
			// 찾은 단어가 없으면 내용을 비운다.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		}

		// 순번을 정한다.
		number = 1;
		if (number > count) {
			number = count;
		}
		SetProp(hWnd, "PROP_NUMBER", number);

		// 순번을 쓴다.
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// 찾은 개수를 쓴다.
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
		// 단어의 위치를 찾는다.
		foundNumber = (Long)GetProp(hWnd, "PROP_NUMBER");
		indexes = (Word * (*)) GetProp(hWnd, "PROP_INDEXES");

		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");
		// 메인윈도우에서 위치로 이동한다.
		wordBook = (WordBook*)GetWindowLong(wordBookFormWindow, GWL_USERDATA);
		index = WordBook_Move(wordBook, indexes[foundNumber - 1]);

		// 메인윈도우에 단어를 쓴다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->spelling);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->meaning);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)index->category);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->example);

		// 위치의 순번을 찾는다.
		number = 1;
		it = WordBook_First(wordBook);
		while (it != index) {
			number++;
			it = WordBook_Next(wordBook);
		}

		SetProp(wordBookFormWindow, "PROP_NUMBER", number);

		// 메인윈도우에서 순번을 쓴다.
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// 찾기 윈도우를 닫는다.
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

		// 처음으로 이동한다.
		SetProp(hWnd, "PROP_NUMBER", 1);
		
		// 찾은 첫 단어를 쓴다.
		indexes = (Word * (*)) GetProp(hWnd, "PROP_INDEXES");

		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[0]->example);

		// 순번을 쓴다.
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
		// 이전으로 이동한다.
		number = (Long)GetProp(hWnd, "PROP_NUMBER");
		number--;
		if (number < 1) {
			number = 1;
		}

		SetProp(hWnd, "PROP_NUMBER", number);

		// 이전 단어를 쓴다.
		indexes = (Word * (*)) GetProp(hWnd, "PROP_INDEXES");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->example);

		// 순번을 쓴다.
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
		// 다음으로 이동한다.
		number = (Long)GetProp(hWnd, "PROP_NUMBER");
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		number++;
		if (number > count) {
			number = count;
		}

		SetProp(hWnd, "PROP_NUMBER", number);

		// 다음 단어를 쓴다.
		indexes = (Word * (*)) GetProp(hWnd, "PROP_INDEXES");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[number - 1]->example);

		// 순번을 쓴다.
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
		// 끝으로 이동한다.
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		SetProp(hWnd, "PROP_NUMBER", count);

		// 마지막 단어를 쓴다.
		indexes = (Word * (*)) GetProp(hWnd, "PROP_INDEXES");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[count - 1]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[count - 1]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[count - 1]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)indexes[count - 1]->example);

		// 순번을 쓴다
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