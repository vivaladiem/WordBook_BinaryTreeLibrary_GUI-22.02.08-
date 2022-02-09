//WordBookForm.c
#include "WordBookForm.h"
#include "PuttingForm.h"
#include "FindingForm.h"
#include "DrawingForm.h"
#include "resource.h"
#include "WordBook.h"
#include "WordIndexCard.h"
#include "WordIndexCardFile.h"
#include <stdlib.h>
#include <CommCtrl.h>
#include <string.h>
#pragma warning(disable: 4996)

BOOL WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevHInstance, LPSTR lpCmdLine, int mCmdShow) {
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
#if 0
	case IDC_BUTTON_FIND: result = WordBookForm_OnFindButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_DRAW: result = WordBookForm_OnDrawButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_FIRST: result = WordBookForm_OnFirstButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_PREVIOUS: result = WordBookForm_OnPreviousButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_NEXT: result = WordBookForm_OnNextButtonClicked(hWnd, wParam, lParam); break;
	case IDC_BUTTON_LAST: result = WordBookForm_OnLastButtonClicked(hWnd, wParam, lParam); break;
#endif
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
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)wordBook);

	// �ܾ����� �ε��Ѵ�.
	count = Load(wordBook);

	// ����ö�� �����Ѵ�.
	wordIndexCardFile = (WordIndexCardFile*)malloc(sizeof(WordIndexCardFile));
	WordIndexCardFile_Create(wordIndexCardFile);
	SetProp(hWnd, "PROP_WORDINDEXCARDFILE", (HANDLE)wordIndexCardFile);

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
				tvInsertStruct.hInsertAfter = hTvCategory;
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
			tvInsertStruct.hInsertAfter = hTvMeaning;
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
		hTvAlphabet = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
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
	HTREEITEM hTvItem;
	HTREEITEM hTvChild;
	HTREEITEM hTvCategory;
	HTREEITEM hTvSpelling;
	TVITEM tvItem = { 0, };
	TCHAR spelling[32];
	TCHAR meaning[64];
	Long i = 0;

	if (wParam == NM_DBLCLK) {
		// Ʈ���信�� ����Ŭ���� �׸��� �д´�.
		hTvItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET, (LPARAM)0);
		hTvChild = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvItem);
		if (hTvChild == NULL) {
			//���� �д´�.
			tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
			tvItem.pszText = meaning;
			tvItem.cchTextMax = sizeof(meaning);
			tvItem.hItem = hTvItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			// Ʈ���信�� ����Ŭ���� �׸��� �θ��׸��� ǰ���׸��� �д´�.
			hTvCategory = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)hTvItem);

			// Ʈ���信�� ǰ���׸��� �θ��׸��� ö���׸��� ã�´�.
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)hTvCategory);

			// ö�ڸ� �д´�.
			tvItem.pszText = spelling;
			tvItem.cchTextMax = sizeof(spelling);
			tvItem.hItem = hTvSpelling;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);

			// ����ö���� ����ī�带 ã�´�.
			wordIndexCardFile = (WordIndexCardFile*)GetProp(hWnd, "PROP_WORDINDEXCARDFILE");
			wordIndexCardLink = WordIndexCardFile_Find(wordIndexCardFile, spelling[0]);

			// ����ī�忡�� ö�ڿ� �� ��� ��ġ�ϴ� �ܾ ã�´�.
			i = 0;
			wordLink = WordIndexCard_GetAt(wordIndexCardLink, i);
			i++;
			while (i < wordIndexCardLink->length
				&& strcmp(wordLink->spelling, spelling) != 0
				&& strcmp(wordLink->meaning, meaning) != 0) {
				wordLink = WordIndexCard_GetAt(wordIndexCardLink, i);
				i++;
			}

			// �ܾ��忡�� �ܾ��� ��ġ�� �̵��Ѵ�.
			wordBook = (WordBook *)GetWindowLong(hWnd, GWL_USERDATA);
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
		hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_PUTTINGFORM), NULL, PuttingFormProc);
	}

	return TRUE;
}

BOOL WordBookForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HINSTANCE hInstance;

	if (HIWORD(wParam) == BN_CLICKED) {
		hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_FINDINGFORM), NULL, FindingFormProc);
	}

	return TRUE;
}


BOOL WordBookForm_OnDrawButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HINSTANCE hInstance;

	if (HIWORD(wParam) == BN_CLICKED) {
		hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DRAWINGFORM), NULL, DrawingFormProc);
	}

	return TRUE;
}


BOOL WordBookForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook = NULL;
	Word* index = NULL;

	if (HIWORD(wParam) == BN_CLICKED) {
		// �ܾ��忡�� ó������ ����.
		wordBook = (WordBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = WordBook_First(wordBook);

		// �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)index->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->example);

	
	}

	return TRUE;
}

BOOL WordBookForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook = NULL;
	Word* index = NULL;

	if (HIWORD(wParam) == BN_CLICKED) {
		// �ܾ��忡�� �������� ����.
		wordBook = (WordBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = WordBook_Previous(wordBook);

		// �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)index->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->example);

	}
	return TRUE;
}

BOOL WordBookForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook = NULL;
	Word* index = NULL;

	if (HIWORD(wParam) == BN_CLICKED) {
		// �ܾ��忡�� �������� ����.
		wordBook = (WordBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = WordBook_Next(wordBook);

		//�ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)index->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->example);
		
	}

	return TRUE;
}

BOOL WordBookForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook = NULL;
	Word* index = NULL;

	if (HIWORD(wParam) == BN_CLICKED) {
		// �ܾ��忡�� ���������� ����.
		wordBook = (WordBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = WordBook_Last(wordBook);

		// �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)index->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->example);

	}

	return TRUE;
}

BOOL WordBookForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordBook* wordBook = (WordBook*)GetWindowLong(hWnd, GWL_USERDATA);

	if (wordBook != NULL) {
		Save(wordBook);
		WordBook_Destroy(wordBook);
		free(wordBook);
	}

	EndDialog(hWnd, 0);

	return TRUE;
}