#include <stdlib.h>
typedef struct Nodo{
  Elem dato;
  struct Nodo *der;
  struct Nodo *izq;
}*ApNodo;

typedef struct{
  ApNodo prim;
  ApNodo ult;
}BNodo;

typedef BNodo *Cola;

Cola nueva(){
  Cola t = (Cola)malloc(sizeof(BNodo));
  t->prim=t->ult=NULL;
  return t;
}
int esnueva(Cola c){ return c->prim==NULL && c->ult==NULL;}

Cola formarI(Cola q,Elem e){

  ApNodo t = (ApNodo)malloc(sizeof(struct Nodo));
  t->dato=e;
  t->izq=NULL;
  t->der=NULL;
  if (esnueva(q)) {
    q->prim=q->ult=t;
  }else{
    t->der=q->prim;
    q->prim->izq=t;
    q->prim=t;
  }
  return q;
}

Cola formarD(Cola q, Elem e){

  ApNodo t = (ApNodo)malloc(sizeof(struct Nodo));
  t->dato=e;
  t->izq=NULL;
  t->der=NULL;
  if (esnueva(q)) {
    q->prim=q->ult=t;
  }else{
    t->izq=q->ult;
    q->ult->der=t;
    q->ult=t;
  }
  return q;
}




Elem izquierdo(Cola q){
  return q->prim->dato;
}
Elem derecho(Cola q){
  return q->ult->dato;
}

Cola desformarI(Cola q){
  if (q->prim == q->ult) {
    return nueva();
  }else{
    q->prim = q->prim->der;
    q->prim->izq=NULL;
  }
  return q;
}
Cola desformarD(Cola q){
  if (q->prim == q->ult) {
    return nueva();
  }else{
    q->ult = q->ult->izq;
    q->ult->der=NULL;
  }
  return q;
}

void printCola(Cola c){
  ApNodo a = c->prim;
  while (a) {
    printf("-|%d|-", a->dato);
    a=a->der;
  }
  printf("\n" );
}

Cola rotaIzq(Cola q){
  return q=formarD(desformarI(q),izquierdo(q));
}

Cola rotaDer(Cola q){
  return q=formarI(desformarD(q),derecho(q));
}
