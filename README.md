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

### Fluxo de Operação

- **Tempo 0s (Início):**  
  1. Servo inicia em 180° com mensagem "Servo em 180 graus".  
  2. Temporizador inicial é criado (muda estados a cada 5s).  

- **Após 5s:**  
  1. Servo move para 90° ("Servo em 90 graus").  

- **Após 10s (5s após o anterior):**  
  1. Servo move para 0° ("Servo em 0 graus").  

- **Após 15s (5s após o anterior):**  
  1. Temporizador inicial é cancelado.  
  2. Segundo temporizador inicia, movendo o servo suavemente de 0° a 180°.  

- **Ao atingir 180°:**  
  1. Segundo temporizador é cancelado.  
  2. Primeiro temporizador é reiniciado, criando um loop.

## 🤓 Explicação

O código utiliza-se de temporizador para desacoplar o gerenciamento do servo da cpu. Para a primeira rotina foi criado um enum para definir qual o estado atual e preparar para a próxima mudança de estado com um delay de 5 segundos entre cada transição. Após finalizada a primeira rotina o primeiro temporizador é cancelado e um segundo é criado para chamar a segunda rotina que fará o servo ir do ângulo 0 ao 180 de forma suave. Após a chegada do servo na posição final, o segundo temporizador é cancelado e o primeiro é recriado, fazendo com que o código funcione em forma de loop, mas sendo gerenciado apenas pelo hardware.

### Código na placa BitDogLab

Devido à baixa frequência do PWM e à forma como o duty cycle é atualizado, o olho humano não consegue processar as mudanças de estado de forma suave, logo as transições de estado ocorrem de forma ou imperceptível, ou bruscas dando um efeito de flickering.

### 🎥 Vídeo

[Vídeo demonstrativo do projeto](https://drive.google.com/file/d/1CdwJAgXApzQgmOPzIIlO-odINWTJ19pA/view?usp=sharing)

## Como executar

O Projeto pode ser executado clonando este repositório e importando como projeto pico no Visual Studio Code

## 👥 Autoria

**Guilherme Emetério Santos Lima**  
[![GitHub](https://img.shields.io/badge/GitHub-Profile-blue?style=flat&logo=github)](https://github.com/DankAlighieri)
