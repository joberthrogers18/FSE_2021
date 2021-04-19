# Trabalho 2 FSE - Comunicação entre Servidores 

### 1) Dados do Aluno:

- **Nome:** Joberth Rogers Tavares Costa
- **Matrícula:** 16/0128013

### 2) Bibliotecas Usadas

- bme280
- wiringPi
- softPwm
- cJSON

### 3) Observações

Esse projeto é dividido em duas camadas, sendo elas o servidor central responsável por renderizar a interface ao usuário e enviar comandos para o servidor distribuído. Há também o servidor distribuído responsável por lidar diretamente com os componentes da GPIO e BME280. Ambos se conectam via protocolo **TCP/IP**.

### 4) Instruções para execução

- Primeiramente abra o diretório do servidor distribuído e compile e execute o código (Comando explicados na tabela abaixo).

- Após executar o servidor distribuído, vá até o diretório cliente onde está o servidor central, compile e execute o código. Logo em seguida aparecerá a interface para o usuário interagir com o programa.

- Para cada camada dentro do projeto existe um Makefile para execução dos scripts. Os comandos principais dentro são:

| Comando         | Descrição                          |
|-----------------|------------------------------------|
| **make**        | Compila o código                   |
| **make run**    | Executa o binário no diretório bin |
| **make clean**  | Remove o binário compilado         |

- A primeira tela do servidor central é essa:

![first screen](https://i.imgur.com/PJlX88K.png)

- O usuário tem a opção de desligar ou ligar algum dos dispositivos presentes no servidor distribuído. Selecionando primeiramente a opção 1 ou 2 e em seguida apertando ENTER.

![second screen](https://i.imgur.com/PgBdhM2.png)

- Após selecionado alguma das opções, o usuário escolhe qual o dispositivo que deve permanecer ligado ou desligado. E as opções podem ser:

|   Opções   |  Descrição        |
|------------|-------------------|
|   **11**   | Lâmpada 1         |
|   **12**   | Lâmpada 2         |
|   **13**   | Lâmpada 3         |
|   **14**   | Lâmpada 4         |
|   **21**   | Ar Condicionado 1 |
|   **22**   | Ar Condicionado 2 |
