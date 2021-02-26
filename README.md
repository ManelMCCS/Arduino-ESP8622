# Comunicação WiFi recorrendo ao módulo ESP-01

# Introdução

O objetivo deste projeto é a utilização de um micro-controlador com o módulo ES-01 para a partir de um browser ligado a uma rede WiFi poder controlar a ocupação por pessoas de um espaço, tendo em conta o número de pessoas por metro quadrado.
O Módulo ESP8266 é um dispositivo IoT (Internet das Coisas) que consiste de um microprocessador de 32 bits com suporte embutido à rede WiFi e memória flash integrada. Essa arquitetura permite que ele possa ser programado de forma independente, sem a necessidade de outras placas como o Arduino, por exemplo.
Neste caso O Módulo WiFi ESP8266 ESP-01 irá ser utilizado junto com a placa chipkit UNO 32 para agregar conexão sem fio à placa. O módulo tem suporte as redes WiFi 802.11 b/g/n e criptografia WPA e WPA2.

Modos de Operação ESP-01

Existem dois principais modos de operação do ESP-01:
**Modo 1 – Comandos AT**
No modo Comandos AT, o módulo possui instalado um programa chamado “Firmware Comandos AT”. Esse programa tem uma função pré-determinada que é receber comandos de operação via UART (pinos TX e RX). Nesse modo, o ESP-01 deverá trabalhar em conjunto com um microcontrolador. O microcontrolador será o responsável por enviar os comandos AT para o ESP-01. A placa Arduino possui um microcontrolador capaz de realizar esses comandos. O Firmware Comandos AT já vem instalado de fábrica podendo estar em diferentes versões, mas pode ser atualizado caso venha com uma versão antiga.
Existem comandos específicos para cada tarefa do ESP-01. Por exemplo o comando AT+CWJAP_CUR é utilizado para conectar o módulo a uma rede WiFi. 

**Modo 2 – Standalone**
No modo Standalone, o ESP-01 trabalha como um microcontrolador por si só, sem a necessidade de um microcontrolador externo. É possível assim utilizar as bibliotecas de WiFi para ESP8266 e desenvolver o software em cima delas, criar web servers e muito mais.
Esse modo é muito utilizado com as placas de desenvolvimento como NodeMCU, pois possuem mais pinos tornando os projetos mais interessantes e livres para customizações. Mas nada impede de também utilizar com ESP-01.

Troca de informações no servidor

O objetivo do XMLHttpRequest, neste projeto, é poder solicitar e enviar dados do browser para um servidor da web, neste caso o ESP. Umadas grandes vantagens deste método é não ser necessário atualizar a página para trocar informações ou qualquer tipo de ações. O ChipKit através das funções da biblioteca ESP8622WiFi cria-se um arquivo XML que posteriormente é enviado pelo módulo. Atráves da função LoadData, escrita em JavaScript, é possível obter os valores enviados nesses arquivos que são pedidos por um “GET request”.

Envio de dados para o Browser:
server.arg(query) (ESP + microcontrolador) -> xhttp.open("GET", id_do_response + query + valor, true) (Browser)
server.send(200, "text/plane", String(n));-> -> xhttp.open("GET", id, true) (Browser)

Receção de dados:

server.arg(query) (ESP + microcontrolador) <- xhttp.open("GET", id_do_response + query + valor, true) (Browser)
Envio de dados:
server.send(200, "text/plane", String(n)); -> <- xhttp.open("GET", id_do_response , true) (Browser)


Posteriormente, os dados desse arquivo são inseridos em parágrafos HTML da página da web sem necessidade de atualizar a página.
Tudo isto é possível devido a tecnologia AJAX (Javascript Assíncrono e XML) que realiza estas solicitações e envios de informações de forma assíncrona.
