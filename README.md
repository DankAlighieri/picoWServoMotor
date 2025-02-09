# Tarefa Servo Motor com PWM

## 🛠️ Componentes Utilizados

- Raspberry pi pico W
- Servo Motor

## 💻 Firmware

Desenvolvido em C utilizando:

- IDE: Visual Studio Code
- Simulador: Wokwi
- SDK: Raspberry Pi Pico SDK
- Servo Motor: GP28

## 🎯 Funcionamento

### Estado Inicial

- Flange do servo motor inicializa num ângulo de 180 graus;
- Mensagem é enviada ao terminal serial informando a posição angular atual da flange do servo;
- É criado um temporizador repetitivo inicial para mudar o estado do servo a cada 5 segundos;

### Fluxo de Operação

- Após 5 segundos:
    1. Flange vai para a posição 90 graus;
    2. Mensagem "Servo em 90 graus" enviado ao terminal serial;
- Após 10 segundos:
    1. Flange vai para a posição 0 graus;
    2. Mensagem "Servo em 0 graus" enviado ao terminal serial;
- Após 15 segundos:
    1. Cancelado o temporizador inicial;
    2. Inicializado um segundo temporizador para fazer a transição suave do servo do ângulo 0 ao 180;
- Após Atingir 180 graus:
    1. Cancelado o segundo temporizador;
    2. Primeiro temporizador é recriado, assim formando um loop.

## 🤓 Explicação

O código utilia-se de temporizador para desacoplar o gerenciamento do servo da cpu. Para a primeira rotina foi criado um enum para definir qual o estado atual e preparar para a proxima mudança de estado com um delay de 5 segundos entre cada transição. Após finalizada a primeira rotina um temporizador repetitivo é criado para chamar a segunda rotina que fará o servo ir do ângulo 0 ao 180 de forma suave. Após a chegada do servo na posição final, o segundo temporizador é cancelado e o primeiro é recriado, fazendo com que o código funcione em forma de loop, mas sendo gerenciado apenas pelo hardware.

### 🎥 Vídeo

[Vídeo demonstrativo do projeto](https://drive.google.com/file/d/1CdwJAgXApzQgmOPzIIlO-odINWTJ19pA/view?usp=sharing)

## 👥 Autoria

**Guilherme Emetério Santos Lima**  
[![GitHub](https://img.shields.io/badge/GitHub-Profile-blue?style=flat&logo=github)](https://github.com/DankAlighieri)
