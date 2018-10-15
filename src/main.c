#include "parser.h"
#include "process.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>



#define TAM_STR 50
#define TAM_STR_MAIN 100

pid_t daddyold;
pid_t daddy;
char ** buffer_safe_mode;
char const ** FILES;
int * sizes;
int * ocupados_a;
int arg;

/**
	@brief			Função responsável por criar um backup do ficheiro inicial.
	@param  source 	Path para o file
*/

void backup_read(char const * source,int o){

	int fd = open(source, O_RDONLY , 00644);
	int size;

	if (fd == -1){
		perror("Não conseguiu abrir a porta do ficheiro.");
		_exit(-1);
	}

	buffer_safe_mode[o] = malloc(100*sizeof(char));
	size = 100;

	char c, * aux;
	int i = 0,n;

	while((n = read(fd,&c,1)) > 0){
		buffer_safe_mode[o][i++] = c;
		if (i == size){
			aux = buffer_safe_mode[o];
			buffer_safe_mode[o] = malloc(2*size*sizeof(char));
			for(n = 0; n < i ; n++)
				buffer_safe_mode[o][n] = aux[n];
			free(aux);
			size *= 2;
		}
	}
	sizes[o]= size;
	ocupados_a[o] = i;
	close(fd);
}

/**
	@brief			Função responsável escrever o backup no ficheiro.
*/

void backup_write(int i){
	int fd = open(FILES[i], O_WRONLY | O_TRUNC , 00644);

	if (fd == -1){
		perror("Não conseguiu abrir a porta do ficheiro.");
		_exit(-1);
	}

	write(fd,buffer_safe_mode[i],ocupados_a[i]);
	close(fd);
}

/**
	@brief			Função utilizada no SIGINT.
*/


void kill_all(int i){

	pid_t self = getpid();
    if (daddyold != self) _exit(-1);
	else{
		printf("A sair de todos os processos\n" );
		int i;
		for(i=0; i < arg; i++){
			if (sizes[i] != -1)
				backup_write(i);
		}
		_exit(0);
	}



/**
	@brief			Função que executa um ficheiro singular.
	@param   argv	Path para o ficheiro.
*/
}

int executaSingleFile(char const * argv){

	int fd = open(argv, O_RDONLY , 00644);

	if (fd == -1){
		perror("Não conseguiu abrir a porta do ficheiro.");
		_exit(-1);
	}

	LCMD aux = parser(fd);
	int d,r,p[2],status;
	LCMD * comandos = parser_split(aux,&r);
	close(fd);
	pid_t a;
	int linha,coluna,dependencias[r];
	char ** output = malloc(r*sizeof(char *));
	char * str_aux;

	if(!calculaDependencias(comandos,dependencias,r)){
		perror("Um dos comandos do tipo $n| não pode ser executado.");
		_exit(-1);
	}

	for(d = 0; d < r; d++){
		pipe(p);
		if(!fork()){
			close(p[0]);
			if (type(comandos[d]->comando) == 1){
				linha = posicaoArray(comandos,d,n_comando(comandos[d]->comando),&coluna);
				str_aux = parseFileToString(coluna,output[linha]);
				executa_n(comandos[d],p[1],str_aux);
			}
			else executa(comandos[d],p[1]);
			close(p[1]);
			_exit(0);
		}
		else{
			close(p[1]);
			str_aux = lerPipe(p[0]);
			wait(&status);
			if (WIFEXITED(status)){
				a = WEXITSTATUS(status);
				if (a == 255){
					_exit(-1);
				}
			}
			output[d] = str_aux;
		}
	}

	for(d=0; d < r; d++){
		aux = comandos[d];
		freeCMD(aux);
	}
	free(comandos);

	fd = open(argv, O_WRONLY | O_TRUNC, 00644);
	if (fd == -1){
		perror("Não conseguiu abrir a porta do ficheiro.");
		_exit(-1);
	}
	for(d = 0; d < r; d++){
		write(fd,output[d],strlen(output[d]));
	}

	close(fd);

	_exit(0);
}

/**
	@brief			Função que verifica se há dois ficheiros com o mesmo nome.
	@param   argv	Nome dos ficheiros.
	@param	 i		Indice em que se começa a procurar.
	@return 		Boolean com o resultado.
*/

int checkFileName(char const *argv[],int i){
	int d,counter = 0;
	for(d=i-1; d >= 1; d--){
		if (strcmp(argv[i],argv[d]) == 0)
			counter++;
	}

	return (counter == 0);
}


int main(int argc, char const *argv[]) {

	signal(SIGINT,kill_all);

	daddyold = getpid();


	if (argc < 2) {
		perror("Numero de argumentos inválido");
		exit(-1);
	}

	sizes = malloc((argc-1)*sizeof(int));
	ocupados_a = malloc((argc-1)*sizeof(int));
	buffer_safe_mode = malloc((argc-1)*sizeof(char *));
	FILES = malloc((argc-1)*sizeof(char *));

	int i;
	arg = argc-1;
	for(i=1; i < argc; i++){
		sizes[i-1] = -1;
		FILES[i-1] = argv[i];
	}

	for(i=1; i < argc; i++){
		if (checkFileName(argv,i)){
			if(!fork()){
				executaSingleFile(argv[i]);
				_exit(0);
			}
			else{
				backup_read(argv[i],i-1);
			}
		}
	}
	for(i=1; i < argc; i++){
		wait(NULL);
	}

	char * aux;
	free(ocupados_a);
	for(i=0; i < arg; i++){
		if (sizes[i] != -1){
			aux = buffer_safe_mode[i];
			free(aux);
		}
	}
	free(buffer_safe_mode);
	free(FILES);
	free(sizes);

	_exit(0);
}
