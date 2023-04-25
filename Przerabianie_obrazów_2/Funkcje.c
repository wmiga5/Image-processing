/********************************************************************/
/*                                                                  */
/* ALTERNATYWNA DO PRZEDSTAWIONEJ NA WYKLADZIE WERSJA OPRACOWANIA   */
/* PARAMETROW WYWOLANIA PROGRAMU UWZGLEDNIAJACA OPCJE Z PARAMETRAMI */
/* Z ODPOWIEDNIO ZAPROPONOWANYMI STRUKTURAMI DANYCH PRZEKAZUJACYMI  */
/* WCZYTANE USTAWIENIA                                              */
/*                                    COPYRIGHT (c) 2007-2020 KCiR  */
/*                                                                  */
/* Autorzy udzielaja kazdemu prawa do kopiowania tego programu      */
/* w calosci lub czesci i wykorzystania go w dowolnym celu, pod     */
/* warunkiem zacytowania zrodla                                     */
/*                                                                  */
/********************************************************************/
#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include "Funkcje.h"

#include "struktury.h"

#include <string.h>

#include <math.h>

#define W_OK 0 /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA - 1 /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY - 2
#define B_BRAKWARTOSCI - 3
#define B_BRAKPLIKU - 4
#define MAX 512 /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024

void wyswietl(t_obraz *obraz) {
   char polecenie[DL_LINII];
  if(obraz->numer=='2')
  {
      /* bufor pomocniczy do zestawienia polecenia */
      strcpy(polecenie, "display "); /* konstrukcja polecenia postaci */
  strcat(polecenie,"tymczasowy.pgm" ); /* display "nazwa_pliku" &       */
  strcat(polecenie, " &");
  printf("%s\n", polecenie); /* wydruk kontrolny polecenia */
  system(polecenie); /* wykonanie polecenia        */
   
  
  }
  else 
  {
      ; /* bufor pomocniczy do zestawienia polecenia */
      strcpy(polecenie, "display "); /* konstrukcja polecenia postaci */
  strcat(polecenie, "tymczasowy.ppm"); /* display "nazwa_pliku" &       */
  strcat(polecenie, " &");
  printf("%s\n", polecenie); /* wydruk kontrolny polecenia */
  system(polecenie); /* wykonanie polecenia        */
  
  }
 

  
}

int czytaj(FILE * plik_we, t_obraz * obraz) {
  char buf[DL_LINII]; /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak; /* zmienna pomocnicza do czytania komentarzy */
  int koniec = 0; /* czy napotkano koniec danych w pliku */
  int i, j;

  /*Sprawdzenie czy podano prawid�owy uchwyt pliku */
  if (plik_we == NULL) {
    fprintf(stderr, "Blad: Nie podano uchwytu do pliku\n");
    return (0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien by� P2 */
  if (fgets(buf, DL_LINII, plik_we) == NULL) {
    /* Wczytanie pierwszej linii pliku do bufora */
    koniec = 1; /* Nie udalo sie? Koniec danych! */
  }
  /* Czy jest magiczne "P2"? */

  if (((buf[0] != 'P' && buf[1] != '2')) || ((buf[0] != 'P') && (buf[1] != '3')) || koniec) {
    fprintf(stderr, "Blad: To nie jest plik PGM\n");
    return (0);
  }
  obraz -> numer = buf[1];
  /* Pominiecie komentarzy */
  do {
    if ((znak = fgetc(plik_we)) == '#') /* Czy linia rozpoczyna sie od znaku '#'? */ {
      if (fgets(buf, DL_LINII, plik_we) == NULL) /* Przeczytaj ja do bufora                */
        koniec = 1; /* Zapamietaj ewentualny koniec danych */
    } else {
      ungetc(znak, plik_we); /* Gdy przeczytany znak z poczatku linii */
    } /* nie jest '#' zwroc go                 */
  } while (znak == '#' && !koniec); /* Powtarzaj dopoki sa linie komentarza */
  /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we, "%d %d %d", & (obraz -> wym_x), & (obraz -> wym_y), & (obraz -> odcieni)) != 3) {
    fprintf(stderr, "Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return (0);
  }

  /*Zależnie od tego jaki format obrazu tak alokujemy pamięć*/
  if (obraz -> numer == '2') {
    obraz -> piksele = (int ** ) malloc(obraz -> wym_y * sizeof(int * )); /*na wiersz */
    for (int i = 0; i < obraz -> wym_y; i++) {
      obraz -> piksele[i] = (int * ) malloc(obraz -> wym_x * sizeof(int)); /*na kolumny */
    }
    /*Wczytujemy piksele*/
    for (int i = 0; i < obraz -> wym_y; i++) {
      for (int j = 0; j < obraz -> wym_x; j++) {
        if (fscanf(plik_we, "%d", & (obraz -> piksele[i][j])) != 1) {
          fprintf(stderr, "Blad: Niewlasciwe wymiary obrazu\n");
          return (0);
        }

      }

    }

  } else if (obraz -> numer == '3') {
    obraz -> czerwony = (int ** ) malloc(obraz -> wym_y * sizeof(int * ));
    obraz -> zielony = (int ** ) malloc(obraz -> wym_y * sizeof(int * ));
    obraz -> niebieski = (int ** ) malloc(obraz -> wym_y * sizeof(int * ));
    for (int i = 0; i < obraz -> wym_y; i++) {
      obraz -> czerwony[i] = (int * ) malloc(obraz -> wym_x * sizeof(int)); /*na kolumny */
      obraz -> zielony[i] = (int * ) malloc(obraz -> wym_x * sizeof(int));
      obraz -> niebieski[i] = (int * ) malloc(obraz -> wym_x * sizeof(int));
    }
    /* Sprawdz czy udalo sie przydzielic pamiec na szerokosc i wysokosc dla P3 */
    if ((obraz -> czerwony != NULL) && (obraz -> zielony != NULL) && (obraz -> niebieski != NULL)) {
      fprintf(stderr, "Udalo ci sie wczytac obraz typu PPM.\nZostal on zapisany jako obraz PGM w celu poprawnej obslugi.\n");
    } else {
      fprintf(stderr, "Cos sie nie udalo \n");
    }

    /*Wczytujemy piksele*/
    /* for (int i = 0; i < obraz -> wym_y; i++) {
       for (int j = 0; j < obraz -> wym_x; j++)
        { printf("test\n");
         if (fscanf(plik_we, "%d %d %d", & (obraz -> czerwony[i][j]), & (obraz -> zielony[i][j]), & (obraz -> niebieski[i][j])) != 1) 
         {
           fprintf(stderr, "Blad: Niewlasciwe wymiary obrazu\n");
           return (0);
         }

       }

     }*/
    /* wypelnienie tablicy obrazem z P3 */
    for (i = 0; i < obraz -> wym_y; i++) {
      for (j = 0; j < obraz -> wym_x; j++) {
        fscanf(plik_we, "%d %d %d", & (obraz -> czerwony[i][j]), & (obraz -> zielony[i][j]), & (obraz -> niebieski[i][j]));
      }
    }

  }

  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/

  return 1; /* Czytanie zakonczone sukcesem    */
} /* Zwroc liczbe wczytanych pikseli */

void odczytywanie(t_obraz * obraz, int * czy) {

  /************************************************************************************
   * Funkcja wczytuje obraz PGM z pliku do tablicy       	       	       	       	    *
   *										    *
   * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM			    *
   * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz		    *
   * \param[out] wymx szerokosc obrazka						    *
   * \param[out] wymy wysokosc obrazka						    *
   * \param[out] szarosci liczba odcieni szarosci					    *
   * \return liczba wczytanych pikseli						    *
   ************************************************************************************/

  int odczytano = 0;
  FILE * plik;
  char nazwa[100];

  /* Wczytanie zawartosci wskazanego pliku do pamieci */
  printf("Podaj nazwe pliku:\n");
  scanf("%s", nazwa);
  plik = fopen(nazwa, "r");

  if (plik != NULL) {
    /* co spowoduje zakomentowanie tego warunku */
    odczytano = czytaj(plik, obraz);

    fclose(plik);

  }

  /* Wyswietlenie poprawnie wczytanego obraza zewnetrznym programem */
  if (odczytano != 0) {
   // wyswietl(nazwa);
    * czy = 1;
  } else {
    printf("Plik nie istnieje.\n");
    * czy = 0;
  }

}

void zapisz(FILE * plik_wy, t_obraz * obraz) {

  //char nazwa[100];

  int help, linia = 0;
  /* printf("Pod jaką nazwą chcesz zapisać swój wczytany obrazek?\n"); Pobieramy od uzytkownika nazwe pliku 
   scanf("%s", nazwa);*/

  /*Zapis dla pgm */

  if (obraz -> numer == '2') {

    fprintf(plik_wy, "P2\n");
    fprintf(plik_wy, "%d %d \n", obraz -> wym_x, obraz -> wym_y);
    fprintf(plik_wy, "%d \n", obraz -> odcieni);

    /*Wpisujemy do pliku wartosc z tabeli z odczytanym obrazem*/

    for (int i = 0; i < obraz -> wym_y; i++) {
      for (int j = 0; j < obraz -> wym_x; j++) {
        fprintf(plik_wy, "%d\t ", obraz -> piksele[i][j]);
      }
      fprintf(plik_wy, "\n");
    }

  }
  /*Zapis dla ppm */
  else if (obraz -> numer == '3') {
    printf("3\n");
    fprintf(plik_wy, "P3\n");
    fprintf(plik_wy, "%d %d \n", obraz -> wym_x, obraz -> wym_y);
    fprintf(plik_wy, "%d \n", obraz -> odcieni);

    for (int i = 0; i < obraz -> wym_y; i++) {
      for (int j = 0; j < obraz -> wym_x; j++) {
        fprintf(plik_wy, "%d\t ", obraz -> czerwony[i][j]);
        fprintf(plik_wy, "%d\t ", obraz -> zielony[i][j]);
        fprintf(plik_wy, "%d\t ", obraz -> niebieski[i][j]);
      }
      fprintf(plik_wy, "\n");
    }

  }

  fclose(plik_wy);
}

void zamien(t_obraz * obraz, t_opcje * opcje) {
  obraz -> numer = '2';
  obraz -> piksele = (int ** ) malloc(obraz -> wym_y * sizeof(int * )); /*na wiersz */
  for (int i = 0; i < obraz -> wym_y; i++) {
    obraz -> piksele[i] = (int * ) malloc(obraz -> wym_x * sizeof(int)); /*na kolumny */
  }

  for (int i = 0; i < obraz -> wym_y; i++) {
    for (int j = 0; j < obraz -> wym_x; j++) {
      obraz -> piksele[i][j] = (((obraz -> czerwony[i][j]) + (obraz -> zielony[i][j]) + (obraz -> niebieski[i][j])) / 3);
    }
  }
  //Zwalniamy nadmiarową pamięć
  for (int i = 0; i < obraz -> wym_y; i++) {
    free(obraz -> czerwony[i]);
    free(obraz -> zielony[i]);
    free(obraz -> niebieski[i]);
  }
  free(obraz -> czerwony);
  free(obraz -> zielony);
  free(obraz -> niebieski);
}

/*Funkcja przerabia obrazek na negatyw*/
void negatyw(t_obraz * obraz, t_opcje * opcje) {

  int help;
  /*Negatyw dla pgm*/
  if (obraz -> numer == '2') {
    for (int a = 0; a < obraz -> wym_y; a++) {
      for (int b = 0; b < obraz -> wym_x; b++) {
        help = obraz -> odcieni - obraz -> piksele[a][b]; /*Uzywamy operacji negatywu*/
        obraz -> piksele[a][b] = help;

      }
    }
  }
  /*Negatyw dla ppm*/
  else {

    for (int a = 0; a < obraz -> wym_y; a++) {
      for (int b = 0; b < obraz -> wym_x; b++) {
        if (opcje -> c == 1) {
          obraz -> czerwony[a][b] = obraz -> odcieni - obraz -> czerwony[a][b];
        } else if (opcje -> z == 1) {
          obraz -> zielony[a][b] = obraz -> odcieni - obraz -> zielony[a][b];
        } else if (opcje -> n == 1) {
          obraz -> niebieski[a][b] = obraz -> odcieni - obraz -> niebieski[a][b];
        } else {
          obraz -> czerwony[a][b] = obraz -> odcieni - obraz -> czerwony[a][b];
          obraz -> zielony[a][b] = obraz -> odcieni - obraz -> zielony[a][b];
          obraz -> niebieski[a][b] = obraz -> odcieni - obraz -> niebieski[a][b];
        }

      }
    }
  }

  printf("Negatyw zakonczony sukcesem.\n");
}
/*Funckja używa mechanizmu progowania*/
void progowanie(t_obraz * obraz, t_opcje * opcje) {

  int help;
  if (obraz -> numer == '2') {

    for (int a = 0; a < obraz -> wym_y; a++) {
      for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
        help = obraz -> piksele[a][b];
        if (help > (opcje -> w_progu)*obraz->odcieni) {
          obraz -> piksele[a][b] = obraz -> odcieni;
        } else {
          obraz -> piksele[a][b] = 0;
        }

      }
    }
  } else {

    if (opcje -> c == 1) {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> czerwony[a][b];

          if (help > (opcje -> w_progu)*obraz->odcieni) {
            obraz -> czerwony[a][b] = obraz -> odcieni;

          } else {
            obraz -> czerwony[a][b] = 0;

          }

        }
      }
    } else if (opcje -> z == 1) {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> zielony[a][b];

          if (help > (opcje -> w_progu)*obraz->odcieni) {
            obraz -> zielony[a][b] = obraz -> odcieni;

          } else {
            obraz -> zielony[a][b] = 0;

          }

        }
      }
    } else if (opcje -> n == 1) {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> niebieski[a][b];

          if (help > (opcje -> w_progu)*obraz->odcieni) {
            obraz -> niebieski[a][b] = obraz -> odcieni;

          } else {
            obraz -> niebieski[a][b] = 0;

          }

        }
      }
    } else { //Dla czerwonego
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> czerwony[a][b];

          if (help > (opcje -> w_progu)*obraz->odcieni) {
            obraz -> czerwony[a][b] = obraz -> odcieni;

          } else {
            obraz -> czerwony[a][b] = 0;

          }

        }
      }
      //Dla zielonego
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> zielony[a][b];

          if (help >(opcje -> w_progu)*obraz->odcieni) {
            obraz -> zielony[a][b] = obraz -> odcieni;

          } else {
            obraz -> zielony[a][b] = 0;

          }

        }
      }
      //Dla niebieskiego
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> niebieski[a][b];

          if (help > (opcje -> w_progu)*obraz->odcieni) {
            obraz -> niebieski[a][b] = obraz -> odcieni;

          } else {
            obraz -> niebieski[a][b] = 0;

          }

        }
      }
    }

  }
}

void progowanie_czern(t_obraz * obraz, t_opcje * opcje) {

  int help;
  if (obraz -> numer == '2') {

    for (int a = 0; a < obraz -> wym_y; a++) {
      for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
        help = obraz -> piksele[a][b];
        if (help > (opcje -> w_progu_czern) * (obraz -> odcieni)) {

        } else {
          obraz -> piksele[a][b] = 0;
        }

      }
    }
  } else {

    if (opcje -> c == 1) {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> czerwony[a][b];

          if (help > (opcje -> w_progu_czern) * (obraz -> odcieni)) {

          } else {
            obraz -> czerwony[a][b] = 0;

          }

        }
      }
    } else if (opcje -> z == 1) {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> zielony[a][b];

          if (help > (opcje -> w_progu_czern) * (obraz -> odcieni)) {

          } else {
            obraz -> zielony[a][b] = 0;

          }

        }
      }
    } else if (opcje -> n == 1) {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> niebieski[a][b];

          if (help > (opcje -> w_progu_czern) * (obraz -> odcieni)) {

          } else {
            obraz -> niebieski[a][b] = 0;

          }

        }
      }
    } else { //Dla czerwonego
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> czerwony[a][b];

          if (help > (opcje -> w_progu_czern) * (obraz -> odcieni)) {

          } else {
            obraz -> czerwony[a][b] = 0;

          }

        }
      }
      //Dla zielonego
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> zielony[a][b];

          if (help > (opcje -> w_progu_czern) * (obraz -> odcieni)) {

          } else {
            obraz -> zielony[a][b] = 0;

          }

        }
      }
      //Dla niebieskiego
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> niebieski[a][b];

          if (help > (opcje -> w_progu_czern) * (obraz -> odcieni)) {

          } else {
            obraz -> niebieski[a][b] = 0;

          }

        }
      }
    }

  }
}

void progowanie_biel(t_obraz * obraz, t_opcje * opcje) {

  int help;
  if (obraz -> numer == '2') {
    printf("OSA %f\n", opcje -> w_progu_biel);
    for (int a = 0; a < obraz -> wym_y; a++) {
      for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
        help = obraz -> piksele[a][b];
        if (help > (opcje -> w_progu_biel) * (obraz -> odcieni)) {
          obraz -> piksele[a][b] = obraz -> odcieni;
        }

      }
    }
  } else {

    if (opcje -> c == 1) {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> czerwony[a][b];

          if (help > (opcje -> w_progu_biel) * (obraz -> odcieni)) {
            obraz -> czerwony[a][b] = obraz -> odcieni;

          }

        }
      }
    } else if (opcje -> z == 1) {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> zielony[a][b];

          if (help > (opcje -> w_progu_biel) * (obraz -> odcieni)) {
            obraz -> zielony[a][b] = obraz -> odcieni;

          }

        }
      }
    } else if (opcje -> n == 1) {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> niebieski[a][b];

          if (help > (opcje -> w_progu_biel) * (obraz -> odcieni)) {
            obraz -> niebieski[a][b] = obraz -> odcieni;

          }

        }
      }
    } else { //Dla czerwonego
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> czerwony[a][b];

          if (help > (opcje -> w_progu_biel) * (obraz -> odcieni)) {
            obraz -> czerwony[a][b] = obraz -> odcieni;

          }

        }
      }
      //Dla zielonego
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> zielony[a][b];

          if (help > (opcje -> w_progu_biel) * (obraz -> odcieni)) {
            obraz -> zielony[a][b] = obraz -> odcieni;

          }

        }
      }
      //Dla niebieskiego
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) /*Warunki progowania*/ {
          help = obraz -> niebieski[a][b];

          if (help > (opcje -> w_progu_biel) * (obraz -> odcieni)) {
            obraz -> niebieski[a][b] = obraz -> odcieni;

          }

        }
      }
    }

  }
}

/*Funkcja zmienia poziomy kolorow*/
void zmiana_poziomow(t_obraz * obraz, t_opcje * opcje) {
  double czern, biel;
  int help;
  czern = opcje -> w_zmiany_c * obraz -> odcieni;
  biel = opcje -> w_zmiany_b * obraz -> odcieni;
  printf("%f\n%f\n", czern, biel);
  //Dla pgm
  if (obraz -> numer == '2') {
    for (int a = 0; a < obraz -> wym_y; a++) {
      for (int b = 0; b < obraz -> wym_x; b++) {
        help = obraz -> piksele[a][b];
        if (help >= biel) {
          obraz -> piksele[a][b] = obraz -> odcieni;
        } else if (help <= czern) {
          obraz -> piksele[a][b] = 0;
        } else {
          obraz -> piksele[a][b] = (help - czern) * (obraz -> odcieni / (biel - czern));
        }

      }
    }
  }
  //Dla ppm
  else {
    if (opcje -> c == 1) //Sam czerwony
    {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) {
          help = obraz -> czerwony[a][b];
          if (help >= biel) {
            obraz -> czerwony[a][b] = obraz -> odcieni;
          } else if (help <= czern) {
            obraz -> czerwony[a][b] = 0;
          } else {
            obraz -> czerwony[a][b] = (help - czern) * (obraz -> odcieni / (biel - czern));
          }

        }
      }
    } else if (opcje -> z == 1) //Sam zielony
    {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) {
          help = obraz -> zielony[a][b];
          if (help >= biel) {
            obraz -> zielony[a][b] = obraz -> odcieni;
          } else if (help <= czern) {
            obraz -> zielony[a][b] = 0;
          } else {
            obraz -> zielony[a][b] = (help - czern) * (obraz -> odcieni / (biel - czern));
          }

        }
      }
    } else if (opcje -> n == 1) //Sam niebieski
    {
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) {
          help = obraz -> niebieski[a][b];
          if (help >= biel) {
            obraz -> niebieski[a][b] = obraz -> odcieni;
          } else if (help <= czern) {
            obraz -> niebieski[a][b] = 0;
          } else {
            obraz -> niebieski[a][b] = (help - czern) * (obraz -> odcieni / (biel - czern));
          }

        }
      }
    } else { //Czerwony
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) {
          help = obraz -> czerwony[a][b];
          if (help >= biel) {
            obraz -> czerwony[a][b] = obraz -> odcieni;
          } else if (help <= czern) {
            obraz -> czerwony[a][b] = 0;
          } else {
            obraz -> czerwony[a][b] = (help - czern) * (obraz -> odcieni / (biel - czern));
          }

        }
      } //Zielony
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) {
          help = obraz -> zielony[a][b];
          if (help >= biel) {
            obraz -> zielony[a][b] = obraz -> odcieni;
          } else if (help <= czern) {
            obraz -> zielony[a][b] = 0;
          } else {
            obraz -> zielony[a][b] = (help - czern) * (obraz -> odcieni / (biel - czern));
          }

        }
      } //Niebieski
      for (int a = 0; a < obraz -> wym_y; a++) {
        for (int b = 0; b < obraz -> wym_x; b++) {
          help = obraz -> niebieski[a][b];
          if (help >= biel) {
            obraz -> niebieski[a][b] = obraz -> odcieni;
          } else if (help <= czern) {
            obraz -> niebieski[a][b] = 0;
          } else {
            obraz -> niebieski[a][b] = (help - czern) * (obraz -> odcieni / (biel - czern));
          }

        }
      }
    }

  }

  printf("Zmiana poziomow zakonczona sukcesem.\n");
}

void konturowanie(t_obraz * obraz, t_opcje * opcje) {
  //Dla pgm
  if (obraz -> numer == '2') {
    for (int i = 0; i < (obraz -> wym_y - 1); i++) {
      for (int j = 0; j < (obraz -> wym_x - 1); j++) {
        obraz -> piksele[i][j] = abs((double)(obraz -> piksele[i][j + 1] - obraz -> piksele[i][j])) + abs((double)(obraz -> piksele[i + 1][j] - obraz -> piksele[i][j]));

      }
    }
  }
  //Dla ppm
  else {
    //Tylko czerwony
    if (opcje -> c == 1) {
      for (int i = 0; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < (obraz -> wym_x - 1); j++) {
          obraz -> czerwony[i][j] = abs((double)(obraz -> czerwony[i][j + 1] - obraz -> czerwony[i][j])) + abs((double)(obraz -> czerwony[i + 1][j] - obraz -> czerwony[i][j]));

        }
      }
    }
    //Tylko zielony
    if (opcje -> z == 1) {
      for (int i = 0; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < (obraz -> wym_x - 1); j++) {
          obraz -> zielony[i][j] = abs((double)(obraz -> zielony[i][j + 1] - obraz -> zielony[i][j])) + abs((double)(obraz -> zielony[i + 1][j] - obraz -> zielony[i][j]));

        }
      }
    }
    //Tylko niebieski
    if (opcje -> n == 1) {
      for (int i = 0; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < (obraz -> wym_x - 1); j++) {
          obraz -> niebieski[i][j] = abs((double)(obraz -> niebieski[i][j + 1] - obraz -> niebieski[i][j])) + abs((double)(obraz -> niebieski[i + 1][j] - obraz -> niebieski[i][j]));

        }
      }
    }
    //Wszystkie
    else {
      //czerwony
      for (int i = 0; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < (obraz -> wym_x - 1); j++) {
          obraz -> czerwony[i][j] = abs((double)(obraz -> czerwony[i][j + 1] - obraz -> czerwony[i][j])) + abs((double)(obraz -> czerwony[i + 1][j] - obraz -> czerwony[i][j]));

        }
      }
      //zielony
      for (int i = 0; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < (obraz -> wym_x - 1); j++) {
          obraz -> zielony[i][j] = abs((double)(obraz -> zielony[i][j + 1] - obraz -> zielony[i][j])) + abs((double)(obraz -> zielony[i + 1][j] - obraz -> zielony[i][j]));

        }
      }
      //niebieski
      for (int i = 0; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < (obraz -> wym_x - 1); j++) {
          obraz -> niebieski[i][j] = abs((double)(obraz -> niebieski[i][j + 1] - obraz -> niebieski[i][j])) + abs((double)(obraz -> niebieski[i + 1][j] - obraz -> niebieski[i][j]));

        }
      }

    }

  }

}

void rozmywanie_poziom(t_obraz * obraz, t_opcje * opcje) {
  //Dla pgm
  float help1;

  if (obraz -> numer == '2') {

    for (int i = 0; i < obraz -> wym_y; i++) {
      for (int j = 1; j < (obraz -> wym_x - 1); j++) {
        help1 = obraz -> piksele[i][j - 1] + obraz -> piksele[i][j] + obraz -> piksele[i][j + 1];
        obraz -> piksele[i][j] = (help1 / 3);
      }

    }

  }
  //Dla ppm
  else {
    //Tylko czerwony
    if (opcje -> c = 1) {
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 1; j < (obraz -> wym_x - 1); j++) {
          help1 = obraz -> czerwony[i][j - 1] + obraz -> czerwony[i][j] + obraz -> czerwony[i][j + 1];
          obraz -> czerwony[i][j] = (help1 / 3);
        }

      }
    }
    //Tylko zielony
    if (opcje -> z == 1) {
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 1; j < (obraz -> wym_x - 1); j++) {
          help1 = obraz -> zielony[i][j - 1] + obraz -> zielony[i][j] + obraz -> zielony[i][j + 1];
          obraz -> zielony[i][j] = (help1 / 3);
        }

      }
    }
    //Tylko niebieski
    if (opcje -> n == 1) {
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 1; j < (obraz -> wym_x - 1); j++) {
          help1 = obraz -> niebieski[i][j - 1] + obraz -> niebieski[i][j] + obraz -> niebieski[i][j + 1];
          obraz -> niebieski[i][j] = (help1 / 3);
        }

      }
    }
    //Wszystkie
    else {
      //czerwony
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 1; j < (obraz -> wym_x - 1); j++) {
          help1 = obraz -> czerwony[i][j - 1] + obraz -> czerwony[i][j] + obraz -> czerwony[i][j + 1];
          obraz -> czerwony[i][j] = (help1 / 3);
        }

      }
      //zielony
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 1; j < (obraz -> wym_x - 1); j++) {
          help1 = obraz -> zielony[i][j - 1] + obraz -> zielony[i][j] + obraz -> zielony[i][j + 1];
          obraz -> zielony[i][j] = (help1 / 3);
        }

      }
      //niebieski
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 1; j < (obraz -> wym_x - 1); j++) {
          help1 = obraz -> niebieski[i][j - 1] + obraz -> niebieski[i][j] + obraz -> niebieski[i][j + 1];
          obraz -> niebieski[i][j] = (help1 / 3);
        }

      }

    }

  }

}

void rozmywanie_pion(t_obraz * obraz, t_opcje * opcje) {
  int help;
  //Dla pgm
  if (obraz -> numer == '2') {
    for (int i = 1; i < (obraz -> wym_y - 1); i++) {
      for (int j = 0; j < obraz -> wym_x; j++) {

        help = obraz -> piksele[i - 1][j] + obraz -> piksele[i][j] + obraz -> piksele[i + 1][j];
        obraz -> piksele[i][j] = help / 3;

      }

    }
  }
  //Dla ppm
  else {
    //Tylko czerwony
    if (opcje -> c = 1) {
      for (int i = 1; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {
          help = obraz -> czerwony[i - 1][j] + obraz -> czerwony[i][j] + obraz -> czerwony[i + 1][j];
          obraz -> czerwony[i][j] = (help / 3);
        }

      }
    }
    //Tylko zielony
    if (opcje -> z == 1) {
      for (int i = 1; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {
          help = obraz -> zielony[i - 1][j] + obraz -> zielony[i][j] + obraz -> zielony[i + 1][j];
          obraz -> zielony[i][j] = (help / 3);
        }

      }
    }
    //Tylko niebieski
    if (opcje -> n == 1) {
      for (int i = 1; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {
          help = obraz -> niebieski[i - 1][j] + obraz -> niebieski[i][j] + obraz -> niebieski[i + 1][j];
          obraz -> niebieski[i][j] = (help / 3);
        }

      }
    }
    //Wszystkie
    else {
      //czerwony
      for (int i = 1; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {
          help = obraz -> czerwony[i - 1][j] + obraz -> czerwony[i][j] + obraz -> czerwony[i + 1][j];
          obraz -> czerwony[i][j] = (help / 3);
        }

      }
      //zielony
      for (int i = 1; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {
          help = obraz -> zielony[i - 1][j] + obraz -> zielony[i][j] + obraz -> zielony[i + 1][j];
          obraz -> zielony[i][j] = (help / 3);
        }

      }
      //niebieski
      for (int i = 1; i < (obraz -> wym_y - 1); i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {
          help = obraz -> niebieski[i - 1][j] + obraz -> niebieski[i][j] + obraz -> niebieski[i + 1][j];
          obraz -> niebieski[i][j] = (help / 3);
        }

      }

    }

  }

}

void kor_gamma(t_obraz * obraz, t_opcje * opcje) {

  opcje -> w_gamma = 2;
  double help = 0, help2;
  float help_delta = 1 / (double) opcje -> w_gamma;
  if (obraz -> numer == '2') //Dla pgm
  {
    for (int i = 0; i < obraz -> wym_y; i++) {
      for (int j = 0; j < obraz -> wym_x; j++) {

        help = ((double) obraz -> piksele[i][j]) / (obraz -> odcieni);
        help2 = pow(help, help_delta);
        obraz -> piksele[i][j] = help2 * obraz -> odcieni;
      }
    }
  } else { // Dla ppm
    if (opcje -> c == 1) //Dla czerwonego
    {
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {

          help = ((double) obraz -> czerwony[i][j]) / (obraz -> odcieni);
          help2 = pow(help, help_delta);
          obraz -> czerwony[i][j] = help2 * obraz -> odcieni;
        }
      }
    } else if (opcje -> z == 1) //Dla zielonego
    {
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {

          help = ((double) obraz -> zielony[i][j]) / (obraz -> odcieni);
          help2 = pow(help, help_delta);
          obraz -> zielony[i][j] = help2 * obraz -> odcieni;
        }
      }
    } else if (opcje -> n == 1) // Dla niebieskiego
    {
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {

          help = ((double) obraz -> niebieski[i][j]) / (obraz -> odcieni);
          help2 = pow(help, help_delta);
          obraz -> niebieski[i][j] = help2 * obraz -> odcieni;
        }
      }
    } else //Dla wszystkich 
    {
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {

          help = ((double) obraz -> czerwony[i][j]) / (obraz -> odcieni);

          help2 = pow(help, help_delta);

          obraz -> czerwony[i][j] = help2 * obraz -> odcieni;
        }
      }
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {

          help = ((double) obraz -> zielony[i][j]) / (obraz -> odcieni);

          help2 = pow(help, help_delta);

          obraz -> zielony[i][j] = help2 * obraz -> odcieni;
          
        }
      }
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {

          help = ((double) obraz -> niebieski[i][j]) / (obraz -> odcieni);

          help2 = pow(help, help_delta);

          obraz -> niebieski[i][j] = help2 * obraz -> odcieni;
        }
      }
    }

  }

}

void histogram(t_obraz * obraz, t_opcje * opcje) {
  int minimum, maximum;
  int max_c, max_z, max_n, min_c, min_z, min_n;
  if (obraz -> numer == '2') {
    minimum = obraz -> piksele[0][0];
    maximum = obraz -> piksele[0][0];
    //Znalezienie min i max
    for (int i = 0; i < obraz -> wym_y; i++) {
      for (int j = 0; j < obraz -> wym_x; j++) {

        if (minimum > obraz -> piksele[i][j]) {
          minimum = obraz -> piksele[i][j];
        }
        if (maximum < obraz -> piksele[i][j]) {
          maximum = obraz -> piksele[i][j];
        }

      }
    }
    //Histogram
    for (int i = 0; i < obraz -> wym_y; i++) {
      for (int j = 0; j < obraz -> wym_x; j++) {

        obraz -> piksele[i][j] = (obraz -> piksele[i][j] - minimum) * (obraz -> odcieni / (maximum - minimum));

      }
    }

  }
  //Dla ppm
  else {

    max_c = obraz -> czerwony[0][0];
    max_z = obraz -> zielony[0][0];
    max_n = obraz -> niebieski[0][0];
    min_c = obraz -> czerwony[0][0];
    min_z = obraz -> zielony[0][0];
    min_n = obraz -> niebieski[0][0];

    //Min i max dla każdego koloru
    for (int i = 0; i < obraz -> wym_y; i++) {
      for (int j = 0; j < obraz -> wym_x; j++) {

        if (min_c > obraz -> czerwony[i][j]) {
          min_c = obraz -> czerwony[i][j];
        }
        if (max_c < obraz -> czerwony[i][j]) {
          max_c = obraz -> czerwony[i][j];
        }

        if (min_z > obraz -> zielony[i][j]) {
          min_z = obraz -> zielony[i][j];
        }
        if (max_z < obraz -> zielony[i][j]) {
          max_z = obraz -> zielony[i][j];
        }

        if (min_n > obraz -> niebieski[i][j]) {
          min_n = obraz -> niebieski[i][j];
        }
        if (max_n < obraz -> niebieski[i][j]) {
          max_n = obraz -> niebieski[i][j];
        }

      }
    }
    //Opcje histogramu
    if (opcje -> c == 1) {
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {

          obraz -> czerwony[i][j] = (obraz -> czerwony[i][j] - min_c) * (obraz -> odcieni / (max_c - min_c));

        }
      }
    }
    if (opcje -> z == 1) {
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {

          obraz -> zielony[i][j] = (obraz -> zielony[i][j] - min_z) * (obraz -> odcieni / (max_z - min_z));

        }
      }
    }
    if (opcje -> n == 1) {
      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {

          obraz -> niebieski[i][j] = (obraz -> niebieski[i][j] - min_n) * (obraz -> odcieni / (max_n - min_n));

        }
      }
    } else {

      for (int i = 0; i < obraz -> wym_y; i++) {
        for (int j = 0; j < obraz -> wym_x; j++) {
          obraz -> czerwony[i][j] = (obraz -> czerwony[i][j] - min_c) * (obraz -> odcieni / (max_c - min_c));
          obraz -> zielony[i][j] = (obraz -> zielony[i][j] - min_z) * (obraz -> odcieni / (max_z - min_z));
          obraz -> niebieski[i][j] = (obraz -> niebieski[i][j] - min_n) * (obraz -> odcieni / (max_n - min_n));

        }
      }

    }

  }

}

void wyzeruj_opcje(t_opcje * wybor) {
  wybor -> plik_we = NULL;
  wybor -> plik_wy = NULL;
  wybor -> negatyw = 0;
  wybor -> konturowanie = 0;
  wybor -> progowanie = 0;
  wybor -> wyswietlenie = 0;
}

int przetwarzaj_opcje(int argc, char ** argv, t_opcje * wybor) {
  int i, prog;
  double prog_cz, prog_b,gamma,biel,czern;
  char * nazwa_pliku_we, * nazwa_pliku_wy;

  wyzeruj_opcje(wybor);
  wybor -> plik_wy = stdout; /* na wypadek gdy nie podano opcji "-o" */

  for (i = 1; i < argc; i++) {
    
    if (argv[i][0] != '-') /* blad: to nie jest opcja - brak znaku "-" */
     
      return B_NIEPOPRAWNAOPCJA;
    switch (argv[i][1]) {
    case 'i': {
      /* opcja z nazwa pliku wejsciowego */
      if (++i < argc) {
        /* wczytujemy kolejny argument jako nazwe pliku */
        nazwa_pliku_we = argv[i];
        if (strcmp(nazwa_pliku_we, "-") == 0) /* gdy nazwa jest "-"        */
          wybor -> plik_we = stdin; /* ustwiamy wejscie na stdin */
        else /* otwieramy wskazany plik   */
          wybor -> plik_we = fopen(nazwa_pliku_we, "r");

      } else
        return B_BRAKNAZWY; /* blad: brak nazwy pliku */
      break;
    }
    case 'o': {
      /* opcja z nazwa pliku wyjsciowego */
      if (++i < argc) {
        /* wczytujemy kolejny argument jako nazwe pliku */
        nazwa_pliku_wy = argv[i];
        if (strcmp(nazwa_pliku_wy, "-") == 0)
        { /* gdy nazwa jest "-"         */
          wybor -> plik_wy = stdout; /* ustwiamy wyjscie na stdout */ }
        else /* otwieramy wskazany plik    */
          {wybor -> plik_wy = fopen(nazwa_pliku_wy, "w");
        //  wybor->nazwa=*nazwa_pliku_wy;
          }
      } else
        return B_BRAKNAZWY; /* blad: brak nazwy pliku */
      break;
    }
    case 'd': {
      /* Sprawdza czy wyswietlic plik po zakonczonych operacjach */
      wybor -> wyswietlenie = 1;
      break;
    }
    case 'm': {
      /* opcja z nazwa pliku wyjsciowego */
      if (++i < argc) {

        /* wczytujemy kolejny argument jako kolor do obróbki albo konwersje szarosci */
        char kolor;
        kolor = * argv[i];

        if (kolor == 'c') {
          wybor -> c = 1;
        } else if (kolor == 'z') {
          wybor -> z = 1;
        } else if (kolor == 'n') {
          wybor -> n = 1;
        } else if (kolor == 's') {
          wybor -> s = 1;
        } else {
          printf("Nie poprawna opcja kolorow. Dostepne opcje to: c,z,n,s\n");
        }
      } else
        return B_BRAKWARTOSCI; /* blad: brak wartosci koloru */
      break;
    }
    case 'p': {
      if (!strcmp(argv[i], "-pc")) {

        if (++i < argc) {
          /* wczytujemy kolejny argument jako wartosc progu */
          if (sscanf(argv[i], "%lf", & prog_cz) == 1) {

            wybor -> progowanie_czern = 1;
            wybor -> w_progu_czern = prog_cz;

          } else
            return B_BRAKWARTOSCI;
        } else {
          return B_BRAKWARTOSCI; /* blad: brak wartosci progu */
        }
      } else if (!strcmp(argv[i], "-pb")) {

        if (++i < argc) {
          /* wczytujemy kolejny argument jako wartosc progu */
          if (sscanf(argv[i], "%lf", & prog_b) == 1) {

            wybor -> progowanie_biel = 1;
            wybor -> w_progu_biel = prog_b;

          } else
            return B_BRAKWARTOSCI;
        } else {
          return B_BRAKWARTOSCI; /* blad: brak wartosci progu */
        }
      } else if (++i < argc) {

        /* wczytujemy kolejny argument jako wartosc progu */
        if (sscanf(argv[i], "%d", & prog) == 1) {
          wybor -> progowanie = 1;
          wybor -> w_progu = prog;
        } else {
          return B_BRAKWARTOSCI; /* blad: niepoprawna wartosc progu */
        }
      } else {
        return B_BRAKWARTOSCI; /* blad: brak wartosci progu */
      }

      break;
    }
    case 'n': {
      /* mamy wykonac negatyw */
      wybor -> negatyw = 1;
      break;
    }
    case 'k': {
      /* mamy wykonac konturowanie */
      wybor -> konturowanie = 1;
      break;
    }
    case 'g': {
       if (++i < argc) {
          /* wczytujemy kolejny argument jako wartosc progu */
          if (sscanf(argv[i], "%lf", & gamma) == 1) {
              
            wybor -> gamma = 1;
            wybor -> w_gamma = gamma;

          } else{
            return B_BRAKWARTOSCI;
          }
        } 
        else {
          return B_BRAKWARTOSCI; /* blad: brak wartosci gammy */
        }
      break;
    }
    case 'z': {
       if (++i < argc) {
          /* wczytujemy kolejny argument jako wartosc progu */
          if (sscanf(argv[i],"%lf", &czern ) == 1) {
              
            wybor -> zmiana_poziomow = 1;
            wybor -> w_zmiany_c = czern;
            sscanf(argv[(i+1)],"%lf",&biel);
            wybor->w_zmiany_b=biel;
          i++;
          } else{
            return B_BRAKWARTOSCI;
          }
        } 
        else {
          return B_BRAKWARTOSCI; /* blad: brak wartosci  */
        }
      break;
    }
    case 'r': {

      if (!strcmp(argv[i], "-rx")) {

          wybor->rozmywanie_poziom=1;
          printf("%d\n",wybor->rozmywanie_poziom);
      }
      else if (!strcmp(argv[i], "-ry")) {
          wybor->rozmywanie_pion=1;
      } 



      break;
    }
      case 'h': {
      /* mamy wykonac negatyw */
      wybor ->histogram = 1;
      break;
    }
    default:{
         /* nierozpoznana opcja */
        
      return B_NIEPOPRAWNAOPCJA;
    }
     
    } /* koniec switch */
  } /* koniec for */

  if (wybor -> plik_we != NULL) /* ok: wej. strumien danych zainicjowany */
    return W_OK;
  else
    return B_BRAKPLIKU; /* blad:  nie otwarto pliku wejsciowego  */
}


void tymczasowy( t_obraz * obraz) {

  //char nazwa[100];

  int help, linia = 0;
  /* printf("Pod jaką nazwą chcesz zapisać swój wczytany obrazek?\n"); Pobieramy od uzytkownika nazwe pliku 
   scanf("%s", nazwa);*/

  /*Zapis dla pgm */
  FILE *plik;
  
  if (obraz -> numer == '2') {
    plik=fopen("tymczasowy.pgm","w");
    fprintf(plik, "P2\n");
    fprintf(plik, "%d %d \n", obraz -> wym_x, obraz -> wym_y);
    fprintf(plik, "%d \n", obraz -> odcieni);

    /*Wpisujemy do pliku wartosc z tabeli z odczytanym obrazem*/

    for (int i = 0; i < obraz -> wym_y; i++) {
      for (int j = 0; j < obraz -> wym_x; j++) {
        fprintf(plik, "%d\t ", obraz -> piksele[i][j]);
      }
      fprintf(plik, "\n");
    }

  }
  /*Zapis dla ppm */
  
  else if (obraz -> numer == '3') {
    plik=fopen("tymczasowy.ppm","w");
    printf("3\n");
    fprintf(plik, "P3\n");
    fprintf(plik, "%d %d \n", obraz -> wym_x, obraz -> wym_y);
    fprintf(plik, "%d \n", obraz -> odcieni);

    for (int i = 0; i < obraz -> wym_y; i++) {
      for (int j = 0; j < obraz -> wym_x; j++) {
        fprintf(plik, "%d\t ", obraz -> czerwony[i][j]);
        fprintf(plik, "%d\t ", obraz -> zielony[i][j]);
        fprintf(plik, "%d\t ", obraz -> niebieski[i][j]);
      }
      fprintf(plik, "\n");
    }

  }

  fclose(plik);
}