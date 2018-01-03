#ifndef IMITATE_KB_AND_MOU
#define IMITATE_KB_AND_MOU
#include<windows.h>
#include<list>
using namespace std;
typedef enum{
	Keyboard,Lmousebutton,Rmousebutton
}Info_type;

typedef union{
	POINT mousecontent;
	TCHAR key;
}Info_content;

typedef struct{
	Info_type Infotype;
	Info_content Info_content;
}Info,*pInfo;

class Imitate_kb_and_mou
{
	
public:~Imitate_kb_and_mou();

bool Lmouse_record();
bool Rmouse_record();
bool key_record();
bool Info_indicate();
bool show_record();
bool clear_record();

protected:
	list<pInfo> Info_list;		//要用list，一是#include<list>,二是using namespace std;

};
#endif