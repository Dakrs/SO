#!/bin/bash
echo "1 para gerar automaticamente, 2 para configurar notebook"

read n
#construir ficheiros automaticamente (definidos aqui)
if [[ $n == 1 ]]; then
	#Primeira linha de cada ficheiro
	echo "ex1.nb" > ex1.nb
	echo "ex2.nb" > ex2.nb



	#Seguintes linhas de cada ficheiro
	echo "$ ls -l" >> ex1.nb
	echo "$| wc" >> ex1.nb

	echo "lista ps" >> ex2.nb
	echo "$   ps" >> ex2.nb
	echo "a segunda cena" >> ex2.nb
	echo "$|  head -2" >> ex2.nb
	echo "wordcount é fixe" >> ex2.nb
	echo "$1| wc" >> ex2.nb
	echo "grep a pid" >> ex2.nb
	echo "$3| grep PID" >> ex2.nb
fi

#construir ficheiro linha após linha
if [[ $n == 2 ]]; then
	echo "nome do ficheiro?"
	read nome
	echo $nome > $nome
	echo "inserir linha a linha"
	while [[ true ]]; do
		read line
		echo $line >> $nome
	done
fi
