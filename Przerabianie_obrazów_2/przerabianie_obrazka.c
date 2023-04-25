#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include "Funkcje.h"

#include "struktury.h"


#define MAX 512 /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024 /* Dlugosc buforow pomocniczych */

/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */

/* Funkcja odczytywania wywołująca później inne funkcje*/


/*Funkcja zapisuje obrazek do pliku*/

int main(int argc, char ** argv) {
  /*Zmienne obrazu potrzebne, zeby je przekazywać miedzy funkcjami*/
  
  t_obraz obraz;
  t_opcje opcje;
  
  int error;

  error=przetwarzaj_opcje(argc,argv,&opcje);
  
  if (error)
    printf("Blad nr %d\n", error);
  else
    printf("Opcje poprawne\n");

    /*Wczytujemy plik*/
    czytaj((opcje.plik_we),&obraz);

    /*Wszystkie opcje */

    /*Konwersja do szarosci*/
    if(opcje.s==1)
    {
      
      zamien(&obraz,&opcje);
    }
    if(opcje.negatyw==1)
    {
      negatyw(&obraz,&opcje);
    }
     if(opcje.progowanie==1)
    {
      progowanie(&obraz,&opcje);
    }
    if(opcje.konturowanie==1)
    {
      konturowanie(&obraz,&opcje);
    }
    printf("%d",opcje.progowanie_czern);
    if(opcje.progowanie_czern==1)
    {
      progowanie_czern(&obraz,&opcje);
    }
      if(opcje.progowanie_biel==1)
    {
      progowanie_biel(&obraz,&opcje);
    }
    if(opcje.gamma==1)
    {
      kor_gamma(&obraz,&opcje);

    }
    if(opcje.zmiana_poziomow==1)
    {
      zmiana_poziomow(&obraz,&opcje);
    }
      if(opcje.rozmywanie_pion==1)
    {
      rozmywanie_pion(&obraz,&opcje);
    }
      if(opcje.rozmywanie_poziom==1)
    {
      rozmywanie_poziom(&obraz,&opcje);
    }
    if(opcje.histogram==1)
    {
      histogram(&obraz,&opcje);
    }
    tymczasowy(&obraz);

    if (opcje.wyswietlenie==1)
    {
        wyswietl(&obraz);
    }
      
    zapisz((opcje.plik_wy),&obraz);
      if (obraz.numer == '2') {
        
          for (int i = 0; i < obraz.wym_y; i++) //Zwalniamy pamięć
          {
            free(obraz.piksele[i]);
          }
          free(obraz.piksele);
        } else {
          
          for (int i = 0; i < obraz.wym_y; i++) //Zwalniamy pamięć
          {
            free(obraz.czerwony[i]);
            free(obraz.zielony[i]);
            free(obraz.niebieski[i]);
          }
          free(obraz.czerwony);
          free(obraz.zielony);
          free(obraz.niebieski);
        }
  sleep(1);
  remove("tymczasowy.pgm");
  remove("tymczasowy.ppm");
 
 
  return error;
}