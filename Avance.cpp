//JUEGO DE ASTEROIDES
//Eder Cornejo Diaz

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

using namespace std;

void gotoxy(int x, int y){
    HANDLE hCon;                            // Creamos el identificador HANDLE
    hCon = GetStdHandle(STD_OUTPUT_HANDLE); // Tomamos el control de salida de la consola guardada en la variable hCon
    COORD dwPos;                            // Estructura de coordenadas por defecto de la libreria de windows.h  x e y
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);  // funcion de la libreria windows.h que permite manipular el cursor en pantalla
}

void OcultarCursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;                //Es una estructura con 2 atributos que te permite controlar la apariencia del cursor en pantalla
    cci.dwSize = 100;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon,&cci);        // funcion de la libreria windows.h que permite manipular la informacion del cursor en pantalla
}

void pintar_limites()
{
    //horizontal
    for(int i=2; i <78;i++){
        gotoxy(i,3);printf("%c",205);
        gotoxy(i,33);printf("%c",205);
    }
    //vertical
    for(int i=4; i<33;i++){
        gotoxy(2,i);printf("%c",186);
        gotoxy(77,i);printf("%c",186);
    }
    gotoxy(2,3);printf("%c",201);
    gotoxy(2,33);printf("%c",200);
    gotoxy(77,3);printf("%c",187);
    gotoxy(77,33);printf("%c",188);
}

class NAVE
{
    private:
        int x,y;
        int corazones;
        int vidas;
    public:
        NAVE(int, int, int, int);
        int getx() {return x;}
        int gety() {return y;}
        void cor()  {corazones--;}
        void pintar();
        void borrar();
        void mover();
        void pintar_corazones();
        void morir();
};

NAVE::NAVE(int _x, int _y, int _corazones, int _vidas)
{
    x = _x;
    y = _y;
    corazones = _corazones;
    vidas = _vidas;
}

void NAVE::pintar()
{
    gotoxy(x,y);printf("  %c",30);
    gotoxy(x,y+1);printf(" %c%c%c",40,207,41);
    gotoxy(x,y+2);printf("%c%c %c%c",30,190,190,30);
}

void NAVE::borrar()
{
    gotoxy(x,y);printf("         ");
    gotoxy(x,y+1);printf("        ");
    gotoxy(x,y+2);printf("        ");
}

void NAVE::mover()
{
    if(kbhit()){                    // kbhit nos permite saber si se ah presionado una tecla
            char tecla = getch();   // guarda el valor de la tecla que se presiona a la variable tecla
            borrar();
            if(tecla==IZQUIERDA && x>3){
                x--;
            }
            if(tecla==DERECHA && x+6<77){
                x++;
            }
            if(tecla==ARRIBA && y>4){
                y--;
            }
            if(tecla==ABAJO && y+3<33){
                y++;
            }
            if(tecla=='e')
                corazones--;
            pintar();
            pintar_corazones();
        }
}

void NAVE::pintar_corazones()
{
    gotoxy(50,2);printf("VIDAS %d", vidas);
    gotoxy(64,2);printf("Salud");
    gotoxy(70,2);printf("      ");
    for(int i=0; i<corazones;i++)
    {
        gotoxy(70+i,2);printf("%c",3);

    }
}

void NAVE::morir()
{
    if(corazones==0)
    {
        borrar();
        gotoxy(x,y);printf("   **   ");
        gotoxy(x,y+1);printf("  ****  ");
        gotoxy(x,y+2);printf("   **   ");
        Sleep(200);

        borrar();
        gotoxy(x,y);printf(" * ** * ");
        gotoxy(x,y+1);printf("  ****  ");
        gotoxy(x,y+2);printf(" * ** * ");
        Sleep(200);
        borrar();
        vidas--;
        corazones = 3;
        pintar_corazones();
        pintar();
    }
}

class ASTEROIDE
{
    private:
        int x,y;
    public:
        ASTEROIDE(int _x, int _y):x(_x),y(_y){}
        void pintar();
        void mover();
        void choque(NAVE &N);
};

void ASTEROIDE::pintar()
{
    gotoxy(x,y);printf("%c",184);
}

void ASTEROIDE::mover()
{
    gotoxy(x,y);printf(" ");
    y++;
    if(y>32)
    {
        x = rand()%75+2;
        y = 4;
    }
    pintar();
}

void ASTEROIDE::choque(NAVE &N)
{
    if(x>=N.getx() && x <N.getx()+6 && y>=N.gety() &&y<=N.gety()+2)
    {
        N.cor();
        N.borrar();
        N.pintar();
        N.pintar_corazones();
        x = rand()%75+2;
        y = 4;
    }
}

class BALA
{
    int x,y;
public:
    BALA(int _x, int _y):   x(_x), y (_y){}
    void mover();
};

void BALA::mover()
{
    gotoxy(x,y);    printf(" ");
    if(y>4) y--;
    gotoxy(x,y);    printf("*");
}

int main()
{
    OcultarCursor();
    pintar_limites();
    NAVE N(35,30,3,3);
    N.pintar();
    N.pintar_corazones();

    ASTEROIDE ast1(10,4),ast2(4,8),ast3(15,10);
    list<BALA*> B;
    list<BALA*>::iterator it;

    bool game_over = false;
    while(!game_over){

        if(kbhit())
        {
            char tecla = getch();
            if(tecla == 'a')
                B.push_back(new BALA(N.getx()+2,N.gety()-1));
        }

        for(it=B.begin(); it!=B.end();it++)
        {
            (*it) -> mover();
        }

        ast1.mover();   ast1.choque(N);
        ast2.mover();   ast2.choque(N);
        ast3.mover();   ast3.choque(N);
        N.morir();
        N.mover();
        Sleep(30);                  // 30 milisegundos
    }
    return 0;
}
