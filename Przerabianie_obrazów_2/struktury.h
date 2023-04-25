#ifndef STRUKTURY_H
#define STRUKTURY_H



/* typ do przechowywania obrazów monochoromatycznych i kolorowych */
typedef struct {


int wym_x, wym_y, odcieni;
int numer;
int **piksele;
int **czerwony,**zielony, **niebieski;
} t_obraz;

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct {
  char *nazwa;
  FILE *plik_we, *plik_wy;        /* uchwyty do pliku wej. i wyj. */
  int negatyw,progowanie,progowanie_czern,progowanie_biel,konturowanie,wyswietlenie,gamma,rozmywanie_pion,rozmywanie_poziom,zmiana_poziomow,histogram;    /* opcje */
  double w_progu,w_progu_czern,w_progu_biel,w_gamma,w_zmiany_c,w_zmiany_b;              /* wartosc progu dla opcji progowanie */ 
  int c,z,n,s; /*opcje koloru*/
} t_opcje;





#endif