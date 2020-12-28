
#include <stdio.h>										//____ZPP - Projekt - Akad. rok 2020/2021___
#include <stdlib.h>												//Jan Agh, FIIT STUBA
#include <string.h>									//Copyright(c) 2020 Jan Agh, all rights reserved
#include "projekt.h"

#define MENOSUBORU "jazdci.csv"		//!!!!!>>>DOLEZITE<<<!!!!! sem zadajte meno suboru s pretekarmi

pretekar* pocet;
FILE* subor;
int riadky = 0;

int main() {
	char prikaz;

	do {
		printf("Zadajte nazov prikazu: ");			//nacitanie znaku prikazu
		scanf_s(" %c", &prikaz, 1);

		rozhodnutie(prikaz);

	} while (prikaz != 'x');

	return 0;
}

void rozhodnutie(char prikaz) {			//tu sa na zaklade nacitaneho znaku rozhodne, ktora funkcia sa vykona
	switch (prikaz) {
	case 's':
		otvaranie("r") ? sum() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'd':
		otvaranie("r") ? driver() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'l':
		otvaranie("r") ? lap() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'g':
		otvaranie("r") ? gender() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'b':
		otvaranie("r") ? brand() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'y':
		otvaranie("r") ? year() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'a':
		otvaranie("r") ? average() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'u':
		otvaranie("r") ? under() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'c':
		otvaranie("r") ? change() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'n':
		otvaranie("r") ? newdriver() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'r':
		otvaranie("r+") ? rmdriver() : printf("\nSubor sa nepodario otvorit.\n\n"); break;
	case 'x':
		printf("\nDakujeme Vam za pouzivanie nasho programu, dovidenia.\n"); break;
	default:
		printf("\nZadali ste nespravny prikaz.\n\n");
	}
}

int otvaranie(char* x) {
	return ((subor = fopen(MENOSUBORU, x)) == NULL ? 0 : 1);		//funkcia na otvaranie suboru
}

void naplnit(void) {				//tato funkcia nacita udaje zo suboru, vytvori instancie structu a vlozi udaje do nich
	char meno[50] = "\0", * prz;
	int medzery, znak, y;
	riadky = 1;

	while ((znak = fgetc(subor)) != EOF) {		//zistenie poctu riadkov suboru (cize poctu pretekarov)
		if (znak == '\n')
			riadky++;
	}
	rewind(subor);

	pocet = (pretekar*)malloc(riadky * sizeof(pretekar));		//dynamicke alokovanie globalneho pola instancii structu

	for (int x = 0; x < riadky; x++) {
		medzery = 0;

		fscanf(subor, "%50[^;];%1[^;];%d;%10[^;];%f;%f;%f;%f;%f\n", meno, pocet[x].pohlavie, &pocet[x].rok, pocet[x].vyrobca,
			&pocet[x].cas[0], &pocet[x].cas[1], &pocet[x].cas[2], &pocet[x].cas[3], &pocet[x].cas[4]);		//nacitanie riadku suboru

		strcpy(pocet[x].meno, meno);

		for (y = 0; y < strlen(meno); y++) {		//zistenie poctu medzier v mene 
			if (meno[y] == ' ')
				medzery++;
		}
		prz = strtok(meno, " ");			//rozdelenie mena na "tokeny" podla medzier a nastavenie ukazovatela na prvy token

		for (y = 0; y < medzery; y++) {		//nastavenie ukazovatela na posledny token (cize priezvisko) podla poctu medzier v mene
			prz = strtok(NULL, " ");
		}
		strcpy(pocet[x].priezvisko, prz);		//vlozenie priezviska do structu
	}
	rewind(subor);
}

void rmdriver(void) {			//funkcia na odstranenie pretekara zo suboru
	naplnit();

	char zadane_prz[30] = "\0", pismeno;
	int cislo = 0, newline = 1, kontrola = 0;
	FILE* kopia;

	printf("Zadajte priezvisko jazdca: ");
	scanf_s("%s", zadane_prz, 29);

	for (int x = 0; x < riadky; x++) {			//zistenie, na ktorom riadku sa nachadza pretekar so zadanym priezviskom
		cislo++;

		if (strcmp(zadane_prz, pocet[x].priezvisko) == 0) {
			kontrola++;
			break;
		}
	}

	if (kontrola == 0) {
		printf("\nJazdec s danym priezviskom sa nenasiel.\n\n");
		fclose(subor);
	}
	else {
		if ((kopia = fopen("kopia.csv", "w")) != NULL) {		//vytvorenie noveho suboru a prekopirovanie celeho obsahu
			while ((pismeno = fgetc(subor)) != EOF) {			//suboru s pretekarmi don okrem pretekara so zadanym priezviskom
				if (newline != cislo) {
					if (newline != (cislo - 1) || newline != (riadky - 1) || pismeno != '\n')
						fputc(pismeno, kopia);
				}
				if (pismeno == '\n')
					newline++;
			}

			if (fclose(subor) == 0 && fclose(kopia) == 0 && remove(MENOSUBORU) == 0 && rename("kopia.csv", MENOSUBORU) == 0) {
				printf("\nJazdec s menom \"%s\" bol vymazany.\n\n", pocet[cislo - 1].meno);
			}
			else {													//ak sa podarilo subory zavriet, zmaze sa subor s pretekarmi
				printf("\nJazdca nie je mozne vymazat.\n\n");		//a prekopirovany subor sa premenuje 
			}
		}
		else {
			printf("\nJazdca nie je mozne vymazat.\n\n");
			fclose(subor);
		}
	}
	free(pocet);
}

void newdriver(void) {			//funkcia na zapis noveho pretekara do suboru
	naplnit();
	fclose(subor);

	char meno[50] = "\0", pohlavie[20] = "\0", vyrobca[10] = "\0", osetrenie[30] = "\0";
	int vek, kontrola = 0, x, cont = 0;
	float cas[5] = { 0.0 };

	do {
		printf("\nZadajte meno a priezvisko jazdca oddelene medzerou (aspon 4 pismena): ");
		scanf_s(" %49[^\n]", meno, 49);
		for (x = 0; x < strlen(meno); x++) {
			if (meno[x] == ' ')
				kontrola++;
		}
	} while (kontrola < 1 || strlen(meno) < 4);
	kontrola = 0;
	for (x = 0; x < riadky; x++) {
		if (strcmp(pocet[x].meno, meno) == 0) {			//kontrola, ci uz pretekar s danym menom existuje
			kontrola++;
			printf("\nJazdec s danym menom uz existuje.\n\n");
		}
	}
	free(pocet);

	if (kontrola == 0) {
		do {
			printf("Zadajte rok narodenia <1900;2020>: ");			//nacitanie vsetkych udajov o novom pretekarovi
			cont = scanf_s("%d", &vek);
			if (cont != 1)
				fgets(osetrenie, 29, stdin);
		} while (vek <= 1900 || vek > 2020 || cont != 1);

		do {
			printf("Zadajte pohlavie jazdca (m - muz, f - zena): ");
			scanf_s("%s", pohlavie, 19);
		} while (strcmp(pohlavie, "m") != 0 && strcmp(pohlavie, "f") != 0);

		do {
			printf("Zadajte znacku auta (bugatti, ferrari, porsche alebo honda): ");
			scanf_s("%s", vyrobca, 9);
		} while (strcmp(vyrobca, "bugatti") != 0 && strcmp(vyrobca, "ferrari") != 0 &&
			strcmp(vyrobca, "porsche") != 0 && strcmp(vyrobca, "honda") != 0);

		do {
			printf("Zadajte 5 casov dosiahnutych jazdcom oddelene bodkociarkou (aspon 60sec): ");
			cont = scanf_s("%f;%f;%f;%f;%f", &cas[0], &cas[1], &cas[2], &cas[3], &cas[4]);
			if (cont != 5) {
				ungetc('a', stdin);
				fgets(osetrenie, 29, stdin);
			}
		} while (cas[0] < 60.0 || cas[1] < 60.0 || cas[2] < 60.0 || cas[3] < 60.0 || cas[4] < 60.0 || cont != 5);

		if (otvaranie("a")) {				//zapis noveho pretekara na koniec suboru
			kontrola = fprintf(subor, "\n%s;%s;%d;%s;%.03f;%.03f;%.03f;%.03f;%.03f", meno, pohlavie, vek, vyrobca,
				cas[0], cas[1], cas[2], cas[3], cas[4]);

			if (kontrola != EOF) {
				printf("\nJazdec bol uspesne zapisany do suboru.\n\n");
				fclose(subor);
				rozhodnutie('s');
			}
			else {
				printf("\nChyba, jazdec nebol do suboru zapisany.\n\n");
				fclose(subor);
			}
		}
		else
			printf("\nSubor sa nepodario otvorit.\n\n");
	}
}

void change(void) {		//funkcia na zmenu casu kola pretekara
	naplnit();
	fclose(subor);
	char zadane_prz[30] = "\0", osetrenie[30] = "\0";
	int cislo = 0, kontrola = 0, c_kolo, x, cont = 0;
	float kolo;

	printf("Zadajte priezvisko jazdca: ");
	scanf_s("%s", zadane_prz, 29);

	for (x = 0; x < riadky; x++) {			//zistenie, na ktorom riadku sa nachadza pretekar so zadanym priezviskom
		cislo++;

		if (strcmp(zadane_prz, pocet[x].priezvisko) == 0) {
			kontrola++;
			break;
		}
	}

	if (kontrola == 0) {
		printf("\nJazdec s danym priezviskom sa nenasiel.\n\n");
		free(pocet);
	}
	else {
		do {
			printf("Zadajte poradove cislo kola <1;5>: ");			//nacitanie potrebnych udajov
			cont = scanf_s("%d", &c_kolo);
			if (cont != 1)
				fgets(osetrenie, 29, stdin);
		} while (c_kolo < 1 || c_kolo > 5 || cont != 1);

		do {
			printf("Zadajte novy cas v sec (minimalne 60sec): ");
			cont = scanf_s("%f", &kolo);
			if (cont != 1)
				fgets(osetrenie, 29, stdin);
		} while (kolo < 60.0 || cont != 1);

		pocet[cislo - 1].cas[c_kolo - 1] = kolo;

		if (otvaranie("w")) {			//vymazanie suboru a zapisanie obsahu structu (pretekarov) uz so zmenenym casom 
			for (x = 0; x < riadky; x++) {
				kontrola = fprintf(subor, "%s;%s;%d;%s;%.03f;%.03f;%.03f;%.03f;%.03f", pocet[x].meno, pocet[x].pohlavie, pocet[x].rok,
					pocet[x].vyrobca, pocet[x].cas[0], pocet[x].cas[1], pocet[x].cas[2], pocet[x].cas[3], pocet[x].cas[4]);

				if (x != riadky - 1)
					fputc('\n', subor);
			}
			fclose(subor);
			free(pocet);
			rozhodnutie('s');
		}
		else {
			printf("\nChyba, nepodarilo sa prepisat dane kolo.\n\n");
			free(pocet);
		}
	}
}

void under(void) {			//funkcia na vypis casov, ktore boli mensie ako zadany cas
	naplnit();
	fclose(subor);

	float zadane_kolo = 0, * kolo;
	int* cislo, suma = 0, x, y, cont = 0;
	char osetrenie[30] = "\0";

	do {
		printf("Zadajte cas kola: ");
		cont = scanf_s("%f", &zadane_kolo);
		if (cont != 1)
			fgets(osetrenie, 29, stdin);
	} while (cont != 1);

	for (x = 0; x < riadky; x++) {
		suma = 0;

		for (y = 0; y < 5; y++) {
			if (pocet[x].cas[y] <= zadane_kolo)		//spocitanie, kolko casov mal dany pretekar lepsich, ako zadany cas
				suma++;
		}
		cislo = (int*)malloc(suma * sizeof(int));		//dynamicka alokacia poli pre cisla kol a kola podla preddosleho spocitania
		kolo = (float*)malloc(suma * sizeof(float));
		suma = 0;

		for (y = 0; y < 5; y++) {
			if (pocet[x].cas[y] <= zadane_kolo)			//ukladanie kol a cisel kol do dynamicky alokovanych poli
				kolo[suma] = pocet[x].cas[y], cislo[suma] = y + 1, suma++;
		}
		printf("%s - %d %s", pocet[x].meno, suma, (suma != 1) ? ((suma == 5 || suma == 0) ? "kol" : "kola") : "kolo");
		for (y = 0; y < suma; y++) {
			printf(", %d (%.03f)", cislo[y], kolo[y]);	//vypis mena pretekara, poctu kol a kol samotnych
		}
		putchar('\n');
		free(cislo);
		free(kolo);
	}
	free(pocet);
}

void average(void) {		//funkcia na vypocitanie priemernych casov pretekarov
	naplnit();
	fclose(subor);

	float sucet, min = 1000.0;
	int x, jazdec = 0;

	for (x = 0; x < riadky; x++) {
		sucet = 0.0;

		for (int y = 0; y < 5; y++) {			//spocitanie casov pretekara a vypocet priemerneho casu
			sucet += pocet[x].cas[y];
		}
		printf("%s - %.03f\n", pocet[x].meno, sucet / 5);

		if ((sucet / 5) < min)					//najdenie najmensieho priemerneho casu
			min = sucet / 5, jazdec = x;
	}
	printf("\nNajlepsie:\n%s - %.03f\n", pocet[jazdec].meno, min);

	free(pocet);
}

void year(void) {		//funkcia na najdenie pretekarov, ktori sa narodili skor, ako zadany rok
	naplnit();
	fclose(subor);

	int rok = 0, x, cislo = 0, cont = 0;
	char osetrenie[30] = "\0";

	do {
		printf("Zadajte rok narodenia: ");
		cont = scanf_s("%d", &rok);
		if (cont != 1)
			fgets(osetrenie, 29, stdin);
	} while (cont != 1);

	if (rok <= 0 || rok > 2020)
		printf("\nZadali ste nespravnu hodnotu.\n\n");
	else {
		for (x = 0; x < riadky; x++) {
			float kolo = 1000.0;

			if (pocet[x].rok < rok) {
				for (int y = 0; y < 5; y++) {		//najdenie najlepsieho kola pretekara a zistenie cisla kola
					if (pocet[x].cas[y] < kolo)
						kolo = pocet[x].cas[y], cislo = y + 1;
				}
				printf("%s\nnar. %d\nNajlepsie kolo: %.03f\nCislo kola: %d\n\n", pocet[x].meno, pocet[x].rok, kolo, cislo);
			}
		}
	}
	free(pocet);
}

void brand(void) {			//funkcia na vypis automobilovych znaciek a pretekarov, ktori nimi dosiahli najlepsie casy
	naplnit();
	fclose(subor);

	char znacka[4][10] = { "porsche", "bugatti", "honda", "ferrari" };
	int cislo[4] = { 0 }, jazdec[4] = { 0 }, z;
	float kolo[4] = { 1000.0, 1000.0, 1000.0, 1000.0 };

	for (z = 0; z < 4; z++) {					//najdenie znaciek pretekarov a zaznamenanie najlepsich casov
		for (int x = 0; x < riadky; x++) {
			if (strcmp(pocet[x].vyrobca, znacka[z]) == 0) {
				for (int y = 0; y < 5; y++) {
					if (pocet[x].cas[y] < kolo[z])
						kolo[z] = pocet[x].cas[y], jazdec[z] = x, cislo[z] = y + 1;
				}
			}
		}
	}
	for (z = 0; z < 4; z++) {
		if (kolo[z] != 1000.0 && cislo[z] != 0)
			printf("Znacka: %s\nNajlepsie kolo: %.03f\nJazdec: %s\nCislo kola: %d\n\n", znacka[z], kolo[z], pocet[jazdec[z]].meno, cislo[z]);
		else
			printf("Ziadny jazdec s autom znacky %s nesutazil.\n\n", znacka[z]);
	}
	free(pocet);
}

void gender(void) {			//funkcia na vypis najlepsieho casu pretekara zadaneho pohlavia
	naplnit();
	fclose(subor);

	float kolo = 1000.0;
	int cislo = 0, jazdec = 0;
	char porovnanie[20] = "\0";

	printf("Zadajte pohlavie (m - muz, f - zena): ");
	scanf_s("%s", porovnanie, 19);
	if (strcmp(porovnanie, "m") == 0 || strcmp(porovnanie, "f") == 0) {

		for (int x = 0; x < riadky; x++) {
			if (strcmp(pocet[x].pohlavie, porovnanie) == 0) {		//ukladanie casov pretekarov podla zadaneho pohlavia
				for (int y = 0; y < 5; y++) {
					if (pocet[x].cas[y] < kolo)
						kolo = pocet[x].cas[y], jazdec = x, cislo = y + 1;
				}
			}
		}
		printf("Najlepsie kolo: %.03f\nJazdec: %s\nCislo kola: %d\n", kolo, pocet[jazdec].meno, cislo);
	}
	else
		printf("\nZadali ste nespravnu hodnotu.\n\n");

	free(pocet);
}

void lap(void) {			//funkcia na vypis najlepsieho kola spomedzi vsetkych pretekarov
	naplnit();
	fclose(subor);

	float kolo = 1000.0;
	int cislo = 0, jazdec = 0;

	for (int x = 0; x < riadky; x++) {			//prehladavanie pretekarov a najdenie najlepsieho kola
		for (int y = 0; y < 5; y++) {
			if (pocet[x].cas[y] < kolo)
				kolo = pocet[x].cas[y], jazdec = x, cislo = y + 1;
		}
	}
	printf("Najlepsie kolo: %.03f\nJazdec: %s\nCislo kola: %d\n", kolo, pocet[jazdec].meno, cislo);

	free(pocet);
}

void driver(void) {		//funkcia na vypis pretekara podla zadaneho priezviska
	naplnit();
	fclose(subor);

	char zadane_prz[30] = "\0";
	int kontrola = 0;
	float min_kolo = 1000.0, max_kolo = 0.0, sucet;

	printf("Zadajte priezvisko jazdca: ");
	scanf_s("%s", zadane_prz, 29);

	for (int x = 0; x < riadky; x++) {
		if (strcmp(zadane_prz, pocet[x].priezvisko) == 0) {		//najdenia pretekara podla zadaneho priezviska
			sucet = 0.0;
			kontrola++;
			printf("%s\nnar. %d, %s\nAutomobil: %s\n", pocet[x].meno, pocet[x].rok, strcmp(pocet[x].pohlavie, "m") ? "zena" : "muz", pocet[x].vyrobca);
			printf("\nCasy okruhov: %.03f;%.03f;%.03f;%.03f;%.03f\n", pocet[x].cas[0], pocet[x].cas[1], pocet[x].cas[2], pocet[x].cas[3], pocet[x].cas[4]);

			for (int y = 0; y < 5; y++) {
				if (pocet[x].cas[y] < min_kolo)			//najdenie najlepsieho, najhorsieho a priemerneho casu pretekara
					min_kolo = pocet[x].cas[y];
				if (pocet[x].cas[y] > max_kolo)
					max_kolo = pocet[x].cas[y];
				sucet += pocet[x].cas[y];
			}
			printf("\nNajlepsie kolo: %.03f\nNajhorsie kolo: %.03f\nPriemerne kolo: %.03f\n\n", min_kolo, max_kolo, sucet / 5);
		}
	}
	if (kontrola == 0)
		printf("\nJazdec s danym priezviskom sa nenasiel.\n\n");

	free(pocet);
}

void sum(void) {			//funkcia na vypis obsahu suboru
	naplnit();
	fclose(subor);

	for (int x = 0; x < riadky; x++) {
		printf("%s, nar. %d, %s, Automobil: %s\n", pocet[x].meno, pocet[x].rok, strcmp(pocet[x].pohlavie, "m") ? "zena" : "muz", pocet[x].vyrobca);
		printf("Casy okruhov: %.03f; %.03f; %.03f; %.03f; %.03f\n", pocet[x].cas[0], pocet[x].cas[1], pocet[x].cas[2], pocet[x].cas[3], pocet[x].cas[4]);
	}
	free(pocet);
}