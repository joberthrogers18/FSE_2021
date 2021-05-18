# Trabalho Final de Fundamentos de Sistema Embarcados

## Link para o Vídeo do Projeto

[Link para o Vídeo](https://www.youtube.com/watch?v=gN2Q8M9hmcA)

## Alunos

| Aluno | Matrícula |
| ----- | --------- |
|  Gabriela Medeiros da Silva | 16/0121817 |
| Jôberth Rogers Tavares Costa | 16/0128013 |

## Descrição 

Esse projeto consiste em tentar replicar um sistema de gerenciamento de uma casa inteligente. Onde é possível cadastrar dispositivos (ESP32), para serem monitorados pelo sistema e o usuário do mesmo ter feedbacks do que esta acontecendo no ambiente (simulado), através de entradas e saídas presentes na placa.

## Requisitos para execução

- Sensor DHT11
- Placa ESP32 conectada a rede WIFI.
- Ter instalado em sua máquina:
      - Docker => [link para instalação](https://docs.docker.com/engine/install/)
      - Docker Compose => [link para instalação](https://docs.docker.com/compose/install/)
      - esp-idf => [link do repositório](https://github.com/espressif/esp-idf/)
      - Python 3

## Como executar

### Servidor Central

Primeiramente rode o servidor Central, executando no diretório **"servidor_central"** o comando :

    docker-compose up --build

O servidor central gera logs em csv pelos comandos executados pelo usuário do sistema, para visualizar esses logs, rode o servidor central usando a virtualenv do Python. Isso pode ser feito no diretório **"servidor_central"** com os comandos:

    virtualenv venv

    source venv/bin/activate

    pip3 install -r requirements.txt

    python3 app.py

### ESP32 e Sensor (Servidores Distribuídos)

Primeiramente conecte o sensor DHT11 no pino 4 da GPIO e no VCC e Ground para que este consiga coletar as informações pertencente a temperatura e umidade. Após liga-la na porta USB de seu computador, escolha entre o servidor para dispositivos com energia (diretório **"servidor_distribuido"**) ou dispositivos com bateria (diretório **"servidor_distribuido_lp"**). Ao acessar algum dos diretórios, exporte os comando via esp-idf presente no diretório **"esp-idf"** do seu computador no arquivo **export.sh**. Depois de exportado configure o WIFI da placa usando o comando:

    idf.py menuconfig

E Após execute o código na ESP usando os comandos:

    idf.py build
    idf.py -p {PORTA} flash monitor

Caso a ESP já tenha algum código executando em background, apague a flash usando o comando:

    idf.py -p {PORTA} erase_flash

