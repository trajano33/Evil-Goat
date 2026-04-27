
# 🐐 Evil Goat

<p align="center"> <img src="https://i.imgur.com/zu0fyTu.png" width="320"/> </p> <p align="center"> <img src="https://img.shields.io/github/license/trajano33/Evil-Goat?style=for-the-badge"> <img src="https://img.shields.io/github/stars/trajano33/Evil-Goat?style=for-the-badge"> <img src="https://img.shields.io/github/forks/trajano33/Evil-Goat?style=for-the-badge"> <img src="https://img.shields.io/github/issues/trajano33/Evil-Goat?style=for-the-badge"> <img src="https://img.shields.io/badge/platform-ESP8266-blue?style=for-the-badge"> <img src="https://img.shields.io/badge/status-active-success?style=for-the-badge"> </p> <p align="center"> Simulador de Evil Twin com captive portal para <b>educação em segurança Wi-Fi</b> </p>

----------

## ⚠️ AVISO LEGAL

Este projeto é destinado **EXCLUSIVAMENTE para fins educacionais e laboratoriais**.

🚫 NÃO utilize sem autorização  
🚫 NÃO utilize em redes públicas ou de terceiros  
⚖️ O uso indevido pode violar leis locais

Ao utilizar este projeto, você concorda que é **totalmente responsável** por suas ações.

----------

## 📌 SOBRE O PROJETO

O **Evil Goat** simula um ataque do tipo _Evil Twin_, criando um ponto de acesso Wi-Fi falso que redireciona requisições para um **captive portal simulado**.

O foco é ENSINAR — não atacar.

### 🧠 Aplicações práticas:

-   Treinamento de segurança (awareness)
-   Demonstração de phishing
-   Laboratórios de pentest
-   Estudo de engenharia social

----------

## ✨ FEATURES

-   📡 Access Point falso (Wi-Fi)
-   🌐 Redirecionamento DNS automático
-   🔐 Portal de login simulado
-   💾 Armazenamento local de dados (laboratório)
-   ⚙️ Painel de configuração via web
-   📥 Exportação de dados coletados

----------


## 🧰 HARDWARE

| Componente     | Suporte |
|:---------------|:-------:|
| ESP8266        |   ✅    |
| NodeMCU        |   ✅    |
| Wemos D1 Mini  |   ✅    |

----------

## 📦 INSTALAÇÃO

### 1. Arduino IDE

[https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

### 2. Suporte ESP8266

- Abra o Arduino IDE  
- Vá em: **Arquivo → Preferências**  
- No campo **"URLs adicionais de Gerenciador de Placas"**, adicione:  
 ```txt 
http://arduino.esp8266.com/stable/package_esp8266com_index.json  
  ```
- Clique em **OK**

### 3. Instalar placa

-   Ferramentas → Gerenciador de Placas
-   Buscar: `esp8266`
-   Instalar: **ESP8266 Community**

----------

## 🔌 CONEXÃO

Se o dispositivo não aparecer:

-   Troque o cabo USB
-   Instale drivers:

CH340  
[https://github.com/samdenty/Wi-PWN/tree/master/drivers/CH34x](https://github.com/samdenty/Wi-PWN/tree/master/drivers/CH34x)

CP210x  
[https://github.com/samdenty/Wi-PWN/tree/master/drivers/CP210x](https://github.com/samdenty/Wi-PWN/tree/master/drivers/CP210x)

----------

## 🚀 USO (LABORATÓRIO)

### 1. Upload do código

```bash
git clone https://github.com/trajano33/Evil-Goat.git
```

----------

### 2. Acessar rede

Conecte-se ao Wi-Fi criado pelo ESP8266

----------

### 3. Portal

http://172.0.0.1

----------

### 4. Configuração

http://172.0.0.1/config

----------

### 5. Credenciais (laboratório)

http://172.0.0.1/creds

----------

### 6. Exportação

http://172.0.0.1/downloadCreds

Formato:

email:senha

----------

## 🧪 CENÁRIO DE TESTE RECOMENDADO

Use em ambiente controlado:

-   Rede isolada
-   Dispositivos próprios
-   Sem acesso à internet real

👉 SIMULE um ataque — NÃO execute um ataque real

----------

## 🛡️ COMO SE DEFENDER

-   Evite Wi-Fi desconhecido
-   Verifique URLs antes de logar
-   Use HTTPS sempre
-   Ative 2FA
-   Desconfie de páginas inesperadas

----------

## 🤝 CONTRIBUIÇÃO

Contribuições são bem-vindas.

1.  Fork
2.  Crie uma branch
3.  Commit
4.  Push
5.  Pull Request

----------

## 📜 LICENÇA

Distribuído sob licença MIT.

----------

## ⭐ APOIE O PROJETO

Se esse projeto te ajudou:

-   Deixe uma ⭐ no repositório
-   Compartilhe com outros devs
