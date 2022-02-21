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
#define IDT_TIMER 1
#define TIMELIMIT 25
#pragma warning(disable : 4996)

// 다음버튼 없이, 포커스 잃을 때 맞았으면 바로 다음 단어로, 뜻에 포커스
// 복습하기 : 답보기면 한번에 다 보여야 할 것으로 느껴짐. 각 컨트롤마다 하나씩 하는게 직관적
//

BOOL CALLBACK WordTestFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG: ret = WordTestForm_OnInitDialog(hWnd, wParam, lParam); break;
	case WM_COMMAND: ret = WordTestForm_OnCommand(hWnd, wParam, lParam); break;
	case WM_TIMER: ret = WordTestForm_OnTimerTick(hWnd, wParam, lParam); break;
	case WM_CLOSE: ret = WordTestForm_OnClose(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}

	return ret;
}

BOOL WordTestForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_EDIT_MEANING: ret = WordTestForm_OnMeaningLostFocus(hWnd, wParam, lParam); break;
	case IDC_EDIT_CATEGORY: ret = WordTestForm_OnCategoryLostFocus(hWnd, wParam, lParam); break;
	case IDC_EDIT_EXAMPLE: ret = WordTestForm_OnExampleLostFocus(hWnd, wParam, lParam); break;
	case IDC_BUTTON_NEXT: ret = WordTestForm_OnNextButtonClicked(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}

	return ret;
}

BOOL WordTestForm_OnTimerTick(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Long remainSeconds;
	TCHAR remainSecondsText[8];

	// 남은 시간을 센다.
	remainSeconds = (Long)GetProp(hWnd, "PROP_REMAINSECONDS");
	remainSeconds--;
	SetProp(hWnd, "PROP_REMAINSECONDS", (HANDLE)remainSeconds);

	// 시간이 남아있으면
	if (remainSeconds > 0) {
		// 남은 시간을 쓴다.
		sprintf(remainSecondsText, "%d초", remainSeconds);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_REMAINSECONDS), WM_SETTEXT, (WPARAM)0, (LPARAM)remainSecondsText);
	}
	else {
		// 시간이 다 됐으면
		
		// 메시지를 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"시간 초과!");

		// 시간을 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_REMAINSECONDS), WM_SETTEXT, (WPARAM)0, (LPARAM)"0초");

		// 뜻, 품사, 예시를 비활성화한다.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_CATEGORY), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), FALSE);

		// 다음 버튼을 비활성화한다.
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_NEXT), FALSE);
	}
	

	return TRUE;
}



BOOL WordTestForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	WordBook *wordBook;
	Word *wordLink;
	Long number = 0;
	TCHAR numberText[64];
	//TCHAR categories[][16] = { "동사", "명사", "형용사", "부사", "접속사", "감탄사" }; // 시험볼 땐 에디트가 맞다.
	Long i = 0;
	TCHAR remainWordsCountText[64];
	TCHAR remainSecondsText[8];


	
	// 메인윈도우를 찾는다.
	wordBookFormWindow = FindWindow("#32770", "단어장");

	// 메인윈도우의 단어장에 단어가 있으면
	wordBook = (WordBook *)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
	if (wordBook->length > 0) {
		// 처음으로 간다.
		wordLink = WordBook_First(wordBook);

		// 순번을 쓴다.
		sprintf(numberText, "%d", number);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)number);
		
		// 철자를 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);

		// 맞춘 단어 수를 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CORRECTWORDSCOUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)"0");
		SetProp(hWnd, "PROP_CORRECTWORDSCOUNT", (HANDLE)0);

		// 남은 단어 수를 쓴다.
		sprintf(remainWordsCountText, "%d", wordBook->length);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_REMAINWORDSCOUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)remainWordsCountText);

		// 타이머를 생성한다.
		SetTimer(hWnd, IDT_TIMER, 1000, NULL);

		// 남은 시간을 초기화한다.
		SetProp(hWnd, "PROP_REMAINSECONDS", (HANDLE) TIMELIMIT);

		// 남은 시간을 쓴다.
		sprintf(remainSecondsText, "%d초", TIMELIMIT);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_REMAINSECONDS), WM_SETTEXT, (WPARAM)0, (LPARAM)remainSecondsText);

		// 진행률을 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PROGRESSRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)"0.0%");


		// 뜻 에디트 컨트롤을 포커스한다.
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_MEANING));
	}
	else {
		// 메인윈도우의 단어장에 단어가 없으면 뜻 에디트 컨트롤을 비활성화한다.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);
	}

	// 품사, 예시를 비활성화한다.
	EnableWindow(GetDlgItem(hWnd, IDC_EDIT_CATEGORY), FALSE);
	EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), FALSE);

	return TRUE;
}

BOOL WordTestForm_OnMeaningLostFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND wordBookFormWindow;
	TCHAR meaning[64];
	WordBook *wordBook;
	Word *wordLink;
	BOOL meaningResult;

	if (HIWORD(wParam) == EN_KILLFOCUS) {

		// 뜻을 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)sizeof(meaning), (LPARAM)meaning);

		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");

		// 단어와 비교해 채점한다.
		wordBook = (WordBook *)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
		wordLink = wordBook->current;
		if (strcmp(wordLink->meaning, meaning) == 0) {
			meaningResult = TRUE;
		} else {
			meaningResult = FALSE;
		}

		// 맞았으면
		if (meaningResult == TRUE) {
			// 채점 결과를 쓴다.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANINGRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");

			// 품사 에디트컨트롤을 활성화한다.
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_CATEGORY), TRUE);

			// 품사 에디트컨트롤을 포커스한다.
			SetFocus(GetDlgItem(hWnd, IDC_EDIT_CATEGORY));
		} else {
			// 틀렸으면 채점 결과를 쓴다.
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

	if (HIWORD(wParam) == EN_KILLFOCUS) {

		// 뜻을 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CATEGORY), WM_GETTEXT, (WPARAM)sizeof(category), (LPARAM)category);

		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");

		// 단어와 비교해 채점한다.
		wordBook = (WordBook *)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);
		wordLink = wordBook->current;
		if (strcmp(wordLink->category, category) == 0) {
			categoryResult = TRUE;
		}
		else {
			categoryResult = FALSE;
		}

		// 맞았으면
		if (categoryResult == TRUE) {
			// 채점 결과를 쓴다.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORYRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");

			// 예시 에디트컨트롤을 활성화한다.
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), TRUE);

			// 예시 에디트컨트롤을 포커스한다.
			SetFocus(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE));
		}
		else {
			// 틀렸으면 채점 결과를 쓴다.
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
	TCHAR countText[64];

	if (HIWORD(wParam) == EN_KILLFOCUS) {

		// 뜻, 품사, 예시를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)sizeof(meaning), (LPARAM)meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CATEGORY), WM_GETTEXT, (WPARAM)sizeof(category), (LPARAM)category);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_GETTEXT, (WPARAM)sizeof(example), (LPARAM)example);

		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");

		// 단어와 비교해 채점한다.
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

		// 맞았으면
		if (result == TRUE) {
			// 메인윈도우의 단어장에서 단어를 빼낸다.
			word = WordBook_Draw(wordBook, wordLink);

			// 메인윈도우의 색인철에서 단어를 빼낸다.
			wordIndexCardFile = (WordIndexCardFile *)GetProp(wordBookFormWindow, "PROP_WORDINDEXCARDFILE");
			wordIndexCardLink = WordIndexCardFile_Draw(wordIndexCardFile, wordLink, word.spelling[0]);

			// 메인윈도우의 외운 단어장에 단어를 끼운다.
			memorizedWordBook = (WordBook*)GetProp(wordBookFormWindow, "PROP_MEMORIZEDWORDBOOK");
			wordLink = WordBook_Put(memorizedWordBook, word);

			// 채점결과를 쓴다.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLERESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");

			// 뜻, 품사, 예시 에디트 컨트롤을 비활성화한다.
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_CATEGORY), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), FALSE);

			// 맞춘 개수를 쓴다.
			correctWordsCount = (Long)GetProp(hWnd, "PROP_CORRECTWORDSCOUNT");
			correctWordsCount++;
			sprintf(correctWordsCountText, "%d", correctWordsCount);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_CORRECTWORDSCOUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)correctWordsCountText);
			SetProp(hWnd, "PROP_CORRECTWORDSCOUNT", (HANDLE)correctWordsCount);

			// 남은 단어 수를 쓴다.
			sprintf(remainWordsCountText, "%d", wordBook->length);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_REMAINWORDSCOUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)remainWordsCountText);

			// 타이머를 없앤다.
			KillTimer(hWnd, IDT_TIMER);

			


			// 정답률을 구한다.
			number = (Long)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (number > 0) {
				correctRate = correctWordsCount / number * 1.00;
				// 정답률에 따라 메시지를 쓴다.
				if (correctRate > 0.9) {
					SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"대단히 잘 하고 계시네요! 거의 모두 맞추고 있어요!");
				}
				else if (correctRate > 0.5) {

				}
				else {
					SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"어려운 싸움을 잘 이어나가고 계시네요! 좀 더 힘내세요!");
				}
			}
			


			// 진행정보를 쓴다.
			progressRate = correctWordsCount / ((correctWordsCount + wordBook->length) * 1.00) * 100.0;
			sprintf(progressRateText, "%.1f%%", progressRate);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_PROGRESSRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)progressRateText);
			
			if (wordBook->length < 10) {
				SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"거의 다 왔어요! 조금만 더 힘내세요..!");
			}

			

			// 메인윈도우의 트리뷰에서 단어들 항목을 찾는다.
			hTvRoot = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);

			// 메인윈도우의 트리뷰의 단어들 항목에서 알파벳 항목을 찾는다.
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

			// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 - 뜻 항목을 지운다.
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvMeaning);

			// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 - 품사 항목의 자식이 없으면 지운다.
			hTvChild = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvCategory);
			if (hTvChild == NULL) {
				SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvCategory);

				// 메인윈도우의 트리뷰의 단어들 - 알파벳 - 철자 항목의 자식이 없으면 지운다.
				hTvChild = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvSpelling);
				if (hTvChild == NULL) {
					SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvSpelling);

					// 메인윈도우의 트리뷰의 단어들 - 알파벳 항목의 자식이 없으면 지운다.
					hTvChild = (HTREEITEM)SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hTvAlphabet);
					if (hTvChild == NULL) {
						SendMessage(GetDlgItem(wordBookFormWindow, IDC_TREE_WORDS), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hTvAlphabet);
					}
				}
			}

			// 메인윈도우에서 개수를 쓴다.
			sprintf(countText, "%d", wordBook->length);
			SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_COUNT), WM_SETTEXT, (WPARAM)0, (LPARAM)countText);
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
	TCHAR remainSecondsText[8];

	if (HIWORD(wParam) == BN_CLICKED) {

		// 메인윈도우를 찾는다.
		wordBookFormWindow = FindWindow("#32770", "단어장");
		wordBook = (WordBook*)GetWindowLongPtr(wordBookFormWindow, GWLP_USERDATA);

		// 채점결과 틀렸으면 메인윈도우의 단어장에서 다음으로 이동한다.
		result = (BOOL)GetProp(hWnd, "PROP_RESULT");
		if (result == FALSE) {
			previousWordLink = wordBook->current;
			WordBook_Next(wordBook);
		}

		// 뜻, 품사, 예시 에디트 컨트롤의 텍스트를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// 뜻, 품사, 예시의 채점결과를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANINGRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORYRESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLERESULT), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// 뜻, 품사, 예시 답안을 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_MEANINGANSWER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_CATEGORYANSWER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_EXAMPLEANSWER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		// 뜻, 품사, 예시를 비활성화한다.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SPELLING), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_CATEGORY), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), FALSE);
		

		// 메인윈도우의 단어장에 현재 단어가 있으면
		wordLink = wordBook->current;
		if (wordLink != NULL) {
			// 마지막 단어이면 처음으로 간다.
			if (wordLink == previousWordLink) {
				wordLink = WordBook_First(wordBook);
			}

			// 순번을 구한다.
			number = (Long)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			number++;

			// 순번을 쓴다.
			sprintf(numberText, "%d", number);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_NUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)numberText);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)number);

			// 철자를 쓴다.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordLink->spelling);

			// 뜻을 활성화한다.
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), TRUE);

			// 타이머를 만든다.
			SetTimer(hWnd, IDT_TIMER, 1000, NULL);
			
			// 남은 시간을 리셋한다.
			SetProp(hWnd, "PROP_REMAINSECONDS", (HANDLE) TIMELIMIT);

			// 남은 시간을 쓴다.
			sprintf(remainSecondsText, "%d초", TIMELIMIT);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_REMAINSECONDS), WM_SETTEXT, (WPARAM)0, (LPARAM)remainSecondsText);


			// 뜻 에디트 컨트롤을 포커스한다.
			SetFocus(GetDlgItem(hWnd, IDC_EDIT_MEANING));
		}
		else {
			// 메인윈도우의 단어장에 단어가 없으면
			
			// 메시지를 쓴다.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_MESSAGE), WM_SETTEXT, (WPARAM)0, (LPARAM)"대단하시네요! 모든 단어를 완벽히 외우셨군요!");

			// 철자를 지운다.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

			// 타이머를 없앤다.
			KillTimer(hWnd, IDT_TIMER);

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

	// 타이머를 없앤다.
	KillTimer(hWnd, IDT_TIMER);

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
	else {
		// 메인윈도우의 단어장에 단어가 없으면 단어텍스트를 지운다.
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_CATEGORY), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(wordBookFormWindow, IDC_STATIC_EXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
	}


	// 윈도우를 닫는다.
	RemoveProp(hWnd, "PROP_CORRECTWORDSCOUNT");
	RemoveProp(hWnd, "PROP_RESULT");

	EndDialog(hWnd, 0);
	
	return TRUE;
}