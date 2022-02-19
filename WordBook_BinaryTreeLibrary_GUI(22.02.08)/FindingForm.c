//FindingForm.c
#include "FindingForm.h"
#include "resource.h"
#include "WordBook.h"
#include "WordIndexCardFile.h"
#include "WordIndexCard.h"
#include <CommCtrl.h>
#include <string.h>
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
	Word* (*wordLinks) = NULL;
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

		wordLinks = (Word * (*))GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (wordLinks != NULL) {
			free(wordLinks);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)NULL);
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
	Word* (*wordLinks) = NULL;
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

		wordLinks = (Word* (*)) GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (wordLinks != NULL) {
			free(wordLinks);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)NULL);
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
	WordBook* wordBook;
	WordIndexCardFile *wordIndexCardFile;
	WordIndexCard *wordIndexCardLink;
	HWND wordBookFormWindow;
	int spellingButtonState;
	int meaningButtonState;
	TCHAR spelling[48];
	TCHAR meaning[64];
	char alphabet;
	Word* (*wordLinks);
	Long count = 0;
	Long number;
	TCHAR numberText[64];
	TCHAR countText[64];
	Long low;
	Long high;
	Long middle;
	Long i;
	Long j = 0;


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

		// ������ ã�� �ܾ���� ���ش�.
		wordLinks = (Word * (*))GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (wordLinks != NULL) {
			free(wordLinks);
			wordLinks = NULL;
		}

		// ö�� ������ư�� üũ�Ǿ� ������
		if (spellingButtonState == BST_CHECKED) {
			// ������������ ����ö���� ����ī�带 ã�´�.
			wordIndexCardFile = (WordIndexCardFile *)GetProp(wordBookFormWindow, "PROP_WORDINDEXCARDFILE");

			alphabet = spelling[0];
			if (alphabet >= 'a' && alphabet <= 'z') {
				alphabet -= 'a' - 'A';
			}
			wordIndexCardLink = WordIndexCardFile_Find(wordIndexCardFile, alphabet);

			// ����ī�尡 ������
			if (wordIndexCardLink != NULL) {
				// ����ī�忡�� ö�ڿ� ��ġ�ϴ� �ܾ���� ã�´�.
				wordLinks = (Word * (*)) calloc(wordIndexCardLink->length, sizeof(Word*));
				low = 0;
				high = wordIndexCardLink->length - 1;
				middle = (low + high) / 2;
				while (low <= high
					&& strcmp(spelling, WordIndexCard_GetAt(wordIndexCardLink, middle)->spelling) != 0) {
					if (strcmp(spelling, WordIndexCard_GetAt(wordIndexCardLink, middle)->spelling) < 0) {
						high = middle - 1;
					}
					else {
						low = middle + 1;
					}

					middle = (low + high) / 2;
				}

				i = middle - 1;
				while (i >= 0 && strcmp(spelling, WordIndexCard_GetAt(wordIndexCardLink, i)->spelling) == 0) {
					i--;
				}

				i++;
				if (i < 0) {
					i = 0;
				}
				while (i < wordIndexCardLink->length
					&& strcmp(spelling, WordIndexCard_GetAt(wordIndexCardLink, i)->spelling) == 0) {
					wordLinks[j] = WordIndexCard_GetAt(wordIndexCardLink, i);
					j++;
					count++;
					i++;
				}
			}
			
		} 
		else if (meaningButtonState == BST_CHECKED) {
			// ������������ �ܾ��忡�� ������ ã�´�.
			wordBook = (WordBook *)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
			FindByMeaning(wordBook, meaning, &wordLinks, &count);
		}

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) wordLinks);


		// ã���� �ܾ ������ ã�� ù �ܾ ����.
		if (count > 0) {
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[0]->spelling);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[0]->meaning);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[0]->category);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[0]->example);
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
		SetProp(hWnd, "PROP_NUMBER", (HANDLE) number);

		// ������ ����.
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);

		// ã�� ������ ����.
		SetProp(hWnd, "PROP_COUNT", (HANDLE)count);
		sprintf(countText, "%d", count);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);
	}

	return TRUE;
}

BOOL FindingForm_OnSelectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Long number;
	HWND wordBookFormWindow;
	Word* (*wordLinks) = NULL;
	Word* wordLink = NULL;
	WordBook* wordBook = NULL;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory;
	HTREEITEM hTvMeaning;
	HTREEITEM hTvIt;
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];

	if (HIWORD(wParam) == BN_CLICKED) {

		// �ܾ��� ��ġ�� ã�´�.
		number = (Long)GetProp(hWnd, "PROP_NUMBER");
		wordLinks = (Word * (*)) GetWindowLongPtr(hWnd, GWLP_USERDATA);
		wordLink = wordLinks[number - 1];

		// ���������츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");

		// ������������ �ܾ��忡�� ��ġ�� �̵��Ѵ�.
		wordBook = (WordBook*)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
		wordLink = WordBook_Move(wordBook, wordLink);

		// ������������ Ʈ���信�� �ܾ�� �׸��� ã�´�.
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);

		// ������������ Ʈ������ �ܾ�� �׸񿡼� ���ĺ� �׸��� ã�´�.
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		alphabet[0] += 'a' - 'A';
		while (hTvIt != NULL && (wordLink->spelling[0] != alphabet[0])) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			alphabet[0] += 'a' - 'A';
		}
		hTvAlphabet = hTvIt;

		// ������������ Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö�� �׸��� ã�´�.
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

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸񿡼� ǰ�� �׸��� ã�´�.
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

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸񿡼� �� �׸��� ã�´�.
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

		// ������������ Ʈ������ �ܾ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvRoot);
		
		// ������������ Ʈ������ �ܾ�� - ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);
		
		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);
		
		// ���������쿡 �ܾ ����.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);

		// ã�� �����츦 �ݴ´�.
		if (wordLinks != NULL) {
			free(wordLinks);
		}

		RemoveProp(hWnd, "PROP_NUMBER");
		RemoveProp(hWnd, "PROP_COUNT");

		EndDialog(hWnd, 0);
	}

	return TRUE;
}

BOOL FindingForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*wordLinks) = NULL;
	TCHAR numberText[64];

	if (HIWORD(wParam) == BN_CLICKED) {

		// ã�� ù �ܾ ����.
		wordLinks = (Word * (*)) GetWindowLongPtr(hWnd, GWLP_USERDATA);

		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[0]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[0]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[0]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[0]->example);

		// ������ ����.
		SetProp(hWnd, "PROP_NUMBER", (HANDLE)1);
		sprintf(numberText, "%d", 1);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
	}

	return TRUE;
}

BOOL FindingForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*wordLinks) = NULL;
	Long number;
	TCHAR numberText[64];

	if (HIWORD(wParam) == BN_CLICKED) {

		// �������� �̵��Ѵ�.
		number = (Long)GetProp(hWnd, "PROP_NUMBER");
		if (number > 1) {
			number--;
		}


		// ���� �ܾ ����.
		wordLinks = (Word * (*)) GetWindowLongPtr(hWnd, GWLP_USERDATA);

		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[number - 1]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[number - 1]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[number - 1]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[number - 1]->example);

		// ������ ����.
		SetProp(hWnd, "PROP_NUMBER", (HANDLE)number);
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
	}

	return TRUE;
}

BOOL FindingForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*wordLinks) = NULL;
	Long number;
	Long count;
	TCHAR numberText[64];

	if (HIWORD(wParam) == BN_CLICKED) {
		// �������� �̵��Ѵ�.
		number = (Long)GetProp(hWnd, "PROP_NUMBER");
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		if (number < count) {
			number++;
		}


		// ���� �ܾ ����.
		wordLinks = (Word * (*))GetWindowLongPtr(hWnd, GWLP_USERDATA);

		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[number - 1]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[number - 1]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[number - 1]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[number - 1]->example);

		// ������ ����.
		SetProp(hWnd, "PROP_NUMBER", (HANDLE)number);
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
	}

	return TRUE;
}

BOOL FindingForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*wordLinks) = NULL;
	TCHAR numberText[64];

	Long count;

	if (HIWORD(wParam) == BN_CLICKED) {
		// ������ �̵��Ѵ�.
		count = (Long)GetProp(hWnd, "PROP_COUNT");

		// ������ �ܾ ����.
		wordLinks = (Word * (*)) GetWindowLongPtr(hWnd, GWLP_USERDATA);

		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[count - 1]->spelling);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[count - 1]->meaning);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[count - 1]->category);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLinks[count - 1]->example);

		// ������ ����
		SetProp(hWnd, "PROP_NUMBER", (HANDLE)count);
		sprintf(numberText, "%d", count);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
	}

	return TRUE;
}


BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Word* (*wordLinks) = NULL;

	wordLinks = (Word * (*)) GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (wordLinks != NULL) {
		free(wordLinks);
	}

	RemoveProp(hWnd, "PROP_NUMBER");
	RemoveProp(hWnd, "PROP_COUNT");


	EndDialog(hWnd, 0);

	return TRUE;
}