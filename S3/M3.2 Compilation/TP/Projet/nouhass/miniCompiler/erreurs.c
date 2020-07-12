#include "erreurs.h"
char *ERREUR_NOM[] = {"caractere inconnu","erreur d'ouverture de fichier","Mot clé program attendu"," attendu"};
ERREUR TAB_ERREUR[]={{CAR_INC_ERR,"caractère inconnu"},{FIC_VID_ERR,"fichier vide"},};

void ERREUR_LEXICAL(CODES_ERR CODE){
    int  ind_err=CODE;
	//printf( "Erreur  numero  %d \t : %s \n", ind_err, TAB_ERREUR[ind_err].message_err);
	printf( "lexical error, Erreur  numero  %d \t : \n", ind_err);
	getch();
	exit(1);
}
void ERREUR_SANTAXIQUE(CODES_ERR CODE){
    int  ind_err=CODE;
	//printf( "Erreur  numero  %d \t : %s \n", ind_err, TAB_ERREUR[ind_err].message_err);
	printf( "syantaxique error , Erreur  numero  %d \t : \n", ind_err);
	getch();
	exit(1);
}
void ERREUR_SYMANTIQUE(CODES_ERR CODE){
    int  ind_err=CODE;
	//printf( "Erreur  numero  %d \t : %s \n", ind_err, TAB_ERREUR[ind_err].message_err);
	printf( "SEMANTIC error , Erreur  numero  %d \t : \n", ind_err);
	getch();
	exit(1);
}
