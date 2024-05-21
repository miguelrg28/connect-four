#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.c>

#define CHARTDIM 7

#define POSX 5
#define POSY 3

#define CT YELLOW
#define CF CYAN
#define FPCT WHITE
#define SPCF YELLOW
#define SPCT BLACK

#define DERECHA 77
#define IZQUIERDA 75
#define ENTER 13

#define VALINI 2
#define WINNER_SEQUENCE 4
#define COLOR_AMOUNT 16
#define PLAYER1 0
#define PLAYER2 1

void genMatz(int[][CHARTDIM], int, int);
int existe(int[][CHARTDIM], int, int);
void showMatzXY(int[][CHARTDIM], int, int, int, int, int, int, int);
void startGame(int[][CHARTDIM], int, int, int, int, int, int, int);
void printPosition(int, int, int);
void showTurnInfo(int, int);
int fallBlock(int[][CHARTDIM], int, int, int, int, int, int, int, int);

int verifyHWinner(int[][CHARTDIM], int, int);
int verifyVWinner(int[][CHARTDIM], int, int);
int verifyDWinner(int[][CHARTDIM], int, int);

void CPUPlay(int[][CHARTDIM], int, char *, int *);
void switchTurn(int *);

int checkMultipleWinOpportunities(int[][CHARTDIM], int, int);
int checkHorizontalOpportunities(int[][CHARTDIM], int, int, int *, int *);
int checkVerticalOpportunities(int[][CHARTDIM], int, int, int *, int *);
int checkDiagonalDownOpportunities(int[][CHARTDIM], int, int, int *, int *);
int checkDiagonalUpOpportunities(int[][CHARTDIM], int, int, int *, int *);

int getPlayOption();
int getSpeed();
int getPlayerColor(const char *[], int);

int randrange(int, int);

void setColor(int, int);
void colorDefault(void);

int main(int argc, char *argv[])
{
   int turn = PLAYER1;
   int play = 1;
   int speed = 600; // 500ms - 1000ms
   int player1_color = RED;

   const char *colors[] = {
       "BLACK",
       "BLUE",
       "GREEN",
       "CYAN",
       "RED",
       "MAGENTA",
       "BROWN",
       "LIGHTGRAY",
       "DARKGRAY",
       "LIGHTBLUE",
       "LIGHTGREEN",
       "LIGHTCYAN",
       "LIGHTRED",
       "LIGHTMAGENTA",
       "YELLOW",
       "WHITE"};

   const int esp = (int)log10(CHARTDIM * CHARTDIM) + 2;

   srand(time(NULL));

   int cuad[CHARTDIM][CHARTDIM];

   while (play == 1)
   {
      setColor(WHITE, CYAN);
      printf("%cConecta 4! \n\n", 173);
      colorDefault();

      play = getPlayOption();
      if (play == 0)
         break;

      speed = getSpeed();

      player1_color = getPlayerColor(colors, COLOR_AMOUNT);

      system("cls");

      genMatz(cuad, CHARTDIM, VALINI);
      startGame(cuad, CHARTDIM, turn, esp, speed, POSX, POSY, player1_color);
   }

   return 0;
}

/*
Función: randrange.
Argumentos:
   int liminf: Límite inferior del rango aleatorio.
   int limsup: Límite superior del rango aleatorio.
Objetivo: Obtener número aleatorio en el rango: ['liminf','limsup'].
Retorno: (int) Equivalente al n�mero aleatorio obtenido.
*/
int randrange(int liminf, int limsup)
{
   return rand() % (limsup - liminf + 1) + liminf;
}

/*
Función: genMatz.
Argumentos:
   int cuad[][CHARTDIM]: Matriz a tratar.
   int dim: Dimensiones de la matriz.
   int valini: Valor por defecto.
Objetivo: Inicializar y generar matriz del juego con el valor por defecto indicado.
Retorno: (void) Nada.
*/
void genMatz(int cuad[][CHARTDIM], int dim, int valini)
{
   int indfil, indcol;
   for (indfil = 0; indfil < dim; indfil++)
      for (indcol = 0; indcol < dim; indcol++)
         cuad[indfil][indcol] = valini;
   return;
}

/*
Función: getPlayOption.
Argumentos: int posx -> Posición en X, int posy -> Posición en Y.
Objetivo: Preguntar y captar si el usuario desea jugar o no.
Retorno: (int) Velocidad en milisegundos.
*/
int getPlayOption()
{
   int option = 0;

   printf("%cQuieres jugar? [1] Si [0] No \n", 168);
   do
   {
      scanf("%d", &option);
      if (option != 0 && option != 1)
      {
         printf("\nPor favor, introduce [1] Si o [0] No.\n");
      }
   } while (option != 0 && option != 1);

   return option;
}

/*
Función: getPlayerColor.
Argumentos: char *color[] -> Array de cadenas de texto de los nombres de los colores, int count -> Cantidad de colores.
Objetivo: Preguntar y captar el color que desea el jugador para si mismo.
Retorno: (int) Número del color.
*/
int getPlayerColor(const char *color[], int count)
{
   int colorSel = RED, i;

   printf("%cQu%c color deseas? \n", 168, 130);
   for (i = 0; i < count; i++)
   {
      setColor(WHITE, i);
      printf("%d. %s\n", i + 1, color[i]);
   }

   colorDefault();

   do
   {
      scanf("%d", &colorSel);
      if (colorSel < 0 || colorSel > colorSel)
      {
         printf("\nPor favor, introduce un valor entre 1 y %d\n", count);
      }
   } while (colorSel < 0 || colorSel > count);

   return colorSel - 1;
}

/*
Función: getSpeed.
Argumentos: Nada.
Objetivo: Preguntar y captar la velocidad por el usuario.
Retorno: (int) Velocidad en milisegundos.
*/
int getSpeed()
{
   int speed = 0;

   do
   {
      printf("%cQu%c velocidad de animaci%c desea? (milisegundos) \n", 168, 130, 162);
      scanf("%d", &speed);
      if (speed < 500 || speed > 1000)
      {
         printf("\nPor favor, introduce un valor entre 500ms y 1000ms\n");
      }
   } while (speed < 500 || speed > 1000);

   return speed;
}

/*
Función: showMatzXY.
Argumentos: int cuad[][CHARTDIM] -> Matriz de juego, int dim -> Dimensiones de matriz, int esp -> Espaciado, int selCol -> Columna seleccionada, int posx -> Posicion en el eje X, int posy -> Posicion en el eje Y, int turn -> Turno actual (0 o 1), int player1_color -> Color del jugador 1.
Objetivo: Mostrar matriz, colores asignados e información de turno.
Retorno: (void) Nada.
*/
void showMatzXY(int cuad[][CHARTDIM], int dim, int esp, int selCol, int posx, int posy, int turn, int player1_color)
{
   gotoxy(posx, posy - 2);
   showTurnInfo(turn, player1_color);

   int indfil, indcol;
   for (indfil = 0; indfil < dim; indfil++)
      for (indcol = 0; indcol < dim; indcol++)
      {
         setColor(CT, CF);

         gotoxy(posx + indcol * (esp + 1), posy + indfil);
         if (indfil == 0)
         {
            colorDefault();
            if (indcol == selCol)
            {
               printf("%*c ", esp, 118);
            }

            printf("%*c ", esp, ' ');
         }
         else
         {
            printPosition(cuad[indfil][indcol], esp, player1_color);
         }
      }
   colorDefault();
   return;
}

/*
Función: startGame.
Argumentos: int cuad[][CHARTDIM] -> Matriz de juego, int dim -> Dimensiones de matriz, int turn -> Turno actual (0 o 1), int esp -> Espaciado, int time -> Tiempo de animación, int posx -> Posicion en el eje X, int posy -> Posicion en el eje Y, int player1_color -> Color del jugador 1.
Objetivo: Mostrar matriz, colores asignados e información de turno.
Retorno: (void) Nada.
*/
void startGame(int cuad[][CHARTDIM], int dim, int turn, int esp, int time, int posx, int posy, int player1_color)
{
   char tecla;
   char area = 'c';
   int selCol = 0;
   int win = 0;
   int preWin = 0;

   _setcursortype(0);
   do
   {
      tecla = 0;
      gotoxy(posx, posy);
      showMatzXY(cuad, dim, esp, selCol, posx, posy + 2, turn, player1_color);

      if (preWin)
      {
         gotoxy(posx, posy + dim + 2);
         setColor(WHITE, GREEN);

         if (checkMultipleWinOpportunities(cuad, dim, turn) >= 2)
         {
            printf("Jugador %d ha ganado por defecto.", turn + 1);
         }
         else
         {
            printf("Jugador %d ha ganado.", turn + 1);
         }

         Sleep(3500);
         colorDefault();
         win = 1;
         system("cls");
      }

      do
      {
         if (turn == 0 && !win && !preWin)
         {
            tecla = getch();
         }

      } while (tecla != DERECHA && tecla != IZQUIERDA && tecla != ENTER && turn != 1 && win != 1);

      if (turn == PLAYER2 && !win && !preWin)
      {
         CPUPlay(cuad, dim, &tecla, &selCol);
      }

      if (tecla == ENTER)
      {
         if (fallBlock(cuad, dim, esp, selCol, turn, time, posx, posy + 2, player1_color))
         {
            preWin = getWin(cuad, dim, turn) || checkMultipleWinOpportunities(cuad, dim, turn) >= 2;

            if (!preWin)
            {
               switchTurn(&turn);
            }
         }
         system("cls");
      }
      if (tecla == IZQUIERDA)
      {
         if (selCol > 0)
         {
            selCol--;
         }
      }

      if (tecla == DERECHA)
      {
         if (selCol < dim - 1)
         {
            selCol++;
         }
      }

   } while (win != 1);
   _setcursortype(100);
}

/*
Función: showTurnInfo.
Argumentos: int turn -> Turno actual (0 o 1), int player1_color -> Color del jugador 1.
Objetivo: Mostrar graficamente la información de turno actual.
Retorno: (void) Nada.
*/
void showTurnInfo(int turn, int player1_color)
{
   printf("Turno actual: ");
   if (turn == PLAYER1)
   {
      setColor(FPCT, player1_color);
   }
   else
   {
      setColor(SPCT, SPCF);
   }

   printf("Jugador %d", turn + 1);
   colorDefault();

   return;
}

/*
Función: fallBlock.
Argumentos: int matz[][CHARTDIM], int dim -> Dimensiones de la matriz, int esp -> Espaciado, int selCol -> Columna Seleccionada, int turn -> Turno actual (0 o 1), int time -> Tiempo de animación, int posx -> Posición X, int posy -> Posición Y, int player1_color -> Color del jugador 1.
Objetivo: Dejar caer bloque en columna que el jugador de turno eligió.
Retorno: (void) Nada.
*/
int fallBlock(int matz[][CHARTDIM], int dim, int esp, int selCol, int turn, int time, int posx, int posy, int player1_color)
{
   int i;
   for (i = 1; i < dim; i++)
   {
      if (matz[i][selCol] != 2)
      {
         break;
      }
      matz[i][selCol] = turn;
      // system("cls");
      showMatzXY(matz, dim, esp, selCol, posx, posy, turn, player1_color);
      Sleep(time); // ms
      if (i < dim - 1 && matz[i + 1][selCol] == 2)
      {
         matz[i][selCol] = 2; // Restablece a espacio vacío si el siguiente espacio está vacío
      }
   }
   if (i > 1)
   {
      matz[i - 1][selCol] = turn; // Coloca la ficha en la posición final
      return 1;
   }
   else
   {
      gotoxy(posx, posy + dim + 2);
      printf("Fila llena.");
      return 0;
   }
}

/*
Función: verifyHWinner
Argumentos: int matz[][CHARTDIM], int dim -> Dimensiones de la matriz, int turn -> Turno actual (0 o 1).
Objetivo:Verificar si hay una serie horizontal y reemplazar las fichas del jugador 1 por la misma sumado a 3 que representa un resaltado para los ganadores, y lo mismo en el caso de que sea el jugador 2/cpu.
Retorno: 1 si hay una serie horizontal, 0 en caso contrario.
*/
int verifyHWinner(int matz[][CHARTDIM], int dim, int turn)
{
   int i, j, k;
   for (i = 1; i < dim; i++)
   {
      for (j = 0; j <= dim - WINNER_SEQUENCE; j++)
      {
         int winner = 1;
         for (k = 0; k < WINNER_SEQUENCE; k++)
         {
            if (matz[i][j + k] != turn)
            {
               winner = 0;
               break;
            }
         }
         if (winner)
         {
            for (k = 0; k < WINNER_SEQUENCE; k++)
            {
               matz[i][j + k] = turn + 3;
            }
            return 1;
         }
      }
   }
   return 0;
}

/*
Función: verifyVWinner
Argumentos: int matz[][CHARTDIM], int dim -> Dimensiones de la matriz, int turn -> Turno actual (0 o 1).
Objetivo:Verificar si hay una serie vertical y reemplazar las fichas del jugador 1 por la misma sumado a 3 que representa un resaltado para los ganadores, y lo mismo en el caso de que sea el jugador 2/cpu.
Retorno: 1 si hay una serie vertical, 0 en caso contrario.
*/
int verifyVWinner(int matz[][CHARTDIM], int dim, int turn)
{
   int i, j, k;
   for (i = 1; i <= dim - WINNER_SEQUENCE; i++)
   {
      for (j = 0; j < dim; j++)
      {
         int winner = 1;
         for (k = 0; k < WINNER_SEQUENCE; k++)
         {
            if (matz[i + k][j] != turn)
            {
               winner = 0;
               break;
            }
         }
         if (winner)
         {
            for (k = 0; k < WINNER_SEQUENCE; k++)
            {
               matz[i + k][j] = turn + 3;
            }

            return 1;
         }
      }
   }
   return 0;
}

/*
Función: verifDVWinner
Argumentos: int matz[][CHARTDIM], int dim -> Dimensiones de la matriz, int turn -> Turno actual (0 o 1).
Objetivo: Verificar si hay una serie diágonal hacia abajo o arriba y reemplazar las fichas del jugador 1 por la misma sumado a 3 que representa un resaltado para los ganadores, y lo mismo en el caso de que sea el jugador 2/cpu.
Retorno: 1 si hay una serie diagonal hacia abajo o arriba, 0 en caso contrario.
*/
int verifyDWinner(int matz[][CHARTDIM], int dim, int turn)
{
   int i, j, k;
   // Diagonal hacia abajo
   for (i = 1; i <= dim - WINNER_SEQUENCE; i++)
   {
      for (j = 0; j <= dim - WINNER_SEQUENCE; j++)
      {
         int winner = 1;
         for (k = 0; k < WINNER_SEQUENCE; k++)
         {
            if (matz[i + k][j + k] != turn)
            {
               winner = 0;
               break;
            }
         }
         if (winner)
         {
            for (k = 0; k < WINNER_SEQUENCE; k++)
            {
               matz[i + k][j + k] = turn + 3;
            }
            return 1;
         }
      }
   }

   int x, y, z;
   // Diagonal hacia arriba
   for (x = WINNER_SEQUENCE; x < dim; x++)
   {
      for (y = 0; y <= dim - WINNER_SEQUENCE; y++)
      {
         int winner = 1;
         for (z = 0; z < WINNER_SEQUENCE; z++)
         {
            if (matz[x - z][y + z] != turn)
            {
               winner = 0;
               break;
            }
         }
         if (winner)
         {
            for (k = 0; k < WINNER_SEQUENCE; k++)
            {
               matz[i - k][j + k] = turn + 3;
            }
            return 1;
         }
      }
   }

   return 0;
}

/*
Función: getWin.
Argumentos:  int matz[][CHARTDIM], int dim -> Dimensiones de la matriz, int turn -> Turno actual (0 o 1).
Objetivo: Verificar si se ha ganado el juego.
Retorno: (int) Retorna 1 si se ganó el juego y 0 si no se ha ganado.
*/
int getWin(int cuad[][CHARTDIM], int dim, int turn)
{
   return verifyHWinner(cuad, dim, turn) || verifyVWinner(cuad, dim, turn) || verifyDWinner(cuad, dim, turn);
}

/*
Función: switchTurn.
Argumentos: int *turn -> Turno actual (0 o 1).
Objetivo: Cambiar entre el turno del jugador 1 y 2.
Retorno: (void) Nada.
*/
void switchTurn(int *turn)
{
   if (*turn == PLAYER1)
   {
      *turn = PLAYER2;
   }
   else
   {
      *turn = PLAYER1;
   }
}

/*
Función: CPUPlay.
Argumentos: int matz[][CHARTDIM] -> Matriz, int dim -> Dimensiones de la matriz, char *tecla -> Tecla, int *selCol -> Columna Seleccionada.
Objetivo: Cambiar entre el turno del jugador 1 y 2.
Retorno: (void) Nada.
*/
void CPUPlay(int matz[][CHARTDIM], int dim, char *tecla, int *selCol)
{
   int colTo = 0;
   int rowTo = 0;

   //* Revisar chances de ganar para el CPU
   if (checkHorizontalOpportunities(matz, dim, PLAYER2, &rowTo, &colTo))
   {
      *selCol = colTo;
   }
   else if (checkVerticalOpportunities(matz, dim, PLAYER2, &rowTo, &colTo))
   {
      *selCol = colTo;
   }
   else if (checkDiagonalDownOpportunities(matz, dim, PLAYER2, &rowTo, &colTo))
   {
      *selCol = colTo;
   }
   else if (checkDiagonalUpOpportunities(matz, dim, PLAYER2, &rowTo, &colTo))
   {
      *selCol = colTo;
   }
   //* Revisar chances para atacar jugador
   else if (checkHorizontalOpportunities(matz, dim, PLAYER1, &rowTo, &colTo))
   {
      *selCol = colTo;
   }
   else if (checkVerticalOpportunities(matz, dim, PLAYER1, &rowTo, &colTo))
   {
      *selCol = colTo;
   }
   else if (checkDiagonalDownOpportunities(matz, dim, PLAYER1, &rowTo, &colTo))
   {
      *selCol = colTo;
   }
   else if (checkDiagonalUpOpportunities(matz, dim, PLAYER1, &rowTo, &colTo))
   {
      *selCol = colTo;
   }
   else
   {
      *selCol = randrange(0, dim - 1);
   }

   // Ejecutar movimiento
   *tecla = ENTER;
}

/*
Función: checkMultipleWinOpportunities.
Argumentos: int matz[][CHARTDIM] -> Matriz, int dim -> Dimensiones de la matriz, int turn -> Turno actual (0 o 1).
Objetivo: Verificar si el jugador tiene más de dos chances de ganar.
Retorno: (int) Retorna 1 si el jugador tiene más de dos chances de ganar y 0 si no.
*/
int checkMultipleWinOpportunities(int matz[][CHARTDIM], int dim, int turn)
{
   int opportunities = 0;

   int colTo = 0;
   int rowTo = 0;

   opportunities += checkHorizontalOpportunities(matz, dim, turn, &rowTo, &colTo);
   opportunities += checkVerticalOpportunities(matz, dim, turn, &rowTo, &colTo);
   opportunities += checkDiagonalDownOpportunities(matz, dim, turn, &rowTo, &colTo);
   opportunities += checkDiagonalUpOpportunities(matz, dim, turn, &rowTo, &colTo);

   return opportunities;
}

/*
Función: checkHorizontalOpportunities.
Argumentos: int matz[][CHARTDIM] -> Matriz, int dim -> Dimensiones de la matriz, int turn -> Turno actual (0 o 1), int *rowTo -> Fila donde se encuentra la ficha ganadora, int *colTo -> Columna donde se encuentra la ficha ganadora.
Objetivo: Verificar si el jugador tiene chance de ganar del lado horizontal.
Retorno: (int) Retorna 1 si el jugador tiene chance y 0 si no tiene.
*/
int checkHorizontalOpportunities(int matz[][CHARTDIM], int dim, int turn, int *rowTo, int *colTo)
{
   int count = 0;
   *rowTo = -1;
   *colTo = -1;

   int i, j, k;
   for (i = 1; i < dim; i++)
   {
      for (j = 0; j <= dim - WINNER_SEQUENCE; j++)
      {
         int potentialWin = 0;
         int emptySpaceIndex = -1;
         for (k = 0; k < WINNER_SEQUENCE; k++)
         {
            if (matz[i][j + k] == turn)
            {
               potentialWin++;
            }
            else if (matz[i][j + k] == VALINI)
            {
               emptySpaceIndex = k;
            }
         }
         if (potentialWin == WINNER_SEQUENCE - 1 && emptySpaceIndex != -1)
         {
            *rowTo = i;
            *colTo = j + emptySpaceIndex;
            count++;
         }
      }
   }

   return count;
}

/*
Función: checkVerticalOpportunities.
Argumentos: int matz[][CHARTDIM] -> Matriz, int dim -> Dimensiones de la matriz, int turn -> Turno actual (0 o 1), int *rowTo -> Fila donde se encuentra la ficha ganadora, int *colTo -> Columna donde se encuentra la ficha ganadora.
Objetivo: Verificar si el jugador tiene chance de ganar del lado vertical.
Retorno: (int) Retorna 1 si el jugador tiene chance y 0 si no tiene.
*/
int checkVerticalOpportunities(int matz[][CHARTDIM], int dim, int turn, int *rowTo, int *colTo)
{
   int count = 0;
   *rowTo = -1;
   *colTo = -1;
   int j, i, k;
   for (j = 0; j < dim; j++)
   {
      for (i = 1; i <= dim - WINNER_SEQUENCE; i++)
      {
         int potentialWin = 0;
         int emptySpaceIndex = -1;
         for (k = 0; k < WINNER_SEQUENCE; k++)
         {
            if (matz[i + k][j] == turn)
            {
               potentialWin++;
            }
            else if (matz[i + k][j] == VALINI)
            {
               emptySpaceIndex = k;
            }
         }
         if (potentialWin == WINNER_SEQUENCE - 1 && emptySpaceIndex != -1)
         {
            count++;

            if (*rowTo == -1 && *colTo == -1)
            {
               *rowTo = i + emptySpaceIndex;
               *colTo = j;
            }
         }
      }
   }
   return count;
}

/*
Función: checkDiagonalDownOpportunities.
Argumentos: int matz[][CHARTDIM] -> Matriz, int dim -> Dimensiones de la matriz, int turn -> Turno actual (0 o 1, int *rowOp -> Fila donde se encuentra la ficha ganadora, int *colOp -> Columna donde se encuentra la ficha ganadora.
Objetivo: Verificar si el jugador tiene chance de ganar en la diagonal de abajo.
Retorno: (int) Retorna 1 si el jugador tiene chance y 0 si no tiene.
*/
int checkDiagonalDownOpportunities(int matz[][CHARTDIM], int dim, int turn, int *rowOp, int *colOp)
{
   int count = 0;
   *rowOp = -1;
   *colOp = -1;

   int i, j, k;

   for (i = 1; i <= dim - WINNER_SEQUENCE; i++)
   {
      for (j = 0; j <= dim - WINNER_SEQUENCE; j++)
      {
         int potentialWin = 0;
         int emptySpaceIndex = -1;
         for (k = 0; k < WINNER_SEQUENCE; k++)
         {
            if (matz[i + k][j + k] == turn)
            {
               potentialWin++;
            }
            else if (matz[i + k][j + k] == VALINI)
            {
               emptySpaceIndex = k;
            }
         }
         if (potentialWin == WINNER_SEQUENCE - 1 && emptySpaceIndex != -1)
         {
            count++;

            if (*rowOp == -1 && *colOp == -1)
            {
               *rowOp = i + emptySpaceIndex;
               *colOp = j + emptySpaceIndex;
            }
         }
      }
   }
   return count;
}

/*
Función: checkDiagonalUpOpportunities.
Argumentos: int matz[][CHARTDIM] -> Matriz, int dim -> Dimensiones de la matriz, int turn -> Turno actual (0 o 1), int *rowOp -> Fila donde se encuentra la ficha ganadora, int *colOp -> Columna donde se encuentra la ficha ganadora.
Objetivo: Verificar si el jugador tiene chance de ganar en la diagonal de arriba.
Retorno: (int) Retorna 1 si el jugador tiene chance y 0 si no tiene.
*/
int checkDiagonalUpOpportunities(int matz[][CHARTDIM], int dim, int turn, int *rowOp, int *colOp)
{
   int count = 0;
   *rowOp = -1;
   *colOp = -1;

   int i, j, k;

   for (i = WINNER_SEQUENCE - 1; i < dim; i++)
   {
      for (j = 0; j <= dim - WINNER_SEQUENCE; j++)
      {
         int potentialWin = 0;
         int emptySpaceIndex = -1;
         for (k = 0; k < WINNER_SEQUENCE; k++)
         {
            if (matz[i - k][j + k] == turn)
            {
               potentialWin++;
            }
            else if (matz[i - k][j + k] == VALINI)
            {
               emptySpaceIndex = k;
            }
         }
         if (potentialWin == WINNER_SEQUENCE - 1 && emptySpaceIndex != -1)
         {
            count++;

            if (*rowOp == -1 && *colOp == -1)
            {
               *rowOp = i - emptySpaceIndex;
               *colOp = j + emptySpaceIndex;
            }
         }
      }
   }
   return count;
}

/*
Función: printPosition.
Argumentos:
   int number: Número a imprimir.
   int esp: Espaciado de impresión.
   int player1_color -> Color del jugador 1.
Objetivo: Impresión dinámica en posición tomando en cuenta si es una ficha del jugador 1 o 2 o vacia.
Retorno: (void) Nada.
*/
void printPosition(int number, int esp, int player1_color)
{
   // Check what color use
   if (number == PLAYER1)
   {
      setColor(FPCT, player1_color);
   }
   else if (number == PLAYER2)
   {
      setColor(SPCT, SPCF);
   }
   else if (number == PLAYER1 + 3)
   {
      setColor(player1_color, FPCT);
   }
   else if (number == PLAYER2 + 3)
   {
      setColor(SPCF, SPCT);
   }
   else
   {
      setColor(CT, CF);
   }

   if (number == PLAYER1 || number == PLAYER2 || number == PLAYER1 + 3 || number == PLAYER2 + 3)
   {
      printf("%*c ", esp, 220);
   }
   else
   {
      printf("%*c ", esp, ' ');
   }
}

int existe(int cuad[][CHARTDIM], int dim, int val)
{
   int indfil, indcol;
   for (indfil = 0; indfil < dim; indfil++)
      for (indcol = 0; indcol < dim; indcol++)
         if (cuad[indfil][indcol] == val)
            return 1;

   return 0;
}

/*
   Función: void setColor
   Argumento: (int) ct -> Color del texto, (int) cf -> Color del fondo.
   Objetivo: Asignar color de fondo y texto del programa.
   Retorno: Nada.
*/
void setColor(int ct, int cf)
{
   textbackground(cf);
   textcolor(ct);
}

/*
   Función: void colorDefault
   Argumento: Nada.
   Objetivo: Asignar los colores de texto y fondo por defecto en el programa.
   Retorno: Nada.
*/
void colorDefault()
{
   setColor(WHITE, BLACK);
}
