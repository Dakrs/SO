#ifndef __PROCESS__
#define __PROCESS__

#include "parser.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <ctype.h>


void 	escreveFicheiroAux		(int fp, char * str , LCMD comando);
void 	juntaFildes				(int d_pai,int d_max_filho,LCMD comando,char ** buffer);
int  	executa					(LCMD comando,int fd_origin);
int 	executa_n				(LCMD comando,int fd_origin,char * input);
int  	type					(char * source);
int  	length					(LCMD a);
int  	posicaoArray			(LCMD * l, int pos, int n,	int * ele);
int  	n_comando				(char * source);
int  	calculaDependencias		(LCMD * comandos, int * v,int r);
char * 	lerPipe					(int fd);

#endif
