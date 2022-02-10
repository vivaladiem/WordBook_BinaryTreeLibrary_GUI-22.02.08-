//DrawingForm.c
#include "DrawingForm.h"
#include "resource.h"
#include "WordBook.h"
#include "WordIndexCard.h"
#include "WordIndexCardFile.h"
#include <CommCtrl.h>
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
	WordBook* wordBook;
	WordIndexCardFile *wordIndexCardFile;
	WordIndexCard *wordIndexCardLink;
	Word* wordLink;
	Word word;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory;
	HTREEITEM hTvMeaning;
	HTREEITEM hTvIt;
	HTREEITEM hTvChild;
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];
	TCHAR countText[64];

	// 메인윈도우를 찾는다.
	wordBookFormWindow = FindWindow("#32770", "단어장");

	// 메인윈도우의 단어장에서 현재 단어를 빼낸다.
	wordBook = (WordBook*)GetWindowLong(wordBookFormWindow, GWL_USERDATA);
	wordLink = wordBook->current;
	word = WordBook_Draw(wordBook, wordLink);

	// 메인윈도우의 색인철에서 단어를 빼낸다.
	wordIndexCardFile = (WordIndexCardFile*)GetProp(wordBookFormWindow, "PROP_WORDINDEXCARDFILE");
	wordIndexCardLink = WordIndexCardFile_Draw(wordIndexCardFile, wordLink, word.spelling[0]);

	// 빼낸 단어를 쓴다.
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)word.spelling);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)word.meaning);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)word.category);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)word.example);

	// 메인윈도우의 트리뷰에서 단어들 항목을 찾는다.
	hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);

	// 메인윈도우의 트리뷰의 단어들 항목에서 알파벳 항목을 찾는다.
	tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
	tvItem.pszText = alphabet;
	tvItem.cchTextMax = sizeof(alphabet);
	hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
	tvItem.hItem = hTvIt;
	SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
	alphabet[0] += 'a' - 'A';
	while (hTvIt != NULL && word.spelling[0] != alphabet[0]) {
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		alphabet[0] += 'a' - 'A';
	}
	hTvAlphabet = hTvIt;

	// 색인카드가 여전히 있으면
	if (wordIndexCardLink != NULL) {
		// 메인윈도우의 트리뷰의 단어들 - 알파벳 항목에서 철자 항목을 찾는다.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(word.spelling, spelling) != 0) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvSpelling = hTvIt;

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 항목에서 품사 항목을 찾는다.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(word.category, category) != 0) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvCategory = hTvIt;

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 항목에서 뜻 항목을 찾는다.
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(word.meaning, meaning) != 0) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvMeaning = hTvIt;

		// 메인윈도우의 트리뷰에서 찾은 뜻 항목을 지운다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvMeaning);

		// 메인윈도우의 트리뷰에서 단어들 - 알파벳 - 철자 - 품사 항목이 자식이 없으면
		hTvChild = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		if (hTvChild == NULL) {

			// 메인윈도우의 트리뷰에서 찾은 품사 항목을 지운다.
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvCategory);

			// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자항목이 자식이 없으면
			hTvChild = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
			if (hTvChild == NULL) {
				// 메인윈도우의 트리뷰에서 찾은 철자 항목을 지운다.
				SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvSpelling);
			}
		}
	}
	else {
		// 색인카드가 없어졌으면 메인윈도우의 트리뷰에서 찾은 알파벳 항목을 지운다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvAlphabet);
	}


	// 메인윈도우의 단어장에 현재 단어가 있으면
	wordLink = wordBook->current;
	if (wordLink != NULL) {

		// 메인윈도우의 트리뷰의 단어들 항목에서 알파벳 항목을 찾는다.
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		alphabet[0] += 'a' - 'A';
		while (hTvIt != NULL && wordLink->spelling[0] != alphabet[0]) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			alphabet[0] += 'a' - 'A';
		}
		hTvAlphabet = hTvIt;

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 항목에서 철자 항목을 찾는다.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->spelling, spelling) != 0) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvSpelling = hTvIt;

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 항목에서 품사 항목을 찾는다.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->category, category) != 0) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvCategory = hTvIt;

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 항목에서 뜻 항목을 찾는다.
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->meaning, meaning) != 0) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvMeaning = hTvIt;

		//SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvRoot);
		
		// 메인윈도우의 트리뷰의 단어들 - 알파벳 항목을 펼친다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);
		
		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 항목을 펼친다
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);
		
		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 항목을 펼친다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 - 뜻 항목을 선택한다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);
		
		// 메인윈도우에 단어를 쓴다.
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
		// 메인윈도우의 단어장에 현재 단어가 없으면 메인윈도우에서 단어의 내용을 지운다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
	}

	// 메인윈도우에서 개수를 쓴다.
	sprintf(countText, "%d", wordBook->length);
	SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);

	return TRUE;

}

BOOL DrawingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	int response;
	HWND wordBookFormWindow;
	WordBook* wordBook;
	WordIndexCardFile *wordIndexCardFile;
	Word word;
	Word* wordLink = NULL;
	WordIndexCard *wordIndexCardLink;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory;
	HTREEITEM hTvMeaning;
	HTREEITEM hTvIt;
	TVINSERTSTRUCT tvInsertStruct = { 0, };
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];
	TCHAR countText[64];

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
		wordLink = WordBook_Put(wordBook, word);

		// 메인윈도우의 색인철에 단어를 끼운다.
		wordIndexCardFile = (WordIndexCardFile*)GetProp(wordBookFormWindow, "PROP_WORDINDEXCARDFILE");
		wordIndexCardLink = WordIndexCardFile_Put(wordIndexCardFile, wordLink);

		// 색인카드에서 정리한다.
		WordIndexCard_Arrange(wordIndexCardLink);

		// 메인윈도우의 트리뷰에서 단어들 항목을 찾는다
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		
		// 메인윈도우의 트리뷰의 단어들 항목에서 알파벳 항목을 찾는다.
		hTvAlphabet = NULL;
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && (wordIndexCardLink->alphabet >= alphabet[0])) {
			hTvAlphabet = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// 알파벳 항목이 없으면 추가한다.
		tvItem.hItem = hTvAlphabet;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvAlphabet == NULL || (wordIndexCardLink->alphabet != alphabet[0])) {
			tvInsertStruct.hParent = hTvRoot;
			if (hTvAlphabet != NULL) {
				tvInsertStruct.hInsertAfter = hTvAlphabet;
			}
			else {
				tvInsertStruct.hInsertAfter = TVI_FIRST;
			}
			tvInsertStruct.item = tvItem;
			alphabet[0] = wordIndexCardLink->alphabet;
			alphabet[1] = '\0';
			tvInsertStruct.item.pszText = alphabet;
			hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
		}

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 항목에서 철자 항목을 찾는다.
		hTvSpelling = NULL;
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->spelling, spelling) >= 0) {
			hTvSpelling = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvSpelling;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// 철자 항목이 없으면 추가한다.
		tvItem.hItem = hTvSpelling;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		if (hTvSpelling == NULL || strcmp(wordLink->spelling, spelling) != 0) {
			tvInsertStruct.hParent = hTvAlphabet;
			if (hTvAlphabet != NULL) {
				tvInsertStruct.hInsertAfter = hTvAlphabet;
			}
			else {
				tvInsertStruct.hInsertAfter = TVI_FIRST;
			}
			tvInsertStruct.item = tvItem;
			tvInsertStruct.item.pszText = wordLink->spelling;
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
		}

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 항목에서 품사 항목을 찾는다.
		hTvCategory = NULL;
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
			tvInsertStruct.item = tvItem;
			tvInsertStruct.item.pszText = wordLink->category;
			hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
		}

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 항목에 뜻 항목을 추가할 위치를 찾는다.
		hTvMeaning = NULL;
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->meaning, meaning) >= 0) {
			hTvMeaning = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// 뜻 항목이 없으면 추가한다.
		tvInsertStruct.hParent = hTvCategory;
		if (hTvMeaning != NULL) {
			tvInsertStruct.hInsertAfter = hTvMeaning;
		}
		else {
			tvInsertStruct.hInsertAfter = TVI_FIRST;
		}
		tvInsertStruct.item = tvItem;
		tvInsertStruct.item.pszText = wordLink->meaning;
		hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
		
		// 메인윈도우의 트리뷰의 단어들 항목을 펼친다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvRoot);

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 항목을 펼친다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 항목을 펼친다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 항목을 펼친다
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 - 뜻 항목을 선택한다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);

		// 메인윈도우에 끼운 단어를 쓴다
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);

		// 메인윈도우에서 개수를 쓴다.
		sprintf(countText, "%d", wordBook->length);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);

		// 윈도우를 닫는다.
		EndDialog(hWnd, 0);
	}
	else if (response == IDNO) {
		// 윈도우를 닫는다.
		EndDialog(hWnd, 0);
	}

	return TRUE;
}