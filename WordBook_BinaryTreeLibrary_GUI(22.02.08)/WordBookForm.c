//WordBookForm.c
#include "WordBookForm.h"
#include "PuttingForm.h"
#include "FindingForm.h"
#include "DrawingForm.h"
#include "WordTestForm.h"
#include "MemorizedWordBookForm.h"
#include "resource.h"
#include "WordBook.h"
#include "WordIndexCard.h"
#include "WordIndexCardFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <CommCtrl.h>
#include <string.h>

#pragma warning(disable: 4996)


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevHInstance, LPSTR lpCmdLine, int mCmdShow) {
	int response;

	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_WORDBOOKFORM), NULL, WordBookFormProc);

	return response;
}

BOOL CALLBACK WordBookFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL result;

	switch (message) {
	case WM_INITDIALOG: result = WordBookForm_OnInitDialog(hWnd, wParam, lParam); break;
	case WM_COMMAND: result = WordBookForm_OnCommand(hWnd, wParam, lParam); break;
	case WM_NOTIFY: result = WordBookForm_OnNotify(hWnd, wParam, lParam); break;
	case WM_CLOSE: result = WordBookForm_OnClose(hWnd, wParam, lParam); break;
	default: result = FALSE; break;
	}
	
	return result;
}

BOOL WordBookForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL result;

	switch (LOWORD(wParam)) {
	case IDC_BUTTON_PUT: result = WordBookForm_OnPutButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_FIND: result = WordBookForm_OnFindButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_DRAW: result = WordBookForm_OnDrawButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_ARRANGE: result = WordBookForm_OnArrangeButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_TEST: result = WordBookForm_OnTestButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_MEMORIZEDWORDS: result = WordBookForm_OnMemorizedWordsButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_FIRST: result = WordBookForm_OnFirstButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_PREVIOUS: result = WordBookForm_OnPreviousButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_NEXT: result = WordBookForm_OnNextButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_LAST: result = WordBookForm_OnLastButtonClicked(hWnd, wParam, lParam); break;
	default: result = FALSE; break;
	}
	return result;
}

BOOL WordBookForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (wParam) {
	case IDC_TREE_WORDS: ret = WordBookForm_OnTreeViewItemDoubleClicked(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}

	return ret;
}

BOOL WordBookForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook;
	WordIndexCardFile *wordIndexCardFile;
	WordBook *memorizedWordBook;
	Word *it;
	Word *previous = NULL;
	Word *wordLink;
	WordIndexCard(*wordIndexCards);
	Long count;
	Long i;
	Long j;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory = NULL;
	HTREEITEM hTvMeaning = NULL;
	HTREEITEM hTvIt;
	TVINSERTSTRUCT tvInsertStruct = { 0, };
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[32];
	TCHAR meaning[64];
	TCHAR category[16];
	TCHAR countText[64];


	// �ܾ����� �����Ѵ�.
	wordBook = (WordBook*)malloc(sizeof(WordBook));
	WordBook_Create(wordBook);
	SetWindowLong(hWnd, GWLP_USERDATA, (LONG)wordBook);

	// �ܾ����� �ε��Ѵ�.
	count = Load(wordBook);

	// ����ö�� �����Ѵ�.
	wordIndexCardFile = (WordIndexCardFile*)malloc(sizeof(WordIndexCardFile));
	WordIndexCardFile_Create(wordIndexCardFile);
	SetProp(hWnd, "PROP_WORDINDEXCARDFILE", (HANDLE)wordIndexCardFile);

	// �ܿ�ܾ����� �����Ѵ�.
	memorizedWordBook = (WordBook*)malloc(sizeof(WordBook));
	WordBook_Create(memorizedWordBook);
	SetProp(hWnd, "PROP_MEMORIZEDWORDBOOK", (HANDLE)memorizedWordBook);

	// �ܿ�ܾ����� �ε��Ѵ�.
	LoadMemorizedWords(memorizedWordBook);

	// �ܾ��忡 �ܾ ������
	if (count > 0) {
		// �ܾ����� ��� �ܾ ����ö�� �ִ´�.
		it = WordBook_First(wordBook);
		while (it != previous) {
			WordIndexCardFile_Put(wordIndexCardFile, it);
			previous = it;
			it = WordBook_Next(wordBook);
		}

		// ����ö�� �����Ѵ�.
		WordIndexCardFile_Arrange(wordIndexCardFile);
	}

	// ����ö���� ����� �����.
	MakeList(wordIndexCardFile, &wordIndexCards, &count);

	// Ʈ���信 �ܾ�� �׸��� �߰��Ѵ�.
	tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
	tvItem.pszText = "�ܾ��";
	tvInsertStruct.item = tvItem;
	hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);

	// ����ö�� ��뷮��ŭ �ݺ��Ѵ�.
	i = 0;
	while (i < wordIndexCardFile->length) {

		// Ʈ������ �ܾ�� �׸� ���ĺ� �׸��� �߰��Ѵ�.
		tvInsertStruct.hParent = hTvRoot;
		tvInsertStruct.hInsertAfter = TVI_LAST;
		alphabet[0] = wordIndexCards[i].alphabet;
		alphabet[1] = '\0';
		tvInsertStruct.item.pszText = alphabet;
		hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);

		// ����ī���� ��뷮��ŭ �ݺ��Ѵ�.
		j = 0;
		while (j < wordIndexCards[i].length) {

			// ���� �ܾ �����´�.
			wordLink = WordIndexCard_GetAt(wordIndexCards + i, j);

			// Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö���׸��� ã�´�.
			tvItem.pszText = spelling;
			tvItem.cchTextMax = sizeof(spelling);
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			while (hTvIt != NULL && strcmp(wordLink->spelling, spelling) != 0) {
				hTvIt = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
				tvItem.hItem = hTvIt;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			}
			hTvSpelling = hTvIt;

			// ö���׸��� ������ �߰��Ѵ�.
			if (hTvSpelling == NULL) {
				tvInsertStruct.hParent = hTvAlphabet;
				tvInsertStruct.hInsertAfter = TVI_LAST;
				tvInsertStruct.item.pszText = wordLink->spelling;
				hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
			}

			// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸񿡼� ǰ�� �׸��� ã�´�.
			tvItem.pszText = category;
			tvItem.cchTextMax = sizeof(category);
			hTvCategory = NULL;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			while (hTvIt != NULL && strcmp(wordLink->category, category) >= 0) {
				hTvCategory = hTvIt;
				hTvIt = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
				tvItem.hItem = hTvIt;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			}

			// ǰ���׸��� ������ �߰��Ѵ�.
			tvItem.hItem = hTvCategory;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			if (hTvCategory == NULL || strcmp(wordLink->category, category) != 0) {
				tvInsertStruct.hParent = hTvSpelling;
				if (hTvCategory != NULL) {
					tvInsertStruct.hInsertAfter = hTvCategory;
				}
				else {
					tvInsertStruct.hInsertAfter = TVI_FIRST;
				}
				tvInsertStruct.item.pszText = wordLink->category;
				hTvCategory = (HTREEITEM) SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
			}

			// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸񿡼� �� �׸��� �߰��� ��ġ�� ã�´�.
			tvItem.pszText = meaning;
			tvItem.cchTextMax = sizeof(meaning);
			hTvMeaning = NULL;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			while (hTvIt != NULL && strcmp(wordLink->meaning, meaning) >= 0) {
				hTvMeaning = hTvIt;
				hTvIt = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
				tvItem.hItem = hTvIt;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			}

			// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �߰��Ѵ�.
			tvInsertStruct.hParent = hTvCategory;
			if (hTvMeaning != NULL) {
				tvInsertStruct.hInsertAfter = hTvMeaning;
			} else{
					tvInsertStruct.hInsertAfter = TVI_FIRST;
			}
			tvInsertStruct.item.pszText = wordLink->meaning;
			hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);

			j++;
		}
		i++;
	}


	// �ܾ��忡 �ܾ ������
	if (wordBook->length > 0) {
		// �ܾ��忡�� ó������ ����.
		wordLink = WordBook_First(wordBook);
		// Ʈ������ �ܾ�� �׸񿡼� ���ĺ� �׸��� ã�´�.
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvAlphabet;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		alphabet[0] += 'a' - 'A';
		while (hTvAlphabet != NULL && wordLink->spelling[0] != alphabet[0]) {
			hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvAlphabet);
			tvItem.hItem = hTvAlphabet;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			alphabet[0] += 'a' - 'A';
		}

		// Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö�� �׸��� ã�´�.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvSpelling;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvSpelling != NULL && strcmp(wordLink->spelling, spelling) != 0) {
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvSpelling);
			tvItem.hItem = hTvSpelling;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		
		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ã�´�.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvCategory;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvCategory != NULL && strcmp(wordLink->category, category) != 0) {
			hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvCategory);
			tvItem.hItem = hTvCategory;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� ã�´�.
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvMeaning;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvMeaning != NULL && strcmp(wordLink->meaning, meaning) != 0) {
			hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvMeaning);
			tvItem.hItem = hTvMeaning;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ���信�� �ܾ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvRoot);
		// Ʈ���信�� �ܾ�� - ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);
		// Ʈ���信�� �ܾ�� - ���ĺ� - ö�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);
		// Ʈ���信�� �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);
		// Ʈ���信�� �ܾ�� - ���ĺ� - ö�� - �� �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);


		// �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);
	}

	// ������ ����
	sprintf(countText, "%d", wordBook->length);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);


	return TRUE;
}

BOOL WordBookForm_OnTreeViewItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook *wordBook;
	WordIndexCardFile *wordIndexCardFile;
	WordIndexCard *wordIndexCardLink;
	Word *wordLink;
	HTREEITEM hTvMeaning;
	HTREEITEM hTvChild;
	HTREEITEM hTvCategory;
	HTREEITEM hTvSpelling;
	TVITEM tvItem = { 0, };
	TCHAR spelling[48];
	TCHAR meaning[64];
	char alphabet;
	Long i = 0;

	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {

		// Ʈ���信�� ����Ŭ���� �׸��� ã�´�.
		hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET, (LPARAM)0);

		// ����Ŭ���� �׸��� �� �׸��� ������
		hTvChild = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvMeaning);
		if (hTvChild == NULL) {

			//���� �д´�.
			tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
			tvItem.pszText = meaning;
			tvItem.cchTextMax = sizeof(meaning);
			tvItem.hItem = hTvMeaning;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);

			// Ʈ���信�� ����Ŭ���� �׸��� �θ��׸��� ǰ���׸��� ã�´�.
			hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)hTvMeaning);

			// Ʈ���信�� ǰ���׸��� �θ��׸��� ö���׸��� ã�´�.
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)hTvCategory);

			// ö�ڸ� �д´�.
			tvItem.pszText = spelling;
			tvItem.cchTextMax = sizeof(spelling);
			tvItem.hItem = hTvSpelling;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);

			// ����ö���� ����ī�带 ã�´�.
			wordIndexCardFile = (WordIndexCardFile*)GetProp(hWnd, "PROP_WORDINDEXCARDFILE");

			alphabet = spelling[0];
			if (alphabet >= 'a' && alphabet <= 'z') {
				alphabet -= 'a' - 'A';
			}
			wordIndexCardLink = WordIndexCardFile_Find(wordIndexCardFile, alphabet);

			// ����ī�忡�� ö�ڿ� �� ��� ��ġ�ϴ� �ܾ ã�´�.
			wordLink = WordIndexCard_GetAt(wordIndexCardLink, i);
			i++;
			while (i < wordIndexCardLink->length
				&& (strcmp(wordLink->spelling, spelling) != 0
				|| strcmp(wordLink->meaning, meaning) != 0)) {

				wordLink = WordIndexCard_GetAt(wordIndexCardLink, i);
				i++;
			}

			// �ܾ��忡�� �ܾ��� ��ġ�� �̵��Ѵ�.
			wordBook = (WordBook *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			wordLink = WordBook_Move(wordBook, wordLink);

			// �ܾ ����.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);
		}
	}

	return TRUE;
}

BOOL WordBookForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HINSTANCE hInstance;

	if (HIWORD(wParam) == BN_CLICKED) {
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_PUTTINGFORM), NULL, PuttingFormProc);
	}

	return TRUE;
}

BOOL WordBookForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HINSTANCE hInstance;

	if (HIWORD(wParam) == BN_CLICKED) {
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_FINDINGFORM), (WPARAM) NULL, FindingFormProc);
	}

	return TRUE;
}


BOOL WordBookForm_OnDrawButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HINSTANCE hInstance;

	if (HIWORD(wParam) == BN_CLICKED) {
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DRAWINGFORM), (WPARAM) NULL, DrawingFormProc);
	}

	return TRUE;
}

BOOL WordBookForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook *wordBook;
	WordIndexCardFile *wordIndexCardFile;
	WordIndexCard (*wordIndexCards);
	Long count;
	Word *wordLink;
	Long i;
	Long j;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory;
	HTREEITEM hTvMeaning;
	HTREEITEM hTvIt;
	TVITEM tvItem = { 0, };
	TVINSERTSTRUCT tvInsertStruct = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];

	if (HIWORD(wParam) == BN_CLICKED) {
		// ����ö���� �����Ѵ�.
		wordIndexCardFile = (WordIndexCardFile*)GetProp(hWnd, "PROP_WORDINDEXCARDFILE");
		WordIndexCardFile_Arrange(wordIndexCardFile);

		// ����ö���� ����� �����.
		MakeList(wordIndexCardFile, &wordIndexCards, &count);

		// Ʈ���信�� ��� �׸��� �����.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);

		// Ʈ���信�� �ܾ�� �׸��� �߰��Ѵ�.
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = "�ܾ��";
		tvInsertStruct.hParent = NULL;
		tvInsertStruct.hInsertAfter = TVI_ROOT;
		tvInsertStruct.item = tvItem;
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);

		// ����ī���� ������ŭ �ݺ��Ѵ�.
		i = 0;
		while (i < count) {
			// Ʈ������ �ܾ�� �׸� ���ĺ� �׸��� �߰��Ѵ�.
			tvInsertStruct.hParent = hTvRoot;
			tvInsertStruct.hInsertAfter = TVI_LAST;
			alphabet[0] = wordIndexCards[i].alphabet;
			alphabet[1] = '\0';
			tvInsertStruct.item.pszText = alphabet;
			hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);

			// ����ī���� ��뷮��ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < wordIndexCards[i].length) {
				wordLink = WordIndexCard_GetAt(wordIndexCards + i, j);

				// Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö�� �׸��� ã�´�.
				tvItem.pszText = spelling;
				tvItem.cchTextMax = sizeof(spelling);
				hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
				tvItem.hItem = hTvSpelling;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
				while (hTvSpelling != NULL && strcmp(wordLink->spelling, spelling) != 0) {
					hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvSpelling);
					tvItem.hItem = hTvSpelling;
					SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
				}

				// ö���׸��� ������ �߰��Ѵ�.
				if (hTvSpelling == NULL) {
					tvInsertStruct.hParent = hTvAlphabet;
					tvInsertStruct.hInsertAfter = TVI_LAST;
					tvInsertStruct.item.pszText = wordLink->spelling;
					hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
				}

				// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸񿡼� ǰ�� �׸��� ã�´�.
				tvItem.pszText = category;
				tvItem.cchTextMax = sizeof(category);
				hTvCategory = NULL;
				hTvIt = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
				tvItem.hItem = hTvIt;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
				while (hTvIt != NULL && strcmp(wordLink->category, category) >= 0) {
					hTvCategory = hTvIt;
					hTvIt = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
					tvItem.hItem = hTvIt;
					SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
				}

				// ǰ�� �׸��� ������ �߰��Ѵ�.
				tvItem.hItem = hTvCategory;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
				if (hTvCategory == NULL || strcmp(wordLink->category, category) != 0) {
					tvInsertStruct.hParent = hTvSpelling;
					if (hTvCategory != NULL) {
						tvInsertStruct.hInsertAfter = hTvCategory;
					}
					else {
						tvInsertStruct.hInsertAfter = TVI_FIRST;
					}
					tvInsertStruct.item.pszText = wordLink->category;
					hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
				}

				// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �߰��� ��ġ�� ã�´�.
				hTvMeaning = NULL;
				tvItem.pszText = meaning;
				tvItem.cchTextMax = sizeof(meaning);
				hTvIt = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
				tvItem.hItem = hTvIt;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
				while (hTvIt != NULL && strcmp(wordLink->meaning, meaning) >= 0) {
					hTvMeaning = hTvIt;
					hTvIt = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
					tvItem.hItem = hTvIt;
					SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
				}

				// �� �׸��� �߰��Ѵ�.
				tvInsertStruct.hParent = hTvCategory;
				if (hTvMeaning != NULL) {
					tvInsertStruct.hInsertAfter = hTvMeaning;
				}
				else {
					tvInsertStruct.hInsertAfter = TVI_FIRST;
				}
				tvInsertStruct.item.pszText = wordLink->meaning;
				(HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);

				j++;
			}

			i++;
		}

		// �ܾ��忡�� ó������ ����.
		wordBook = (WordBook *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		wordLink = WordBook_First(wordBook);
		
		// Ʈ������ �ܾ�� - ���ĺ� �׸��� ã�´�.
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvAlphabet;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		alphabet[0] += 'a' - 'A';
		while (hTvAlphabet != NULL && wordLink->spelling[0] != alphabet[0]) {
			hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvAlphabet);
			tvItem.hItem = hTvAlphabet;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			alphabet[0] += 'a' - 'A';
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸��� ã�´�.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvSpelling;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvSpelling != NULL && strcmp(wordLink->spelling, spelling) != 0) {
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvSpelling);
			tvItem.hItem = hTvSpelling;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ã�´�.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvCategory;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvCategory != NULL && strcmp(wordLink->category, category) != 0) {
			hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvCategory);
			tvItem.hItem = hTvCategory;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� ã�´�.
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvMeaning;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvMeaning != NULL && strcmp(wordLink->meaning, meaning) != 0) {
			hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvMeaning);
			tvItem.hItem = hTvMeaning;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvRoot);

		// Ʈ������ �ܾ�� - ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸��� ��ģ��
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ��ģ��
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);

		// �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);
	}

	return TRUE;
}


BOOL WordBookForm_OnTestButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HINSTANCE hInstance;

	if (HIWORD(wParam) == BN_CLICKED) {
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_WORDTESTFORM), (WPARAM)NULL, WordTestFormProc);
	}

	return TRUE;
}

BOOL WordBookForm_OnMemorizedWordsButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HINSTANCE hInstance;

	if (HIWORD(wParam) == BN_CLICKED) {
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_MEMORIZEDWORDBOOKFORM), (WPARAM)NULL, MemorizedWordBookFormProc);
	}

	return TRUE;
}


BOOL WordBookForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook = NULL;
	Word* wordLink = NULL;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory;
	HTREEITEM hTvMeaning;
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];


	if (HIWORD(wParam) == BN_CLICKED) {

		// �ܾ��忡�� ó������ ����.
		wordBook = (WordBook*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		wordLink = WordBook_First(wordBook);

		// Ʈ���信�� �ܾ�� �׸��� ã�´�.
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		
		// Ʈ������ �ܾ�� �׸񿡼� ���ĺ� �׸��� ã�´�.
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvAlphabet;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		if (alphabet[0] > 'A' && alphabet[0] < 'Z') {
			alphabet[0] += 'a' - 'A';
		}
		while (hTvAlphabet != NULL && wordLink->spelling[0] != alphabet[0]) {
			hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvAlphabet);
			tvItem.hItem = hTvAlphabet;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			if (alphabet[0] > 'A' && alphabet[0] < 'Z') {
				alphabet[0] += 'a' - 'A';
			}
		}

		// Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö�� �׸��� ã�´�.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvSpelling;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvSpelling != NULL && strcmp(wordLink->spelling, spelling) != 0) {
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvSpelling);
			tvItem.hItem = hTvSpelling;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸񿡼� ǰ�� �׸��� ã�´�.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvCategory;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvCategory != NULL && strcmp(wordLink->category, category) != 0) {
			hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvCategory);
			tvItem.hItem = hTvCategory;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸񿡼� �� �׸��� ã�´�.
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvMeaning;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvMeaning != NULL && strcmp(wordLink->meaning, meaning) != 0) {
			hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvMeaning);
			tvItem.hItem = hTvMeaning;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);

		// �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);

	
	}

	return TRUE;
}

BOOL WordBookForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook = NULL;
	Word* wordLink = NULL;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory;
	HTREEITEM hTvMeaning;
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];


	if (HIWORD(wParam) == BN_CLICKED) {

		// �ܾ��忡�� �������� ����.
		wordBook = (WordBook*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		wordLink = WordBook_Previous(wordBook);

		// Ʈ���信�� �ܾ�� �׸��� ã�´�.
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);

		// Ʈ������ �ܾ�� �׸񿡼� ���ĺ� �׸��� ã�´�.
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvAlphabet;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		alphabet[0] += 'a' - 'A';
		while (hTvAlphabet != NULL && wordLink->spelling[0] != alphabet[0]) {
			hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvAlphabet);
			tvItem.hItem = hTvAlphabet;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			alphabet[0] += 'a' - 'A';
		}

		// Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö�� �׸��� ã�´�.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvSpelling;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvSpelling != NULL && strcmp(wordLink->spelling, spelling) != 0) {
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvSpelling);
			tvItem.hItem = hTvSpelling;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸񿡼� ǰ�� �׸��� ã�´�.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvCategory;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvCategory != NULL && strcmp(wordLink->category, category) != 0) {
			hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvCategory);
			tvItem.hItem = hTvCategory;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸񿡼� �� �׸��� ã�´�.
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvMeaning;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvMeaning != NULL && strcmp(wordLink->meaning, meaning) != 0) {
			hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvMeaning);
			tvItem.hItem = hTvMeaning;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);

		// �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);


	}

	return TRUE;
}

BOOL WordBookForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook = NULL;
	Word* wordLink = NULL;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory;
	HTREEITEM hTvMeaning;
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];


	if (HIWORD(wParam) == BN_CLICKED) {

		// �ܾ��忡�� �������� ����.
		wordBook = (WordBook*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		wordLink = WordBook_Next(wordBook);

		// Ʈ���信�� �ܾ�� �׸��� ã�´�.
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);

		// Ʈ������ �ܾ�� �׸񿡼� ���ĺ� �׸��� ã�´�.
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvAlphabet;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		alphabet[0] += 'a' - 'A';
		while (hTvAlphabet != NULL && wordLink->spelling[0] != alphabet[0]) {
			hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvAlphabet);
			tvItem.hItem = hTvAlphabet;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			alphabet[0] += 'a' - 'A';
		}

		// Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö�� �׸��� ã�´�.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvSpelling;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvSpelling != NULL && strcmp(wordLink->spelling, spelling) != 0) {
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvSpelling);
			tvItem.hItem = hTvSpelling;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸񿡼� ǰ�� �׸��� ã�´�.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvCategory;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvCategory != NULL && strcmp(wordLink->category, category) != 0) {
			hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvCategory);
			tvItem.hItem = hTvCategory;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸񿡼� �� �׸��� ã�´�.
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvMeaning;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvMeaning != NULL && strcmp(wordLink->meaning, meaning) != 0) {
			hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvMeaning);
			tvItem.hItem = hTvMeaning;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);

		// �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);


	}

	return TRUE;
}

BOOL WordBookForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook = NULL;
	Word* wordLink = NULL;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory;
	HTREEITEM hTvMeaning;
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];


	if (HIWORD(wParam) == BN_CLICKED) {

		// �ܾ��忡�� ������ ����.
		wordBook = (WordBook*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		wordLink = WordBook_Last(wordBook);

		// Ʈ���信�� �ܾ�� �׸��� ã�´�.
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);

		// Ʈ������ �ܾ�� �׸񿡼� ���ĺ� �׸��� ã�´�.
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvAlphabet;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		alphabet[0] += 'a' - 'A';
		while (hTvAlphabet != NULL && wordLink->spelling[0] != alphabet[0]) {
			hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvAlphabet);
			tvItem.hItem = hTvAlphabet;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			alphabet[0] += 'a' - 'A';
		}

		// Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö�� �׸��� ã�´�.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvSpelling;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvSpelling != NULL && strcmp(wordLink->spelling, spelling) != 0) {
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvSpelling);
			tvItem.hItem = hTvSpelling;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸񿡼� ǰ�� �׸��� ã�´�.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvCategory;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvCategory != NULL && strcmp(wordLink->category, category) != 0) {
			hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvCategory);
			tvItem.hItem = hTvCategory;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸񿡼� �� �׸��� ã�´�.
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvMeaning;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvMeaning != NULL && strcmp(wordLink->meaning, meaning) != 0) {
			hTvMeaning = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvMeaning);
			tvItem.hItem = hTvMeaning;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// Ʈ������ �ܾ�� - ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);

		// �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);


	}

	return TRUE;
}

BOOL WordBookForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook;
	WordIndexCardFile *wordIndexCardFile;
	WordBook *memorizedWordBook;

	wordBook = (WordBook *)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (wordBook != NULL) {
		Save(wordBook);
		WordBook_Destroy(wordBook);
		free(wordBook);
	}

	wordIndexCardFile = (WordIndexCardFile *)RemoveProp(hWnd, "PROP_WORDINDEXCARDFILE");
	if (wordIndexCardFile != NULL) {
		WordIndexCardFile_Destroy(wordIndexCardFile);
		free(wordIndexCardFile);
	}

	memorizedWordBook = (WordBook *)RemoveProp(hWnd, "PROP_MEMORIZEDWORDBOOK");

	if (memorizedWordBook != NULL) {
		SaveMemorizedWords(memorizedWordBook);
		WordBook_Destroy(memorizedWordBook);
		free(memorizedWordBook);
	}

	EndDialog(hWnd, 0);

	return TRUE;
}