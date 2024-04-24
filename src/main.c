#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <intelfpgaup/SW.h>
#include "constantes.h"
#include "tabuleiro.h"

int main() {
    int xsoma = 0, ysoma = 0;
    int x, y;
    int escolha;
    char jogador = 'X';
    int jogadas = 0;
    int valor;
    int partida_despausada; // verdadeiro ou falso
    ssize_t n; // número de bytes retornado pelo arquivo do driver do mouse
    int nova_atualizacao_a_direita, nova_atualizacao_a_esquerda; // detecta atualização de movimento após despausar a partida
    char vencedor; // caractere do jogador que venceu 
    struct input_event ev; // struct que recebe as informações do evento de locomoção do mouse
    
    const char *dev = "/dev/input/event0";  // endereço do driver utilizado
    int fd = open(dev, O_RDONLY);
    if (fd == -1) { // falha na abertura do arquivo
        perror("Cannot open input device");
        return 1;
    }


    printf("Bem-vindo ao Jogo da Velha!\n");
    exibir_tabuleiro();
    valor = 0;
    int *dados_sw = &valor;
    partida_despausada = 0;
    SW_open(); // abertura do driver das chaves HH
    SW_read(dados_sw); // leitura inicial para descobrir se o jogo começou pausado
    // Loop infinito para receber as coordenadas
    while (1) {
    SW_read(dados_sw);

      // leitura do driver do mouse
      n = read(fd, &ev, sizeof(ev));

      if(*dados_sw==1){
      if (n == (ssize_t)-1) { // se não foi lido nenhum byte no driver do mouse
          perror("Error reading");
          break;
      } else if (n != sizeof(ev)) { // espera se a leitura de 16 bytes do driver do mouse. Qualquer número de bytes fora do esperado levanta uma exceção.
          fprintf(stderr, "Error: read %d bytes, expecting %d\n", n, sizeof(ev));
          break;
      }

      if(partida_despausada==1){
        system("clear");
        partida_despausada=0; // toggle do valor simbolizando retorno da partida
        ev.value=0; 
      }

      if (ev.type == EV_REL && ev.code == REL_X) { // se a locomoção detectada no mouse é horizontal
        x =  ev.value;
      }
      if (ev.type == EV_REL && ev.code == REL_Y) {  // se a locomoção detectada no mouse é vertical
        y = ev.value;
      }

      // detecção de bordas dos deslocamentos X e Y
      if(x > 50){ 
        x = 50;
      }else if (x < -50){
        x = -50;
      }

      if(y > 50){
        y = 50;
      }else if (y < -50){
        y = -50;
      }

      // Acumula as coordenadas
      if(xsoma <= MAX && xsoma >= MIN){
        xsoma += x;
        if(xsoma > MAX) {
          xsoma = MAX;
        }
        if(xsoma < MIN) {
          xsoma = MIN;
        }
      }
      if(ysoma <= MAX && ysoma >= MIN){
        ysoma += y;
        if(ysoma > MAX) {
          ysoma = MAX;
        }
        if(ysoma < MIN) {
          ysoma = MIN;
        }
      }
      // Verifica em qual quadrante as coordenadas caem
      if (xsoma >= MIN && xsoma <= MAX_QUADRANTE_1 && ysoma >= MIN && ysoma <= MAX_QUADRANTE_1) {
          escolha = 1;
      } else if (xsoma > MAX_QUADRANTE_1 && xsoma <= MAX_QUADRANTE_2 && ysoma >= MIN && ysoma <= MAX_QUADRANTE_1) {
        escolha = 2;
      } else if (xsoma > MAX_QUADRANTE_2 && xsoma <= MAX_QUADRANTE_3 && ysoma >= MIN && ysoma <= MAX_QUADRANTE_1) {
        escolha = 3;
      } else if (xsoma >= MIN && xsoma <= MAX_QUADRANTE_1 && ysoma > MAX_QUADRANTE_1 && ysoma <= MAX_QUADRANTE_2) {
        escolha = 4;
      } else if (xsoma > MAX_QUADRANTE_1 && xsoma <= MAX_QUADRANTE_2 && ysoma > MAX_QUADRANTE_1 && ysoma <= MAX_QUADRANTE_2) {
        escolha = 5;
      } else if (xsoma > MAX_QUADRANTE_2 && xsoma <= MAX_QUADRANTE_3 && ysoma > MAX_QUADRANTE_1 && ysoma <= MAX_QUADRANTE_2) {
        escolha = 6;
      } else if (xsoma >= MIN && xsoma <= MAX_QUADRANTE_1 && ysoma > MAX_QUADRANTE_2 && ysoma <= MAX_QUADRANTE_3) {
        escolha = 7;
      } else if (xsoma > MAX_QUADRANTE_1 && xsoma <= MAX_QUADRANTE_2 && ysoma > MAX_QUADRANTE_2 && ysoma <= MAX_QUADRANTE_3) {
        escolha = 8;
      } else if (xsoma > MAX_QUADRANTE_2 && xsoma <= MAX_QUADRANTE_3 && ysoma > MAX_QUADRANTE_2 && ysoma <= MAX_QUADRANTE_3) {
        escolha = 9;
      }
      nova_atualizacao_a_direita = escolha+1;
      nova_atualizacao_a_esquerda = escolha-1;
      if(escolha == nova_atualizacao_a_direita || escolha == nova_atualizacao_a_esquerda || ev.value ==1){ // se foi detectada novo evento de locomoção do mouse ou clique
        system("clear"); // atualização do tabuleiro impresso na tela
        exibir_tabuleiro();
        printf("Quadrante %d\n", escolha);
      }

      //se foi detectado um clique com o botão esquerdo do mouse
      if(ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1){
            // cálculo do quadrante adequado para o tabuleiro
            int linha = (escolha - 1) / 3; 
            int coluna = (escolha - 1) % 3;
            system("clear");
            // Verifica se a posição escolhida já está ocupada
            if (posicao_ocupada(linha, coluna)) {
                printf("Posição já ocupada. Escolha outra posição.\n");
                continue;
            }
    
            // Marca a posição escolhida com o símbolo do jogador atual
            preencher_no_tabuleiro(linha, coluna, jogador);
            exibir_tabuleiro();
            printf("Quadrante %d\n", escolha);
    
            // Verifica se houve um vencedor
            vencedor = verificar_vencedor();
            if (vencedor != ' ') {
                printf("Parabéns! O jogador %c venceu!\n", vencedor);
                break;
            }
    
            // Verifica se o jogo terminou em empate
            jogadas++; // contabiliza o número de rodadas. 9 rodadas sem um vencedor implica em um empate
            if (jogadas == 9) {
                printf("O jogo terminou em empate!\n");
                break;
            }
    
            // Alternar jogador
            jogador = (jogador == 'X') ? 'O' : 'X';
      }
      }

}
  SW_close();
  return 0;
}
