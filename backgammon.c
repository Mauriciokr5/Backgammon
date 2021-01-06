#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "Elem.h"
#include "biCola.h"
#include "Circular.h"

typedef Cola Tablero;

Tablero nuevoTablero();
void printTablero(Tablero q);
Elem tirarDado(Circular c);
Circular dado();
Tablero mueveFicha(Tablero q, int posIni, int dado, int turno);
Tablero mueveFichaTerminada(Tablero q, int posIni, int turno);
int valPosIni(Tablero q, int posIni, int turno);
int valPosFin(Tablero q, int posIni, int dado, int turno, int fichasTerminadas);
int valCuadrante(Tablero q, int turno, int fichasTerminadas);
int valGanador(int fichasTerminadasPositivas,int fichasTerminadasNegativas);
int hayCapturadas(int turno, int capturadaNegativa, int capturadaPositiva);
int valDado(int numDado,int aux);



int main(int argc, char const *argv[]) {
  Tablero q = nuevoTablero();//tablero con sus fichas colocadas respectivamente
  int capturadaPositiva = 0, capturadaNegativa = 0;
  int turno, turnos = 0;//par=positivas impar=negativas
  int fichasTerminadasPositivas = 0, fichasTerminadasNegativas = 0;
  int dados[2];
  int dado1, dado2, posIni, numDado=0;

//no esta implementado que los dados den lo mismo
  while(valGanador(fichasTerminadasPositivas,fichasTerminadasNegativas)){

    turno = turnos%2;
    printf("\n\t////////////////////////////\tTURNO: ");
    printf(turno?"Negativas":"Positivas");
    printf("\t////////////////////////////\n");
    printf("\n Tiene %d fichas capturadas\n\n",turno?capturadaNegativa:capturadaPositiva);
    printTablero(q);
    printf("Dado 1:\n");
    dado1 = tirarDado(dado());
    //printf("%d\n", dado1);
    printf("Dado 2:\n");
    dado2 = tirarDado(dado());
    //printf("%d\n", dado2);

    int aux=0;
    for (int i = 0; i < 2; i++) {
      int tiroDisponible = 1;
      aux=numDado;
      do {

        if (hayCapturadas(turno,capturadaNegativa,capturadaPositiva)) {
          printf("Tiene fichas capturadas\n");
          posIni=0;
        }else{
          do{
            printf("Ingrese posicion de la ficha que quiere mover:\n");
            scanf("%d",&posIni);
          }while(!valPosIni(q, posIni, turno));

        }
        do{
          printf("Escoge numero de dado\n");
          scanf("%d", &numDado);
        }while(valDado(numDado,aux));


        switch (tiroDisponible=valPosFin(q, posIni, (numDado==1)?dado1:dado2, turno, turno?fichasTerminadasNegativas:fichasTerminadasPositivas)) {
          case 1:
            q = mueveFicha(q, posIni, dado1, turno);
          break;
          case 2:
            q = mueveFicha(q, posIni, (numDado==1)?dado1:dado2, turno);
            turno?capturadaPositiva++:capturadaNegativa++;
          break;
          case 3:
            q = mueveFichaTerminada(q, posIni, turno);
          break;
          default:
            printf("**No se pudo completar el tiro**\n");
        }
      }while(tiroDisponible==0);
      if (turno) //negativos
        if (capturadaNegativa)
          capturadaNegativa--;
      else//positivos
        if (capturadaPositiva)
          capturadaPositiva--;





      printTablero(q);

    }



    usleep(100000);



    turnos++;
  }






  return 0;
}

Tablero nuevoTablero(){
  //blancas=negativos     negras=positivos
  Tablero q = nueva();
  for (int i = 1; i <= 24; i++) {
    switch (i) {
      case 1:
        q=formarD(q,-2);
      break;
      case 6:
        q=formarD(q,5);
      break;
      case 8:
        q=formarD(q,3);
      break;
      case 12:
        q=formarD(q,-5);
      break;
      case 13:
        q=formarD(q,5);
      break;
      case 17:
        q=formarD(q,-3);
      break;
      case 19:
        q=formarD(q,-5);
      break;
      case 24:
        q=formarD(q,2);
      break;
      default:
        q=formarD(q,0);

    }
  }
  return q;
}

void printTablero(Tablero q){
  for (int i = 0; i < 12; i++) {
    rotaIzq(q);
  }
  for (int i = 0; i < 12; i++) {
    rotaDer(q);
    if (izquierdo(q)>=0) {
      printf("|   %d   |", izquierdo(q));
    }else{
      printf("|  %d   |", izquierdo(q));
    }
  }
  printf("\n");
  for (int i = 0; i < 12; i++) {
    rotaIzq(q);
  }
  for (int i = 0; i < 12; i++) {
    if (izquierdo(q)>=0) {
      printf("|   %d   |", izquierdo(q));
    }else{
      printf("|  %d   |", izquierdo(q));
    }
    rotaIzq(q);

  }
  printf("\n\n");
}

Elem tirarDado(Circular c){
	srand (time(NULL));
	int altr =rand() % 100;
	for (int i = 0; i < altr; i++) {
		ImpElemDado(top(c));
		c =rotar(c);
		fflush(stdout);
		usleep((5*i*i)/(altr*altr)*100000);
		printf("  \r ");
	}
	ImpElemDado(top(c));
	printf("\n");
	return top(c);
}

Circular dado(){
	Circular c = empty();
	for (int i = 1; i <=6; i++) {
		c = push(i,c);
	}
	return c;
}

Tablero mueveFicha(Tablero q, int posIni, int dado, int turno){
  printf("dado %d\n", dado);
  if (turno) {//negativos
    for (int i = 1; i < posIni; i++)
      q = rotaIzq(q);
    q = formarI(desformarI(q),izquierdo(q)+1);
    for (int i = 0; i < dado; i++)
      q = rotaIzq(q);
    if (izquierdo(q)==1)
      q = formarI(desformarI(q),izquierdo(q)-2);
    else
      q = formarI(desformarI(q),izquierdo(q)-1);
    for (int i = 1; i < posIni+dado; i++)
      q = rotaDer(q);
  }else{//positivas
    for (int i = 1; i < posIni; i++)
      q = rotaDer(q);
    q = formarD(desformarD(q),derecho(q)-1);
    for (int i = 0; i < dado; i++)
      q = rotaDer(q);
    if (derecho(q)==-1)
      q = formarD(desformarD(q),derecho(q)+2);
    else
      q = formarD(desformarD(q),derecho(q)+1);
    for (int i = 1; i < posIni+dado; i++)
      q = rotaIzq(q);
  }

  return q;

}

Tablero mueveFichaTerminada(Tablero q, int posIni, int turno){
  if (turno) {//negativos
    for (int i = 1; i < posIni; i++)
      q = rotaIzq(q);
    q = formarI(desformarI(q),izquierdo(q)+1);
    for (int i = 1; i < posIni; i++)
      q = rotaDer(q);
  }else{//positivas
    for (int i = 1; i < posIni; i++)
      q = rotaDer(q);
    q = formarD(desformarD(q),derecho(q)-1);
    for (int i = 1; i < posIni; i++)
      q = rotaIzq(q);
  }
  return q;
}

int valPosIni(Tablero q, int posIni, int turno){
  int flag = 0;
  if (turno) {//negativas
    for (int i = 1; i < posIni; i++)
      rotaIzq(q);
    if (izquierdo(q)<0)
      flag=1;
    for (int i = 1; i < posIni; i++)
      rotaDer(q);

  }else{//positivas
    for (int i = 1; i < posIni; i++)
      rotaDer(q);
    if (derecho(q)>0)
      flag=1;
    for (int i = 1; i < posIni; i++)
      rotaIzq(q);
  }
  return flag;
}

int valPosFin(Tablero q, int posIni, int dado, int turno,int fichasTerminadas){
  int movimientoT = posIni+dado;
  int flag = 0;
  if (movimientoT<=24) {
    if (turno) {//negativas
      for (int i = 1; i < movimientoT; i++)
        rotaIzq(q);
      if (izquierdo(q)<=1){
        if (izquierdo(q)==1)
          flag=2;//posible ficha positiva atrapada
        else
          flag=1;

      }
      for (int i = 1; i < movimientoT; i++)
        rotaDer(q);

    }else{//positivas
      for (int i = 1; i < movimientoT; i++)
        rotaDer(q);
      if (derecho(q)>=-1){
        if (derecho(q)==-1)
          flag=2;//posible ficha negativa atrapada
        else
          flag=1;
      }
      for (int i = 1; i < movimientoT; i++)
        rotaIzq(q);
    }

  }else{
    if (valCuadrante(q, turno, fichasTerminadas))
      flag=3;

  }
  return flag;
}

int valCuadrante(Tablero q, int turno, int fichasTerminadas){
  int suma = fichasTerminadas;
  if (turno) {//negativas
    for (int i = 0; i < 6; i++) {
      if (derecho(q)<0)
        suma+=derecho(q);
      rotaDer(q);
    }
    for (int i = 0; i < 6; i++)
      rotaIzq(q);

  }else{//positivas
    for (int i = 0; i < 6; i++) {
      if (izquierdo(q)>0)
        suma+=izquierdo(q);
      rotaIzq(q);
    }
    for (int i = 0; i < 6; i++)
      rotaDer(q);
  }
  if (suma==-15||suma==15) {
    return 1;
  }else{
    printf("**Aun no estan todas tus fichas en ultimo cuadrante**\n");
  }
  return 0;
}

int valGanador(int fichasTerminadasPositivas,int fichasTerminadasNegativas){
  if (fichasTerminadasPositivas==15) {
    printf("Ha ganado positivas\n");
    return 0;
  }
  if (fichasTerminadasNegativas==-15) {
    printf("Ha ganado negativas\n");
    return 0;
  }
  return 1;
}

int hayCapturadas(int turno, int capturadaNegativa, int capturadaPositiva){
  if (turno) {//negativas
    if (capturadaNegativa) {
      return 1;
    }
  }else{//positivas
    if (capturadaPositiva) {
      return 1;
    }
  }
  return 0;
}

int valDado(int numDado,int aux){
  if (numDado<3&&numDado>0) {
    if (numDado!=aux)
      return 0;
    else
      printf("**No se puede usar el mismo numero de dado 2 veces**\n");

  }else
    printf("**Solo se puede escoger 1 o 2**\n");
  return 1;
}

int hacerTiro(){

}
