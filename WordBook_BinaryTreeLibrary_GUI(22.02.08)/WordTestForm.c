//WordTestForm.c
#include "WordTestForm.h"
#include "WordBook.h"
#include "WordIndexCardFile.h"
#include "WordIndexCard.h"
#include "resource.h"
#include <CommCtrl.h>
#include <string.h>
#include <WinUser.h>
#include <stdio.h>
#include <Windows.h>
#pragma warning(disable : 4996)


BOOL CALLBACK WordTestFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG: ret = WordTestForm_OnInitDialog(hWnd, wParam, lParam); break;
	//case WM_COMMAND: ret = WordTestForm_OnCommand(hWnd, wParam, lParam); break;
	//case WM_NOTIFY: ret = WordTestForm_OnNotify(hWnd, wParam, lParam); break;
	case WM_CLOSE: ret = WordTestForm_OnClose(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}

	return ret;
}

BOOL WordTestForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	// case IDC_BUTTON_NEXT: ret = WordTestForm_OnNextButtonClicked(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}

	return ret;
}

BOOL WordTestForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (wParam) {
	case IDC_EDIT_EXAMPLE: ret = WordTestForm_OnExampleEditControlTabKeyDown(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}

	return ret;
}

BOOL WordTestForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *wordBook;
	Word *wordLink;
	Long number = 0;
	TCHAR numberText[64];
	TCHAR categories[][16] = { "����", "���", "�����", "�λ�", "���ӻ�", "��ź��" };
	Long i = 0;
	TCHAR remainWordsCountText[64];

	// ���������츦 ã�´�.
	wordBookFormWindow = FindWindow("#32770", "�ܾ���");

	// ������������ �ܾ��忡 �ܾ ������
	wordBook = (WordBook *)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
	if (wordBook->length > 0) {
		// ó������ ����.
		wordLink = WordBook_First(wordBook);

		// ������ ����.
		number++;
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)number);
		
		// ö�ڸ� ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);

		// ǰ�� �޺��ڽ��� ����� �߰��Ѵ�.
		while (i < sizeof(categories) / sizeof(categories[0])) {
			SendMessage(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), CB_ADDSTRING, (WPARAM)0, (LPARAM)categories[i]);
			i++;
		}

		// ���� �ܾ� ���� ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CORRECTWORDSCOUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)"0");
		SetProp(hWnd, "PROP_CORRECTWORDSCOUNT", (HANDLE)0);

		// ���� �ܾ� ���� ����.
		sprintf(remainWordsCountText, "%d", wordBook->length);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_REMAINWORDSCOUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)remainWordsCountText);

		// �� ����Ʈ ��Ʈ���� ��Ŀ���Ѵ�.
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_MEANING));
	}
	else {
		// ������������ �ܾ��忡 �ܾ ������ ��, ǰ��, ���� ����Ʈ ��Ʈ���� ��Ȱ��ȭ�Ѵ�.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), FALSE);
	}

	return TRUE;
}


BOOL WordTestForm_OnExampleEditControlTabKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	TCHAR alphabet[2];
	TCHAR wordAlphabet[2];
	TCHAR spelling[48];
	TCHAR meaning[64];
	TCHAR category[16];
	TCHAR example[64];
	WordBook *wordBook;
	Word word;
	Word *wordLink;
	Long correctWordsCount;
	TCHAR correctWordsCountText[64];
	TCHAR remainWordsCountText[64];
	WordIndexCardFile *wordIndexCardFile;
	WordIndexCard *wordIndexCardLink;
	WordBook *memorizedWordBook;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet;
	HTREEITEM hTvSpelling;
	HTREEITEM hTvCategory;
	HTREEITEM hTvMeaning;
	HTREEITEM hTvIt;
	TVITEM tvItem = { 0, };
	BOOL result;
	BOOL meaningResult;
	BOOL categoryResult;
	BOOL exampleResult;
	TCHAR meaningAnswer[68];
	TCHAR categoryAnswer[20];
	TCHAR exampleAnswer[68];

	if (((LPNMHDR)lParam)->code == NM_KEYDOWN && ((LPNMKEY)lParam)->nVKey == VK_TAB) {
		
		// ��, ǰ��, ���ø� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)sizeof(meaning), (LPARAM)meaning);
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), WM_GETTEXT, (WPARAM)sizeof(category), (LPARAM)category);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_GETTEXT, (WPARAM)sizeof(example), (LPARAM)example);

		// ���������츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");
		
		// �ܾ�� ���� ä���Ѵ�.
		wordBook = (WordBook *)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
		wordLink = wordBook->current;
		if (strcmp(wordLink->meaning, meaning) != 0) {
			meaningResult = FALSE;
			result = FALSE;
		}
		else if (strcmp(wordLink->category, category) != 0) {
			categoryResult = FALSE;
			result = FALSE;
		}
		else if (strcmp(wordLink->example, example) != 0) {
			exampleResult = FALSE;
			result = FALSE;
		}
		else {
			result = TRUE;
		}

		SetProp(hWnd, "PROP_RESULT", (HANDLE)result);

		// �¾�����
		if(result == TRUE) {
			// ������������ �ܾ��忡�� �ܾ ������.
			word = WordBook_Draw(wordBookFormWindow, wordLink);

			// ������������ ����ö���� �ܾ ������.
			wordIndexCardFile = (WordIndexCardFile *)GetProp(wordBookFormWindow, "PROP_WORDINDEXCARDFILE");
			wordIndexCardLink = WordIndexCardFile_Draw(wordIndexCardFile, wordLink, word.spelling[0]);

			// ������������ �ܿ� �ܾ��忡 �ܾ �����.
			memorizedWordBook = (WordBook*)GetProp(wordBookFormWindow, "PROP_MEMORIZEDWORDBOOK");
			wordLink = WordBook_Put(memorizedWordBook, word);

			// ä������� ����.
			//Sendmessage(GetDlgItem(hWnd, IDC_STATIC_MEANINGRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");
			
		}
	}
}


BOOL WordTestForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *wordBook;
	Word *wordLink;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet = NULL;
	HTREEITEM hTvSpelling = NULL;
	HTREEITEM hTvCategory = NULL;
	HTREEITEM hTvMeaning = NULL;
	HTREEITEM hTvIt;
	TVITEM tvItem = { 0, };
	TCHAR alphabet[2];
	TCHAR wordAlphabet[2];
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];

	// ���������츦 ã�´�.
	wordBookFormWindow = FindWindow("#32770", "�ܾ���");
	
	// ������������ �ܾ��忡 ���� �ܾ ������
	wordBook = (WordBook*)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
	wordLink = wordBook->current;
	if (wordLink != NULL) {
		
		// ������������ Ʈ���信�� �ܾ�� �׸��� ã�´�
		hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
			(WPARAM)TVGN_ROOT, (LPARAM)0);

		// ������������ Ʈ������ �ܾ�� �׸񿡼� ���ĺ� �׸��� ã�´�.
		tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
		tvItem.pszText = alphabet;
		tvItem.cchTextMax = sizeof(alphabet);
		wordAlphabet[0] = wordLink->spelling[0];
		if (wordAlphabet[0] >= 'a' && wordAlphabet[0] <= 'z') {
			wordAlphabet[0] -= 'a' - 'A';
		}
		wordAlphabet[1] = '\0';
		hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
			(WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
		tvItem.hItem = hTvIt;
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		while (hTvIt != NULL && (wordAlphabet[0] != alphabet[0])) {
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
		}
		hTvAlphabet = hTvIt;

		// ������������ Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö�� �׸��� ã�´�.
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

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸񿡼� ǰ�� �׸��� ã�´�.
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

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸񿡼� �� �׸��� ã�´�.

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

		// ������������ Ʈ������ �ܾ�� - ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvAlphabet);

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvSpelling);

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� ��ģ��.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hTvCategory);

		// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �����Ѵ�
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hTvMeaning);

		// ���������쿡�� �ܾ ����.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->meaning);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->category);
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->example);
	}
	else {
		// ������������ �ܾ��忡 �ܾ ������ �ܾ��ؽ�Ʈ�� �����.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
	}

	// �����츦 �ݴ´�.
	RemoveProp(hWnd, "PROP_CORRECTWORDSCOUNT");
	RemoveProp(hWnd, "PROP_RESULT");

	return TRUE;
}