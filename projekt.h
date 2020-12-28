
#ifndef PROJEKT_H
#define PROJEKT_H

typedef struct Pretekar {		//struct obsahujuci udaje o kazdom pretekarovi
	char meno[60];
	char priezvisko[30];
	char pohlavie[2];
	int rok;
	char vyrobca[10];
	float cas[5];
}pretekar;

void rozhodnutie(char);
int otvaranie(char*);
void naplnit(void);
void rmdriver(void);
void newdriver(void);
void change(void);
void under(void);
void average(void);
void year(void);
void brand(void);
void gender(void);
void lap(void);
void driver(void);
void sum(void);

#endif
