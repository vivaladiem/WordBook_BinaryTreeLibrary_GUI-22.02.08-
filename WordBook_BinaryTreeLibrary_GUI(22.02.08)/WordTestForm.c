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
#include <timeapi.h>
#include <mmsystem.h>
#pragma warning(disable : 4996)


BOOL CALLBACK WordTestFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG: ret = WordTestForm_OnInitDialog(hWnd, wParam, lParam); break;
	case WM_COMMAND: ret = WordTestForm_OnCommand(hWnd, wParam, lParam); break;
	case WM_CLOSE: ret = WordTestForm_OnClose(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}

	return ret;
}

#if 0
void CALLBACK WordTestFormTimerProc(UINT uTimerId, UINT message, DWORD dwUserData, DWORD dw1, DWORD dw2) {
	HWND hWnd;
	TCHAR remainSecondText[16];
	Long remainedSeconds;

	// �����츦 �����´�.
	hWnd = FindWindow("#32770", "����");

	// �ð��� ���.
	remainedSeconds = (Long) GetProp(hWnd, "PROP_TIME");


	// ���� �ð��� ������ 
	if (remainedSeconds > 0) {
		// ���� �ð��� ����.
		sprintf(remainSecondText, "%d��", remainedSeconds);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_REMAINSECONDS), WM_SETTEXT, (WPARAM)0, (LPARAM)remainSecondText);
	}
	else {
		// ���� �ð��� ������
		// �޽����� �ð��ʰ���� ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"�ð� �ʰ�..!");
		
		// ��, ǰ��, ���ø� ��Ȱ��ȭ�Ѵ�.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), FALSE);

		// ���� ��ư�� ��Ȱ��ȭ�Ѵ�.
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_NEXT), FALSE);

		// Ÿ�̸Ӹ� ���ش�.
		timeKillEvent(uTimerId);
	}


}
#endif

VOID CALLBACK WaitOrTimerCallback(PVOID timer, BOOLEAN TimerOrWaitFired) {
	HWND hWnd;
	TCHAR remainSecondText[16];
	Long remainedSeconds;

	// �����츦 �����´�.
	hWnd = FindWindow("#32770", "����");

	// �ð��� ���.
	remainedSeconds = (Long)GetProp(hWnd, "PROP_TIME");
	remainedSeconds--;
	SetProp(hWnd, "PROP_TIME", (HANDLE)remainedSeconds);

		// ���� �ð��� ����.
		sprintf(remainSecondText, "%d��", remainedSeconds);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_REMAINSECONDS), WM_SETTEXT, (WPARAM)0, (LPARAM)remainSecondText);
		

}

VOID CALLBACK OnTimeEndCallback() {
	HWND hWnd = FindWindow("#32770", "����");

	// ���� �ð��� ������
		// �޽����� �ð��ʰ���� ����.
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"�ð� �ʰ�..!");

	// ��, ǰ��, ���ø� ��Ȱ��ȭ�Ѵ�.
	EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);
	EnableWindow(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), FALSE);
	EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), FALSE);

	// ���� ��ư�� ��Ȱ��ȭ�Ѵ�.
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_NEXT), FALSE);
}
BOOL WordTestForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_EDIT_MEANING: ret = WordTestForm_OnMeaningLostFocus(hWnd, wParam, lParam); break;
	case IDC_COMBO_CATEGORY: ret = WordTestForm_OnCategoryLostFocus(hWnd, wParam, lParam); break;
	case IDC_EDIT_EXAMPLE: ret = WordTestForm_OnExampleLostFocus(hWnd, wParam, lParam); break;
	case IDC_BUTTON_NEXT: ret = WordTestForm_OnNextButtonClicked(hWnd, wParam, lParam); break;
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
	PHANDLE timer;
	Long remainedSeconds = 20;


	// ���������츦 ã�´�.
	wordBookFormWindow = FindWindow("#32770", "�ܾ���");

	// ������������ �ܾ��忡 �ܾ ������
	wordBook = (WordBook *)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
	if (wordBook->length > 0) {
		// ó������ ����.
		wordLink = WordBook_First(wordBook);

		// ������ ����.
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

		// ������� ����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PROGRESSRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)"0.0%");

		// Ÿ�̸Ӹ� �����Ѵ�.
		//timer = timeSetEvent(1000, 5, WordTestFormTimerProc, (DWORD_PTR) NULL, TIME_PERIODIC);
		CreateTimerQueueTimer(timer, NULL, WaitOrTimerCallback, NULL, 20 * 1000, 1000, WT_EXECUTEDEFAULT);
		SetProp(hWnd, "PROP_TIMER", (HANDLE)timer);
		SetProp(hWnd, "PROP_TIME", (HANDLE)20);
		
		

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

BOOL WordTestForm_OnMeaningLostFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	TCHAR meaning[64];
	WordBook *wordBook;
	Word *wordLink;
	BOOL meaningResult;

	if (HIWORD(wParam) == EN_KILLFOCUS) {

		// ���� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)sizeof(meaning), (LPARAM)meaning);

		// ���������츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");

		// �ܾ�� ���� ä���Ѵ�.
		wordBook = (WordBook *)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
		wordLink = wordBook->current;
		if (strcmp(wordLink->meaning, meaning) == 0) {
			meaningResult = TRUE;
		} else {
			meaningResult = FALSE;
		}

			// ä������� ����.
		if (meaningResult == TRUE) {
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANINGRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");
		} else {
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANINGRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"X");
		}
	}

	return TRUE;
}

BOOL WordTestForm_OnCategoryLostFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	TCHAR category[64];
	WordBook *wordBook;
	Word *wordLink;
	BOOL categoryResult;

	if (HIWORD(wParam) == CBN_KILLFOCUS) {

		// ���� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), WM_GETTEXT, (WPARAM)sizeof(category), (LPARAM)category);

		// ���������츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");

		// �ܾ�� ���� ä���Ѵ�.
		wordBook = (WordBook *)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
		wordLink = wordBook->current;
		if (strcmp(wordLink->category, category) == 0) {
			categoryResult = TRUE;
		}
		else {
			categoryResult = FALSE;
		}

			// ä������� ����.
		if (categoryResult == TRUE) {
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORYRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");
		}
		else {
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORYRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"X");
		}
	}

	return TRUE;
}

BOOL WordTestForm_OnExampleLostFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	TCHAR alphabet[2];
	char wordAlphabet;
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
	Long number;
	double correctRate;
	double progressRate;
	TCHAR progressRateText[16];
	MMRESULT timer;
	WordIndexCardFile *wordIndexCardFile;
	WordIndexCard *wordIndexCardLink;
	WordBook *memorizedWordBook;
	HTREEITEM hTvRoot;
	HTREEITEM hTvAlphabet = NULL;
	HTREEITEM hTvSpelling = NULL;
	HTREEITEM hTvCategory = NULL;
	HTREEITEM hTvMeaning = NULL;
	HTREEITEM hTvIt;
	HTREEITEM hTvChild;
	TVITEM tvItem = { 0, };
	BOOL result;
	BOOL meaningResult;
	BOOL categoryResult;
	BOOL exampleResult;

	if (HIWORD(wParam) == EN_KILLFOCUS) {

		// ��, ǰ��, ���ø� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)sizeof(meaning), (LPARAM)meaning);
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), WM_GETTEXT, (WPARAM)sizeof(category), (LPARAM)category);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_GETTEXT, (WPARAM)sizeof(example), (LPARAM)example);

		// ���������츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");

		// �ܾ�� ���� ä���Ѵ�.
		wordBook = (WordBook *)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
		wordLink = wordBook->current;
		if (strcmp(wordLink->meaning, meaning) == 0) {
			meaningResult = TRUE;
		}
		else {
			meaningResult = FALSE;
		}

		if (strcmp(wordLink->category, category) == 0) {
			categoryResult = TRUE;
		}
		else {
			categoryResult = FALSE;
		}

		if (strcmp(wordLink->example, example) == 0) {
			exampleResult = TRUE;
		}
		else {
			exampleResult = FALSE;
		}

		if (meaningResult == TRUE && categoryResult == TRUE && exampleResult == TRUE) {
			result = TRUE;
		}
		else {
			result = FALSE;
		}

		SetProp(hWnd, "PROP_RESULT", (HANDLE)result);

		// �¾�����
		if (result == TRUE) {
			// ������������ �ܾ��忡�� �ܾ ������.
			word = WordBook_Draw(wordBook, wordLink);

			// ������������ ����ö���� �ܾ ������.
			wordIndexCardFile = (WordIndexCardFile *)GetProp(wordBookFormWindow, "PROP_WORDINDEXCARDFILE");
			wordIndexCardLink = WordIndexCardFile_Draw(wordIndexCardFile, wordLink, word.spelling[0]);

			// ������������ �ܿ� �ܾ��忡 �ܾ �����.
			memorizedWordBook = (WordBook*)GetProp(wordBookFormWindow, "PROP_MEMORIZEDWORDBOOK");
			wordLink = WordBook_Put(memorizedWordBook, word);

			// ä������� ����.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLERESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");

			// ���� ������ ����.
			correctWordsCount = (Long)GetProp(hWnd, "PROP_CORRECTWORDSCOUNT");
			correctWordsCount++;
			sprintf(correctWordsCountText, "%d", correctWordsCount);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CORRECTWORDSCOUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)correctWordsCountText);
			SetProp(hWnd, "PROP_CORRECTWORDSCOUNT", (HANDLE)correctWordsCount);

			// ���� �ܾ� ���� ����.
			sprintf(remainWordsCountText, "%d", wordBook->length);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_REMAINWORDSCOUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)remainWordsCountText);

			// Ÿ�̸Ӹ� �����.
			timer = (MMRESULT)GetProp(hWnd, "PROP_TIMER");
			DeleteTimerQueueEx(timer, NULL);

			// ��, ǰ��, ���� ����Ʈ ��Ʈ���� ��Ȱ��ȭ�Ѵ�.
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), FALSE);


			// ������� ���Ѵ�.
			number = (Long)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (number > 0) {
				correctRate = correctWordsCount / number * 1.00;
				// ������� ���� �޽����� ����.
				if (correctRate > 0.9) {
					SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"����� �� �ϰ� ��ó׿�! ���� ��� ���߰� �־��!");
				}
				else if (correctRate > 0.5) {

				}
				else {
					SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"����� �ο��� �� �̾���� ��ó׿�! �� �� ��������!");
				}
			}
			


			// ���������� ����.
			progressRate = correctWordsCount / ((correctWordsCount + wordBook->length) * 1.00) * 100.0;
			sprintf(progressRateText, "%.1f%%", progressRate);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_PROGRESSRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)progressRateText);
			
			if (wordBook->length < 10) {
				SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"���� �� �Ծ��! ���ݸ� �� ��������..!");
			}

			

			// ������������ Ʈ���信�� �ܾ�� �׸��� ã�´�.
			hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);

			// ������������ Ʈ������ �ܾ�� �׸񿡼� ���ĺ� �׸��� ã�´�.
			tvItem.mask = TVIF_HANDLE | TVIF_TEXT;
			tvItem.pszText = alphabet;
			tvItem.cchTextMax = sizeof(alphabet);
			wordAlphabet = word.spelling[0];
			if (wordAlphabet >= 'a' && wordAlphabet <= 'z') {
				wordAlphabet -= 'a' - 'A';
			}
			hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvRoot);
			tvItem.hItem = hTvIt;
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			while (hTvIt != NULL && (wordAlphabet != alphabet[0])) {
				hTvIt = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hTvIt);
				tvItem.hItem = hTvIt;
				SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvItem);
			}
			hTvAlphabet = hTvIt;

			// ������������ Ʈ������ �ܾ�� - ���ĺ� �׸񿡼� ö�� �׸��� ã�´�.
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

			// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸񿡼� ǰ�� �׸��� ã�´�.
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

			// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸񿡼� �� �׸��� ã�´�.
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

			// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� - �� �׸��� �����.
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvMeaning);

			// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� - ǰ�� �׸��� �ڽ��� ������ �����.
			hTvChild = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
			if (hTvChild == NULL) {
				SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvCategory);

				// ������������ Ʈ������ �ܾ�� - ���ĺ� - ö�� �׸��� �ڽ��� ������ �����.
				hTvChild = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
				if (hTvChild == NULL) {
					SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvSpelling);

					// ������������ Ʈ������ �ܾ�� - ���ĺ� �׸��� �ڽ��� ������ �����.
					hTvChild = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
					if (hTvChild == NULL) {
						SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvAlphabet);
					}
				}
			}
		}
		else {
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLERESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"X");
		}
	}

	return TRUE;
}


BOOL WordTestForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *wordBook;
	Word *wordLink;
	Word *previousWordLink = NULL;
	BOOL result;
	Long number;
	TCHAR numberText[64];
	MMRESULT timer;

	if (HIWORD(wParam) == BN_CLICKED) {

		// ���������츦 ã�´�.
		wordBookFormWindow = FindWindow("#32770", "�ܾ���");
		wordBook = (WordBook*)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);

		// ä����� Ʋ������ ������������ �ܾ��忡�� �������� �̵��Ѵ�.
		result = (BOOL)GetProp(hWnd, "PROP_RESULT");
		if (result == FALSE) {
			previousWordLink = wordBook->current;
			WordBook_Next(wordBook);
		}

		// ��, ǰ��, ���� ����Ʈ ��Ʈ���� �ؽ�Ʈ�� �����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// ��, ǰ��, ������ ä������� �����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANINGRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORYRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLERESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// ��, ǰ��, ���� ����� �����.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANINGANSWER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORYANSWER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLEANSWER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// ������������ �ܾ��忡 ���� �ܾ ������
		wordLink = wordBook->current;
		if (wordLink != NULL) {
			// ������ �ܾ��̸� ó������ ����.
			if (wordLink == previousWordLink) {
				wordLink = WordBook_First(wordBook);
			}

			// ������ ���Ѵ�.
			number = (Long)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			number++;

			// ������ ����.
			sprintf(numberText, "%d", number);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)number);

			// ö�ڸ� ����.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);

			// ��, ǰ��, ���ø� Ȱ��ȭ�Ѵ�.
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), TRUE);
			EnableWindow(GetDlgItem(hWnd, IDC_COMBO_CATEGORY), TRUE);
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), TRUE);

			// �ð��� �����Ѵ�.
			SetProp(hWnd, "PROP_TIME", (HANDLE) 20);

			// �ð��� ���.
			timeSetEvent(1000, 5, WordTestFormTimerProc, (DWORD_PTR) NULL, TIME_PERIODIC);

			// �� ����Ʈ ��Ʈ���� ��Ŀ���Ѵ�.
			SetFocus(GetDlgItem(hWnd, IDC_EDIT_MEANING));
		}
		else {
			// ������������ �ܾ��忡 �ܾ ������
			
			// �޽����� ����.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"����Ͻó׿�! ��� �ܾ �Ϻ��� �ܿ�̱���!");

			// ö�ڸ� �����.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

			// �ð��� �����.
			timer = (MMRESULT)GetProp(hWnd, "PROP_TIMER");
			DeleteTimerQueueEx(timer, NULL);
		}

	}

	return TRUE;
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
	char wordAlphabet;
	TCHAR spelling[48];
	TCHAR category[16];
	TCHAR meaning[64];
	MMRESULT timer;

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

	// �ð��� �����.
	timer = (MMRESULT) GetProp(hWnd, "PROP_TIMER");
	timeKillEvent(timer);

	// �����츦 �ݴ´�.
	RemoveProp(hWnd, "PROP_CORRECTWORDSCOUNT");
	RemoveProp(hWnd, "PROP_RESULT");

	EndDialog(hWnd, 0);
	
	return TRUE;
}