#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>

// EV_REL é destinado a eventos de movimento relativo. Adequa-se a mouses;
// EV_KEY refere-se a cliques. Adequa-se aos botões dos mouses;

// REL_X, REL_Y, REL_WHEEL: movimento do mouse, respectivamente, nos eixos
// X e Y e no scroll do mouse. Trata-se da velocidade que o dispositivo foi
// mexido;
int eixo_x_alterado(struct input_event ev) {
  return ev.type == EV_REL && ev.code == REL_X;
}

int eixo_y_alterado(struct input_event ev) {
  return ev.type == EV_REL && ev.code == REL_Y;
}

int scroll_alterado(struct input_event ev) {
  return ev.type == EV_REL && ev.code == REL_WHEEL;
}

// BTN_LEFT: Botão esquerdo do mouse
int botao_esquerdo_clicado(struct input_event ev) {
  return ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1;
  // ev.value == 1: botão clicado
}

// BTN_RIGHT: Botão direito do mouse
int botao_direito_clicado(struct input_event ev) {
  return ev.type == EV_KEY && ev.code == BTN_RIGHT && ev.value == 1;
}

int main() {
  const char *INPUT_FILE =
      "/dev/input/event0"; // arquivo do driver para o mouse conectado

  int file_buffer = open(INPUT_FILE, O_RDONLY);
  if (file_buffer == -1) {
    perror("Não conseguiu abrir o dispositivo");
    return 1;
  }

  struct input_event event;

  /*
  struct input_event {
      struct timeval time;
      __u16 type; // tipo do dispositivo.
      __u16 code; // código da ação executada
      __u32 value; // valor retornado
  }
  */

  while (1) {
    ssize_t n = read(file_buffer, &event, sizeof(event));
    // bytes lidos são passados para os atributos da struct event;
    if (n == (ssize_t)-1) {
      perror("Erro na leitura");
      break;
    }
    else if (n != sizeof(event))
    // numero de bytes retornados pelo read != tamanho esperado pra struct (16
    // bytes)
    {
      fprintf(stderr, "Erro: %d bytes foram lidos. Esperava-se %d\n", n,
              sizeof(event));
      break;
    }

    if (eixo_x_alterado(event)) {
      printf("X: %d\n", event.value);
    } else if (eixo_y_alterado(event)) {
      printf("Y: %d\n", event.value);
    } else if (scroll_alterado(event)) {
      printf("SCROLL: %d\n", event.value);
    } else if (botao_esquerdo_clicado(event)) {
      printf("CLIQUE BOTÃO ESQUERDO: %d\n", event.value);
    } else if (botao_direito_clicado(event)) {
      printf("CLIQUE BOTÃO DIREITO: %d\n", event.value);
    }
  }

  close(file_buffer);
  return 0;
}
