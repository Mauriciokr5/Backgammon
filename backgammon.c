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
int escogeDado(int numDado,int dado1,int dado2);
void printInstrucciones();



int main(int argc, char const *argv[]) {
  Tablero q = nuevoTablero();//tablero con sus fichas colocadas respectivamente
  int capturadaPositiva = 0, capturadaNegativa = 0;
  int turno = 0, turnos = 0;//par=positivas impar=negativas
  int fichasTerminadasPositivas = 0, fichasTerminadasNegativas = 0;
  int dado1, dado2, posIni, numDado,aux,dadoF;
  printInstrucciones();
//miNota: no esta implementado que los dados den lo mismo


  while(valGanador(fichasTerminadasPositivas,fichasTerminadasNegativas)){

    turno = turnos%2;
    printf("\n////////////////////////////\tTURNO: ");
    printf(turno?"Negativas":"Positivas");
    printf("\t////////////////////////////\n");
    printf("\n\t%d Fichas capturadas\t%d Fichas Terminadas\n\n",turno?capturadaNegativa:capturadaPositiva,turno?fichasTerminadasNegativas:fichasTerminadasPositivas);
    printTablero(q);
    printf("Dado 1:\n");
    dado1 = tirarDado(dado());
    //usleep(1000000);
    printf("Dado 2:\n");
    dado2 = tirarDado(dado());

    aux=0;
    numDado=0;
    for (int i = 0; i < 2; i++) {//for de 2 para dos tiros
      int tiroDisponible = 1;
      aux=numDado;
      if (!valGanador(fichasTerminadasPositivas,fichasTerminadasNegativas)) {
        continue;
      }
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

        dadoF = escogeDado(numDado,dado1,dado2);
        if (dadoF) {//Opcion Omitir tiro
          switch (tiroDisponible=valPosFin(q, posIni, dadoF, turno, turno?fichasTerminadasNegativas:fichasTerminadasPositivas)) {
            case 1:
              q = mueveFicha(q, posIni, dadoF, turno);
            break;
            case 2:
              q = mueveFicha(q, posIni, dadoF, turno);
              printf("/////Ficha Capturada/////\n");
              turno?capturadaPositiva++:capturadaNegativa++;
            break;
            case 3:
              q = mueveFichaTerminada(q, posIni, turno);
              turno?fichasTerminadasNegativas--:fichasTerminadasPositivas++;
              printf("/////Tiene %d fichas que han terminado/////\n", turno?fichasTerminadasNegativas:fichasTerminadasPositivas);
            break;
            default:
              printf("**No se pudo completar el tiro**\n");
          }
          if (tiroDisponible) {
            if (turno){ //negativos
              if (capturadaNegativa){
                capturadaNegativa--;
                printf("/////-1 Capturada/////N\n");
              }

            }else{//positivos
              if (capturadaPositiva){
                capturadaPositiva--;
                printf("/////-1 Capturada/////P\n");
              }

            }
          }
        }else{
          printf("/////Tiro Omitido/////\n");
        }

      }while(tiroDisponible==0);

      printTablero(q);
    }

    turnos++;
  }
  printf("\n\t////////////////////////////\t HA GANADO: ");
  printf(turno?"NEGATIVAS":"POSITIVAS");
  printf("\t////////////////////////////\n");





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
  printf("\t");
  for (int i = 0; i < 12; i++) {
    rotaDer(q);
    if (izquierdo(q)>=0) {
      printf("|   %d   |", izquierdo(q));
    }else{
      printf("|  %d   |", izquierdo(q));
    }
  }
  printf("\n\t");
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
		usleep((5*i*i)/(altr*altr)*80000);
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
  if (turno) {//negativos
    if (posIni) {
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
    }else{
      for (int i = 1; i < dado; i++)
        q = rotaIzq(q);
      if (izquierdo(q)==1)
        q = formarI(desformarI(q),izquierdo(q)-2);
      else
        q = formarI(desformarI(q),izquierdo(q)-1);
      for (int i = 1; i < dado; i++)
        q = rotaDer(q);
    }

  }else{//positivas
    if (posIni) {
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
    }else{
      for (int i = 1; i < dado; i++)
        q = rotaDer(q);
      if (derecho(q)==-1)
        q = formarD(desformarD(q),derecho(q)+2);
      else
        q = formarD(desformarD(q),derecho(q)+1);
      for (int i = 1; i < dado; i++)
        q = rotaIzq(q);
    }

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

      }else{
        printf("**Hay mas de una ficha contraria en la posicion final**\n");
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
      }else{
        printf("**Hay mas de una ficha contraria en la posicion final**\n");
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
  if (fichasTerminadasPositivas==15||fichasTerminadasNegativas==-15) {
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
  if (numDado<3&&numDado>=0) {
    if (numDado!=aux){
      return 0;
    }else{
      if (numDado==0&&aux==0)
        return 0;
      printf("**No se puede usar el mismo numero de dado 2 veces**\n");
    }


  }else
    printf("**Solo se puede escoger 1 o 2   [0 para omitir tiro]**\n");
  return 1;
}

int escogeDado(int numDado,int dado1,int dado2){
  if (numDado) {
    if (numDado==1)
      return dado1;
    else
      return dado2;
  }else
    return 0;

}

void printInstrucciones(){
  printf("\n////////////////////////////\tINSTRUCCIONES\t////////////////////////////\n");
  printf(" Para hacer un movimiento se necesita la posicion de la ficha que se quiere mover y \n el numero del dado que quiere aplicar (cada dado solo se puede usar una vez).\n");
  printf("\n\tPosiciones de tablero fichas NEGATIVAS\n" );
  printf("\t|   12   ||   11   ||   10   ||    9   ||    8   ||    7   ||    6   ||    5   ||    4   ||    3   ||    2   ||    1   |\n");
  printf("\t|   13   ||   14   ||   15   ||   16   ||   17   ||   18   ||   19   ||   20   ||   21   ||   22   ||   23   ||   24   |\n");

  printf("\n\tPosiciones de tablero fichas POSITIVAS\n" );
  printf("\t|   13   ||   14   ||   15   ||   16   ||   17   ||   18   ||   19   ||   20   ||   21   ||   22   ||   23   ||   24   |\n");
  printf("\t|   12   ||   11   ||   10   ||    9   ||    8   ||    7   ||    6   ||    5   ||    4   ||    3   ||    2   ||    1   |\n");
}
