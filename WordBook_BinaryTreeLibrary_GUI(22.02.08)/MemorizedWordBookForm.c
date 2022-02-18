//MemorizedWordBookForm.c
#include "MemorizedWordBookForm.h"
#include "WordBook.h"
#include "WordIndexCardFile.h"
#include "WordIndexCard.h"
#include "resource.h"
#include <CommCtrl.h>
#include <string.h>
#include <stdio.h>
#pragma warning(disable : 4996)

BOOL CALLBACK MemorizedWordBookFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG: ret = MemorizedWordBookForm_OnInitDialog(hWnd, wParam, lParam); break;
	case WM_COMMAND: ret = MemorizedWordBookForm_OnCommand(hWnd, wParam, lParam); break;
	case WM_CLOSE: ret = MemorizedWordBookForm_OnClose(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}

	return ret;
}

BOOL MemorizedWordBookForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_BUTTON_SHOWANSWER: ret = MemorizedWordBookForm_OnShowAnswerButtonClicked(hWnd, wParam, lParam); break;
	//case IDC_STATIC_WORD: ret = MemorizedWordBookForm_OnWordGroupBoxClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_PUT: ret = MemorizedWordBookForm_OnPutButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_FIRST: ret = MemorizedWordBookForm_OnFirstButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_PREVIOUS: ret = MemorizedWordBookForm_OnPreviousButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_NEXT: ret = MemorizedWordBookForm_OnNextButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_LAST: ret = MemorizedWordBookForm_OnLastButtonClicked(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}

	return ret;
}

BOOL MemorizedWordBookForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *memorizedWordBook;
	Word *wordLink;
	TCHAR countText[64];

	// 메인윈도우를 찾는다.
	wordBookFormWindow = FindWindow("#32770", "단어장");

	// 메인윈도우의 외운단어장에서 사용량이 0보다 크면
	memorizedWordBook = (WordBook*)GetProp(wordBookFormWindow, "PROP_MEMORIZEDWORDBOOK");
	if (memorizedWordBook->length > 0) {
		// 메인윈도우의 외운단어장에서 처음으로 간다.
		wordLink = WordBook_First(memorizedWordBook);

		// 철자를 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);

		// 개수를 쓴다.
		sprintf(countText, "%d개", memorizedWordBook->length);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);
	}

	return TRUE;
}


BOOL MemorizedWordBookForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *memorizedWordBook;
	WordBook *wordBook;
	WordIndexCardFile *wordIndexCardFile;
	Word word;
	Word *wordLink;
	WordIndexCard *wordIndexCardLink;
	TCHAR countText[64];
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet = NULL;
	HTREEITEM hTvSpelling = NULL;
	HTREEITEM hTvCategory = NULL;
	HTREEITEM hTvMeaning = NULL;
	HTREEITEM hTvIt;
	TVITEM tvItem = { 0, };
	TVINSERTSTRUCT tvInsertStruct = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];

	if (HIWORD(wParam) == BN_CLICKED) {
		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");
		
		// 메인윈도우의 외운 단어장에서 단어를 뺴낸다.
		memorizedWordBook = (WordBook*)GetProp(wordBookFormWindow, "PROP_MEMORIZEDWORDBOOK");
		word = WordBook_Draw(memorizedWordBook, memorizedWordBook->current);
		
		// 메인윈도우의 단어장에 단어를 끼운다.
		wordBook = (WordBook*)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
		wordLink = WordBook_Put(wordBook, word);

		// 메인윈도우의 색인철에 단어를 끼운다.
		wordIndexCardFile = (WordIndexCardFile*)GetProp(wordBookFormWindow, "PROP_WORDINDEXCARDFILE");
		wordIndexCardLink = WordIndexCardFile_Put(wordIndexCardFile, wordLink);
		
		// 메인윈도우의 외운단어장에 현재 단어가 있으면 철자를 쓴다.
		if (memorizedWordBook->current != NULL) {
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, 
				(LPARAM)memorizedWordBook->current->spelling);
		}
		else {
			// 없으면 철자를 지운다.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		}

		// 뜻, 품사, 예시를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		
		// 개수를 쓴다.
		sprintf(countText, "%d개", memorizedWordBook->length);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);

		// 클릭횟수를 리셋한다.
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)0);

		// 메인윈도우의 트리뷰에서 단어들 항목을 찾는다.
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
			(WPARAM)TVGN_ROOT, (LPARAM)0);

		// 메인윈도우의 트리뷰의 단어들 항목에서 알파벳 항목을 찾는다.
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)hTvRoot);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && (wordIndexCardLink->alphabet >= alphabet[0])) {
			hTvAlphabet = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// 알파벳항목이 없으면 추가한다.
		tvItem.hItem = hTvAlphabet;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		if (hTvAlphabet == NULL || wordIndexCardLink->alphabet != alphabet[0]) {
			tvInsertStruct.hParent = hTvRoot;
			if (hTvAlphabet != NULL) {
				tvInsertStruct.hInsertAfter = hTvAlphabet;
			}
			else {
				tvInsertStruct.hInsertAfter = TVI_FIRST;
			}

			alphabet[0] = wordIndexCardLink->alphabet;
			alphabet[1] = '\0';
			tvItem.pszText = alphabet;
			tvInsertStruct.item = tvItem;
			hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
		}

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 항목에서 철자 항목을 찾는다.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->spelling, spelling) >= 0) {
			hTvSpelling = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// 철자항목이 없으면 추가한다.
		tvItem.hItem = hTvSpelling;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		if (hTvSpelling == NULL || strcmp(wordLink->spelling, spelling) != 0) {
			tvInsertStruct.hParent = hTvAlphabet;
			if (hTvSpelling != NULL) {
				tvInsertStruct.hInsertAfter = hTvSpelling;
			}
			else {
				tvInsertStruct.hInsertAfter = TVI_FIRST;
			}
			tvItem.pszText = wordLink->spelling;
			tvInsertStruct.item = tvItem;
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
		}

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 항목에서 품사 항목을 찾는다.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->category, category) >= 0) {
			hTvCategory = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// 품사 항목이 없으면 추가한다.
		tvItem.hItem = hTvCategory;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		if (hTvCategory == NULL || strcmp(wordLink->category, category) != 0) {
			tvInsertStruct.hParent = hTvSpelling;
			if (hTvCategory != NULL) {
				tvInsertStruct.hInsertAfter = hTvCategory;
			}
			else {
				tvInsertStruct.hInsertAfter = TVI_FIRST;
			}
			tvItem.pszText = wordLink->category;
			tvInsertStruct.item = tvItem;
			hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
		}

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 항목에서 뜻 항목을 추가할 위치를 찾는다.
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
			(WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->meaning, meaning) >= 0) {
			hTvMeaning = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// 뜻 항목을 추가한다.
		tvInsertStruct.hParent = hTvCategory;
		if (hTvMeaning != NULL) {
			tvInsertStruct.hInsertAfter = hTvMeaning;
		}
		else {
			tvInsertStruct.hInsertAfter = TVI_FIRST;
		}
		tvItem.pszText = wordLink->meaning;
		tvInsertStruct.item = tvItem;
		hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
	}

	return TRUE;
}


BOOL MemorizedWordBookForm_OnShowAnswerButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *memorizedWordBook;
	Word *wordLink;
	Long clickCount;

	if (HIWORD(wParam) == BN_CLICKED) {
		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");

		// 메인윈도우의 외운단어장에서 현재 단어가 있으면
		memorizedWordBook = (WordBook*)GetProp(wordBookFormWindow, "PROP_MEMORIZEDWORDBOOK");
		wordLink = memorizedWordBook->current;

		if (wordLink != NULL) {
			// 클릭횟수를 센다.
			clickCount = (Long)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			clickCount++;

			// 클릭횟수에 따라 뜻, 품사, 예시를 쓴다.
			if (clickCount == 1) {
				SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
			}
			else if (clickCount == 2) {
				SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
			}
			else if (clickCount == 3) {
				SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);
			}
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)clickCount);
		}
	}

	return TRUE;
}


BOOL MemorizedWordBookForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *memorizedWordBook;
	Word *wordLink;

	if (HIWORD(wParam) == BN_CLICKED) {
		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");
		
		// 메인윈도우의 외운 단어장에서 처음으로 간다.
		memorizedWordBook = (WordBook*)GetProp(wordBookFormWindow, "PROP_MEMORIZEDWORDBOOK");
		wordLink = WordBook_First(memorizedWordBook);
		
		// 철자를 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);

		// 뜻, 품사, 예시를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// 클릭횟수를 리셋한다.
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)0);
	}

	return TRUE;
}

BOOL MemorizedWordBookForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *memorizedWordBook;
	Word *wordLink;
	
	if (HIWORD(wParam) == BN_CLICKED) {
		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");

		// 메인윈도우의 외운 단어장에서 이전으로 간다.
		memorizedWordBook = (WordBook*)GetProp(wordBookFormWindow, "PROP_MEMORIZEDWORDBOOK");
		wordLink = WordBook_Previous(memorizedWordBook);
		
		// 철자를 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);

		// 뜻, 품사, 예시를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// 클릭 횟수를 리셋한다.
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)0);

	}

	return TRUE;
}

BOOL MemorizedWordBookForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *memorizedWordBook;
	Word *wordLink;

	if (HIWORD(wParam) == BN_CLICKED) {
		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");

		// 메인윈도우의 외운 단어장에서 다음으로 간다.
		memorizedWordBook = (WordBook*)GetProp(wordBookFormWindow, "PROP_MEMORIZEDWORDBOOK");
		wordLink = WordBook_Next(memorizedWordBook);

		// 철자를 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);

		// 뜻, 품사, 예시를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// 클릭횟수를 리셋한다.
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)0);

	}


	return TRUE;
}

BOOL MemorizedWordBookForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *memorizedWordBook;
	Word *wordLink;

	if (HIWORD(wParam) == BN_CLICKED) {
		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");

		// 메인윈도우의 외운 단어장에서 마지막으로 간다.
		memorizedWordBook = (WordBook*)GetProp(wordBookFormWindow, "PROP_MEMORIZEDWORDBOOK");
		wordLink = WordBook_Last(memorizedWordBook);

		// 철자를 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);

		// 뜻, 품사, 예시를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// 클릭횟수를 리셋한다.
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)0);
	}

	return TRUE;
}

BOOL MemorizedWordBookForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *wordBook;
	Word *wordLink;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory;
	HTREEITEM hTvMeaning;
	HTREEITEM hTvIt;
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	char wordAlphabet;
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];

	// 메인윈도우를 찾는다.
	wordBookFormWindow = FindWindow("#32770", "단어장");

	// 메인윈도우의 단어장에 현재 단어가 있으면
	wordBook = (WordBook*)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
	wordLink = wordBook->current;
	if (wordLink != NULL) {
		// 메인윈도우의 트리뷰에서 단어들 항목을 찾는다
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
			(WPARAM)TVGN_ROOT, (LPARAM)0);

		// 메인윈도우의 트리뷰의 단어들 항목에서 알파벳 항목을 찾는다.
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		wordAlphabet = wordLink->spelling[0];
		if (wordAlphabet >= 'a' && wordAlphabet <= 'z') {
			wordAlphabet -= 'a' - 'A';
		}
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
			(WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && (wordAlphabet != alphabet[0])) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvAlphabet = hTvIt;

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 항목에서 철자 항목을 찾는다.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
			(WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->spelling, spelling) != 0) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvSpelling = hTvIt;

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 항목에서 품사 항목을 찾는다.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
			(WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->category, category) != 0) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvCategory = hTvIt;

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 항목에서 뜻 항목을 찾는다.

		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
			(WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->meaning, meaning) != 0) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvMeaning = hTvIt;

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 항목을 펼친다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 항목을 펼친다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 항목을 펼친다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 - 뜻 항목을 선택한다
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);

		// 메인윈도우에서 단어를 쓴다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);
	}

	// 윈도우를 닫는다.
	EndDialog(hWnd, 0);

	return TRUE;
}