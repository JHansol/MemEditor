#include <stdio.h>
#include <windows.h>
#include <urlmon.h>
#include "resource.h"
#include <Tlhelp32.h>
#include <malloc.h>
#pragma comment(lib,"urlmon.lib")

static const unsigned char SpeedValue1[] = {0x80,0x3b,0x45,0x00};
static const unsigned char SpeedValue2[] = {0x80,0x3b,0x46,0x00};
static const unsigned char SpeedValue3[] = {0x80,0x3b,0x47,0x00};
static const unsigned char SpeedValue4[] = {0x80,0x3b,0x48,0x00};
static const unsigned char SpeedValue5[] = {0x80,0x3b,0x49,0x00};
static const unsigned char HouseBlue[] = {0x64,0xC4,0x48,0x72,0xc3,0xc3,0xfd,0x9f,0x9a,0xc4}; // 6A C4 48 72 C3 C3 FD 9F 9a c4
static const unsigned char HouseRed[] = {0xEA,0xC4,0x40,0x92,0x24,0x42,0xF6,0xA3,0xB8,0xC3}; // EA C4 40 92 24 42 F6 A3 B8 C3
int WarpCount=0;
int warpONOFF=0;
HINSTANCE g_hInst;

int il = 123;
int Comy[8];

HWND Find1=0;
HWND Find2text=0;
HWND Find3=0;
HWND Wnd=0;

DWORD Dos = 0x0049E2A0;

LPCTSTR str="123";
LPARAM qqq = 1;

int j , ScanONOFF;

HWND g_hMsgWnd = NULL;
BOOL CreateMsgWnd(HINSTANCE hInst, HWND *phWnd);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK InfoDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK InfoDlgProc2(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK InfoDlgProc3(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK InfoDlgProc4(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);

DWORD Respone = 0x02080000;
DWORD Man;
DWORD Respones;
DWORD CodeMemoryBefore;
DWORD CodeMemoryBack;
DWORD CodeMemoryAfter;

char Res[100];
DWORD WarpAddd[30];

////������������������������������������ὺĵ��ɡ������������������������������������
#define ARRAY_EACH_SIZE 100     // �迭 ũ��
int MAMA;
int izz;
DWORD *p;

BYTE *byteArray(unsigned int n, unsigned int size){
    BYTE *arr = new BYTE [size];
    unsigned int i;
    for (i=0; i<size; i++){
        arr[i] = n % 256;
        n /= 256;
    }
    return arr;
}

PDWORD FindMem(HANDLE hProc, DWORD nFind, unsigned int nSize , DWORD Start , DWORD END){
    SYSTEM_INFO si; // �޸� �ּ� �ּҰ�, �ִ밪�� ���
    MEMORY_BASIC_INFORMATION mbi;   // ������ ���� ���
    DWORD nMem = 0, i, j, result_ct = 0;        // ���� �޸� �ּ� ������ �� �� ���꿡 �ʿ��� ����

	DWORD *FindData;
    BYTE *srcArray; // �޸𸮿��� ã�Ƴ� ��
    BYTE *destArray;    // �޸𸮿��� �о ��
    FindData = (DWORD *)malloc(ARRAY_EACH_SIZE*4);   // ã�Ƴ� ���� ����
    srcArray = byteArray(nFind, nSize); // ��ȯ
	MAMA = 0;

    GetSystemInfo(&si);
    //nMem = (DWORD)si.lpMinimumApplicationAddress; //�޸� �ּ��� �ּҰ��� ���Ѵ�.
	nMem = Start; // ��������

    do{
        if (VirtualQueryEx(hProc, (LPVOID)nMem, &mbi, sizeof(mbi)) == sizeof(mbi)){         // �������� ������ �о��
            if (mbi.RegionSize > 0 && mbi.Type == MEM_PRIVATE && mbi.State == MEM_COMMIT || mbi.Type == MEM_MAPPED || mbi.Type == MEM_IMAGE){      // �������� ��밡������ �˾Ƴ���
                destArray = new BYTE [mbi.RegionSize];      // �޸𸮸� ���� �غ� �Ѵ�
                if (ReadProcessMemory(hProc, mbi.BaseAddress, destArray, mbi.RegionSize, NULL) != 0){       // �޸𸮸� �д´�
                    for (i=0; i<(DWORD)mbi.RegionSize; i++){        // ���� �޸𸮿� ã�� �޸𸮸� ���Ѵ�
                        for (j=0; j<nSize; j++){
                            if (i+nSize+1 > mbi.RegionSize)
                                break;
                            if (destArray[i+j] != srcArray[j]){
                                break;
                            }else if (j == nSize-1){        // ���� ã�Ƴ�
                                if (result_ct % ARRAY_EACH_SIZE == 0)   // �迭�� ũ�⸦ �����Ѵ�
                                    FindData = (DWORD *)realloc(FindData, nSize*ARRAY_EACH_SIZE*(result_ct / ARRAY_EACH_SIZE + 1));
                                FindData[result_ct] = (DWORD)mbi.BaseAddress + i;       // �迭�� �ּҸ� �����Ѵ�
                                result_ct++;
								MAMA++;
                            }
                        }
                    }
                }
                delete destArray;       // �޸𸮸� �о����� ����
            }
            nMem = (DWORD)mbi.BaseAddress + (DWORD)mbi.RegionSize;  //���� ������ �ּ� ���
        }
    }while(nMem < END); //(nMem < (DWORD)si.lpMaximumApplicationAddress);       // �ִ� �ּҸ� �Ѿ���� �������� ��������
    delete srcArray;    // �޸� ����
    return FindData;    // ����� ����
}
////����������/////////��ĵ���//����������������������������������������������������������������
HWND Combo1;
void ComboYo(HWND hDlg){
		Combo1 = GetDlgItem(hDlg, IDC_COMBO1);
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"1Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"2Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"4Byte");
		SendMessage(Combo1,CB_SETCURSEL,0,0);
		Combo1 = GetDlgItem(hDlg, IDC_COMBO2);
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"1Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"2Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"4Byte");
		SendMessage(Combo1,CB_SETCURSEL,0,0);
		Combo1 = GetDlgItem(hDlg, IDC_COMBO3);
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"1Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"2Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"4Byte");
		SendMessage(Combo1,CB_SETCURSEL,0,0);
		Combo1 = GetDlgItem(hDlg, IDC_COMBO4);
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"1Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"2Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"4Byte");
		SendMessage(Combo1,CB_SETCURSEL,0,0);
		Combo1 = GetDlgItem(hDlg, IDC_COMBO5);
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"1Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"2Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"4Byte");
		SendMessage(Combo1,CB_SETCURSEL,0,0);
		Combo1 = GetDlgItem(hDlg, IDC_COMBO6);
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"1Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"2Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"4Byte");
		SendMessage(Combo1,CB_SETCURSEL,0,0);
		Combo1 = GetDlgItem(hDlg, IDC_COMBO7);
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"1Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"2Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"4Byte");
		SendMessage(Combo1,CB_SETCURSEL,0,0);
		Combo1 = GetDlgItem(hDlg, IDC_COMBO8);
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"1Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"2Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"4Byte");
		SendMessage(Combo1,CB_SETCURSEL,0,0);
		Combo1 = GetDlgItem(hDlg, IDC_COMBO9);
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"1Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"2Byte");
		SendMessage(Combo1,CB_ADDSTRING,0,(LPARAM)"4Byte");
		SendMessage(Combo1,CB_SETCURSEL,0,0);
}

void Run(HINSTANCE hInst){
	/*
	int state;
	char* pState;
	char str[4];
	char Ver[4] = "1.3";

URLDownloadToFile(0, "http://linears.mireene.com/bbs/CleverTool.txt", "C:/temps", 0, 0);
  FILE* f = fopen("C:/temps", "rt");
  if(f==NULL)
  {
		ExitProcess(0);
  }
  while(1){
            pState = fgets(str, sizeof(str), f);
            if(pState == NULL)
            break;
            fputs(str, stdout);
    }
state = fclose(f);
if(strcmp(str,"6.0")==0){
}
else
{
	ExitProcess(0);
	while(1)
	{
		Sleep(100000);
	}
}

DeleteFile("C:/temps");
*/
DialogBox(g_hInst,MAKEINTRESOURCE(IDD_DIALOG1),HWND_DESKTOP,InfoDlgProc);
g_hInst = hInst;
	}



BOOL APIENTRY DllMain(HINSTANCE module, DWORD dwReason, LPVOID lpvReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH){
		/*
		Wnd = FindWindow("ThunderRT6Main","Clever");   // ������ �ִ��� Ȯ�ο�
		if(!Wnd){   // If not of window...
		}else{      // Else... �������
				Wnd = FindWindow("ThunderRT6FormDC","Clever��");   // ����Ŀ ������ ������ !
				if(!Wnd){   // If not of window...
				}else{      // Else... �������
				*/
		g_hInst=module;
		CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Run, NULL, 0, 0 ); //������ ����
		return TRUE; 
	}
return TRUE;
}
	
void PointerScan(){
			for(j=1;j<1300;j++)
				{
					Man = Man + 0x00010000;
					ReadProcessMemory(GetCurrentProcess(),(void*)(Respone+Man),(void*)&Respones, 2, NULL);
					if(Respones==0x5A4D)
					{
						CodeMemoryBefore = Respone + Man + 0x00001000;
						ReadProcessMemory(GetCurrentProcess(),(void*)CodeMemoryBefore,(void*)&CodeMemoryBack,4,NULL);
						if(CodeMemoryBack==0x90C3C18B)
						{
							CodeMemoryAfter = CodeMemoryBefore - 0x00001000;
						}
					}
				}	
	}

char Names[1024];
char TypeNames1[1024];
char *NamesB1[1024];
char Names2[1024];
DWORD Adr;
DWORD AdrR;// = 0x00401000;

char PN[1024];
char PNz[1024];
char PNq[1024];
DWORD PND;
DWORD PNDz;
DWORD PNDr;

char PN2[1024];
char PNz2[1024];
char PNq2[1024];
DWORD PND2;
DWORD PNDz2;
DWORD PNDr2;

char PN3[1024];
char PNz3[1024];
char PNq3[1024];
DWORD PND3;
DWORD PNDz3;
DWORD PNDr3;

char PN4[1024];
char PNz4[1024];
char PNq4[1024];
DWORD PND4;
DWORD PNDz4;
DWORD PNDr4;

char AdrW[1024];
char Value1[1024];
char Type1[1024];
DWORD AdrWz;
DWORD Value1z;// = 0x00401000;
int Typez1;

static const unsigned char Damage[] = {0x80,0x3b,0x48,0x01};

			char str1[]="00401000+004020000";
			char *ptr=strchr(str1,'+');

char Ang[1024];
BOOL CALLBACK InfoDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage) {
	case WM_INITDIALOG:
		SetDlgItemText(hDlg,IDC_P,"00400000"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Pz,"0"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_P2,"00400000"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_EDIT1,"00401000"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Adr1,"00400000"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Adr2,"00400000"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Adr3,"00400000"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_EDIT2,"4"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Type1,"4"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Type2,"4"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Type3,"4"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Value1,"0"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Value2,"0"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Value3,"0"); //��Ʈ�ѿ��ٰ� �����ִ°�
		//SetDlgItemInt(
		//GetDlgItemInt(hDlg,IDC_X,NULL,FALSE);
		//GetDlgItemText(hDlg,IDC_EDIT1,Names,sizeof(Names));
		break;
	case WM_COMMAND:
		switch (wParam) {
		case IDC_test1:
			break;
		case IDC_R1:
			GetDlgItemText(hDlg,IDC_EDIT1,Names,sizeof(Names)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_EDIT2,TypeNames1,sizeof(TypeNames1)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			Adr = strtoul(Names, NULL, 16);
			ReadProcessMemory(GetCurrentProcess(),(void*)Adr,(void*)&AdrR, 4, NULL); // WeaponHackP2+4�� �а� WeaponHackP3���ٰ� ���� 
			_ultoa(AdrR, Names2, 10);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT3,Names2);
			break;
		case IDC_R2:
			GetDlgItemText(hDlg,IDC_Adr1,Names,sizeof(Names)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_Value1,TypeNames1,sizeof(TypeNames1)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			Adr = strtoul(Names, NULL, 16);
			ReadProcessMemory(GetCurrentProcess(),(void*)Adr,(void*)&AdrR, 4, NULL); // WeaponHackP2+4�� �а� WeaponHackP3���ٰ� ���� 
			_ultoa(AdrR, Names2, 10);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_Value1,Names2);
			break;
		case IDC_R3:
			GetDlgItemText(hDlg,IDC_Adr2,Names,sizeof(Names)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_Value2,TypeNames1,sizeof(TypeNames1)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			Adr = strtoul(Names, NULL, 16);
			ReadProcessMemory(GetCurrentProcess(),(void*)Adr,(void*)&AdrR, 4, NULL); // WeaponHackP2+4�� �а� WeaponHackP3���ٰ� ���� 
			_ultoa(AdrR, Names2, 10);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_Value2,Names2);
			break;
		case IDC_R4:
			GetDlgItemText(hDlg,IDC_Adr3,Names,sizeof(Names)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_Value3,TypeNames1,sizeof(TypeNames1)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			Adr = strtoul(Names, NULL, 16);
			ReadProcessMemory(GetCurrentProcess(),(void*)Adr,(void*)&AdrR, 4, NULL); // WeaponHackP2+4�� �а� WeaponHackP3���ٰ� ���� 
			_ultoa(AdrR, Names2, 10);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_Value3,Names2);
			break;
		case IDC_LTD:
			PointerScan();
			_ultoa(CodeMemoryAfter, Ang, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT3,Ang);
			break;
		case IDC_W1:
			GetDlgItemText(hDlg,IDC_EDIT1,AdrW,sizeof(AdrW)); // ��巹���ּҸ� AdrW���� ����
			GetDlgItemText(hDlg,IDC_EDIT2,Type1,sizeof(Type1)); // ��巹���ּҸ� AdrW���� ����
			GetDlgItemText(hDlg,IDC_EDIT3,Value1,sizeof(Value1)); // ��巹���ּҸ� Value1���� ����
			AdrWz = strtoul(AdrW, NULL, 16); // AdrW���� adrWz���� ����
			Value1z = strtoul(Value1, NULL, 10); // Value1���� Value1z���� ����
			Typez1 = atoi(Type1);
			if(Typez1==1) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 1, NULL);
			else if(Typez1==2) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 2, NULL);
			else if(Typez1==4) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 4, NULL);
			else if(Typez1==8) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 8, NULL);
			break;
		case IDC_W2:
			GetDlgItemText(hDlg,IDC_Adr1,AdrW,sizeof(AdrW)); // ��巹���ּҸ� AdrW���� ����
			GetDlgItemText(hDlg,IDC_Type1,Type1,sizeof(Type1)); // ��巹���ּҸ� AdrW���� ����
			GetDlgItemText(hDlg,IDC_Value1,Value1,sizeof(Value1)); // ��巹���ּҸ� Value1���� ����
			AdrWz = strtoul(AdrW, NULL, 16); // AdrW���� adrWz���� ����
			Value1z = strtoul(Value1, NULL, 10); // Value1���� Value1z���� ����
			Typez1 = atoi(Type1);
			if(Typez1==1) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 1, NULL);
			else if(Typez1==2) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 2, NULL);
			else if(Typez1==4) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 4, NULL);
			else if(Typez1==8) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 8, NULL);
			break;
		case IDC_W3:
			GetDlgItemText(hDlg,IDC_Adr2,AdrW,sizeof(AdrW)); // ��巹���ּҸ� AdrW���� ����
			GetDlgItemText(hDlg,IDC_Type2,Type1,sizeof(Type1)); // ��巹���ּҸ� AdrW���� ����
			GetDlgItemText(hDlg,IDC_Value2,Value1,sizeof(Value1)); // ��巹���ּҸ� Value1���� ����
			AdrWz = strtoul(AdrW, NULL, 16); // AdrW���� adrWz���� ����
			Value1z = strtoul(Value1, NULL, 10); // Value1���� Value1z���� ����
			Typez1 = atoi(Type1);
			if(Typez1==1) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 1, NULL);
			else if(Typez1==2) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 2, NULL);
			else if(Typez1==4) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 4, NULL);
			else if(Typez1==8) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 8, NULL);
			break;
		case IDC_W4:
			GetDlgItemText(hDlg,IDC_Adr3,AdrW,sizeof(AdrW)); // ��巹���ּҸ� AdrW���� ����
			GetDlgItemText(hDlg,IDC_Type3,Type1,sizeof(Type1)); // ��巹���ּҸ� AdrW���� ����
			GetDlgItemText(hDlg,IDC_Value3,Value1,sizeof(Value1)); // ��巹���ּҸ� Value1���� ����
			AdrWz = strtoul(AdrW, NULL, 16); // AdrW���� adrWz���� ����
			Value1z = strtoul(Value1, NULL, 10); // Value1���� Value1z���� ����
			Typez1 = atoi(Type1);
			if(Typez1==1) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 1, NULL);
			else if(Typez1==2) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 2, NULL);
			else if(Typez1==4) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 4, NULL);
			else if(Typez1==8) WriteProcessMemory(GetCurrentProcess(),(void*)AdrWz, (void*)&Value1z, 8, NULL);
			break;
		case IDC_PR1:
			GetDlgItemText(hDlg,IDC_P,PN,sizeof(PN)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_Pz,PNz,sizeof(PNz)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			PND = strtoul(PN, NULL, 16);
			PNDz = strtoul(PNz, NULL, 16);
			ReadProcessMemory(GetCurrentProcess(),(void*)PND,(void*)&PNDr, 4, NULL); // WeaponHackP2+4�� �а� WeaponHackP3���ٰ� ���� 
			PNDr = PNDr + PNDz;
			_ultoa(PNDr, PNq, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_P2,PNq);

			GetDlgItemText(hDlg,IDC_P2,PN2,sizeof(PN2)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_Pz2,PNz2,sizeof(PNz2)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			PND2 = strtoul(PN2, NULL, 16);
			PNDz2 = strtoul(PNz2, NULL, 16);
			ReadProcessMemory(GetCurrentProcess(),(void*)PND2,(void*)&PNDr2, 4, NULL); // WeaponHackP2+4�� �а� WeaponHackP3���ٰ� ���� 
			PNDr2 = PNDr2 + PNDz2;
			_ultoa(PNDr2, PNq2, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_P3,PNq2);
			
			GetDlgItemText(hDlg,IDC_P3,PN3,sizeof(PN3)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_Pz3,PNz3,sizeof(PNz3)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			PND3 = strtoul(PN3, NULL, 16);
			PNDz3 = strtoul(PNz3, NULL, 16);
			ReadProcessMemory(GetCurrentProcess(),(void*)PND3,(void*)&PNDr3, 4, NULL); // WeaponHackP3+4�� �а� WeaponHackP3���ٰ� ���� 
			PNDr3 = PNDr3 + PNDz3;
			_ultoa(PNDr3, PNq3, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_P4,PNq3);

			GetDlgItemText(hDlg,IDC_P4,PN4,sizeof(PN4)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_Pz4,PNz4,sizeof(PNz4)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			PND4 = strtoul(PN4, NULL, 16);
			PNDz4 = strtoul(PNz4, NULL, 16);
			ReadProcessMemory(GetCurrentProcess(),(void*)PND4,(void*)&PNDr4, 4, NULL); // WeaponHackP4+4�� �а� WeaponHackP4���ٰ� ���� 
			PNDr4 = PNDr4 + PNDz4;
			_ultoa(PNDr4, PNq4, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_P5,PNq4);
			break;
		case IDC_Scan:
			DialogBox(g_hInst,MAKEINTRESOURCE(IDD_ScanMenu),HWND_DESKTOP,InfoDlgProc2);
			break;
		case IDC_MemV:
			DialogBox(g_hInst,MAKEINTRESOURCE(IDD_Mem ),HWND_DESKTOP,InfoDlgProc3);
			break;
		case IDC_FT:
			DialogBox(g_hInst,MAKEINTRESOURCE(IDD_FrozenTool ),HWND_DESKTOP,InfoDlgProc4);
			break;
			}
		}
return false;
}

int ScanTypes = 0;
int ScanArray = 0;
HWND hLISTEdit1 , NextB1;
/*
------CreateWindow�� ����Ʈ�ڽ��� ���鶧 ����° ���ڷ� �ִ� ��Ÿ�� ����Ʈ------------
LBS_MULTIPLESEL : �������� ����� ������ �� �ִ�.
LBS_NOTIFY : ����ڰ� ����� �ϳ��� ���������� �θ�������� ���������� (�����ʼ����Ѵ�)
LBS_SORT : �߰��� �׸���� �ڵ����� �����Ѵ�.
LBS_OWNERDRAW : ���ڿ��� �ƴ� ��Ʈ���̳� �׸��� ���� �� �ִ�.
LBS_STANDARD : LBS_NOTIFY | LBS_SORT | WS_BORDER 
------------------------------------------------------------------------------------
------------------�θ� �����찡 �ش� ����Ʈ�ڽ����� ������ �޽���------------------
LB_ADDSTRING : ����Ʈ �ڽ��� �׸��� �߰��Ѵ� lParam���� ���ڿ��� ���� �ѱ��.
LB_DELETESTRING : �׸��� �����Ѵ�. wParam���� �׸��� ��ȣ�� �ѱ��.
LB_GETCURSEL :  ���� ���õ� �׸��� ��ȣ�� �����Ѵ�. (���ϰ�)
LB_GETTEXT : ������ �׸��� ���ڿ��� �д´�. wParam ���� �׸��ȣ  lParam�� ����Ǵ¹���
LB_GETCOUNT : �׸��� ������ �����Ѵ�.
LBa_SETCURSEL : wParam�� ������ �׸��� �����Ѵ�.
------------------------------------------------------------------------
------ �ڽ������찡(����Ʈ�ڽ���) �θ�������� ������ �޽���-----------
LBN_DBLCLK : ����Ʈ �ڽ��� ����Ŭ���Ͽ���.
LBN_ERRSPACE : �޸𸮰� �����ϴ�
LBN_KILLFOCUS : Ű���� ��Ŀ���� �Ҿ���.
LBN_SELCANCEL : ����ڰ� ������ ����Ͽ���.
LBN_SELCHANGE : ����ڿ� ���� ���±� ����Ǿ���.
LBN_SETFOCUS : Ű���� ��Ŀ���� �����.
------------------------------------------------------------------
*/
#define BTN_DISABLED    3

			int Kz , Finds=0;
			char ScanValuez[1024];
			DWORD ScanValuesz = 0x00000000;
			char ScanValuez2[1024];
			DWORD ScanValuesz2 = 0x00000000;
			char ReadBufferS[1024];
			DWORD ReadBuffer = 0x00000000;
			DWORD ReadBuffer2 = 0x00000000;
			char ReadBufferS2[1024];
			int is=0,ks=0,SSS ,KKK=0;
char ASDSAD[1024];

char TESTING[1024];

	DWORD addoffset;
	char ASDASD[1024];
	char ASDASD2[1024];
	char ASDASD3[1024];
	DWORD AutoWriterValue[2];

void WarpOn(){
	while(1){
		for(SSS=1;SSS<WarpCount;SSS++) //is�� �׸��ǰ���
		{
		WriteProcessMemory(GetCurrentProcess(),(void*)(WarpAddd[SSS]+addoffset),&AutoWriterValue[0], strlen(ASDASD)/2, NULL);
		WriteProcessMemory(GetCurrentProcess(),(void*)(WarpAddd[SSS]+addoffset+0x4),&AutoWriterValue[1], strlen(ASDASD2)/2, NULL);
		WriteProcessMemory(GetCurrentProcess(),(void*)(WarpAddd[SSS]+addoffset+0x8),&AutoWriterValue[2], strlen(ASDASD3)/2, NULL);
		}

		if(warpONOFF==1)
		{
			warpONOFF=0;
			break;
			break;
		}
		Sleep(10);
	}
}
BOOL CALLBACK InfoDlgProc2(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage) {
	case WM_INITDIALOG:
		hLISTEdit1 = GetDlgItem(hDlg, IDC_LIST1);
		SetDlgItemText(hDlg,IDC_EDIT1,"0"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_EDIT2,"00400000"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_EDIT3,"00400005"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_EDIT4,"0"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Offset,"10"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Value1,"93447761"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Value2,"cc43b0de"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Value3,"bec3"); //��Ʈ�ѿ��ٰ� �����ִ°�
		CheckDlgButton(hDlg, IDC_RADIO1,1);
		break;
	case WM_COMMAND:
		switch (wParam) {
		case IDC_Cancel:
			EndDialog(hDlg,0);
			break;
		case IDC_Minus:
			int MyAme;
			MyAme = SendMessage(hLISTEdit1,LB_GETCURSEL ,0,0);
			SendMessage(hLISTEdit1,LB_DELETESTRING,MyAme,0);
			break;
		case IDC_Next:
			if(IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED) ScanTypes = 1; // IsDlgButtonChecked üũ,��üũ Ȯ��
			else if(IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED) ScanTypes = 2; // IsDlgButtonChecked üũ,��üũ Ȯ��
			else if(IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED) ScanTypes = 3; // IsDlgButtonChecked üũ,��üũ Ȯ��
			else if(IsDlgButtonChecked(hDlg, IDC_RADIO4) == BST_CHECKED) ScanTypes = 4; // IsDlgButtonChecked üũ,��üũ Ȯ��
			if(IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED) ScanArray = 1; // IsDlgButtonChecked üũ,��üũ Ȯ��
			else ScanArray =0; // IsDlgButtonChecked üũ,��üũ Ȯ��
			KKK = 0;
			is = SendMessage(hLISTEdit1,LB_GETCOUNT,0,0);
			GetDlgItemText(hDlg,IDC_EDIT1,ScanValuez,sizeof(ScanValuez)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_EDIT6,ScanValuez2,sizeof(ScanValuez2)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			if(ScanArray==1){
				char ch , ch2;
				int kma;
				for (int Maui =0; Maui < 4; Maui = Maui + 2)
				{
					ch = ScanValuez[7-Maui-1];
					ch2 = ScanValuez[7-Maui];
					ScanValuez[7-Maui-1] = ScanValuez[Maui];
					ScanValuez[7-Maui] = ScanValuez[Maui+1];
					ScanValuez[Maui] = ch;
					ScanValuez[Maui+1] = ch2;
				}
				for (int Maui =0; Maui < 4; Maui = Maui + 2)
				{
					ch = ScanValuez2[7-Maui-1];
					ch2 = ScanValuez2[7-Maui];
					ScanValuez2[7-Maui-1] = ScanValuez2[Maui];
					ScanValuez2[7-Maui] = ScanValuez2[Maui+1];
					ScanValuez2[Maui] = ch;
					ScanValuez2[Maui+1] = ch2;
				}
				ScanValuesz = strtoul(ScanValuez, NULL, 16);
				ScanValuesz2 = strtoul(ScanValuez2, NULL, 16);
			}
			else ScanValuesz = strtoul(ScanValuez, NULL, 10);
			//_ultoa(ScanValuesz, TESTING, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			//MessageBox(0,TESTING,TESTING,MB_OK);
			if(ScanTypes==1){
			for(SSS=0;SSS<is;SSS++){ // ����Ʈ�� ����� �׸���� ������ �װ����� �ݺ����� ����
				SendMessage(hLISTEdit1,LB_GETTEXT,KKK,(LPARAM)ReadBufferS); //SSS - �׸�� , ReadBuffer���ٰ� ���ڿ�����
				//MessageBox(0,ReadBufferS,ReadBufferS,MB_OK);
				ReadBuffer = strtoul(ReadBufferS, NULL, 16); // ReadBuffer�� DWORD������ ��ȯ���� �ּҰ�����
				ReadProcessMemory(GetCurrentProcess(),(void*)ReadBuffer,(void*)&ReadBuffer2, 1, NULL);
					if(ReadBuffer2==ScanValuesz){ // ������ �F�ΰ� Ʋ���� ����, 
					}
					else{
						SendMessage(hLISTEdit1,LB_DELETESTRING,KKK,0);
						--KKK;
					}
				KKK = KKK + 1;
				}
			}
			else if(ScanTypes==2){
			for(SSS=0;SSS<is;SSS++){ // ����Ʈ�� ����� �׸���� ������ �װ����� �ݺ����� ����
				SendMessage(hLISTEdit1,LB_GETTEXT,KKK,(LPARAM)ReadBufferS); //SSS - �׸�� , ReadBuffer���ٰ� ���ڿ�����
				//MessageBox(0,ReadBufferS,ReadBufferS,MB_OK);
				ReadBuffer = strtoul(ReadBufferS, NULL, 16); // ReadBuffer�� DWORD������ ��ȯ���� �ּҰ�����
				ReadProcessMemory(GetCurrentProcess(),(void*)ReadBuffer,(void*)&ReadBuffer2, 2, NULL);
					if(ReadBuffer2==ScanValuesz){ // ������ �F�ΰ� Ʋ���� ����, 
					}
					else{
						SendMessage(hLISTEdit1,LB_DELETESTRING,KKK,0);
						--KKK;
					}
				KKK = KKK + 1;
				}
			}
			else if(ScanTypes==3){
			for(SSS=0;SSS<is;SSS++){ // ����Ʈ�� ����� �׸���� ������ �װ����� �ݺ����� ����
				SendMessage(hLISTEdit1,LB_GETTEXT,KKK,(LPARAM)ReadBufferS); //SSS - �׸�� , ReadBuffer���ٰ� ���ڿ�����
				//MessageBox(0,ReadBufferS,ReadBufferS,MB_OK);
				ReadBuffer = strtoul(ReadBufferS, NULL, 16); // ReadBuffer�� DWORD������ ��ȯ���� �ּҰ�����
				ReadProcessMemory(GetCurrentProcess(),(void*)ReadBuffer,(void*)&ReadBuffer2, 4, NULL);
					if(ReadBuffer2==ScanValuesz){ // ������ �F�ΰ� Ʋ���� ����, 
					}
					else{
						SendMessage(hLISTEdit1,LB_DELETESTRING,KKK,0);
						--KKK;
					}
				KKK = KKK + 1;
			}
			}
			else if(ScanTypes==4){
				DWORD ReadBuffer3;
			for(SSS=0;SSS<is;SSS++){ // ����Ʈ�� ����� �׸���� ������ �װ����� �ݺ����� ����
				SendMessage(hLISTEdit1,LB_GETTEXT,KKK,(LPARAM)ReadBufferS); //SSS - �׸�� , ReadBuffer���ٰ� ���ڿ�����
				//MessageBox(0,ReadBufferS,ReadBufferS,MB_OK);
				ReadBuffer = strtoul(ReadBufferS, NULL, 16); // ReadBuffer�� DWORD������ ��ȯ���� �ּҰ�����
				ReadProcessMemory(GetCurrentProcess(),(void*)ReadBuffer,(void*)&ReadBuffer2, 4, NULL);
					if(ReadBuffer2==ScanValuesz){ // ������ �F�ΰ� Ʋ���� ����, 
						ReadProcessMemory(GetCurrentProcess(),(void*)(ReadBuffer+0x00000004),(void*)&ReadBuffer3, 4, NULL);
						if(ReadBuffer3==ScanValuesz2){
						}
						else{
							SendMessage(hLISTEdit1,LB_DELETESTRING,KKK,0);
							--KKK;
						}
					}
					else{
							SendMessage(hLISTEdit1,LB_DELETESTRING,KKK,0);
							--KKK;
						}
				KKK = KKK + 1;
			}
			}
			ks = SendMessage(hLISTEdit1,LB_GETCOUNT,0,0);
			itoa(ks,ASDSAD,10);
			SetDlgItemText(hDlg,IDC_EDIT4,ASDSAD); //��Ʈ�ѿ��ٰ� �����ִ°�
			MessageBox(0,"�ؽ�Ʈ ��ĵ �Ϸ�!!","Clever Memory Editor",MB_OK);
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case IDC_Warp:
				is = SendMessage(hLISTEdit1,LB_GETCOUNT,0,0);
				char addoffsetC[1024];
				char ch , ch2;
				GetDlgItemText(hDlg,IDC_Offset,addoffsetC,sizeof(addoffsetC));// addoffsetC�� ���� ������ ����
				addoffset = strtoul(addoffsetC, NULL, 16); // addoffset ���� ������ ����
				GetDlgItemText(hDlg,IDC_Value1,ASDASD,sizeof(ASDASD));// ASDASD�� �������� ����
				GetDlgItemText(hDlg,IDC_Value2,ASDASD2,sizeof(ASDASD2));// ASDASD�� �������� ����
				GetDlgItemText(hDlg,IDC_Value3,ASDASD3,sizeof(ASDASD3));// ASDASD�� �������� ����
				for (int Maui =0; Maui < (strlen(ASDASD)/2); Maui = Maui + 2) // ASDASD�� ��̷� ������Ŵ �ݴ��
				{
					ch = ASDASD[(strlen(ASDASD)-1)-Maui-1];
					ch2 = ASDASD[(strlen(ASDASD)-1)-Maui];
					ASDASD[(strlen(ASDASD)-1)-Maui-1] = ASDASD[Maui];
					ASDASD[(strlen(ASDASD)-1)-Maui] = ASDASD[Maui+1];
					ASDASD[Maui] = ch;
					ASDASD[Maui+1] = ch2;
				}
				for (int Maui =0; Maui < (strlen(ASDASD2)/2); Maui = Maui + 2) // ASDASD�� ��̷� ������Ŵ �ݴ��
				{
					ch = ASDASD2[(strlen(ASDASD)-1)-Maui-1];
					ch2 = ASDASD2[(strlen(ASDASD)-1)-Maui];
					ASDASD2[(strlen(ASDASD)-1)-Maui-1] = ASDASD2[Maui];
					ASDASD2[(strlen(ASDASD)-1)-Maui] = ASDASD2[Maui+1];
					ASDASD2[Maui] = ch;
					ASDASD2[Maui+1] = ch2;
				}
				for (int Maui =0; Maui < (strlen(ASDASD3)/2); Maui = Maui + 2) // ASDASD�� ��̷� ������Ŵ �ݴ��
				{
					ch = ASDASD3[(strlen(ASDASD)-1)-Maui-1];
					ch2 = ASDASD3[(strlen(ASDASD)-1)-Maui];
					ASDASD3[(strlen(ASDASD)-1)-Maui-1] = ASDASD3[Maui];
					ASDASD3[(strlen(ASDASD)-1)-Maui] = ASDASD3[Maui+1];
					ASDASD3[Maui] = ch;
					ASDASD3[Maui+1] = ch2;
				}
				WarpCount = 0; // �ʱ�ȭ
				KKK = 0;// �ʱ�ȭ
				AutoWriterValue[0] = strtoul(ASDASD, NULL, 16);
				AutoWriterValue[1] = strtoul(ASDASD2, NULL, 16);
				AutoWriterValue[2] = strtoul(ASDASD3, NULL, 16);
				for(SSS=0;SSS<is;SSS++) //is�� �׸��ǰ��� ��������
				{
				SendMessage(hLISTEdit1,LB_GETTEXT,KKK,(LPARAM)ReadBufferS); //SSS - �׸�� , ReadBuffer���ٰ� ���ڿ�����
				//MessageBox(0,ReadBufferS,ReadBufferS,MB_OK);
				ReadBuffer = strtoul(ReadBufferS, NULL, 16); // ReadBuffer�� DWORD������ ��ȯ���� �ּҰ�����
				//WriteProcessMemory(GetCurrentProcess(),(void*)ReadBuffer, (void*)&HouseBlue, 10, NULL);
				WarpAddd[KKK] = ReadBuffer;
				KKK = KKK + 1; // KKK�� �׸��ȣ +1������
				WarpCount = WarpCount + 1;
				}
				CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)WarpOn, NULL, 0, 0 ); //������ ����
				break;
			case IDC_WarpOff:
				warpONOFF = 1;
				break;
		case IDC_Scan:
			int i=0,k=0;
			i = SendMessage(hLISTEdit1,LB_GETCOUNT,0,0);
			while(i>k){
			SendMessage(hLISTEdit1,LB_DELETESTRING,0,0);
			k++;
			}
			if(IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED) ScanTypes = 1; // IsDlgButtonChecked üũ,��üũ Ȯ��
			else if(IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED) ScanTypes = 2; // IsDlgButtonChecked üũ,��üũ Ȯ��
			else if(IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED) ScanTypes = 3; // IsDlgButtonChecked üũ,��üũ Ȯ��
			else if(IsDlgButtonChecked(hDlg, IDC_RADIO4) == BST_CHECKED) ScanTypes = 4; // IsDlgButtonChecked üũ,��üũ Ȯ��
			if(IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED) ScanArray = 1; // IsDlgButtonChecked üũ,��üũ Ȯ��
			else ScanArray =0; // IsDlgButtonChecked üũ,��üũ Ȯ��
			char ScanRange1[1024];
			DWORD ScanRanger1;
			char ScanRange2[1024];
			DWORD ScanRanger2;
			int Kz , minus=0 , Finds=0;
			char ScanValue[1024];
			char ScanValue2[1024];
			DWORD ScanValues = 0x00000000;
			DWORD ScanValues2 = 0x00000000;
			DWORD ScanStart = 0x00000000;
			DWORD ScanRespon = 0x00000000;
			DWORD ScanRespon2 = 0x00000000;
			char List1[1024] , Test2[1024] , FindS[1024];
			GetDlgItemText(hDlg,IDC_EDIT2,ScanRange1,sizeof(ScanRange1)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_EDIT3,ScanRange2,sizeof(ScanRange2)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_EDIT1,ScanValue,sizeof(ScanValue)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_EDIT6,ScanValue2,sizeof(ScanValue2)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			ScanRanger1 = strtoul(ScanRange1, NULL, 16);
			ScanRanger2 = strtoul(ScanRange2, NULL, 16);
			if(ScanArray==1){
				char ch , ch2;
				int kma;
				for (int Maui =0; Maui < 4; Maui = Maui + 2)
				{
					ch = ScanValue[7-Maui-1];
					ch2 = ScanValue[7-Maui];
					ScanValue[7-Maui-1] = ScanValue[Maui];
					ScanValue[7-Maui] = ScanValue[Maui+1];
					ScanValue[Maui] = ch;
					ScanValue[Maui+1] = ch2;
				}
				ScanValues = strtoul(ScanValue, NULL, 16);
				ScanValues2 = strtoul(ScanValue2, NULL, 16);
			}
			else{
				ScanValues = strtoul(ScanValue, NULL, 10);
			}
			minus = ScanRanger2 - ScanRanger1; // 0090.0000 - 0040.0000 = 50.0000
			if(ScanTypes==1){
			for(Kz=0;Kz<minus;Kz++)////////////////////////////////////////
				{
					ReadProcessMemory(GetCurrentProcess(),(void*)(ScanRanger1+ScanStart),(void*)&ScanRespon, 1, NULL);
					ScanStart = ScanStart + 0x00000001;
					//_ultoa(ScanRespon, Test2, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
					//MessageBox(0,Test2,"ã��",MB_OK);
					//_ultoa(ScanValues, Test1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
					//MessageBox(0,Test1,"ã��",MB_OK);
					if(ScanRespon==ScanValues)
					{
						Finds++;
						ultoa((ScanRanger1+ScanStart-0x00000001), List1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
						SendMessage(hLISTEdit1,LB_ADDSTRING,0,(LPARAM)List1);
					}
				}
			}
			else if(ScanTypes==2){
							for(Kz=0;Kz<minus;Kz++)////////////////////////////////////////
				{
					ReadProcessMemory(GetCurrentProcess(),(void*)(ScanRanger1+ScanStart),(void*)&ScanRespon, 2, NULL);
					ScanStart = ScanStart + 0x00000001;
					if(ScanRespon==ScanValues)
					{
						Finds++;
						ultoa((ScanRanger1+ScanStart-0x00000001), List1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
						SendMessage(hLISTEdit1,LB_ADDSTRING,0,(LPARAM)List1);
					}
				}
			}
			else if(ScanTypes==3){
				p = FindMem(GetCurrentProcess(), ScanValues, 4 ,ScanRanger1,ScanRanger2);
				for (izz=0; izz<MAMA; izz++){
					ultoa(p[izz], List1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
					SendMessage(hLISTEdit1,LB_ADDSTRING,0,(LPARAM)List1);
				}
				Finds = MAMA;
			}
				/*
							for(Kz=0;Kz<minus;Kz++)////////////////////////////////////////
				{
					ReadProcessMemory(GetCurrentProcess(),(void*)(ScanRanger1+ScanStart),(void*)&ScanRespon, 4, NULL);
					ScanStart = ScanStart + 0x00000001;
					if(ScanRespon==ScanValues)
					{
						Finds++;
						ultoa((ScanRanger1+ScanStart-0x00000001), List1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
						SendMessage(hLISTEdit1,LB_ADDSTRING,0,(LPARAM)List1);
					}
				}
			}
			*/
			else if(ScanTypes==4){
			p = FindMem(GetCurrentProcess(), ScanValues, 8 ,ScanRanger1,ScanRanger2);
				for (izz=0; izz<MAMA; izz++){
					ultoa(p[izz], List1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
					SendMessage(hLISTEdit1,LB_ADDSTRING,0,(LPARAM)List1);
				}
				Finds = MAMA;
				/*
							for(Kz=0;Kz<minus;Kz++)////////////////////////////////////////
				{
					ReadProcessMemory(GetCurrentProcess(),(void*)(ScanRanger1+ScanStart),(void*)&ScanRespon, 4, NULL);
					ScanStart = ScanStart + 0x00000001;
					if(ScanRespon==ScanValues)
					{
						ReadProcessMemory(GetCurrentProcess(),(void*)(ScanRanger1+ScanStart+0x00000003),(void*)&ScanRespon2, 4, NULL);
						if(ScanRespon2==ScanValues2)
						{
							Finds++;
							ultoa((ScanRanger1+ScanStart-0x00000001), List1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
							SendMessage(hLISTEdit1,LB_ADDSTRING,0,(LPARAM)List1);
						}
					}
				}
				*/
			}
			itoa(Finds,FindS,10);
			SetDlgItemText(hDlg,IDC_EDIT4,FindS); //��Ʈ�ѿ��ٰ� �����ִ°�
			//MessageBox(0,"��ĵ �Ϸ�!!","Clever Memory Editor",MB_OK);
			break;/////////////////////////////////////////////////////
		}
	}
return false;
}

char Line1[1024];
char Lines1[1024];
char Line2[1024];
char Lines2[1024];
char Line3[1024];
char Lines3[1024];
DWORD LineD;
DWORD LineD2;
DWORD LineD3;
DWORD LineOne[15];

char addition1[1024];
char addition2[1024];
char addition3[1024];
char addition4[1024];
DWORD additionp1;
DWORD additionp2;
DWORD additionp3;
DWORD additionp4;

BOOL CALLBACK InfoDlgProc3(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage) {
	case WM_INITDIALOG:
		SetDlgItemText(hDlg,IDC_EDIT1,"00400000"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_EDIT2,"00400010"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_EDIT3,"00400020"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_EDIT4,"00400030"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_EDIT6,"00400050"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_P2,"0031e4d8"); //��Ʈ�ѿ��ٰ� �����ִ°�
		break;
	case WM_COMMAND:
		switch (wParam) {
		case IDC_Cancel:
			EndDialog(hDlg,0);
			break;
		case IDC_Plus:
			GetDlgItemText(hDlg,IDC_P1,addition1,sizeof(addition1)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_P2,addition2,sizeof(addition2)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_P3,addition3,sizeof(addition3)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			additionp1 = strtoul(addition1, NULL, 16);
			additionp2 = strtoul(addition2, NULL, 16);
			additionp3 = strtoul(addition3, NULL, 16);
			additionp4 = additionp1 + additionp2 + additionp3;
			ultoa(additionp4, addition4, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_P4,addition4); //��Ʈ�ѿ��ٰ� �����ִ°�
			break;
		case IDC_On:
			GetDlgItemText(hDlg,IDC_EDIT1,Line1,sizeof(Line1)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_EDIT2,Line2,sizeof(Line2)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			GetDlgItemText(hDlg,IDC_EDIT3,Line3,sizeof(Line3)); // IDC_EDIT1�� �����ִ°��� Names���ٰ� ����
			LineD = strtoul(Line1, NULL, 16);
			LineD2 = strtoul(Line2, NULL, 16);
			LineD3 = strtoul(Line3, NULL, 16);
			ReadProcessMemory(GetCurrentProcess(),(void*)LineD,(void*)&LineOne[0], 1, NULL);
			ultoa(LineOne[0], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT7,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
			ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+1),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT8,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+2),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT9,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+3),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT10,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+4),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT11,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+5),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT12,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+6),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT13,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+7),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT14,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+8),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT15,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+9),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT16,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+10),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT17,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+11),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT18,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+12),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT20,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+13),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT21,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+14),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT22,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD+15),(void*)&LineOne[1], 1, NULL);
			ultoa(LineOne[1], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT23,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
////////////
			ReadProcessMemory(GetCurrentProcess(),(void*)LineD2,(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT24,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
			ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+1),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT25,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+2),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT26,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+3),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT27,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+4),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT28,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+5),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT29,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+6),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT30,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+7),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT31,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+8),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT32,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+9),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT33,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+10),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT34,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+11),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT35,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+12),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT36,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+13),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT37,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+14),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT38,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD2+15),(void*)&LineOne[2], 1, NULL);
			ultoa(LineOne[2], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT39,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
////////////
						ReadProcessMemory(GetCurrentProcess(),(void*)LineD3,(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT40,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+1),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT41,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+2),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT42,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+3),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT43,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+4),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT44,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+5),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT45,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+6),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT46,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+7),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT47,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+8),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT48,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+9),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT49,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+10),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT50,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+11),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT51,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+12),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT52,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+13),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT53,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+14),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT54,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
						ReadProcessMemory(GetCurrentProcess(),(void*)(LineD3+15),(void*)&LineOne[3], 1, NULL);
			ultoa(LineOne[3], Lines1, 16);// _ultoa�� long���� ���ڿ��� ��ȯ
			SetDlgItemText(hDlg,IDC_EDIT55,Lines1); //��Ʈ�ѿ��ٰ� �����ִ°�
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
return false;
}

HWND Check1 , Enabledz;
char F_TTT[1024];
char F_adr[1024];
char F_Value[1024];
DWORD F_Adr1 , F_Value1 , F_Adr2 , F_Value2, F_Adr3 , F_Value3, F_Adr4 , F_Value4, F_Adr5 , F_Value5;
DWORD F_Adr6 , F_Value6 , F_Adr7 , F_Value7, F_Adr8 , F_Value8, F_Adr9 , F_Value9;
int Interval;
BOOL Checking;

void F_Thread(){
	while(1){
		WriteProcessMemory(GetCurrentProcess(),(void*)F_Adr1, (void*)&F_Value1, Comy[0], NULL);
		WriteProcessMemory(GetCurrentProcess(),(void*)F_Adr2, (void*)&F_Value2, Comy[1], NULL);
		WriteProcessMemory(GetCurrentProcess(),(void*)F_Adr3, (void*)&F_Value3, Comy[2], NULL);
		WriteProcessMemory(GetCurrentProcess(),(void*)F_Adr4, (void*)&F_Value4, Comy[3], NULL);
		WriteProcessMemory(GetCurrentProcess(),(void*)F_Adr5, (void*)&F_Value5, Comy[4], NULL);
		WriteProcessMemory(GetCurrentProcess(),(void*)F_Adr6, (void*)&F_Value6, Comy[5], NULL);
		WriteProcessMemory(GetCurrentProcess(),(void*)F_Adr7, (void*)&F_Value7, Comy[6], NULL);
		WriteProcessMemory(GetCurrentProcess(),(void*)F_Adr8, (void*)&F_Value8, Comy[7], NULL);
		WriteProcessMemory(GetCurrentProcess(),(void*)F_Adr9, (void*)&F_Value9, Comy[8], NULL);
		if(Checking==0){
			break;
			break;
		}
		Sleep(Interval);
	}
}

BOOL CALLBACK InfoDlgProc4(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage) {
	case WM_INITDIALOG:
		ComboYo(hDlg);
		SetDlgItemText(hDlg,IDC_EDIT1,"00400000"); //��Ʈ�ѿ��ٰ� �����ִ°�
		SetDlgItemText(hDlg,IDC_Interval,"50"); //��Ʈ�ѿ��ٰ� �����ִ°�
		break;
	case WM_COMMAND:
		switch (wParam) {
		case IDC_Cancel:
			EndDialog(hDlg,0);
			break;
		case IDC_CHECK1:
			Combo1 = GetDlgItem(hDlg, IDC_COMBO1);
			Comy[0] = SendMessage(Combo1,CB_GETCURSEL,0,0);
			if(Comy[0]==0) Comy[0] = 1;
			else if(Comy[0]==1) Comy[0] = 2;
			else if(Comy[0]==2) Comy[0] = 4;
			Check1 = GetDlgItem(hDlg, IDC_CHECK1);
			if(SendMessage(Check1,BM_GETCHECK,0,0) == BST_CHECKED){
				GetDlgItemText(hDlg,IDC_EDIT1,F_adr,sizeof(F_adr)); // ��巹���ּҸ� AdrW���� ����
				GetDlgItemText(hDlg,IDC_EDIT2,F_Value,sizeof(F_Value)); // ��巹���ּҸ� AdrW���� ����
				F_Adr1 = strtoul(F_adr, NULL, 16); // AdrW���� adrWz���� ����
				F_Value1 = strtoul(F_Value, NULL, 10); // Value1���� Value1z���� ����
				Enabledz = GetDlgItem(hDlg, IDC_EDIT1);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO1);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT2);
				EnableWindow(Enabledz,0);
			}
			else{
				F_Adr1 = 0;
				F_Value1 = 0;
				Enabledz = GetDlgItem(hDlg, IDC_EDIT1);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO1);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT2);
				EnableWindow(Enabledz,1);
			}
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case IDC_CHECK2:
			Combo1 = GetDlgItem(hDlg, IDC_COMBO2);
			Comy[1] = SendMessage(Combo1,CB_GETCURSEL,0,0);
			if(Comy[1]==0) Comy[1] = 1;
			else if(Comy[1]==1) Comy[1] = 2;
			else if(Comy[1]==2) Comy[1] = 4;
			Check1 = GetDlgItem(hDlg, IDC_CHECK2);
			if(SendMessage(Check1,BM_GETCHECK,0,0) == BST_CHECKED){
				GetDlgItemText(hDlg,IDC_EDIT3,F_adr,sizeof(F_adr)); // ��巹���ּҸ� AdrW���� ����
				GetDlgItemText(hDlg,IDC_EDIT11,F_Value,sizeof(F_Value)); // ��巹���ּҸ� AdrW���� ����
				F_Adr2 = strtoul(F_adr, NULL, 16); // AdrW���� adrWz���� ����
				F_Value2 = strtoul(F_Value, NULL, 10); // Value1���� Value1z���� ����
				Enabledz = GetDlgItem(hDlg, IDC_EDIT3);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO2);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT11);
				EnableWindow(Enabledz,0);
			}
			else{
				F_Adr2 = 0;
				F_Value2 = 0;
				Enabledz = GetDlgItem(hDlg, IDC_EDIT3);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO2);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT11);
				EnableWindow(Enabledz,1);
			}
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case IDC_CHECK3:
			Combo1 = GetDlgItem(hDlg, IDC_COMBO3);
			Comy[2] = SendMessage(Combo1,CB_GETCURSEL,0,0);
			if(Comy[2]==0) Comy[2] = 1;
			else if(Comy[2]==1) Comy[2] = 2;
			else if(Comy[2]==2) Comy[2] = 4;
			Check1 = GetDlgItem(hDlg, IDC_CHECK3);
			if(SendMessage(Check1,BM_GETCHECK,0,0) == BST_CHECKED){
				GetDlgItemText(hDlg,IDC_EDIT4,F_adr,sizeof(F_adr)); // ��巹���ּҸ� AdrW���� ����
				GetDlgItemText(hDlg,IDC_EDIT12,F_Value,sizeof(F_Value)); // ��巹���ּҸ� AdrW���� ����
				F_Adr3 = strtoul(F_adr, NULL, 16); // AdrW���� adrWz���� ����
				F_Value3 = strtoul(F_Value, NULL, 10); // Value1���� Value1z���� ����
				Enabledz = GetDlgItem(hDlg, IDC_EDIT4);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO3);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT12);
				EnableWindow(Enabledz,0);
			}
			else{
				F_Adr3 = 0;
				F_Value3 = 0;
				Enabledz = GetDlgItem(hDlg, IDC_EDIT4);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO3);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT12);
				EnableWindow(Enabledz,1);
			}
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case IDC_CHECK4:
			Combo1 = GetDlgItem(hDlg, IDC_COMBO4);
			Comy[3] = SendMessage(Combo1,CB_GETCURSEL,0,0);
			if(Comy[3]==0) Comy[3] = 1;
			else if(Comy[3]==1) Comy[3] = 2;
			else if(Comy[3]==2) Comy[3] = 4;
			Check1 = GetDlgItem(hDlg, IDC_CHECK4);
			if(SendMessage(Check1,BM_GETCHECK,0,0) == BST_CHECKED){
				GetDlgItemText(hDlg,IDC_EDIT5,F_adr,sizeof(F_adr)); // ��巹���ּҸ� AdrW���� ����
				GetDlgItemText(hDlg,IDC_EDIT13,F_Value,sizeof(F_Value)); // ��巹���ּҸ� AdrW���� ����
				F_Adr4 = strtoul(F_adr, NULL, 16); // AdrW���� adrWz���� ����
				F_Value4 = strtoul(F_Value, NULL, 10); // Value1���� Value1z���� ����
				Enabledz = GetDlgItem(hDlg, IDC_EDIT5);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO4);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT13);
				EnableWindow(Enabledz,0);
			}
			else{
				F_Adr4 = 0;
				F_Value4 = 0;
				Enabledz = GetDlgItem(hDlg, IDC_EDIT5);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO4);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT13);
				EnableWindow(Enabledz,1);
			}
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case IDC_CHECK5:
			Combo1 = GetDlgItem(hDlg, IDC_COMBO5);
			Comy[4] = SendMessage(Combo1,CB_GETCURSEL,0,0);
			if(Comy[4]==0) Comy[4] = 1;
			else if(Comy[4]==1) Comy[4] = 2;
			else if(Comy[4]==2) Comy[4] = 4;
			Check1 = GetDlgItem(hDlg, IDC_CHECK5);
			if(SendMessage(Check1,BM_GETCHECK,0,0) == BST_CHECKED){
				GetDlgItemText(hDlg,IDC_EDIT6,F_adr,sizeof(F_adr)); // ��巹���ּҸ� AdrW���� ����
				GetDlgItemText(hDlg,IDC_EDIT14,F_Value,sizeof(F_Value)); // ��巹���ּҸ� AdrW���� ����
				F_Adr5 = strtoul(F_adr, NULL, 16); // AdrW���� adrWz���� ����
				F_Value5 = strtoul(F_Value, NULL, 10); // Value1���� Value1z���� ����
				Enabledz = GetDlgItem(hDlg, IDC_EDIT6);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO5);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT14);
				EnableWindow(Enabledz,0);
			}
			else{
				F_Adr5 = 0;
				F_Value5 = 0;
				Enabledz = GetDlgItem(hDlg, IDC_EDIT6);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO5);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT14);
				EnableWindow(Enabledz,1);
			}
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case IDC_CHECK6:
			Combo1 = GetDlgItem(hDlg, IDC_COMBO6);
			Comy[5] = SendMessage(Combo1,CB_GETCURSEL,0,0);
			if(Comy[5]==0) Comy[5] = 1;
			else if(Comy[5]==1) Comy[5] = 2;
			else if(Comy[5]==2) Comy[5] = 4;
			Check1 = GetDlgItem(hDlg, IDC_CHECK6);
			if(SendMessage(Check1,BM_GETCHECK,0,0) == BST_CHECKED){
				GetDlgItemText(hDlg,IDC_EDIT7,F_adr,sizeof(F_adr)); // ��巹���ּҸ� AdrW���� ����
				GetDlgItemText(hDlg,IDC_EDIT15,F_Value,sizeof(F_Value)); // ��巹���ּҸ� AdrW���� ����
				F_Adr6 = strtoul(F_adr, NULL, 16); // AdrW���� adrWz���� ����
				F_Value6 = strtoul(F_Value, NULL, 10); // Value1���� Value1z���� ����
				Enabledz = GetDlgItem(hDlg, IDC_EDIT7);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO6);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT15);
				EnableWindow(Enabledz,0);
			}
			else{
				F_Adr6 = 0;
				F_Value6 = 0;
				Enabledz = GetDlgItem(hDlg, IDC_EDIT7);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO6);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT15);
				EnableWindow(Enabledz,1);
			}
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case IDC_CHECK7:
			Combo1 = GetDlgItem(hDlg, IDC_COMBO7);
			Comy[6] = SendMessage(Combo1,CB_GETCURSEL,0,0);
			if(Comy[6]==0) Comy[6] = 1;
			else if(Comy[6]==1) Comy[6] = 2;
			else if(Comy[6]==2) Comy[6] = 4;
			Check1 = GetDlgItem(hDlg, IDC_CHECK7);
			if(SendMessage(Check1,BM_GETCHECK,0,0) == BST_CHECKED){
				GetDlgItemText(hDlg,IDC_EDIT8,F_adr,sizeof(F_adr)); // ��巹���ּҸ� AdrW���� ����
				GetDlgItemText(hDlg,IDC_EDIT16,F_Value,sizeof(F_Value)); // ��巹���ּҸ� AdrW���� ����
				F_Adr7 = strtoul(F_adr, NULL, 16); // AdrW���� adrWz���� ����
				F_Value7 = strtoul(F_Value, NULL, 10); // Value1���� Value1z���� ����
				Enabledz = GetDlgItem(hDlg, IDC_EDIT8);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO7);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT16);
				EnableWindow(Enabledz,0);
			}
			else{
				F_Adr7 = 0;
				F_Value7 = 0;
				Enabledz = GetDlgItem(hDlg, IDC_EDIT8);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO7);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT16);
				EnableWindow(Enabledz,1);
			}
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case IDC_CHECK8:
			Combo1 = GetDlgItem(hDlg, IDC_COMBO8);
			Comy[7] = SendMessage(Combo1,CB_GETCURSEL,0,0);
			if(Comy[7]==0) Comy[7] = 1;
			else if(Comy[7]==1) Comy[7] = 2;
			else if(Comy[7]==2) Comy[7] = 4;
			Check1 = GetDlgItem(hDlg, IDC_CHECK8);
			if(SendMessage(Check1,BM_GETCHECK,0,0) == BST_CHECKED){
				GetDlgItemText(hDlg,IDC_EDIT9,F_adr,sizeof(F_adr)); // ��巹���ּҸ� AdrW���� ����
				GetDlgItemText(hDlg,IDC_EDIT17,F_Value,sizeof(F_Value)); // ��巹���ּҸ� AdrW���� ����
				F_Adr8 = strtoul(F_adr, NULL, 16); // AdrW���� adrWz���� ����
				F_Value8 = strtoul(F_Value, NULL, 10); // Value1���� Value1z���� ����
				Enabledz = GetDlgItem(hDlg, IDC_EDIT9);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO8);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT17);
				EnableWindow(Enabledz,0);
			}
			else{
				F_Adr8 = 0;
				F_Value8 = 0;
				Enabledz = GetDlgItem(hDlg, IDC_EDIT9);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO8);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT17);
				EnableWindow(Enabledz,1);
			}
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case IDC_CHECK9:
			Combo1 = GetDlgItem(hDlg, IDC_COMBO9);
			Comy[8] = SendMessage(Combo1,CB_GETCURSEL,0,0);
			if(Comy[8]==0) Comy[8] = 1;
			else if(Comy[8]==1) Comy[8] = 2;
			else if(Comy[8]==2) Comy[8] = 4;
			Check1 = GetDlgItem(hDlg, IDC_CHECK9);
			if(SendMessage(Check1,BM_GETCHECK,0,0) == BST_CHECKED){
				GetDlgItemText(hDlg,IDC_EDIT10,F_adr,sizeof(F_adr)); // ��巹���ּҸ� AdrW���� ����
				GetDlgItemText(hDlg,IDC_EDIT18,F_Value,sizeof(F_Value)); // ��巹���ּҸ� AdrW���� ����
				F_Adr9 = strtoul(F_adr, NULL, 16); // AdrW���� adrWz���� ����
				F_Value9 = strtoul(F_Value, NULL, 10); // Value1���� Value1z���� ����
				Enabledz = GetDlgItem(hDlg, IDC_EDIT10);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO9);
				EnableWindow(Enabledz,0);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT18);
				EnableWindow(Enabledz,0);
			}
			else{
				F_Adr9 = 0;
				F_Value9 = 0;
				Enabledz = GetDlgItem(hDlg, IDC_EDIT10);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_COMBO9);
				EnableWindow(Enabledz,1);
				Enabledz = GetDlgItem(hDlg, IDC_EDIT18);
				EnableWindow(Enabledz,1);
			}
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case IDC_On:
			Enabledz = GetDlgItem(hDlg, IDC_On);
			EnableWindow(Enabledz,0);
			Enabledz = GetDlgItem(hDlg, IDC_OFF);
			EnableWindow(Enabledz,1);
			Checking = 1;
			GetDlgItemText(hDlg,IDC_Interval,F_TTT,sizeof(F_TTT)); // ��巹���ּҸ� AdrW���� ����
			Interval = atoi(F_TTT);
			CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)F_Thread, NULL, 0, 0 ); //������ ����
			break;/////////////////////////////////////////////////////
		case IDC_OFF:
			Enabledz = GetDlgItem(hDlg, IDC_On);
			EnableWindow(Enabledz,1);
			Enabledz = GetDlgItem(hDlg, IDC_OFF);
			EnableWindow(Enabledz,0);
			Checking = 0;
			break;/////////////////////////////////////////////////////
		}
	}
return false;
}
