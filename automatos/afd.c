#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#define IMPRIME

int main (int argc, char **argv){
	int i, j, k, flag = 0;
	int n_let_alf = 3;	//Numero de letras do alfabeto
	int n_est_aut = 5;	//Numero de Estados do Automato
	int n_est_fin = 3;	//Numero de Estados Finais
	int n_trans_max = n_let_alf * n_est_aut;		//Numero maximo de transicoes em um AFD
	char alfabeto[] = {'A', 'B', 'C', '\0'};		//Letras do Alfabeto
	char estados[] = "q0 q1 q2 q3 q4";				//Estados do Automato
	char transicoes[] = "q1 -- q3 q1 q2 q3 q3 q3 q3 q3 q3 q4 q3 q3 q0";	//Funcao de transicao Ex:(q0A q0B q1A q1B q2A q2B q3A q3B)
	char transi_est[] = "q0 q0 q0 q1 q1 q1 q2 q2 q2 q3 q3 q3 q4 q4 q4";	//Funcao de transicao Estados
	char transi_var[] = {'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C', '\0'};	//Funcao de transicao Estados
	char est_inicial[] = "q0";						//Estado inicial
	char est_finais[] = "q1 q3 q4"; 				//Conjunto de estados finais
	char *palavra;
	FILE *saida1, *saida2;
	char **est, **trans, **trans_est, **est_fin, *atual, *prox;
	char *cmd=NULL;
	saida1 = fopen("automato_inicial.dot", "w");
	saida2 = fopen("automato_minimizado.dot", "w");
	
	if(argc!=2){
		printf("Digite: %s <palavra>\n\n", argv[0]);
		exit(-1);
	}
	
	//ALOCACAO DOS VALORES DE ENTRADA
	est = (char**)calloc(n_est_aut, sizeof(char*));
	for(i=0; i<n_est_aut; i++){
		est[i] = (char*)calloc(3, sizeof(char));
	}
	trans = (char**)calloc(n_trans_max, sizeof(char*));
	for(i=0; i<n_trans_max; i++){
		trans[i] = (char*)calloc(3, sizeof(char));
	}
	trans_est = (char**)calloc(n_trans_max, sizeof(char*));
	for(i=0; i<n_trans_max; i++){
		trans_est[i] = (char*)calloc(3, sizeof(char));
	}
	est_fin = (char**)calloc(n_est_fin, sizeof(char*));
	for(i=0; i<n_est_fin; i++){
		est_fin[i] = (char*)calloc(3, sizeof(char));
	}
	atual = (char*)calloc(3, sizeof(char));
	prox = (char*)calloc(3, sizeof(char));
	palavra = (char*)calloc(strlen(argv[1]), sizeof(char));
	strcpy(palavra, argv[1]);
	
	//SEPARA ESTADOS DO AUTOMATO
	cmd = strtok(estados, " ");
	strcpy(est[0], cmd);
	for(i=1; i<n_est_aut; i++){
		cmd = strtok(NULL, " ");
		strcpy(est[i], cmd);
	}
		#ifdef IMPRIME
		for(i=0; i<n_est_aut; i++){
			printf("est[%d]: %s\n", i, est[i]);
		}
		#endif
	
	//SEPARA TRANSICOES
	cmd = strtok(transicoes, " ");
	strcpy(trans[0], cmd);
	for(i=1; i<n_trans_max; i++){
		cmd = strtok(NULL, " ");
		strcpy(trans[i], cmd);
	}
		#ifdef IMPRIME
		for(i=0; i<n_trans_max; i++){
			printf("trans[%d]: %s\n", i, trans[i]);
		}
		#endif
	
	//SEPARA TRANSICOES ESTADOS
	cmd = strtok(transi_est, " ");
	strcpy(trans_est[0], cmd);
	for(i=1; i<n_trans_max; i++){
		cmd = strtok(NULL, " ");
		strcpy(trans_est[i], cmd);
	}
		#ifdef IMPRIME
		for(i=0; i<n_trans_max; i++){
			printf("trans_est[%d]: %s\n", i, trans_est[i]);
		}
		#endif
	
	//SEPARA ESTADOS FINAIS
	cmd = strtok(est_finais, " ");
	strcpy(est_fin[0], cmd);
	for(i=1; i<n_est_fin; i++){
		cmd = strtok(NULL, " ");
		strcpy(est_fin[i], cmd);
	}
		#ifdef IMPRIME
		for(i=0; i<n_est_fin; i++){
			printf("est_fin[%d]: %s\n", i, est_fin[i]);
		}
		#endif
		
	/********************************************************/
	//MONTA o AUTOMATO INICIAL
	fprintf(saida1, "digraph {\n");
	for (i=0; i<n_est_aut; i++){
		for(j=0; j<n_let_alf; j++){
			if(strcmp(trans[i*n_let_alf+j], "--")!=0){
				fprintf(saida1, "\t%s -> %s [label=\"%c\", weight=100];\n", trans_est[i*n_let_alf+j], trans[i*n_let_alf+j], alfabeto[j]);
			}
		}
	}
	fprintf(saida1, "\t%s [label=\"Inicio\n%s\", color=blue, style=filled];\n", est_inicial, est_inicial);
	for(i=0; i<n_est_fin; i++){
		fprintf(saida1, "\t%s [label=\"Final\n%s\", color=green, style=filled];\n", est_fin[i], est_fin[i]);
	}
	fprintf(saida1, "}");
	
	//TESTA SE A PALAVRA E ACEITA
	strcpy(atual, est_inicial);
	for(i=0; i<strlen(palavra); i++){
		for(j=0; j<n_let_alf; j++){
			for(k=0; k<n_est_fin; k++){
				if(strcmp(atual, est_fin[k]) == 0){
					printf("A palavra %s foi Aceita!\n\n", palavra);
					break;
				}
			}
			if((palavra[i] == transi_var[j]) && (strcmp(atual, trans_est[j]) == 0) && (strcmp(trans[j], "--") != 0)){
				strcpy(atual, trans[j]);
			}
			else{
				flag = 1;
				break;
			}
		}
		if(flag == 1){
			printf("A palavra %s foi rejeitada!\n\n", palavra);
			break;
		}
	}
	/********************************************************/
	
	//DESALOCACAO DOS VALORES DE ENTRADA
	for(i=0; i<n_est_aut; i++){
		free(est[i]);
	}
	free(est);
	for(i=0; i<n_trans_max; i++){
		free(trans[i]);
	}
	free(trans);
	for(i=0; i<n_trans_max; i++){
		free(trans_est[i]);
	}
	free(trans_est);
	for(i=0; i<n_est_fin; i++){
		free(est_fin[i]);
	}
	free(est_fin);
	fclose(saida1);
	fclose(saida2);
	system("dot -Tpng automato_inicial.dot -o automato_inicial.png");
	return 0;
}
