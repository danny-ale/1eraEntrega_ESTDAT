#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <locale.h>
#include <Windows.h>
#include <CommDlg.h>
#include <Commctrl.h>
#include "resource.h"
using namespace std;

HWND ghDlg = 0;
HWND h_edit;
HWND h_boton;
HWND h_radio;
HWND h_listbox;
HWND h_combo;
HWND h_PicControl;
//HWND calendario;
//SYSTEMTIME st;
//HINSTANCE regis;
HMENU h_menu;
//HBITMAP bmp;
HINSTANCE INSTANCIA;
//OPENFILENAME ofn;
BOOL numOK;

//CHECARLO ES TODO Y PONER LAS FUNCIONES DE LEER Y GUARDAR

struct Persona {
	char a_ma[15];
	char a_pa[15];
	char name[15];
	char ID[15];
	char estadocivil[15];
	char fecha_nac[15];
	char direccion[200];
	char genero[15];
	char curp[20];
	char tel[15];
	char trabajo[15];
	Persona* anterior = nullptr;
	Persona* siguiente = nullptr;
};
Persona *primero = nullptr;
Persona* ultimo = nullptr;
Persona* temporal = nullptr;

struct Vacuna {
	char tipo[15];
	char marca[15];
	char num_dosis[15];
	char clave_v[15];
	char desc[MAX_PATH];
	float precio;
	Vacuna* anterior = nullptr;
	Vacuna* siguiente = nullptr;
};
Vacuna* primer = nullptr;
Vacuna* ultim = nullptr;
Vacuna* tempora = nullptr;

struct User {
	int clave_u;
	char contraseña[15];
	char a_materno[15];
	char a_paterno[15];
	char nombres[15];
	char namecom[52];
	User* anterior = nullptr;
	User* siguiente = nullptr;
};
User* prime = nullptr;
User* ulti = nullptr;
User* tempor = nullptr;

struct Carnet {
	char Curp[20];
	char vacuna[15];
	char No_dosis[15];
	char date[15];
	char lote[15];
	char lugar[15];
	int ID_carnet;
	Carnet* anterior = nullptr;
	Carnet* siguiente = nullptr;
};
Carnet* prim = nullptr;
Carnet* ult = nullptr;
Carnet* tempo = nullptr;

void guardarPertxt();
void guardarVtxt();
void guardarUtxt();
void guardarCartxt();
void guardarPer();
void guardarV();
void guardarU();
void guardarCar();
void leer_Per();
void leer_Vac();
void leer_User();
void leer_Carnet();
void dvacio(char datop[70]);

char gender[3][9] = { "Mujer","Hombre","Otro" };
char estcivil[4][15] = { "Soltero(a)","Casado(a)","Divorciado(a)","Viudo(a)" };
char dosisop[4][4] = { "1","2","3","4+" };
char datos[12][21] = {};
char direcc[200] = {};
char de[MAX_PATH] = {};
char nuevo[15], conteiner[52], dummy[15],datop[70],iniciarse[52];
int datonum1, datonum2,ID_CARNET,numeroID,c_user,prueba,tamaño,k;

//Contadores
float dno1;
int i,a,cur,detonante= 0;
fstream Archivo;
ofstream archivo;
ifstream txt;

BOOL CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Menu(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RegistroU(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RegistroV(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RegistroP(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RegistroC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	ghDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)WindowProc);
	ShowWindow(ghDlg, nCmdShow);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (ghDlg == 0 || !IsDialogMessage(ghDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
BOOL CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_INITDIALOG:
		h_boton = GetDlgItem(hwnd, iniciarsesion);
		h_boton = GetDlgItem(hwnd, regis);
		h_edit = GetDlgItem(hwnd, nameuser);
		h_edit = GetDlgItem(hwnd, contra);
		setlocale(LC_CTYPE, "Spanish");
		leer_Per();
		leer_Vac();
		leer_User();
		leer_Carnet();
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case iniciarsesion:

			GetDlgItemText(hwnd, nameuser, iniciarse, 52);
			GetDlgItemText(hwnd, contra, datos[2], 15);

			if (prime == nullptr) {
			MessageBox(hwnd, "No hay usuarios registrados aun", "MENSAJE", MB_ICONERROR);
			}

			else {
				ulti = prime;
				while (ulti) {
					if (strcmp(ulti->namecom, iniciarse) == 0 && strcmp(ulti->contraseña, datos[2]) == 0) {
						strcpy_s(iniciarse, 52, ulti->namecom);
						DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG2), 0, (DLGPROC)Menu);
					}
					else {
						ulti = ulti->siguiente;
					}
				}

				if (ulti == nullptr) {
					MessageBox(hwnd, "El usuario es incorrecto", "MENSAJE", MB_ICONERROR);
				}
			}

			break;

		case regis:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), 0, (DLGPROC)RegistroU);
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}
BOOL CALLBACK Menu(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//LE QUIERO PONER UNA IMAGEN PARA QUE NO SE VEA TAN SOLO
	switch (uMsg) {
	case WM_CREATE:
		MessageBox(hwnd, "Nose", "a", MB_CANCELTRYCONTINUE);
		break;
	case WM_INITDIALOG:
		h_menu = LoadMenu(INSTANCIA, MAKEINTRESOURCE(IDR_MENU1));
		h_edit = GetDlgItem(hwnd, nombreuser);
		SetMenu(hwnd, h_menu);
		SetDlgItemText(hwnd, nombreuser, iniciarse);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_USUARIO_REGISTRODEUSUARIO:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), 0, (DLGPROC)RegistroU);
			break;

		case ID_USUARIO_LISTADODEUSUARIO:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG4), 0, (DLGPROC)ActU);
			break;

		case ID_USUARIO_EDICIONDEUSUARIO:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG9), 0, (DLGPROC)BorrarU);
			break;

		case  ID_USUARIO_BAJADEUSUARIO:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG6), 0, (DLGPROC)ContraseU);
			break;

		case ID_VACUNA_REGISTRODEVACUNA:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG4), 0, (DLGPROC)RegistroV);
			break;

		case ID_VACUNA_LISTADODEVACUNA:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG7), 0, (DLGPROC)AltaCL);
			break;

		case ID_VACUNA_EDICIONDEVACUNA:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG11), 0, (DLGPROC)ActCL);
			break;

		case ID_VACUNA_BAJADEVACUNA:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG10), 0, (DLGPROC)BorrarCL);
			break;

		case ID_PERSONA_REGISTRODEPERSONA:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG5), 0, (DLGPROC)RegistroP);
			break;

		case ID_PERSONA_LISTADODEPERSONA:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG12), 0, (DLGPROC)AltaPromo);
			break;

		case ID_PERSONA_EDICIONDEPERSONA:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG15), 0, (DLGPROC)BorrarPromo);
			break;

		case ID_PERSONA_BAJADEPERSONA:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG13), 0, (DLGPROC)ListPromo);
			break;

		case ID_CARNET_REGISTRODECARNETDEVACUNACION:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG6), 0, (DLGPROC)RegistroC);
			break;

		case ID_CARNET_LISTADODECARNETDEVACUNACION:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG17), 0, (DLGPROC)AltaConsu);
			break;

		case ID_CARNET_EDICIONDECARNETDEVACUNACION:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG18), 0, (DLGPROC)ListaConsu);
			break;
		case ID_CARNET_BAJADECA:
			//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG18), 0, (DLGPROC)ListaConsu);
			break;
		case ID_SALIR_SALIR40018:
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
		}
		break;

		break;
	case WM_CLOSE:
		EndDialog(hwnd, wParam);
		return true;
	}

	return FALSE;
}
BOOL CALLBACK RegistroU(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		MessageBox(hwnd, "Nose", "a", MB_CANCELTRYCONTINUE);
		break;
	case WM_INITDIALOG:
		h_edit = GetDlgItem(hwnd, nameu);
		h_edit = GetDlgItem(hwnd, ap);
		h_edit = GetDlgItem(hwnd, am);
		h_edit = GetDlgItem(hwnd, contras);
		h_edit = GetDlgItem(hwnd, nombreuser2);
		h_boton = GetDlgItem(hwnd, G_RU);
		h_boton = GetDlgItem(hwnd, R_RU);
		SetDlgItemText(hwnd, nombreuser2, iniciarse);
		//ESTO ES DE LA FOTO
		//h_PicControl = GetDlgItem(hwnd, FOTON4);
		//bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 70, 50, LR_LOADFROMFILE);
		//SendDlgItemMessage(hwnd, FOTON4, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		//SetDlgItemText(hwnd, NOM4, Unom);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case G_RU:
			prueba = 0;
			detonante = 0;
			GetDlgItemText(hwnd,nameu, datos[1], 15);
			GetDlgItemText(hwnd,ap, datos[2], 15);
			GetDlgItemText(hwnd,am, datos[3], 15);
			GetDlgItemText(hwnd,contras, datos[4], 15);
			datonum1 = GetDlgItemInt(hwnd, contras2, &numOK, FALSE);
			if (prime != nullptr) {
				while (detonante == 0) {
					if (prime) {
						tempor = prime;
						while (tempor) {
							if (tempor->clave_u == datonum1)
							{
								prueba=1;
							}
							tempor = tempor->siguiente;
						}
						detonante = 1;
					}
				}
			}
			dvacio(datos[1]);
			dvacio(datos[2]);
			dvacio(datos[3]);
			dvacio(datos[4]);

			if (prueba == 0) {
				if (prime == nullptr) {
					prime = new User;
					strcpy_s(prime->nombres, 15, datos[1]);
					strcpy_s(prime->a_paterno, 15, datos[2]);
					strcpy_s(prime->a_materno, 15, datos[3]);
					strcpy_s(prime->contraseña, 15, datos[4]);
					prime->clave_u = datonum1;
					strcpy_s(conteiner, 15, "");
					strcat_s(conteiner, prime->nombres);
					strcat_s(conteiner, " ");
					strcat_s(conteiner, prime->a_paterno);
					strcat_s(conteiner, " ");
					strcat_s(conteiner, prime->a_materno);
					strcpy_s(prime->namecom, 52, conteiner);
					prime->siguiente = nullptr;
					prime->anterior = nullptr;
					ulti = prime;
				}
				else {
					ulti = prime;
					while (ulti->siguiente != nullptr) {
						ulti = ulti->siguiente;
					}
					ulti->siguiente = new User;
					ulti->siguiente->siguiente = nullptr;
					ulti->siguiente->anterior = ulti;
					ulti = ulti->siguiente;
					strcpy_s(ulti->nombres, 15, datos[1]);
					strcpy_s(ulti->a_paterno, 15, datos[2]);
					strcpy_s(ulti->a_materno, 15, datos[3]);
					strcpy_s(ulti->contraseña, 15, datos[4]);
					ulti->clave_u = datonum1;
					strcpy_s(conteiner, 15, "");
					strcat_s(conteiner, ulti->nombres);
					strcat_s(conteiner, " ");
					strcat_s(conteiner, ulti->a_paterno);
					strcat_s(conteiner, " ");
					strcat_s(conteiner, ulti->a_materno);
					strcpy_s(ulti->namecom, 52, conteiner);
				}
				MessageBox(hwnd, "El registro se ha realizado", "MENSAJE", MB_ICONINFORMATION);
				guardarUtxt();
				guardarU();
			}
			else {
				MessageBox(hwnd, "Ha dejado espacios en blanco o la clave de usuario se ha repetido", "MENSAJE", MB_ICONINFORMATION);

			}
			EndDialog(hwnd, wParam);
			break;

		case R_RU:
			EndDialog(hwnd, wParam);
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, wParam);
		return true;
	}

	return FALSE;
}
BOOL CALLBACK RegistroV(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		MessageBox(hwnd, "Nose", "a", MB_CANCELTRYCONTINUE);
		break;
	case WM_INITDIALOG:
		h_edit = GetDlgItem(hwnd, tv);
		h_edit = GetDlgItem(hwnd, m);
		h_edit = GetDlgItem(hwnd, cv);
		h_edit = GetDlgItem(hwnd, d);
		h_edit = GetDlgItem(hwnd, ninero);
		h_edit = GetDlgItem(hwnd, nombreuser3);
		h_boton = GetDlgItem(hwnd, G_VA);
		h_boton = GetDlgItem(hwnd, R_VA);
		h_combo = GetDlgItem(hwnd, IDC_COMBO1);
		SetDlgItemText(hwnd, nombreuser3, iniciarse);
		//ESTO ES DE LA FOTO
		//h_PicControl = GetDlgItem(hwnd, FOTON4);
		//bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 70, 50, LR_LOADFROMFILE);
		//SendDlgItemMessage(hwnd, FOTON4, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		//SetDlgItemText(hwnd, NOM4, Unom);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case G_VA:
			prueba = 0;
			detonante = 0;
			GetDlgItemText(hwnd, tv, datos[1], 15);
			GetDlgItemText(hwnd, m, datos[2], 15);
			GetDlgItemText(hwnd, d, de, MAX_PATH);
			GetDlgItemText(hwnd, cv, datos[4], 15);
			dno1 = GetDlgItemInt(hwnd, ninero, &numOK, FALSE);
			if (primer != nullptr) {
				while (detonante == 0) {
					if (primer) {
						tempora = primer;
						while (tempora) {
							if (strcmp(tempora->marca,datos[2])==0)
							{
								prueba = 2;
							}
							tempora = tempora->siguiente;
						}
						detonante = 1;
					}
				}
			}
			dvacio(datos[1]);
			dvacio(datos[2]);
			dvacio(datos[3]);
			dvacio(datos[3]);
			dvacio(de);

			if (prueba == 0 && dno1 > 0) {
				if (primer == nullptr) {
					primer = new Vacuna;
					strcpy_s(primer->tipo, 15, datos[1]);
					strcpy_s(primer->marca, 15, datos[2]);
					strcpy_s(primer->num_dosis, 15, datos[3]);
					strcpy_s(primer->desc, MAX_PATH, de);
					primer->precio = dno1;
					//Validar que sea mayor a 0
					//---------------------
					strcpy_s(primer->clave_v,15, datos[4]);
					//Se debe checar que no se repita con ninguna ya registrada
					primer->siguiente = nullptr;
					primer->anterior = nullptr;
					ultim = primer;
				}
				else {
					ultim = primer;
					while (ultim->siguiente != nullptr) {
						ultim = ultim->siguiente;
					}
					ultim->siguiente = new Vacuna;
					ultim->siguiente->siguiente = nullptr;
					ultim->siguiente->anterior = ultim;
					ultim = ultim->siguiente;
					strcpy_s(ultim->tipo, 15, datos[1]);
					strcpy_s(ultim->marca, 15, datos[2]);
					strcpy_s(ultim->num_dosis, 15, datos[3]);
					strcpy_s(ultim->desc, MAX_PATH, de);
					ultim->precio = dno1;
					//validar que sea mayor a 0
					//---------------------
					strcpy_s(ultim->clave_v,15,datos[4]);
					//Se debe checar que no se repita con ninguna ya registrada
				}
				MessageBox(hwnd, "El registro se ha realizado", "MENSAJE", MB_ICONINFORMATION);
				guardarVtxt();
				guardarV();
			}

			else {
				MessageBox(hwnd, "Ha dejado espacios en blanco o se ha repetido la clave de  marca de vacuna", "MENSAJE", MB_ICONERROR);
			}

			EndDialog(hwnd, wParam);
			break;

		case R_VA:
			EndDialog(hwnd, wParam);
			break;

		case IDC_COMBO1:
			for (a = 0; a < 4; a++) {
				SendDlgItemMessage(hwnd, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)a, (LPARAM)dosisop[a]);
			}
			
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				cur = SendDlgItemMessage(hwnd, IDC_COMBO1, CB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_COMBO1, CB_GETLBTEXT, cur, (LPARAM)datos[3]);
			}
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, wParam);
		return true;
	}

	return FALSE;
}
BOOL CALLBACK RegistroP(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		MessageBox(hwnd, "Nose", "a", MB_CANCELTRYCONTINUE);
		break;
	case WM_INITDIALOG:
		h_edit = GetDlgItem(hwnd, np);
		h_edit = GetDlgItem(hwnd, app);
		h_edit = GetDlgItem(hwnd, ampersona);
		h_edit = GetDlgItem(hwnd, curpi);
		h_edit = GetDlgItem(hwnd, fnp);
		h_edit = GetDlgItem(hwnd, dp);
		h_edit = GetDlgItem(hwnd, telp);
		h_edit = GetDlgItem(hwnd, ine);
		h_edit = GetDlgItem(hwnd, work);
		h_edit = GetDlgItem(hwnd, nombreuser4);
		h_combo = GetDlgItem(hwnd, ec);
		h_combo = GetDlgItem(hwnd, gene);
		h_boton = GetDlgItem(hwnd, G_RP);
		h_boton = GetDlgItem(hwnd, R_RP);
		SetDlgItemText(hwnd, nombreuser4, iniciarse);
		//ESTO ES DE LA FOTO
		//h_PicControl = GetDlgItem(hwnd, FOTON4);
		//bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 70, 50, LR_LOADFROMFILE);
		//SendDlgItemMessage(hwnd, FOTON4, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		//SetDlgItemText(hwnd, NOM4, Unom);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case G_RP:
			prueba = 0;
			detonante = 0;
			GetDlgItemText(hwnd, np, datos[1], 15);
			GetDlgItemText(hwnd, app, datos[2], 15);
			GetDlgItemText(hwnd, ampersona, datos[3], 15);
			GetDlgItemText(hwnd, curpi, datos[4], 20);
			GetDlgItemText(hwnd, fnp, datos[11], 15);
			GetDlgItemText(hwnd, dp, direcc, 200);
			GetDlgItemText(hwnd, telp, datos[7], 15);
			GetDlgItemText(hwnd, ine, datos[8], 15);
			GetDlgItemText(hwnd, work, datos[10], 15);
			if (primero != nullptr) {
				while (detonante == 0) {
					if (primero) {
						temporal = primero;
						while (temporal) {
							if (strcmp(temporal->curp,datos[4]) == 0)
							{
								prueba = 1;
							}
							temporal = temporal->siguiente;
						}
						detonante = 1;
					}
				}
			}
			dvacio(datos[1]);
			dvacio(datos[2]);
			dvacio(datos[3]);
			dvacio(datos[4]);
			dvacio(direcc);
			dvacio(datos[6]);
			dvacio(datos[7]);
			dvacio(datos[8]);
			dvacio(datos[9]);
			dvacio(datos[10]);
			dvacio(datos[11]);
			if (prueba == 0) {
				if (primero == nullptr) {
					primero = new Persona;
					strcpy_s(primero->name, 15, datos[1]);
					strcpy_s(primero->a_pa, 15, datos[2]);
					strcpy_s(primero->a_ma, 15, datos[3]);
					strcpy_s(primero->curp, 20, datos[4]);
					//Revisar que no se repita el curp
					strcpy_s(primero->fecha_nac, 15, datos[11]);
					strcpy_s(primero->direccion, 200, direcc);
					strcpy_s(primero->estadocivil, 15, datos[6]);
					strcpy_s(primero->tel, 15, datos[7]);
					strcpy_s(primero->ID, 15, datos[8]);
					strcpy_s(primero->genero, 15, datos[9]);
					strcpy_s(primero->trabajo, 15, datos[10]);
					primero->siguiente = nullptr;
					primero->anterior = nullptr;
					ultimo = primero;
				}
				else {
					ultimo = primero;
					while (ultimo->siguiente != nullptr) {
						ultimo = ultimo->siguiente;
					}
					ultimo->siguiente = new Persona;
					ultimo->siguiente->siguiente = nullptr;
					ultimo->siguiente->anterior = ultimo;
					ultimo = ultimo->siguiente;
					strcpy_s(ultimo->name, 15, datos[1]);
					strcpy_s(ultimo->a_pa, 15, datos[2]);
					strcpy_s(ultimo->a_ma, 15, datos[3]);
					strcpy_s(ultimo->curp, 20, datos[4]);
					//revisar que no se repita el curp
					strcpy_s(ultimo->fecha_nac, 15, datos[11]);
					strcpy_s(ultimo->direccion, 200, direcc);
					strcpy_s(ultimo->estadocivil, 15, datos[6]);
					strcpy_s(ultimo->tel, 15, datos[7]);
					strcpy_s(ultimo->ID, 15, datos[8]);
					strcpy_s(ultimo->genero, 15, datos[9]);
					strcpy_s(ultimo->trabajo, 15, datos[10]);
				}
				MessageBox(hwnd, "El registro se ha realizado", "MENSAJE", MB_ICONINFORMATION);
				guardarPertxt();
				guardarPer();
			}
			else {
				MessageBox(hwnd, "Ha dejado espacios en blanco o hay un curp ya registrado coincidente", "MENSAJE", MB_ICONERROR);
			}
			EndDialog(hwnd, wParam);
			break;

		case R_RP:
			EndDialog(hwnd, wParam);
			break;

		case ec:
			for (a = 0; a < 4; a++) {
				SendDlgItemMessage(hwnd, ec, CB_INSERTSTRING, (WPARAM)a, (LPARAM)estcivil[a]);
			}

			if (HIWORD(wParam) == LBN_SELCHANGE) {
				cur = 0;
				cur = SendDlgItemMessage(hwnd, ec, CB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, ec, CB_GETLBTEXT, cur, (LPARAM)datos[6]);
			}
			break;


		case gene:
			for (a = 0; a < 3; a++) {
				SendDlgItemMessage(hwnd, gene, CB_INSERTSTRING, (WPARAM)a, (LPARAM)gender[a]);
			}

			if (HIWORD(wParam) == LBN_SELCHANGE) {
				cur = 0;
				cur = SendDlgItemMessage(hwnd,gene, CB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, gene, CB_GETLBTEXT, cur, (LPARAM)datos[9]);
			}
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, wParam);
		return true;
	}

	return FALSE;
}
BOOL CALLBACK RegistroC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		MessageBox(hwnd, "Nose", "a", MB_CANCELTRYCONTINUE);
		break;
	case WM_INITDIALOG:
		h_edit = GetDlgItem(hwnd, datee);
		h_edit = GetDlgItem(hwnd, datee2);
		h_edit = GetDlgItem(hwnd, centro);
		h_edit = GetDlgItem(hwnd, nombreuser5);
		h_combo = GetDlgItem(hwnd, C_Personas);
		h_combo = GetDlgItem(hwnd, C_Vacunas);
		h_combo = GetDlgItem(hwnd, C_NumDosis);
		h_boton = GetDlgItem(hwnd, G_RC);
		h_boton = GetDlgItem(hwnd, R_RC);
		SetDlgItemText(hwnd, nombreuser5, iniciarse);
		//ESTO ES DE LA FOTO
		//h_PicControl = GetDlgItem(hwnd, FOTON4);
		//bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 70, 50, LR_LOADFROMFILE);
		//SendDlgItemMessage(hwnd, FOTON4, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		//SetDlgItemText(hwnd, NOM4, Unom);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case G_RC:
			prueba = 0;
			detonante = 0;
			GetDlgItemText(hwnd, datee, datos[3], 15);
			GetDlgItemText(hwnd, centro, datos[5], 15);
			GetDlgItemText(hwnd, datee2, datos[4], 15);
			ID_CARNET++;
			if (prim != nullptr) {
				while (detonante == 0) {
					if (prim) {
						tempo = prim;
						while (tempo) {
							if (tempo->ID_carnet == ID_CARNET)
							{
								ID_CARNET++;
							}
							else {
								datonum2 = ID_CARNET;
								detonante = 1;
							}
							tempo = tempo->siguiente;
						}
					}
				}
			}
			else {
				datonum2 = ID_CARNET;
			}
			dvacio(datos[1]);
			dvacio(datos[2]);
			dvacio(datos[3]);
			dvacio(datos[4]);
			dvacio(datos[5]);
			dvacio(datos[6]);
			if (prueba == 0) {
				if (prim == nullptr) {

					prim = new Carnet;
					strcpy_s(prim->Curp, 20, datos[1]);
					strcpy_s(prim->vacuna, 15, datos[2]);
					strcpy_s(prim->No_dosis, 15, datos[6]);
					strcpy_s(prim->date, 15, datos[3]);
					strcpy_s(prim->lote, 15, datos[4]);
					strcpy_s(prim->lugar, 15, datos[5]);
					prim->ID_carnet = datonum2;
					prim->siguiente = nullptr;
					prim->anterior = nullptr;
					ult = prim;


				}
				else {
					ult = prim;
					while (ult->siguiente != nullptr) {
						ult = ult->siguiente;
					}
					ult->siguiente = new Carnet;
					ult->siguiente->siguiente = nullptr;
					ult->siguiente->anterior = ult;
					ult = ult->siguiente;
					strcpy_s(ult->Curp, 20, datos[1]);
					strcpy_s(ult->vacuna, 15, datos[2]);
					strcpy_s(ult->No_dosis, 15, datos[6]);
					strcpy_s(ult->date, 15, datos[3]);
					strcpy_s(ult->lote, 15, datos[4]);
					strcpy_s(ult->lugar, 15, datos[5]);
					ult->ID_carnet = datonum2;

				}
				MessageBox(hwnd, "El registro se ha realizado", "MENSAJE", MB_ICONINFORMATION);
//				if (primer) {
//					tempora = primer;
//					while (tempora) {
//						if (strcmp(tempora->marca,datos[2])==0){
//							if (strcmp(tempora->num_dosis, datos[6])==0) {
//								MessageBox(hwnd, "Has acabado tu esquema de vacunacion", "INFO", MB_ICONINFORMATION);
//							}
//							else {
//								MessageBox(hwnd, "Tienes dosis pendientes", "INFO", MB_ICONINFORMATION);
//							}
//						
//						}
//						tempora = tempora->siguiente;
//					}
//				}
				guardarCartxt();
				guardarCar();
			}
			else {
				MessageBox(hwnd, "Ha dejado espacios en blanco", "MENSAJE", MB_ICONERROR);
			}
			EndDialog(hwnd, wParam);
			break;

		case R_RC:
			EndDialog(hwnd, wParam);
			break;

		case C_Personas:
			if (primero) {
				temporal = primero;
				while (temporal) {
					SendDlgItemMessage(hwnd, C_Personas, CB_INSERTSTRING, 0, (LPARAM)temporal->curp);
					temporal = temporal->siguiente;
				}
			}
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				cur = SendDlgItemMessage(hwnd, C_Personas, CB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, C_Personas, CB_GETLBTEXT, cur, (LPARAM)datos[1]);
			}
			break;

		case C_Vacunas:
			if (primer) {
				tempora = primer;
				while (tempora) {
					SendDlgItemMessage(hwnd, C_Vacunas, CB_INSERTSTRING, 0, (LPARAM)tempora->marca);
					tempora = tempora->siguiente;
				}
			}
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				cur = SendDlgItemMessage(hwnd, C_Vacunas, CB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, C_Vacunas, CB_GETLBTEXT, cur, (LPARAM)datos[2]);
			}

			break;

		case C_NumDosis:
			for (a = 0; a < 4; a++) {
				SendDlgItemMessage(hwnd,C_NumDosis, CB_INSERTSTRING, (WPARAM)a, (LPARAM)dosisop[a]);
			}

			if (HIWORD(wParam) == LBN_SELCHANGE) {
				cur = SendDlgItemMessage(hwnd, C_NumDosis, CB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, C_NumDosis, CB_GETLBTEXT, cur, (LPARAM)datos[6]);
			}
			break;

		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, wParam);
		return true;
	}

	return FALSE;
}

void guardarPertxt() {
archivo.open("Personatexto.txt",ios::out);
	if (primero) {
	ultimo = primero;
		while (ultimo) {
			archivo << ultimo->a_ma << endl;
			archivo << ultimo->a_pa << endl;
			archivo << ultimo->name<< endl;
			archivo << ultimo->ID << endl;
			archivo << ultimo->estadocivil << endl;
			archivo<<ultimo->fecha_nac<<endl;
			archivo<<ultimo->direccion<<endl;
			archivo<<ultimo->genero<<endl;
			archivo<<ultimo->curp<<endl;
			archivo<<ultimo->tel<<endl;
			archivo<<ultimo->trabajo<<endl;
			ultimo = ultimo->siguiente;
		}
	}
archivo.close();

}
void guardarVtxt() {
	archivo.open("Vacunatexto.txt", ios::out);
	if (primer) {
		ultim = primer;
		while (ultim) {
			archivo << ultim->tipo<<endl;
			archivo << ultim->marca<<endl;
			archivo << ultim->num_dosis<<endl;
			archivo << ultim->clave_v<<endl;
			archivo << ultim->desc<<endl;
			archivo << ultim->precio<<endl;
			ultim = ultim->siguiente;
		}
	}
	archivo.close();
}
void guardarUtxt() {
	archivo.open("Usuariotexto.txt", ios::out);
	if (prime) {
		ulti = prime;
		while (ulti) {
			archivo << ulti->clave_u<<endl;
			archivo << ulti->contraseña<<endl;
			archivo << ulti->a_materno<<endl;
			archivo << ulti->a_paterno<<endl;
			archivo << ulti->nombres<<endl;
			archivo << ulti->namecom<<endl;
			ulti = ulti->siguiente;
		}
	}
	archivo.close();
}
void guardarCartxt() {
	archivo.open("Carnettexto.txt", ios::out);
	if (prim) {
		ult = prim;
		while (ult) {
			archivo << ult->Curp<<endl;
			archivo << ult->vacuna << endl;
			archivo << ult->No_dosis << endl;
			archivo << ult->date<<endl;
			archivo << ult->lote<<endl;
			archivo << ult->lugar<<endl;
			archivo << ult->ID_carnet << endl;
			ult = ult->siguiente;
		}
	}
	archivo.close();
}
void guardarPer() {
	Archivo.open("Persona.bin", ios::out | ios::binary);

	if (Archivo.is_open()) {

		ultimo = primero;

		while (ultimo != nullptr) {

			Archivo.write(reinterpret_cast<char*>(ultimo), sizeof(Persona));
			ultimo = ultimo->siguiente;
		}

	}

	Archivo.close();

}
void guardarV() {
	Archivo.open("Vacuna.bin", ios::out | ios::binary);

	if (Archivo.is_open()) {

		ultim = primer;

		while (ultim != nullptr) {

			Archivo.write(reinterpret_cast<char*>(ultim), sizeof(Vacuna));
			ultim = ultim->siguiente;
		}

	}
	Archivo.close();
}
void guardarU() {
	Archivo.open("Usuario.bin", ios::out | ios::binary);

	if (Archivo.is_open()) {

		ulti = prime;

		while (ulti != nullptr) {

			Archivo.write(reinterpret_cast<char*>(ulti), sizeof(User));
			ulti = ulti->siguiente;
		}

	}
	Archivo.close();

}
void guardarCar() {
	Archivo.open("Carnet.bin", ios::out | ios::binary);

	if (Archivo.is_open()) {

		ult = prim;

		while (ult != nullptr) {

			Archivo.write(reinterpret_cast<char*>(ult), sizeof(Carnet));
			ult = ult->siguiente;
		}

	}
	Archivo.close();

}
void leer_Per() {
	Archivo.open("Persona.bin", ios::in | ios::binary);
	if (Archivo.is_open()) {
		Archivo.seekg(0, ios::end);
		tamaño = Archivo.tellg();
		if (tamaño != 0) {
			Persona* l = new Persona;
			while (!Archivo.eof()) {
				Archivo.seekg(k * sizeof(Persona));
				Archivo.read(reinterpret_cast<char*>(l), sizeof(Persona));
				if (primero == nullptr) {
					primero = new Persona;
					strcpy_s(primero->a_ma,15,l->a_ma);
					strcpy_s(primero->a_pa,15,l->a_pa);
					strcpy_s(primero->name,15,l->name);
					strcpy_s(primero->ID,15,l->ID);
					strcpy_s(primero->estadocivil,15,l->estadocivil);
					strcpy_s(primero->fecha_nac ,15,l->fecha_nac);
					strcpy_s(primero->direccion,200, l->direccion);
					strcpy_s(primero->genero,15,l->genero);
					strcpy_s(primero->curp,20,l->curp);
					strcpy_s(primero->tel,15,l->tel);
					strcpy_s(primero->trabajo,15,l->trabajo);
					primero->siguiente = nullptr;
					primero->anterior = nullptr;
					ultimo = primero;
					k++;
				}
				else {
					ultimo = primero;
					while (ultimo->siguiente != nullptr) {
						ultimo = ultimo->siguiente;
					}
					ultimo->siguiente = new Persona;
					ultimo->siguiente->siguiente = nullptr;
					ultimo->siguiente->anterior = ultimo;
					ultimo = ultimo->siguiente;
					strcpy_s(ultimo->a_ma, 15, l->a_ma);
					strcpy_s(ultimo->a_pa, 15, l->a_pa);
					strcpy_s(ultimo->name, 15, l->name);
					strcpy_s(ultimo->ID, 15, l->ID);
					strcpy_s(ultimo->estadocivil, 15, l->estadocivil);
					strcpy_s(ultimo->fecha_nac, 15, l->fecha_nac);
					strcpy_s(ultimo->direccion, 200, l->direccion);
					strcpy_s(ultimo->genero, 15, l->genero);
					strcpy_s(ultimo->curp, 20, l->curp);
					strcpy_s(ultimo->tel, 15, l->tel);
					strcpy_s(ultimo->trabajo, 15, l->trabajo);
					k++;
				}
			}
			k = 0;

			if (primero->a_ma == "") {
				primero->siguiente = nullptr;
				primero = nullptr;
			}
			else {
				temporal = ultimo;
				if (temporal->siguiente == nullptr) {
					temporal->anterior->siguiente = temporal->siguiente;
					if (temporal->siguiente != nullptr) {
						temporal->siguiente->anterior = temporal->anterior;
					}
					delete temporal;
				}
			}
		}
	}

	Archivo.close();
}
void leer_Vac() {
	Archivo.open("Vacuna.bin", ios::in | ios::binary);
	if (Archivo.is_open()) {
		Archivo.seekg(0, ios::end);
		tamaño = Archivo.tellg();
		if (tamaño != 0) {
			Vacuna* le = new Vacuna;
			while (!Archivo.eof()) {
				Archivo.seekg(k * sizeof(Vacuna));
				Archivo.read(reinterpret_cast<char*>(le), sizeof(Vacuna));
				if (primer == nullptr) {
					primer = new Vacuna;
					strcpy_s(primer->tipo,15,le->tipo);
					strcpy_s(primer->marca ,15,le->marca);
					strcpy_s(primer->num_dosis,15,le->num_dosis);
					strcpy_s(primer->clave_v,15,le->clave_v);
					strcpy_s(primer->desc,259, le->desc);
					primer->precio = le->precio;
					primer->siguiente = nullptr;
					primer->anterior = nullptr;
					ultim = primer;
					k++;
				}
				else {
					ultim = primer;
					while (ultim->siguiente != nullptr) {
						ultim = ultim->siguiente;
					}
					ultim->siguiente = new Vacuna;
					ultim->siguiente->siguiente = nullptr;
					ultim->siguiente->anterior = ultim;
					ultim = ultim->siguiente;
					strcpy_s(ultim->tipo,15,le->tipo);
					strcpy_s(ultim->marca,15,le->marca);
					strcpy_s(ultim->num_dosis,15,le->num_dosis);
					strcpy_s(ultim->clave_v,15,le->clave_v);
					strcpy_s(ultim->desc,259, le->desc);
					ultim->precio = le->precio;
					k++;
				}
			}
			k = 0;

			if (primer->tipo == "") {
				primer->siguiente = nullptr;
				primer = nullptr;
			}
			else {
				tempora = ultim;
				if (tempora->siguiente == nullptr) {
					tempora->anterior->siguiente = tempora->siguiente;
					if (tempora->siguiente != nullptr) {
						tempora->siguiente->anterior = tempora->anterior;
					}
					delete tempora;
				}
			}
		}
	}

	Archivo.close();
}
void leer_User() {
	Archivo.open("Usuario.bin", ios::in | ios::binary);
	if (Archivo.is_open()) {
		Archivo.seekg(0, ios::end);
		tamaño = Archivo.tellg();
		if (tamaño != 0) {
			User* lec = new User;
			while (!Archivo.eof()) {
				Archivo.seekg(k * sizeof(User));
				Archivo.read(reinterpret_cast<char*>(lec), sizeof(User));
				if (prime == nullptr) {
					prime = new User;
					prime->clave_u = lec->clave_u;
					strcpy_s(prime->contraseña,15,lec->contraseña);
					strcpy_s(prime->a_materno,15,lec->a_materno);
					strcpy_s(prime->a_paterno,15,lec->a_paterno);
					strcpy_s(prime->nombres,15,lec->nombres);
					strcat_s(conteiner ,"");
					strcat_s(conteiner,prime->nombres);
					strcat_s(conteiner," ");
					strcat_s(conteiner,prime->a_paterno);
					strcat_s(conteiner," ");
					strcat_s(conteiner,prime->a_materno);
					strcpy_s(prime->namecom,52,conteiner);
					prime->siguiente = nullptr;
					prime->anterior = nullptr;
					ulti = prime;
					k++;
				}
				else {
					ulti = prime;
					while (ulti->siguiente != nullptr) {
						ulti = ulti->siguiente;
					}
					ulti->siguiente = new User;
					ulti->siguiente->siguiente = nullptr;
					ulti->siguiente->anterior = ulti;
					ulti = ulti->siguiente;
					ulti->clave_u = lec->clave_u;
					strcpy_s(ulti->contraseña,15,lec->contraseña);
					strcpy_s(ulti->a_materno,15,lec->a_materno);
					strcpy_s(ulti->a_paterno,15,lec->a_paterno);
					strcpy_s(ulti->nombres,15,lec->nombres);
					strcat_s(conteiner,"");
					strcat_s(conteiner, ulti->nombres);
					strcat_s(conteiner, " ");
					strcat_s(conteiner, ulti->a_paterno);
					strcat_s(conteiner, " ");
					strcat_s(conteiner,ulti->a_materno);
					strcpy_s(ulti->namecom,52,conteiner);
					k++;
				}
			}
			k = 0;

			if (prime->contraseña == "") {
				prime->siguiente = nullptr;
				prime = nullptr;
			}
			else {
				tempor = ulti;
				if (tempor->siguiente == nullptr) {
					tempor->anterior->siguiente = tempor->siguiente;
					if (tempor->siguiente != nullptr) {
						tempor->siguiente->anterior = tempor->anterior;
					}
					delete tempor;
				}
			}
		}
	}

	Archivo.close();
}
void leer_Carnet() {
	Archivo.open("Carnet.bin", ios::in | ios::binary);
	if (Archivo.is_open()) {
		Archivo.seekg(0, ios::end);
		tamaño = Archivo.tellg();
		if (tamaño != 0) {
			Carnet* lect = new Carnet;
			while (!Archivo.eof()) {
				Archivo.seekg(k * sizeof(Carnet));
				Archivo.read(reinterpret_cast<char*>(lect), sizeof(Carnet));
				if (prim == nullptr) {
					prim = new Carnet;
					strcpy_s(prim->Curp,20,lect->Curp);
					strcpy_s(prim->vacuna ,15,lect->vacuna);
					strcpy_s(prim->No_dosis ,15,lect->No_dosis);
					strcpy_s(prim->date,15,lect->date);
					strcpy_s(prim->lote,15,lect->lote);
					strcpy_s(prim->lugar,15,lect->lugar);
					prim->ID_carnet = lect->ID_carnet;
					prim->siguiente = nullptr;
					prim->anterior = nullptr;
					ult = prim;
					k++;
				}
				else {
					ult = prim;
					while (ult->siguiente != nullptr) {
						ult = ult->siguiente;
					}
					ult->siguiente = new Carnet;
					ult->siguiente->siguiente = nullptr;
					ult->siguiente->anterior = ult;
					ult = ult->siguiente;
					strcpy_s(ult->Curp, 20, lect->Curp);
					strcpy_s(ult->vacuna, 15, lect->vacuna);
					strcpy_s(ult->No_dosis, 15, lect->No_dosis);
					strcpy_s(ult->date, 15, lect->date);
					strcpy_s(ult->lote, 15, lect->lote);
					strcpy_s(ult->lugar, 15, lect->lugar);
					ult->ID_carnet = lect->ID_carnet;
					k++;
				}
			}
			k = 0;

			if (prim->Curp == "") {
				prim->siguiente = nullptr;
				prim = nullptr;
			}
			else {
				tempo = ult;
				if (tempo->siguiente == nullptr) {
					tempo->anterior->siguiente = tempo->siguiente;
					if (tempo->siguiente != nullptr) {
						tempo->siguiente->anterior = tempo->anterior;
					}
					delete tempo;
				}
			}
		}
	}

	Archivo.close();
}
void dvacio(char datop[70]) {
	if (strcmp(datop, "") == 0) {
		prueba++;
	}
}
