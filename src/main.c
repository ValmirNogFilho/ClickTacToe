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
    int t;
    ssize_t n;
    int eUm, eDois;
    char vencedor;
    struct input_event ev;
    
    const char *dev = "/dev/input/event0"; 
    int fd = open(dev, O_RDONLY);
    if (fd == -1) {
        perror("Cannot open input device");
        return 1;
    }


    printf("Bem-vindo ao Jogo da Velha!\n");
    exibir_tabuleiro();
    valor = 0;
    int *dados_sw = &valor;
    t = 0;
    SW_open();
    SW_read(dados_sw);
    // Loop infinito para receber as coordenadas
    while (1) {
    SW_read(dados_sw);
      
      n = read(fd, &ev, sizeof(ev));

      if(*dados_sw==1){
      if (n == (ssize_t)-1) {
          perror("Error reading");
          break;
      } else if (n != sizeof(ev)) {
          fprintf(stderr, "Error: read %d bytes, expecting %d\n", n, sizeof(ev));
          break;
      }

      if(t==1){
        system("clear");
  t=0;
  ev.value=0; 
}

      if (ev.type == EV_REL && ev.code == REL_X) {
        x =  ev.value;}
      if (ev.type == EV_REL && ev.code == REL_Y) {
        y = ev.value;}

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
      eUm = escolha+1;
      eDois = escolha-1;
      if(escolha == eUm || escolha ==eDois || ev.value ==1){
        system("clear");
        exibir_tabuleiro();
        printf("Quadrante %d\n", escolha);
      }
      if(ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1){
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
        jogadas++;
        if (jogadas == 9) {
            printf("O jogo terminou em empate!\n");
            break;
        }

        // Alternar jogador
        jogador = (jogador == 'X') ? 'O' : 'X';
      }}

}
  SW_close();
  return 0;
}
