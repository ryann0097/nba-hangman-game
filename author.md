# Introdução NBA Hangman
O jogo "Hangman" é um jogo de adivinhação de palavras em que os jogadores tentam descobrir uma palavra, podendo chutar letra a letra ou fazer um chute inteiro de uma palavra. Em caso de erros, é desenhado uma parte de um boneco em uma forca, desenho que dá nome ao jogo. Se o jogador completar o boneco, ele perde. 
Nesta implementação, todas as categorias do jogo estão relacionadas a jogadores da NBA, tornando-o um desafio interessante para os fãs de basquete. As categorias incluem:

1. Posições como: Ala, Armador e Pivô (PF, SF, SG, PG, C);
2. Jogadores que já foram campeões;
3. Jogadores que ganharam o prêmio de Novato do Ano;
4. Jogadores que ganharam o prêmio de Defensor do Ano;
5. Jogadores que ganham o prêmio de Mais Valioso;
6. Jogadores que foram selecionados para o time comemorativo de 75 anos da NBA.

O jogo exige um bom conhecimento histórico e atual da NBA, desafiando os jogadores a lembrar nomes icônicos e contemporâneos que marcaram a liga. Como dizemos: "are you a casual or an OG hooper?"

# Autor
Autor: Ryan David dos Santos Silvestre<br>
E-mail: ryan.silvestre.718@ufrn.edu.br

# Problemas e possíveis limitações de código
A única limitação do projeto é uma seleção de categoria como principal. No caso, a categoria 3. Não temos como selecionar ela como principal,
mas se você tiver outra categoria em que a seleção de categoria de número 3 é secundária, ela roda normalmente. Por precaução, não escolha ela.

# Para compilar e rodar
Para compilar e rodar o jogo Hangman, você precisará ter instalado um compilador para C/C++ em seu computador.
Ao abrir seu terminal na pasta raiz do projeto (src), digite o seguinte comando: `g++ -o hgm_game main.cpp gameManager.cpp wordHangman.cpp` 
Logo após, digite `./hgm_game`. Depois disso, diversão garantida.
