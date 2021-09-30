#define _CRT_SECURE_NO_WARNINGS
#include "Complier.h"

extern jmp_buf jmpbuf;
extern int JmpJud;
extern int Row;
extern pTkWord pcur;
extern TkWord word[MAX];
extern int wordlen;

int main() {
	static TkWord keywords[] = {
	{TK_PLUS, "+"},
	{TK_MINUS, "-"},
	{TK_MUL, "*"},
	{TK_DIVIDE, "/"},
	{TK_MOD, "%"},
	{TK_EQ, "=="},
	{TK_NEQ, "!="},
	{TK_LT, "<"},
	{TK_LEQ, "<="},
	{TK_GT, ">"},
	{TK_GEQ, ">="},
	{TK_ASSIGN, "="},
	{TK_POINTSTO, "->"},
	{TK_DOT, "."},
	{TK_AND, "&"},
	{TK_OPENPA, "("},
	{TK_CLOSEPA, ")"},
	{TK_OPENBR, "["},
	{TK_CLOSEBR, "]"},
	{TK_BEGIN, "{"},
	{TK_END, "}"},
	{TK_SEMICOLON, ";"},
	{TK_COMMA, ","},
	{TK_ELLIPSIS, "..."},
	{TK_REF, "//"},
	{TK_EOF, "End_Of_File"},

	{TK_CINT, "ÕûÐÍ³£Á¿"},
	{TK_CCHAR, "×Ö·û³£Á¿"},
	{TK_CSTR, "×Ö·û´®³£Á¿"},

	{KW_CHAR, "char"},
	{KW_SHORT, "short"},
	{KW_INT, "int"},
	{KW_VOID, "void"},
	{KW_STRUCT, "struct"},

	{KW_IF, "if"},
	{KW_ELSE, "else"},
	{KW_FOR, "for"},
	{KW_WHILE, "while"},
	{KW_CONTINUE, "continue"},
	{KW_BREAK, "break"},
	{KW_RETURN, "return"},
	{KW_SIZEOF, "sizeof"},
	{KW_ALIGN, "__align"},
	{KW_CDECL, "__cdecl"},
	{KW_STDCALL, "__stdcall"},
	{0}
	};
	vector <TkWord> hashtable[MAX];
	int keywordsLen = sizeof(keywords) / sizeof(TkWord);
	std::hash<std::string> h;
	int value = 0;

	for (int i = 0; i < keywordsLen - 1; ++i) {
		int value = h(keywords[i].s) % MAX;
		hashtable[value].push_back(keywords[i]);
	}

	int iRow = 1;
	char c[MAX] = { 0 };
	TkWord tmp;
	string s;
	FILE* fp = fopen("C:\\Users\\lordx\\Desktop\\code.txt", "r");
	PrintBegin();
	/*------------------------------- ´Ê·¨·ÖÎö------------------------------------*/
	while (fgets(c, 1000, fp) != NULL) {
		printf("%-4d", iRow);
		int i = 0;
		while(i < strlen(c)){
			if (c[i] == ' ') {
				printf("%c", c[i++]);
			}
			else if (c[i] == '\n') {
				printf("%c", c[i++]);
			}
			else if (c[i] == '\t') {
				printf("%c", c[i++]);
			}
			else if (c[i] == '/' && c[i + 1] == '/') {
				s += c[i++];
				s += c[i++];
				tmp.token = TK_REF;
				tmp.s = s;
				tmp.row = iRow;
				word[wordlen++] = tmp;
				Print(s, 29);
				while (i < strlen(c)) {
					string t;
					t += c[i];
					Print(t, 29);
					i++;
				}
				tmpClera(&tmp);
				s.clear();
			}
			else if (IS_LETTER(c[i])) {
				while (IS_LETTER_OR_NUM(c[i])) {
					s += c[i++];
				}
				tmp.s = s;
				tmp.row = iRow;
				value = h(s) % MAX;
				int j = 0;
				for(j=0;j<hashtable[value].size();j++)
					if (tmp.s == hashtable[value][j].s) {
						tmp.token = hashtable[value][j].token;
						break;
					}
				if(tmp.token==0){
					tmp.token = TK_IDENTV;
				}
				hashtable[value].push_back(tmp);
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				tmpClera(&tmp);
				s.clear();
			}
			else if (IS_NUM(c[i])) {
				while (IS_NUM(c[i])) {
					s += c[i++];
				}
				tmp.s = s;
				tmp.row = iRow;
				value = h(s) % MAX;
				int j = 0;
				for (j; j < hashtable[value].size(); j++)
					if (s == hashtable[value][j].s) {
						tmp.token = hashtable[value][j].token;
						break;
					}
				if (j == hashtable[value].size()) {
					tmp.token = TK_CINT;
					hashtable[value].push_back(tmp);
				}
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				tmpClera(&tmp);
				s.clear();
			}
			else if (c[i] == 39) {
				for (int k = 0; k < 3; k++)
					s += c[i++];
				tmp.token = TK_CCHAR;
				tmp.s = s;
				tmp.row = iRow;
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				s.clear();
				tmpClera(&tmp);
			}
			else if (c[i] == 34) {
				s += c[i++];
				while (c[i] != 34) {
					s += c[i++];
				}
				s += c[i++];
				tmp.token = TK_CSTR;
				tmp.s = s;
				tmp.row = iRow;
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				s.clear();
				tmpClera(&tmp);
			}
			else {
				if (c[i] == '=' && c[i+1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_EQ;
				}else if (c[i] == '!' && c[i + 1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_NEQ;
				}else if (c[i] == '<' && c[i + 1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_LEQ;
				}else if (c[i] == '>' && c[i + 1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_GEQ;
				}else if (c[i] == '-' && c[i + 1] == '>') {
					s += c[i++];	s += c[i++];	tmp.token = TK_POINTSTO;
				}else { s += c[i++]; }
				tmp.row = iRow;
				tmp.s = s;
				value = h(s) % MAX;
				int j = 0;
				for (j; j < hashtable[value].size(); j++)
					if (s == hashtable[value][j].s) {
						tmp.token = hashtable[value][j].token;
						break;
					}
				Print(s, tmp.token);
				word[wordlen++] = tmp;
				s.clear();
				tmpClera(&tmp);
			}
		}
		iRow++;
		PrintWhite();
	}
	printf("\n");

	for (int i = 0; i < wordlen; i++) {
		if (word[i].token == TK_IDENTV && word[i + 1].token == TK_OPENPA)
			word[i].token = TK_IDENTF;
	}
	/*------------------------------- Óï·¨·ÖÎö------------------------------------*/
	string res;
	JmpJud = setjmp(jmpbuf);
	switch (JmpJud)
	{
	case SOMETHINGWRONG:res="±àÒëÊ§°Ü£¬Î´Öª´íÎó£¡";break;
	case LACK_SEMICOLON:res="±àÒëÊ§°Ü£¬È±ÉÙ';'£¡" ; break;
	case EXTER_DEC:res="±àÒëÊ§°Ü£¬Íâ²¿±äÁ¿»òº¯Êý´íÎó£¡" ; break;
	case LACK_TYPE:res="±àÒëÊ§°Ü£¬È±ÉÙÀàÐÍ·û£¡" ; break;
	case VRA_WRONG:res="±àÒëÊ§°Ü£¬±äÁ¿´íÎó£¡" ; break;
	case FUNC_WRONG:res = "±àÒëÊ§°Ü£¬º¯Êý´íÎó£¡"; break;
	case LACK_IDENT:res="±àÒëÊ§°Ü£¬È±ÉÙ±êÊ¶·û£¡" ; break;
	case LACK_OPENPA:res="±àÒëÊ§°Ü£¬È±ÉÙ'('£¡" ; break;
	case LACK_CLOSEPA:res="±àÒëÊ§°Ü£¬È±ÉÙ' '£¡" ; break;
	case LACK_OPENBR:res="±àÒëÊ§°Ü£¬È±ÉÙ'['£¡" ; break;
	case LACK_CLOSEBR:res="±àÒëÊ§°Ü£¬È±ÉÙ']'£¡" ; break;
	case LACK_BEGIN:res="±àÒëÊ§°Ü£¬È±ÉÙ'{'£¡" ; break;
	case LACK_END:res="±àÒëÊ§°Ü£¬È±ÉÙ'}'£¡" ; break;
	case NESTED_FUNC_DECLEAR:res="±àÒëÊ§°Ü£¬º¯ÊýÇ¶Ì×¶¨Òå£¡" ; break;
	case LACK_PRIMARY:res="±àÒëÊ§°Ü£¬³õµÈ±í´ïÊ½´íÎó£¡" ; break;
	case RETURN_ERROR:res="±àÒëÊ§°Ü£¬returnÀàÐÍ´íÎó£¡" ; break;
	case BREAK_LACK_SEMICOLON:res="±àÒëÊ§°Ü£¬breakÈ±ÉÙ';'£¡" ; break;
	case CONTINUE_LACK_SEMICOLON:res="±àÒëÊ§°Ü£¬continueÈ±ÉÙ';'£¡" ; break;
	case IF_LACK_OPENPA:res="±àÒëÊ§°Ü£¬ifÖÐÈ±ÉÙ'('£¡" ; break;
	case IF_LACK_CLOSEPA:res="±àÒëÊ§°Ü£¬ifÖÐÈ±ÉÙ')'!"; break;
	case FOR_LACK_OPENPA:res="±àÒëÊ§°Ü£¬forÈ±ÉÙ'('£¡"; break;
	case FOR_LACK_CLOSEPA:res="±àÒëÊ§°Ü£¬forÈ±ÉÙ')'£¡"; break;
	case FUNCVAR_WRONG:res = "±àÒëÊ§°Ü£¬º¯Êýµ÷ÓÃ±äÁ¿´íÎó£¡"; break;
	case LACK_ST:res = "±àÒëÊ§°Ü£¬È±ÉÙÓï¾ä£¡"; break;
	case CONTINUE_ERROR:res = "±àÒëÊ§°Ü£¬continueÊ¹ÓÃ´íÎó£¡"; break;
	case ELSE_ERROR:res = "±àÒëÊ§°Ü£¬elseÊ¹ÓÃ´íÎó£¡"; break;
	case FOR_ERROR:res = "±àÒëÊ§°Ü£¬forÊ¹ÓÃ´íÎó£¡"; break;
	case WHILE_LACK_OPENPA:res = "±àÒëÊ§°Ü£¬whileÈ±ÉÙ×óÔ²À¨ºÅ£¡"; break;
	case WHILE_LACK_CLOSEPA:res = "±àÒëÊ§°Ü£¬whileÈ±ÉÙÓÒÔ²À¨ºÅ£¡"; break;
	case WHILE_ERROR:res = "±àÒëÊ§°Ü£¬whileÊ¹ÓÃ´íÎó£¡"; break;
	case PROGRAM_BEGIN:Compile();
	default:
		res="\n==========³É¹¦ 1 ¸ö£¬Ê§°Ü 0 ¸ö£¬Ìø¹ý 0 ¸ö==========\n";
		break;
	}
	if (SUCC == JmpJud) {
		printf("============ÒÑÍê³ÉÏîÄ¿code.cµÄ±àÒë¹¤×÷=============");
		printf("\n%s", res.c_str());
		return 0;
	}
	else if (SOMETHINGWRONG == JmpJud) {
		printf("\n%s", res.c_str());
	}
	else {
		printf("\n==========%s==========\n", res.c_str());
		printf("\n=========== ´íÎóÐÐÊý£º%d ============\n", Row);
	}
}