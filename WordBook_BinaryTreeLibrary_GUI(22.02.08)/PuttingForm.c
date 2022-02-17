//PuttingForm.c
#include "PuttingForm.h"
#include "resource.h"
#include "WordBook.h"
#include "WordIndexCardFile.h"
#include "WordIndexCard.h"
#include <CommCtrl.h>
#pragma warning(disable : 4996)

BOOL CALLBACK PuttingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL result;
	

	switch (message) {
	case WM_INITDIALOG: result = PuttingForm_OnInitDialog(hWnd, wParam, lParam); break;
	case WM_COMMAND: result = PuttingForm_OnCommand(hWnd, wParam, lParam); break;
	case WM_CLOSE: result = PuttingForm_OnClose(hWnd, wParam, lParam); break;
	default: result = FALSE; break;
	}

	return result;
}

BOOL PuttingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL result;

	switch (LOWORD(wParam)) {
	case IDC_EDIT_SPELLING: result = PuttingForm_OnSpellingLostFocus(hWnd, wParam, lParam); break;
	case IDC_BUTTON_PUT: result = PuttingForm_OnPutButtonClicked(hWnd, wParam, lParam); break;
	default: result = FALSE; break;
	}

	return result;
}


BOOL PuttingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	TCHAR categories[][16] = { TEXT("���"), TEXT("����"), TEXT("����"), TEXT("�����"), TEXT("�λ�"), TEXT("���ӻ�"), TEXT("��ġ��"), TEXT("��ź��") };
	Long i = 0;

	// ǰ�� ������ŭ ǰ�� �޺��ڽ��� ����� �߰��Ѵ�.
	while (i < sizeof(categories) / sizeof(categories[0])) {
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), CB_ADDSTRING, (WPARAM)0, (LPARAM)categories[i]);
		i++;
	}

	return TRUE;
}

BOOL PuttingForm_OnSpellingLostFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordIndexCardFile *wordIndexCardFile;
	TCHAR spelling[48];
	WordIndexCard *wordIndexCardLink;
	Word *wordLink;
	Long low;
	Long high;
	Long middle;


	if (HIWORD(wParam) == EN_KILLFOCUS) {
		// ö�ڸ� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)sizeof(spelling), (LPARAM)spelling);

		// ���������츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");

		// ������������ ����ö���� ���ĺ����� ����ī�带 ã�´�.
		wordIndexCardFile = (WordIndexCardFile*)GetProp(wordBookFormWindow, "PROP_WORDINDEXCARDFILE");
		wordIndexCardLink = WordIndexCardFile_Find(wordIndexCardFile, spelling[0]);

		// ����ī�尡 ������
		if (wordIndexCardLink != NULL) {

			// ö�ڿ� ��ġ�ϴ� �ܾ �ϳ� ã�´�
			low = 0;
			high = wordIndexCardLink->length - 1;
			middle = (low + high) / 2;
			while (low <= high
				&& strcmp(WordIndexCard_GetAt(wordIndexCardLink, middle)->spelling, spelling) != 0) {
				if (strcmp(WordIndexCard_GetAt(wordIndexCardLink, middle)->spelling, spelling) > 0) {
					high = middle - 1;
				}
				else {
					low = middle + 1;
				}

				middle = (low + high) / 2;
			}

			// �ܾ ã������ ��, ǰ��, ���ø� ����.
			if (low <= high) {
				wordLink = WordIndexCard_GetAt(wordIndexCardLink, middle);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
				SendMessage(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);
			}
		}

	}

	return TRUE;
}

BOOL PuttingForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word word;
	HWND wordBookFormWindow;
	WordBook* wordBook;
	Word *wordLink;
	WordIndexCardFile *wordIndexCardFile;
	WordIndexCard *wordIndexCardLink;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet = NULL;
	HTREEITEM hTvSpelling = NULL;
	HTREEITEM hTvCategory = NULL;
	HTREEITEM hTvMeaning = NULL;
	HTREEITEM hTvIt;
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[32];
	TCHAR category[16];
	TCHAR meaning[64];
	TVINSERTSTRUCT tvInsertStruct = { 0, };
	TCHAR countText[64];

	if (HIWORD(wParam) == BN_CLICKED) {
		// �ܾ �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)48, (LPARAM)word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)64, (LPARAM)word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), WM_GETTEXT, (WPARAM)16, (LPARAM)word.category);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_GETTEXT, (WPARAM)64, (LPARAM)word.example);

		// ���� �����츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");

		// ���� �������� �ܾ��忡 �����.
		wordBook = (WordBook*)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
		wordLink = WordBook_Put(wordBook, word);

		// ������������ ����ö�� �ܾ �����.
		wordIndexCardFile = (WordIndexCardFile *)GetProp(wordBookFormWindow, "PROP_WORDINDEXCARDFILE");
		wordIndexCardLink = WordIndexCardFile_Put(wordIndexCardFile, wordLink);

		// ����ī�带 �����Ѵ�.
		WordIndexCard_Arrange(wordIndexCardLink);

		// ������������ Ʈ���信�� �ܾ�� �׸��� ã�´�.
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);

		// ������������ Ʈ������ �ܾ�� �׸񿡼� ���ĺ� �׸��� ã�´�.
		hTvAlphabet = NULL;
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && wordIndexCardLink->alphabet >= alphabet[0]) {
			hTvAlphabet = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// ������������ Ʈ������ �ܾ�� - ���ĺ� �׸��� ������ �߰��Ѵ�.
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
			tvInsertStruct.item = tvItem;
			tvInsertStruct.item.pszText[0] = wordIndexCardLink->alphabet;
			tvInsertStruct.item.pszText[1] = '\0';
			hTvAlphabet = (HTREEITEM) SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
		}

		// ������������ Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö�� �׸��� ã�´�.
		tvItem.pszText = spelling;
		tvItem.cchTextMax = sizeof(spelling);
		hTvSpelling = NULL;
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->spelling, spelling) >= 0) {
			hTvSpelling = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// ������������ Ʈ������ �ܾ��- ���ĺ� - ö�� �׸��� ������ �߰��Ѵ�.
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
			tvInsertStruct.item = tvItem;
			tvInsertStruct.item.pszText = wordLink->spelling;
			hTvSpelling = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);
		}

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸񿡼� ǰ�� �׸��� ã�´�.
		tvItem.pszText = category;
		tvItem.cchTextMax = sizeof(category);
		hTvCategory = NULL;
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->category, category) >= 0) {
			hTvCategory = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// ǰ���׸��� ������ �߰��Ѵ�.
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

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸񿡼� �� �׸��� �߰��� ��ġ�� ã�´�
		tvItem.pszText = meaning;
		tvItem.cchTextMax = sizeof(meaning);
		hTvMeaning = NULL;
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
		tvItem.hItem = hTvIt;
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && strcmp(wordLink->meaning, meaning) >= 0) {
			hTvMeaning = hTvIt;
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸� �� �׸��� �߰��Ѵ�.
		tvInsertStruct.hParent = hTvCategory;
		if (hTvMeaning != NULL) {
			tvInsertStruct.hInsertAfter = hTvMeaning;
		}
		else {
			tvInsertStruct.hInsertAfter = TVI_FIRST;
		}
		tvInsertStruct.item = tvItem;
		tvInsertStruct.item.pszText = wordLink->meaning;
		hTvMeaning = (HTREEITEM) SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&tvInsertStruct);

		// ������������ Ʈ������ �ܾ�� - ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);

		// ���������쿡�� �ܾ ����.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);

		// ���������쿡�� ������ ����.
		sprintf(countText, "%d", wordBook->length);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);

		// �����츦 �ݴ´�.
		EndDialog(hWnd, 0);
	}

	return TRUE;
}


BOOL PuttingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	EndDialog(hWnd, 0);

	return TRUE;
}