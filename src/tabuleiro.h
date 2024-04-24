// Imprime o tabuleiro no terminal
void exibir_tabuleiro();

// Analisa qual dos jogadores completou uma linha, coluna ou diagonal do tabuleiro.
char verificar_vencedor();

// Insere os valores dos jogadores ("X" ou "O") no tabuleiro.
void preencher_no_tabuleiro(int linha, int coluna, char valor);

// Confere se um espaço no tabuleiro já foi preenchido.
int posicao_ocupada(int linha, int coluna);
