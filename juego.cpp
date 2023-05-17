#include<time.h>//Permite usar el time(NULL)
#include <stdio.h>//standard input output nos permiter usar el cout y cin
#include <windows.h>//nos permie usar el system
#include<conio.h>//Nos permite usar la funcion kbhit
#include<stdlib.h>//Nos permite usar el rand y srand

//teclas
#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77
using namespace std;

bool gameOver = false;

//Esta funcion es la que controla las coordenadas del objeto
void gotoxy(int x, int y){
	
	HANDLE hCon; 
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos; //Las coordenadas de posicion el objeto
	dwPos.X = x;
	dwPos.Y = y;
	
	SetConsoleCursorPosition(hCon, dwPos); //Le da una posicion a nuestro objeto para poder cambiar y asi moverlo
	
}

void ocultarCursor(){
	
	HANDLE hCon; 
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 50;//Toma valores del 1 al 100 para cambiar el tamaño del cursor
	cci.bVisible = FALSE;//Si es verdadero se mostrara el cursor, si es falso no se vera el cursor
	
	SetConsoleCursorInfo(hCon, &cci); //Nos muestra donde esta el cursor
	
}

//Creamos los limites del escenario
/*void pintarLimites(){
	//el 120 indica el largo de la pantalla
	for(int i = 2; i < 118; i++){
		//el 1 y el 29 indican que tan arriba o tan abajo van a estar las lineas
		gotoxy(i, 1);
		printf("%c", 205);
		gotoxy(i, 29);
		printf("%c", 205);
	}
	
	for(int i = 2; i < 29; i++){
		gotoxy(1, i);
		printf("%c", 186);
		gotoxy(117, i);
		printf("%c", 186);
	}
	
}*/

//Creamos el jugador
class jugador{
	
	int x, y;
	int corazones;
	int vidas;
	
	public:
		jugador(int _x, int _y, int _corazones, int _vidas): x(_x), y(_y), corazones(_corazones), vidas(_vidas){}//El guion bajo es para que no se confunda el argumento con el atributo
		//Estas tres son funciones que creamos para que el objeto bala pueda interactuar con el jugador
		int X(){return x;}
		int Y(){return y;}
		void COR(){corazones--;}
		
		void pintarCorazones();
		void pintar();
		void borrar();
		void mover();
		void morir();
	
};

void jugador::pintar(){
	
	gotoxy(x, y);
	printf("  %c%c%c%c%c", 40, 92, 95, 47,41);//El %c es para imprimir un caracter y si le agregamos un entero hacemos referencia al codifo ASCII
	//El codigo ASCII es el codigo numero del teclado
	gotoxy(x, y+1);
	printf("  %c%c %c%c", 40, 94, 94, 41);
	gotoxy(x, y+2);
	printf("  %c%c%c%c%c%c", 40, 95, 95, 95, 41, 48);
	
	
}

void jugador::borrar(){
	
	gotoxy(x, y);
	printf("          ");
	gotoxy(x, y+1);
	printf("          ");
	gotoxy(x, y+2);
	printf("          ");
	
}

void jugador::mover(){
	
	if(kbhit()){
	    	//kbhit() detecta si se presiona una tecla
	    	char tecla = getch();
	    	borrar();
	    	if(tecla == DERECHA && x+6 < 113) x ++;//Las teclas deben ponerse entre esas comillas
	    	if(tecla == IZQUIERDA && x > 2) x --;
			if(tecla == ABAJO && y+3 < 29 ) y ++;//Las coordenas Y estan invertidas por eso para bajar se suma
	    	if(tecla == ARRIBA && y > 2) y --;
	    	pintar();
	    	pintarCorazones();
		}
	
}

//Esta funcion dibuja la vida del jugador
void jugador::pintarCorazones(){
	
	gotoxy(50,1);
	printf("Vidas %d", vidas);
	gotoxy(100,1);
	printf("Salud");
	gotoxy(106,1);
	printf("     ");
	for(int i = 0; i < corazones; i++){
		gotoxy(106+i,1);
		printf("%c", 3);//3 es el codigo numerico ASCII para el corazon
	}
	
}

void jugador::morir(){
	
	if(corazones == 0){
		borrar();
		gotoxy(x, y);
	    printf("  %c%c%c%c%c", 40, 92, 95, 47,41);
		gotoxy(x, y+1);
		printf("  %c%c%c%c%c", 40, 62, 46, 60, 41);
	    gotoxy(x, y+2);
	    printf("  %c%c%c%c%c%c", 40, 95, 95, 95, 41, 48);
	    Sleep(1000);
	    borrar();
	    
	    vidas --;
	    corazones = 3;
	    pintarCorazones();
	    pintar();
	}
	if (vidas == 0){
		borrar();
		gotoxy(x, y+1);
		printf("    %c  ", 3);
		Sleep(700);
		
		gotoxy(x, y+1);
		printf("    %c  ", 42);
		Sleep(500);
		
		gameOver = true;
	}
	
}

class bala{
	
	int x, y;
	
	public:
		bala(int _x, int _y): x(_x), y(_y){}
		void pintarBala();
		void moverBalaenY();
		void moverBalaenX();
		void moverBalaenMenosY();
		void moverBalaenMenosX();
		void impacto(struct jugador &C );
	
};

void bala::pintarBala(){
	
	gotoxy(x, y);
	printf("%c", 111);
	
}

void bala::moverBalaenY(){
	
	gotoxy(x, y);
	printf(" ");
	y++;
	if( y > 28){
		x = rand()%114+2;//rand() devuelve un numero aleatorio del 0 al que le digamos
		y = 2;
	}
	
	pintarBala();
	
}

void bala::moverBalaenMenosY(){
	
	gotoxy(x, y);
	printf(" ");
	y--;
	if( y < 2){
		x = rand()%114+2;
		y = 26;
	}
	
	pintarBala();
	
}

void bala::moverBalaenX(){
	
	gotoxy(x, y);
	printf(" ");
	x++;
	if( x > 116){
		y =rand()%27+1;
		x = 2;
	}
	
	pintarBala();
	
}

void bala::moverBalaenMenosX(){
	
	gotoxy(x, y);
	printf(" ");
	x--;
	if( x < 2){
		y = rand()%27+1;
		x = 114;
	}
	
	pintarBala();
	
}

void bala::impacto(struct jugador &C){
	//Las x corresponde a las balas, el + es para indicar la posicion con respecto a la imagen del conejo, el conejo tiene trres posiciones en el eje Y y 5 en el X 
	if(x == C.X() && y == C.Y() || x == C.X()+2 && y == C.Y()+1 || x == C.X()+3 && y == C.Y()+3)
	{
		
		C.COR();
		C.borrar();
		C.pintar();
		C.pintarCorazones();
		
		moverBalaenMenosX();
		moverBalaenMenosY();
		moverBalaenX();
		moverBalaenY();
		
	}
	
}

int main(){
	
	int x, y;
	int i = 0;
	
	ocultarCursor();
	
	//pintarLimites();
	
	jugador C(10, 10, 3, 3);//coordenadas del jugador
	C.pintar();
	C.pintarCorazones();
	
	bala B1(3, 3), B2(3, 3), B3(x, y), B4(3, 115);
		//Funciona siempre que gameOver sea false
		while(!gameOver){
		
		B1.moverBalaenY(); B1.impacto(C);
		B2.moverBalaenX(); B2.impacto(C);
		B3.moverBalaenMenosY(); B3.impacto(C);
		B4.moverBalaenMenosX(); B4.impacto(C);
		
		C.morir();
		C.mover();
		Sleep(30);
		
		gotoxy(2, 2);
		printf("%d", i++);
		
		} 	
		
	system("cls");
	
	printf("GAME OVER ");
	printf("Tu puntuacion es: %d", i);
	
return 0;	
}
