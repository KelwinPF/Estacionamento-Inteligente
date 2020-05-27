# Sistemas-Digitais

Foi utilizada a placa galileo para fazer este protótipo de estacionamento inteligente. Esse projeto consiste em quando um carro
estacionar em uma vaga, o sensor(LDR) alertar e acender um led mostrando se existe ou
não uma vaga. Ao mesmo tempo, um display de 7 segmentos mostra a quantidade de
vagas disponíveis, colocamos também 2 potenciômetros, um para ter um controle do set
point de luminosidade dos LDRs e outro para controle intensidade da luz dos LEDs.
Também como foi pedido para este projeto, seria necessário colocar alguns tópicos
para compor o mesmo. São eles:

  ● I/O: Como entradas o nosso projeto possui os sensores LDR e dois
  potenciômetros, como saídas possui 4 Leds e um display de 7 segmentos.
  ● ADC: Nas entradas ADC utilizamos os sensores LDR e os Potenciômetros.
  ● Threads por contagem de tempo : Nosso projeto possui uma Thread(tarefa_2)
  por contagem de tempo, que a cada 0,015 segundos verifica o valor dos
  sensores LDR para cada vaga e atualiza o contador com o número de vagas
  disponíveis.
  ● Threads : Ao todo o projeto possui 3 Threads, sendo uma por contagem de
  tempo(tarefa_2) como explicado acima, outra Thread que recebe os valores
  dos potenciômetros(tarefa_4) e uma que inicializa os PWMs(tarefa_3),
  verifica as vagas e liga-os de acordo com a intensidade que é recebida em
  um dos potenciômetros.
  ● PWM: Utilizamos como PWM os 4 LEDs que indicam a disponibilidade das
  vagas,onde a intensidade da luz emitida pelos LEDs é controlada por um dos
  potenciômetros.

Para o projeto, utilizamos:

  ● 2(Dois) Potenciômetros,
  ● 4(Quatro) LEDs
  ● 1(Um) display 7 segmentos
  ● 2(Dois) Sensores LDR
  ● 2(Dois) resistores de 10k ohms
  ● 5(Cinco) resistores de 220 Ohms
  ● Jumpers.
