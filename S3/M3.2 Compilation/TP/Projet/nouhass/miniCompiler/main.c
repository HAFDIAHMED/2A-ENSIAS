#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<string.h>
#include <ctype.h>

#include "code_lex.h"

#include "erreurs.h"

#define TAILLEMEM 100
#define TAILLECODE 100
//////////////////////////ANALYSEUR LEXICAL/////////////////////////////////////////////////////////////



/********************DECLARATION DE TYPE SYMBOLES*******************************/
typedef struct s {
CODES_LEX code;// contient le token du symbole
char nom[20];// contient la forme textuelle du symbole
struct s *suiv;
}SYMBOLES;

/***************List *************************/
struct pp {
     void *premier;
     int taill;
};
typedef struct pp pile;
void initialisep(pile *lp);
void ajouter(pile *lp,char *nom,CODES_LEX code);
void affiche (pile *pp);
/*****************Scanner************************/
typedef struct {
char Eof ;
pile *mots_cles;//TABLE DES MOTS CLE
SYMBOLES SYMB_COUR;//SYMBOLE COURANT
char CAR_COUR;//CARACTERE COURANT
FILE * fluxSource;// FICHIER CODE SOURCE
}Scanner;
Scanner scanner;
void CONSTRUCT_SCANNER(char *file_name);
void CHECK_FILE_EXISTANCE(char* file_name);
void INIT_MOTS_CLES();// INITIALISATION DES MOTS CLES
void CODAGE_LEX();//RETOURNE LE TOKEN DU MOT EN ENTREE
/***************FONCTIONS DE CATEGORIES**************************************/
void LIRE_CAR();
void LIRE_NOMBRE();
void LIRE_MOT();
void SYM_SUIV();
int IS_SEPARATOR();
void AFFICHER_TOKEN(SYMBOLES symbole);

/////////////////////////////////////ANALYSEUR SYNTAXIQUE/////////////////////////////////////////////////////
void TEST_SYMBOLE(CODES_LEX cl,CODES_ERR ce);
void PROGRAM();
void BLOCK();
void CONSTS();
void VARS();
void INSTS();
void INST();
void AFFEC();
void SI();
void TANQUE();
void ECRIRE();
void LIRE();
void COND();
void EXPR();
void TERM();
void FACT();
void POUR();
void REPETER();
void CAS();

///////////////////////////////ANLYSE SEMANTIQUE/////////////////////////////////////
typedef enum {TPROG, TCONST, TVAR} TSYM;

typedef  struct ti {
    char NOM[20];
    TSYM TIDF;
    int adresse;
    struct ti *suiv;

} T_TAB_IDF;

pile *TAB_IDFS;//TABLE DES MOTS CLE
void ajouterT(pile *lp,char *nom,TSYM code);
void IDF_NON_DECLARED(char *);
void IDF_DOUBLE_DECLARED(char *);
void UNAUTORISER_PROGRAM_ID(char *);
void UNCHANBLE_CONST_VALUE(char *);
void test_for_up_down(int num_start,int num_end,int up_down);

void parser();

///////////////GENERATION DU CODE /////////////

int offset=-1;

int MEM[TAILLEMEM];///pile de la machine
int SP;///pointeur de la pile
typedef enum  {
	ADD, SUB, MUL, DIV,
	EQL, NEQ, GTR, LSS,
	GEQ, LEQ, PRN, INN,
	INT, LDI, LDA, LDV,
	STO, BRN, BZE, HLT
}MNEMONIQUE;

char * pcode[]={"ADD", "SUB", "MUL", "DIV",
	"EQL", "NEQ", "GTR", "LSS",
	"GEQ", "LEQ", "PRN", "INN",
	"INT", "LDI", "LDA", "LDV",
	"STO", "BRN", "BZE", "HLT"};
typedef struct {
    MNEMONIQUE mne;
    int suite;
}instruction;

instruction PCODE[TAILLECODE];
int pc=0;


void GENERER1 (MNEMONIQUE M){
        if(pc==TAILLECODE)
            perror("debordoment de pile");
        pc=pc+1;
        PCODE[pc].mne = M;

}
void GENERER2(MNEMONIQUE M , int A){
    if(pc==TAILLECODE)
            perror("debordoment de pile");
        pc=pc+1;
        PCODE[pc].mne = M;
        PCODE[pc].suite=A;
}
int get_symbole_table_symole(char *nom);
T_TAB_IDF sym_t;///store the identifier of T_TAB_IDF
int VAL_NOMBRE;

/*****************************MAIN PROGRAM**************************************/
int main()
{
    /*CONSTRUCT_SCANNER("./SOURCE.c");
    INIT_MOTS_CLES();
    LIRE_CAR();
    while(scanner.CAR_COUR!=scanner.Eof){
       SYM_SUIV();
        printf("nom : %s\t\tcode: %d\t\tcode:%s\n",scanner.SYMB_COUR.nom,scanner.SYMB_COUR.code,TOKEN_CLE[scanner.SYMB_COUR.code]);
    }
*/
CONSTRUCT_SCANNER("./test00.txt");
INIT_MOTS_CLES();
LIRE_CAR();
SYM_SUIV();
parser();
PROGRAM();
if (scanner.SYMB_COUR.code==EOF_TOKEN)
			printf("Analyse symantique reussie");
		else
			ERREUR_LEXICAL(EOF_ERR);

int i =0;
for(;i<=pc;i++)
    printf(" %d %d %s\n",PCODE[i].mne,PCODE[i].suite,pcode[PCODE[i].mne]);
    return 0;
}
/****************************************************************************/

/********************DEFINITION FONCTION ANALYSEUR LEXICAL*********************/

void CHECK_FILE_EXISTANCE(char* file_name){
    if(!access(file_name, F_OK )){
        printf("The File %s\t was Found\n",file_name);
    }
    else{
         ERREUR_LEXICAL(CAR_INC_ERR);
    }
}
void CONSTRUCT_SCANNER(char *file_name){
    CHECK_FILE_EXISTANCE(file_name);
    scanner.fluxSource = fopen(file_name,"r");
    if(scanner.fluxSource == NULL)
        ERREUR_LEXICAL(FIC_VID_ERR);
        scanner.Eof = '\0';
    scanner.mots_cles=(pile *) calloc(2, sizeof(pile));
}
void INIT_MOTS_CLES(){
    initialisep(scanner.mots_cles);
    ajouter(scanner.mots_cles,"Begin",BEGIN_TOKEN);
    ajouter(scanner.mots_cles,"end",END_TOKEN);
    ajouter(scanner.mots_cles,"if",IF_TOKEN);
    ajouter(scanner.mots_cles,"then",THEN_TOKEN);
    ajouter(scanner.mots_cles,"while",WHILE_TOKEN);
    ajouter(scanner.mots_cles,"do",DO_TOKEN);
    ajouter(scanner.mots_cles,"read",READ_TOKEN);
    ajouter(scanner.mots_cles,"write",WRITE_TOKEN);
    ajouter(scanner.mots_cles,"const",CONST_TOKEN);
    ajouter(scanner.mots_cles,"program",PROGRAM_TOKEN);
    ajouter(scanner.mots_cles,"var",VAR_TOKEN);

}

void CODAGE_LEX(){
    SYMBOLES *courant;
    int i,taille_tab_mot_cle=scanner.mots_cles->taill;
    courant = scanner.mots_cles->premier;
    for(i=0;i<taille_tab_mot_cle;i++){
        if(strcmp(scanner.SYMB_COUR.nom,courant->nom)==0){
            scanner.SYMB_COUR.code=courant->code;
            return;
        }
        courant = courant->suiv;
    }
    scanner.SYMB_COUR.code=ID_TOKEN;
}

void LIRE_CAR(){
    scanner.CAR_COUR = fgetc(scanner.fluxSource);
    if(scanner.CAR_COUR  == EOF)
         scanner.CAR_COUR='\0';
}
void LIRE_MOT(){
    int i =0;
    scanner.SYMB_COUR.nom[i]=scanner.CAR_COUR;
    LIRE_CAR();
		while(isalnum(scanner.CAR_COUR)) {
            i++;
            scanner.SYMB_COUR.nom[i]=scanner.CAR_COUR;
			LIRE_CAR();
		}
		CODAGE_LEX();
}

void LIRE_NOMBRE(){
    int i =0;
    scanner.SYMB_COUR.nom[i]=scanner.CAR_COUR;
    LIRE_CAR();
		while(isdigit(scanner.CAR_COUR)) {
            i++;
            scanner.SYMB_COUR.nom[i]=scanner.CAR_COUR;
			LIRE_CAR();
		}
		scanner.SYMB_COUR.code=NUM_TOKEN;
		VAL_NOMBRE=atoi(scanner.SYMB_COUR.nom);
}

int IS_SEPARATOR(){
    if(scanner.CAR_COUR==' ' || scanner.CAR_COUR=='\n' ||scanner.CAR_COUR=='\t' )
        return 1;
    else
        return 0;

}
void AFFICHER_TOKEN(SYMBOLES symbole){
printf("\n code token:%d nom token:%s",symbole.code,symbole.nom);
}
/*********jjjgjg*********8jgjgjjg*/
void SYM_SUIV(){
			    char car_precedent;

	memset(scanner.SYMB_COUR.nom,'\0',20);
        while(IS_SEPARATOR())
			LIRE_CAR();
        if(scanner.CAR_COUR=='{'){
            LIRE_CAR();
            switch(scanner.CAR_COUR) {
			case '*':
			    LIRE_CAR();
			    car_precedent=scanner.CAR_COUR;
			    if(scanner.CAR_COUR=='}'){
                   scanner.SYMB_COUR.code=ERREUR_TOKEN;
                   ERREUR_LEXICAL(CAR_INC_ERR);
			    }
                LIRE_CAR();
			    while(scanner.CAR_COUR!='}' &&scanner.CAR_COUR!=scanner.Eof){
                    car_precedent=scanner.CAR_COUR;
                    LIRE_CAR();
			    }
			    if(car_precedent!='*'){
                    scanner.SYMB_COUR.code=ERREUR_TOKEN;
                    ERREUR_LEXICAL(CAR_INC_ERR);
			    }
                LIRE_CAR();
                break;
			default:
				scanner.SYMB_COUR.code=ERREUR_TOKEN;
				ERREUR_LEXICAL(CAR_INC_ERR);
			}
        }
		while(IS_SEPARATOR())
			LIRE_CAR();
		if (isalpha(scanner.CAR_COUR)) {
			LIRE_MOT();
			return;
		}
		if(isdigit(scanner.CAR_COUR)) {
			LIRE_NOMBRE();
			return;
		}
		switch(scanner.CAR_COUR) {
		case '+':
            scanner.SYMB_COUR.code=PLUS_TOKEN;
			LIRE_CAR();
			break;
		case '-':
		    strcpy(scanner.SYMB_COUR.nom,"-");
			scanner.SYMB_COUR.code=MOINS_TOKEN;
			LIRE_CAR();
			break;
		case '*':
            strcpy(scanner.SYMB_COUR.nom,"*");
			scanner.SYMB_COUR.code=MULT_TOKEN;
			LIRE_CAR();
			break;
		case '/':
            strcpy(scanner.SYMB_COUR.nom,"/");
			scanner.SYMB_COUR.code=DIV_TOKEN;
			LIRE_CAR();
			break;
		case '(':
            strcpy(scanner.SYMB_COUR.nom,"(");
			scanner.SYMB_COUR.code=PO_TOKEN;
			LIRE_CAR();
			break;
		case ')':
		    strcpy(scanner.SYMB_COUR.nom,")");
			scanner.SYMB_COUR.code=PF_TOKEN;
			LIRE_CAR();
			break;
		case '.':
		    strcpy(scanner.SYMB_COUR.nom,".");
			scanner.SYMB_COUR.code=PT_TOKEN;
			LIRE_CAR();
			break;
		case ',':
		    strcpy(scanner.SYMB_COUR.nom,",");
			scanner.SYMB_COUR.code=VIR_TOKEN;
			LIRE_CAR();
			break;

		case ';':
		    strcpy(scanner.SYMB_COUR.nom,";");
			scanner.SYMB_COUR.code=PTVIR_TOKEN;
			LIRE_CAR();
			break;
		case '=':
            strcpy(scanner.SYMB_COUR.nom,"=");
			scanner.SYMB_COUR.code=EG_TOKEN;
			LIRE_CAR();
			break;
		case EOF:
			scanner.SYMB_COUR.code=EOF_TOKEN;
			break;
		case ':':
			LIRE_CAR();
			switch(scanner.CAR_COUR) {
			case '=':
			    strcpy(scanner.SYMB_COUR.nom,":=");
				scanner.SYMB_COUR.code=AFF_TOKEN;
				LIRE_CAR();
				break;
			default:
				scanner.SYMB_COUR.code=ERREUR_TOKEN;
				ERREUR_LEXICAL(CAR_INC_ERR);
			}
			break;
		case '>':
			LIRE_CAR();
			switch(scanner.CAR_COUR) {
			case '=':
			    strcpy(scanner.SYMB_COUR.nom,">=");
				scanner.SYMB_COUR.code=SUPEG_TOKEN;
				LIRE_CAR();
				break;
			default:
			    strcpy(scanner.SYMB_COUR.nom,">");
				scanner.SYMB_COUR.code=SUP_TOKEN;
				break;
			}
			break;
                    case '<':
                        LIRE_CAR();
                        switch(scanner.CAR_COUR) {
                        case '=':
			    strcpy(scanner.SYMB_COUR.nom,"<=");
				scanner.SYMB_COUR.code=INFG_TOKEN;
				LIRE_CAR();
				break;
			case '>':
			    strcpy(scanner.SYMB_COUR.nom,"<>");
				scanner.SYMB_COUR.code=DIFF_TOKEN;
				LIRE_CAR();
				break;
			default:
			    strcpy(scanner.SYMB_COUR.nom,"<");
				scanner.SYMB_COUR.code=INF_TOKEN;
				break;
			}
			break;
        case '\0':
            strcpy(scanner.SYMB_COUR.nom,"eof");
			scanner.SYMB_COUR.code=EOF_TOKEN;
			LIRE_CAR();
			break;
		default:
			ERREUR_LEXICAL(CAR_INC_ERR);
		}
}

/***************************************/
void initialisep(pile *lp)
{
  lp->premier=NULL;
  lp->taill=0;
}

void ajouter(pile *lp,char *nom,CODES_LEX code)
{
    SYMBOLES *nouv = (SYMBOLES*)malloc(sizeof(SYMBOLES));
     strcpy (nouv->nom, nom);
     nouv->code=code;
     if(lp==NULL)
     {
         lp->premier=nouv;
         lp->taill++;
     }
     else{
        nouv->suiv=lp->premier;
        lp->premier=nouv;
        lp->taill++;
     }
}
void ajouterT(pile *lp,char *nom,TSYM code)
{
    T_TAB_IDF *nouv = (T_TAB_IDF*)malloc(sizeof(T_TAB_IDF));
     strcpy (nouv->NOM, nom);
     nouv->TIDF=code;
     offset++;
     nouv->adresse=offset;
     if(lp==NULL)
     {
         lp->premier=nouv;
         lp->taill++;
     }
     else{
        nouv->suiv=lp->premier;
        lp->premier=nouv;
        lp->taill++;
     }
}
void affiche (pile *pp){
  T_TAB_IDF *courant;
  int i;
  courant = pp->premier;

  for(i=0;i<pp->taill;++i){
    printf("\n%s  %d \n", courant->NOM,courant->TIDF);
    courant = courant->suiv;
  }
}


///////////////////////////////////////////

void parser(){
    TAB_IDFS=(T_TAB_IDF *) calloc(2, sizeof(T_TAB_IDF));

    initialisep(TAB_IDFS);
}
void TEST_SYMBOLE(CODES_LEX cl,CODES_ERR ce){


    if(scanner.SYMB_COUR.code!=cl)
        ERREUR_SANTAXIQUE(ce);
}
///program ID ; BLOCK .
void PROGRAM(){
    TEST_SYMBOLE (PROGRAM_TOKEN,PROGRAM_ERR);
    SYM_SUIV();
    TEST_SYMBOLE (ID_TOKEN,ID_ERR);
    SYM_SUIV();
    ajouterT(TAB_IDFS,scanner.SYMB_COUR.nom,TPROG);///semantic
    TEST_SYMBOLE (PTVIR_TOKEN,PTVIR_ERR);
    SYM_SUIV();
    BLOCK();
    GENERER1(HLT);
    TEST_SYMBOLE(PT_TOKEN,PT_ERR);
    SYM_SUIV();

}
//CONSTS VARS INSTS /////////
void BLOCK(){
        offset =0;
    CONSTS();
    VARS();
    PCODE[0].mne = INT;
    PCODE[0].suite =offset;
    INSTS();
}
//const ID = NUM ; { ID = NUM ; } | e ///////
void CONSTS(){

    switch(scanner.SYMB_COUR.code){
    case CONST_TOKEN:
        SYM_SUIV();
        TEST_SYMBOLE(ID_TOKEN,ID_ERR);
        UNAUTORISER_PROGRAM_ID(scanner.SYMB_COUR.nom);///semantic
        ajouterT(TAB_IDFS,scanner.SYMB_COUR.nom,TCONST);///semantic
        ///GENERATION DE CODE
        GENERER2(LDA,get_symbole_table_symole(scanner.SYMB_COUR.nom));
        ///
        SYM_SUIV();
        TEST_SYMBOLE(EG_TOKEN,EG_ERR);
        SYM_SUIV();
        TEST_SYMBOLE(NUM_TOKEN,NUM_ERR);
        ///GENERATION DE CODE
        GENERER2(LDI,VAL_NOMBRE);
        GENERER1(STO);
        ///
        SYM_SUIV();
        TEST_SYMBOLE(PTVIR_TOKEN,PTVIR_ERR);
        SYM_SUIV();
        while(scanner.SYMB_COUR.code==ID_TOKEN){
            UNAUTORISER_PROGRAM_ID(scanner.SYMB_COUR.nom);///semantic
            IDF_DOUBLE_DECLARED(scanner.SYMB_COUR.nom);///semantic
            ajouterT(TAB_IDFS,scanner.SYMB_COUR.nom,TCONST);///semantic
            ///GENERATION DE CODE
            GENERER2(LDA,get_symbole_table_symole(scanner.SYMB_COUR.nom));
            ///
            SYM_SUIV();
            TEST_SYMBOLE(EG_TOKEN,EG_ERR);
            SYM_SUIV();
            TEST_SYMBOLE(NUM_TOKEN,NUM_ERR);
             ///GENERATION DE CODE
            GENERER2(LDI,VAL_NOMBRE);
            GENERER1(STO);
            ///
            SYM_SUIV();
            TEST_SYMBOLE(PTVIR_TOKEN,PTVIR_ERR);
            SYM_SUIV();
        }
        break;
    case VAR_TOKEN :
        break;
    case BEGIN_TOKEN:
        break;
    default:
        ERREUR_SANTAXIQUE(CONSTS_ERR);
    }

}
//var ID { , ID } ; | e  ////////
void VARS(){
    switch(scanner.SYMB_COUR.code){
    case VAR_TOKEN:
        SYM_SUIV();
        TEST_SYMBOLE(ID_TOKEN,ID_ERR);
            UNAUTORISER_PROGRAM_ID(scanner.SYMB_COUR.nom);///semantic
            IDF_DOUBLE_DECLARED(scanner.SYMB_COUR.nom);///semantic
            ajouterT(TAB_IDFS,scanner.SYMB_COUR.nom,TVAR);///semantic
            SYM_SUIV();
        while(scanner.SYMB_COUR.code==VIR_TOKEN){

            SYM_SUIV();
            TEST_SYMBOLE(ID_TOKEN,ID_ERR);
            UNAUTORISER_PROGRAM_ID(scanner.SYMB_COUR.nom);///semantic
            IDF_DOUBLE_DECLARED(scanner.SYMB_COUR.nom);///semantic
            ajouterT(TAB_IDFS,scanner.SYMB_COUR.nom,TVAR);///semantic
            SYM_SUIV();
        }
        TEST_SYMBOLE(PTVIR_TOKEN,PTVIR_ERR);
        SYM_SUIV();
        break;

    case BEGIN_TOKEN:
        break;
    default:
        ERREUR_SANTAXIQUE(VARS_ERR);

    }


}
//begin INST { ; INST } end //////
void INSTS(){
    TEST_SYMBOLE(BEGIN_TOKEN,BEGIN_ERR);
    SYM_SUIV();
    INST();
    while(scanner.SYMB_COUR.code==PTVIR_TOKEN){
        SYM_SUIV();
        INST();
    }
    TEST_SYMBOLE(END_TOKEN,END_ERR);
    SYM_SUIV();
}
// INSTS | AFFEC | SI | TANTQUE | ECRIRE | LIRE | e //////////
void INST(){
    switch(scanner.SYMB_COUR.code) {
		case BEGIN_TOKEN :
			INSTS();
			break;
		case ID_TOKEN:
			AFFEC();
			break;
		case IF_TOKEN:
			SI();
			break;
		case WHILE_TOKEN:
			TANQUE();
			break;
		case WRITE_TOKEN:
			ECRIRE();
			break;
		case READ_TOKEN:
			LIRE();
			break;
        case FOR_TOKEN:
            POUR();
            break;
        case REPEAT_TOKEN:
            REPETER();
            break;
        case CASE_TOKEN:
            CAS();
            break;
        case END_TOKEN:
			break;
		case PTVIR_TOKEN:
			break;

		default:
			ERREUR_SANTAXIQUE(INST_ERR);
		}
}
// ID := EXPR /////////////
void AFFEC(){
    TEST_SYMBOLE(ID_TOKEN,ID_ERR);

    UNAUTORISER_PROGRAM_ID(scanner.SYMB_COUR.nom);///semantic
    IDF_NON_DECLARED(scanner.SYMB_COUR.nom);///semantic
    UNCHANBLE_CONST_VALUE(scanner.SYMB_COUR.nom);
    ///GENERATION DE CODE
    GENERER2(LDA,get_symbole_table_symole(scanner.SYMB_COUR.nom));
    ///
    SYM_SUIV();
    TEST_SYMBOLE(AFF_TOKEN,AFFEC_ERR);
    SYM_SUIV();
    EXPR();
    ///GENERATION DE CODE
    GENERER1(STO);
    ///

}
//if COND then INST //////////////
void SI(){
    TEST_SYMBOLE(IF_TOKEN,IF_ERR);
    SYM_SUIV();
    COND();
    ///GENERATION DE CODE
    GENERER1(BZE);
    int IND_BZE = pc;
    ///
    TEST_SYMBOLE(THEN_TOKEN,THEN_ERR);
    SYM_SUIV();
    INST();
    GENERER1(BRN);
    int INDBRN = pc;
    ///generation de code
    PCODE[IND_BZE].suite=pc +1;

    if(ELSE_TOKEN){
        INST();


    }
     PCODE[INDBRN].suite=pc +1;

}
// For  ID=NUM [to | downto] NUM do INST
void POUR(){
    int num_start;
    int num_end;
    int to_down; ///up=0 , down =1

    TEST_SYMBOLE(FOR_TOKEN,FOR_ERR);
    SYM_SUIV();
    TEST_SYMBOLE(ID_TOKEN,ID_ERR);
    UNAUTORISER_PROGRAM_ID(scanner.SYMB_COUR.nom);///semantic
    IDF_NON_DECLARED(scanner.SYMB_COUR.nom);///semantic
    UNCHANBLE_CONST_VALUE(scanner.SYMB_COUR.nom);/// semantic

    SYM_SUIV();
    TEST_SYMBOLE(NUM_TOKEN,NUM_ERR);
    num_start = VAL_NOMBRE;
    SYM_SUIV();

    switch(scanner.SYMB_COUR.code){
    case TO_TOKEN:
        to_down=0;
        SYM_SUIV();
        break;
    case DOWNTO_TOKEN:
        to_down=1;
        SYM_SUIV();
        break;
    default :
        ERREUR_SYMANTIQUE(FOR_ERR);
        break;
    }
    TEST_SYMBOLE(NUM_TOKEN,NUM_ERR);
    num_end = VAL_NOMBRE;
    SYM_SUIV();
    TEST_SYMBOLE(DO_TOKEN,DO_ERR);
    SYM_SUIV();

    ///GENERER2(lda , id_addr)
    ///generer2(ldi, num_start)
    /// generer1(sto)
    INST();





}
// Repeat INST until COND
void REPETER(){
    TEST_SYMBOLE(REPEAT_TOKEN,REPEAT_ERR);
    SYM_SUIV();
    INST();
    TEST_SYMBOLE(UNTIL_TOKEN,UNTIL_ERR);
    COND();
}
// case ID of NUM : INST {;NUM : INST} [ELSE INST |EPSILON] end
void CAS(){
    TEST_SYMBOLE(CASE_TOKEN,CASE_ERR);
    TEST_SYMBOLE(ID_TOKEN,ID_ERR);
    TEST_SYMBOLE(OF_TOKEN,OF_ERR);

}

//while COND do INST
void TANQUE(){
    TEST_SYMBOLE(WHILE_TOKEN,WHILE_ERR);
    SYM_SUIV();
     ///GENERATION DE CODE
    int LABEL_BRN=pc+1;
    ///
    COND();
     ///GENERATION DE CODE
    GENERER1(BZE);
    int IND_BZE = pc;
    ///
    TEST_SYMBOLE(DO_TOKEN,DO_ERR);
    SYM_SUIV();
    INST();
     ///GENERATION DE CODE
     GENERER2(BRN,LABEL_BRN);
    PCODE[IND_BZE].suite=pc +1;
    ///
}
//write ( EXPR { , EXPR } )//////////////////
void ECRIRE(){
    TEST_SYMBOLE(WRITE_TOKEN,WRITE_ERR);
    SYM_SUIV();
    TEST_SYMBOLE(PO_TOKEN,PO_ERR);
    SYM_SUIV();
    EXPR();
    ///GENERATION DE CODE
    GENERER1(PRN);
    ///
    while(scanner.SYMB_COUR.code==VIR_TOKEN){
        SYM_SUIV();
        EXPR();
        ///GENERATION DE CODE
        GENERER1(PRN);
        ///

    }
    TEST_SYMBOLE(PF_TOKEN,PF_ERR);
    SYM_SUIV();

}
// read ( ID { , ID } )/////////////////
void LIRE(){
    TEST_SYMBOLE(READ_TOKEN,READ_ERR);
    SYM_SUIV();
    TEST_SYMBOLE(PO_TOKEN,PO_ERR);
    SYM_SUIV();
    TEST_SYMBOLE(ID_TOKEN,ID_ERR);
    UNAUTORISER_PROGRAM_ID(scanner.SYMB_COUR.nom);///semantic
    IDF_NON_DECLARED(scanner.SYMB_COUR.nom);///semantic
    UNCHANBLE_CONST_VALUE(scanner.SYMB_COUR.nom);///semantic
    ///GENERATION DE CODE
    GENERER2(LDA,get_symbole_table_symole(scanner.SYMB_COUR.nom));
    GENERER1(INN);
    ///
    SYM_SUIV();
    while(scanner.SYMB_COUR.code==VIR_TOKEN){
        SYM_SUIV();
        TEST_SYMBOLE(ID_TOKEN,ID_ERR);
        ///GENERATION DE CODE
        GENERER2(LDA,get_symbole_table_symole(scanner.SYMB_COUR.nom));
        GENERER1(INN);
        ///
        SYM_SUIV();
    }
    TEST_SYMBOLE(PF_TOKEN,PF_ERR);
    SYM_SUIV();
}
//EXPR [= | <> | < | > | <= | >=] EXPR
void COND(){
    EXPR();
    MNEMONIQUE m;
    	switch(scanner.SYMB_COUR.code) {
		case EG_TOKEN:
		    GENERER1(EQL);
			SYM_SUIV();
			break;
		case DIFF_TOKEN:
            GENERER1(NEQ);
			SYM_SUIV();
			break;
		case INF_TOKEN:
		    GENERER1(LSS);
			SYM_SUIV();
			break;
		case SUP_TOKEN:
		    GENERER1(GTR);
			SYM_SUIV();
			break;
		case INFG_TOKEN:
		    GENERER1(LEQ);
			SYM_SUIV();
			break;
		case SUPEG_TOKEN:
		    GENERER1(GEQ);
			SYM_SUIV();
			break;
		default:
		    ERREUR_SANTAXIQUE(RELOP_ERR);
		}
    EXPR();
    GENERER1(m);
}
//TERM { [+ | -] TERM }
void EXPR(){
    TERM();

    while(scanner.SYMB_COUR.code==PLUS_TOKEN || scanner.SYMB_COUR.code==MOINS_TOKEN){
       MNEMONIQUE m;
        switch (scanner.SYMB_COUR.code){
        case PLUS_TOKEN:
            m=ADD;
            break;
        case MOINS_TOKEN:
            m=SUB;
            break;
        }
        SYM_SUIV();
        TERM();
        GENERER1(m);
    }
}
// FACT { [* | /] FACT } ////////////////////
void TERM(){
    FACT();
    while(scanner.SYMB_COUR.code==MULT_TOKEN || scanner.SYMB_COUR.code==DIV_TOKEN){
         MNEMONIQUE m;
        switch (scanner.SYMB_COUR.code){
        case MULT_TOKEN:
            m=MUL;
            break;
        case DIV_TOKEN:
            m=DIV;
            break;
        }
        SYM_SUIV();
        FACT();
        GENERER1(m);
    }
}
// ID | NUM | ( EXPR )/////////////////
void FACT(){
    switch(scanner.SYMB_COUR.code){
    case ID_TOKEN:
        UNAUTORISER_PROGRAM_ID(scanner.SYMB_COUR.nom);///semantic
        IDF_NON_DECLARED(scanner.SYMB_COUR.nom);///semantic
        ///GENERATION DE CODE
        GENERER2(LDA,get_symbole_table_symole(scanner.SYMB_COUR.nom));
        GENERER1(LDV);
        ///
        SYM_SUIV();
        break;
    case NUM_TOKEN:
        ///GENERATION DE CODE
        GENERER2(LDI,VAL_NOMBRE);
        ///
        SYM_SUIV();
        break;
    case PO_TOKEN:
        SYM_SUIV();
        EXPR();
        TEST_SYMBOLE(PF_TOKEN,PF_ERR);
        SYM_SUIV();
    default:
        ERREUR_SANTAXIQUE(FACT_ERR);
    }

}
//////////////////////////////////////////////
void IDF_NON_DECLARED(char *nom){

    T_TAB_IDF *courant;
    int i,taille_tab_mot_cle=TAB_IDFS->taill;
    courant = TAB_IDFS->premier;
    for(i=0;i<taille_tab_mot_cle;i++){
        if(strcmp(nom,courant->NOM)==0){
            return;
        }
        courant = courant->suiv;
    }
    ERREUR_SYMANTIQUE(UNDECLARED_ID_ERR);
}

void IDF_DOUBLE_DECLARED(char *nom){
    T_TAB_IDF *courant;
    int i,taille_tab_mot_cle=TAB_IDFS->taill;
    courant = TAB_IDFS->premier;
    for(i=0;i<taille_tab_mot_cle;i++){
        if(strcmp(nom,courant->NOM)==0){
            ERREUR_SYMANTIQUE(DOUBLE_DECLARATION);
        }
        courant = courant->suiv;
    }
}
void UNAUTORISER_PROGRAM_ID(char *nom){
    T_TAB_IDF *courant;
    int i,taille_tab_mot_cle=TAB_IDFS->taill;
    courant = TAB_IDFS->premier;
    for(i=0;i<taille_tab_mot_cle;i++){
        if(strcmp(nom,courant->NOM)==0 && courant->TIDF==TPROG){
            ERREUR_SYMANTIQUE(UNAUTORISER_PROGRAM_ID_ERR);
        }
        courant = courant->suiv;
    }
}
void UNCHANBLE_CONST_VALUE(char *nom){
    T_TAB_IDF *courant;
    int i,taille_tab_mot_cle=TAB_IDFS->taill;
    courant = TAB_IDFS->premier;
    for(i=0;i<taille_tab_mot_cle;i++){
        if(strcmp(nom,courant->NOM)==0 && courant->TIDF==TCONST){
            ERREUR_SYMANTIQUE(CONST_VALUE_CHANGE_ERR);
        }
        courant = courant->suiv;
    }


}
int get_symbole_table_symole(char *nom){

   T_TAB_IDF *courant;
    int i,taille_tab_mot_cle=TAB_IDFS->taill;
    courant = TAB_IDFS->premier;
    for(i=0;i<taille_tab_mot_cle;i++){
        if(strcmp(nom,courant->NOM)==0){
            return courant->adresse;
        }
        courant = courant->suiv;
    }
}
