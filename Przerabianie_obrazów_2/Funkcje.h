#ifndef FILTRY_H
#define FILTRY_H
#define MAX 512            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024   
#include "struktury.h"


void negatyw(t_obraz *obraz,t_opcje *opcje);

void progowanie(t_obraz *obraz,t_opcje *opcje);

void progowanie_czern(t_obraz *obraz,t_opcje *opcje);

void progowanie_biel(t_obraz *obraz,t_opcje *opcje);

void zmiana_poziomow(t_obraz *obraz,t_opcje *opcje);

void rozmywanie_pion(t_obraz *obraz,t_opcje *opcje);

void rozmywanie_poziom(t_obraz *obraz,t_opcje *opcje);

void kor_gamma(t_obraz *obraz,t_opcje *opcje);

void konturowanie(t_obraz *obraz,t_opcje *opcje);

void histogram(t_obraz *obraz,t_opcje *opcje);

void wyswietl(t_obraz *obraz);

int czytaj(FILE *plik_we,t_obraz *obraz) ;

void odczytywanie(t_obraz *obraz,int *czy);

void zapisz(FILE *plik_wy,t_obraz *obraz);

void zamien(t_obraz *obraz,t_opcje *opcje);

void wyzeruj_opcje(t_opcje * wybor);

int przetwarzaj_opcje(int argc, char **argv, t_opcje *wybor);

void tymczasowy(t_obraz *obraz);
#endif