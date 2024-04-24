"# PBL_1_Sistemas-Digitais" 

### Introdução
Os jogos eletrônicos têm desempenhado um papel crucial na evolução da tecnologia e do entretenimento, proporcionando experiências interativas que transcendem os limites físicos. Mesmo em sua forma mais simples, como o jogo da velha, eles oferecem uma oportunidade fascinante para explorar conceitos de estratégia, competição e diversão.

O projeto visa a criação de um sistema de uma versão do jogo da velha, integrado com a locomoção dos quadrantes do tabuleiro a partir de um mouse. O projeto foi elaborado no kit de desenvolvimento De1-SoC, bem como seus periféricos inclusos.

- Os requisitos do projeto são:
-> Elaboração do código-fonte em C
-> Utilização dos dispositivos periféricos da placa para controle do jogo e opções associadas (como pausar e retornar  à partida)


### Metodologia
- Criação do tabuleiro:
  Para criar o tabuleiro do jogo da velha, foi utilizada uma matriz de caracteres de tamanho 3x3, onde cada célula foi numerada de 1 a 9. Essa abordagem foi implementada para facilitar a visualização do jogador e ajudá-lo a identificar a localização das células.

Figura X representativa do tabuleiro do jogo da velha numerado.

Fonte:

- Verificar vencedor:
  Para determinar o vencedor, foi criada uma função que inspeciona o estado atual do tabuleiro e realiza três tipos de verificações: nas linhas, colunas e diagonais. Essa função examina todas as possibilidades de vitória e ao identificar uma sequência de 3 peças iguais em qualquer direção, declara que o jogo foi vencido. Abaixo pode-se ver como as verificações são feitas:
Verifica as linhas:

- Verifica as colunas:

- Verifica as diagonais:

- Delimitação do tabuleiro:

  Para garantir o funcionamento correto do tabuleiro, foi estabelecida uma delimitação de área em cada célula. Isso permite determinar, com base nos valores de coordenadas x e y, qual célula está sendo selecionada em determinado momento. Dessa forma as células possuem: 

  Área das células: 
  Célula 1: x de 0 a 100, y de 0 a 100
  Célula 2: x de 101 a 200, y de 0 a 100
  Célula 3: x de 201 a 300, y de 0 a 100
  Célula 4: x de 0 a 100, y de 101 a 200
  Célula 5: x de 101 a 200, y de 101 a 200
  Célula 6: x de 201 a 300, y de 101 a 200
  Célula 7: x de 0 a 100, y de 201 a 300
  Célula 8: x de 101 a 200, y de 201 a 300
  Célula 9: x de 201 a 300, y de 201 a 300

- Abaixo é uma figura representativa dessa delimitação:

- Captura de dados do mouse:
  A captura dos dados dos eventos do mouse é realizada quando o programa abre o arquivo correspondente onde esses eventos são registrados. Esse arquivo é geralmente encontrado em `/dev/input/` e tem um nome como "eventX", onde X é um número que identifica o dispositivo específico. Uma vez que o arquivo é aberto usando a função `open()` do sistema, o programa recebe um descritor de arquivo que pode ser usado para ler os eventos.
Após a abertura do arquivo, ocorre uma leitura contínua dos dados do dispositivo para uma estrutura de eventos definida na biblioteca `linux/input.h`, chamada de `struct input_event`. Essa estrutura contém informações detalhadas sobre os eventos do mouse.
Com base nos dados contidos na estrutura `struct input_event`, o programa pode facilmente extrair informações relevantes, como o deslocamento do mouse nas coordenadas x e y.

- Dinâmica do jogo no código:
  O jogo começa com o primeiro jogador, representado pelo "X", e é controlado usando variáveis que capturam o deslocamento do mouse. Esse deslocamento é adicionado às variáveis relacionadas às coordenadas x e y. Essas coordenadas são restritas a valores entre 0 e 300 para garantir que não ultrapassem os limites do tabuleiro, conforme visto na seção de Delimitação do tabuleiro.
Em seguida, uma série de verificações são realizadas. Primeiro, verifica-se se as coordenadas estão dentro dos limites do tabuleiro. Em seguida, as coordenadas x e y capturadas pelo movimento do mouse são convertidas na célula correspondente do tabuleiro.
A segunda parte das verificações ocorre quando há um evento de clique esquerdo do mouse. Nesse momento, verifica-se se a célula está vazia para que a peça do jogador possa ser colocada. Se a célula já estiver ocupada, a peça do jogador não será inserida. No entanto, se a célula estiver vazia, ela será preenchida com a peça do jogador atual. Após isso, é feita uma verificação para determinar se o jogador venceu o jogo. Se o jogador venceu, uma mensagem de parabéns é exibida para o vencedor; caso contrário, é a vez do próximo jogador.
Esse processo continua até que um jogador vença ou até que ocorram mais de 9 jogadas, momento em que o jogo termina. Se não houver vencedor, uma mensagem de empate é exibida.

### Conexão USB (Hardware/Software)

- Sobre a configuração de um dispositivo-host:
  Quando um dispositivo usb é conectado num host, esse host irá enviar ‘’setup requests’’ para o dispositivo, o dispositivo reporta os atributos deles, através de descritores, e o host, através do processo de ‘’bus enumeration’’ configura o dispositivo com base na leitura de seus descritores (pelo endpoint 0). No caso da passagem de dados do mouse para o host, são usados endpoints (parte física de um dispositivo que transmite dados) do tipo interrupt nas transações de dados. Esses Endpoints interrupts são monitorados periodicamente pelo host e foram escolhidos pois os dados são enviados regularmente, sendo o que melhor se encaixa no contexto mouse-host.

- Sobre o sistema geral do USB:
  Todo o processo de passagem dos dados USB pode ser divididos como mostrado a seguir, por camadas hardware-software:
A primeira camada, de baixo para cima, é a camada física propriamente dita, ela que provê a conexão entre o host e o dispositivo. A camada do meio(abrigaria uma API do Linux, USB core por exemplo), é a camada onde os softwares que lidam com o USB  ficam, ela que vai lidar com operações genéricas no usb, como ler/escolher as melhores configurações host/device.
A última camada, é a responsável pela função do dispositivo USB específico,se for um mouse, por exemplo, os cliques no dispositivo físico, seriam interpretados como dados pelo ‘’client software’’(que nesse contexto seria o driver do próprio mouse).

- O fluxo de comunicação é o seguinte:
  O ‘’software client’’ (driver do mouse) geralmente solicita que seja movido os dados entre o buffer no lado do host e os endpoints nos dispositivos (dá-se o nome de pipe a essa relação). O ‘’host controller’’(a placa DE1SOC+sistema operacional) é quem inicia essa troca e gera atividade para movimento de dados. De maneira mais específica, o processo ocorre assim: para ocorrer uma transação a nível de barramento é necessário que o driver do mouse consuma ou gere dados para uma função específica do dispositivo através de endpoints, para isso ele tem que fazer uma ligação com o sistema USB(USB core), gerando um “interrupt request”(IRP), que é quando o software gera um sinal para movimentar dados entre o software e um endpoint. Por sua vez, o USB core faz uma chamada para o ‘’host controller driver’’(uma abstração do host controller) que por fim transformará os dados do IRP em transações, que são as entregas dos dados para um endpoint, que o Host controller usará para criar uma atividade a nível de barramento. O processo pode ser feito de maneira reversa.

A imagem a seguir resume todos os conceitos já ditos até aqui:
