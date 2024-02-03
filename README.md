
# 🐐 Evil Goat

**Autor:** [trajano](https://github.com/trajano33)

<img src="https://i.imgur.com/zu0fyTu.png" alt="Logo" width="400"/>



## Descrição 🚀

O Evil Goat é um projeto Arduino que funciona como um criador de Evil Twin, implementando um captive portal para login Wi-Fi por meio de um ESP8266. Este dispositivo cria uma rede Wi-Fi simulada, atrai dispositivos próximos e, ao usar uma interface web convincente, induz os usuários a realizar logins comprometedores. O Evil Goat é uma ferramenta versátil para testes de segurança em redes sem fio.


## 🔧 Instalação

### Requisitos de Hardware:
- Adquira um módulo ESP8266.

### 🛠️ Requisitos de Software:
1. Instale o [Arduino IDE](https://www.arduino.cc/en/software).
2. Vá para "Arquivo" > "Preferências".
3. Adicione a seguinte URL em "URLs Adicionais de Gerenciador de Placas":
  ```http://arduino.esp8266.com/stable/package_esp8266com_index.json```
4. Clique em "OK".
5. Vá para "Ferramentas" > "Placa" > "Gerenciador de Placas...". Pesquise por "esp8266" e instale a placa correspondente.

### 🔄 Carregamento do Código:
1. Conecte o ESP8266 ao computador.
2. Selecione o tipo de placa e a porta no Arduino IDE.
3. Faça o upload do código para o ESP8266.

### ⚙️ Configuração do Projeto:
1. Conecte-se à rede Wi-Fi fornecida pelo ESP8266.
2. Abra um navegador e acesse `http://172.0.0.1/config`.
3. Na página de configuração, altere o SSID para o desejado.

### 📱 Como Utilizar:
1. Conecte-se à rede Wi-Fi do ESP8266.
2. Clique em "Entrar na Rede".
3. Insira seu e-mail e senha quando solicitado.
4. Após o login bem-sucedido, visite `http://172.0.0.1/creds` para visualizar a lista completa de e-mails e senhas registrados.

**Nota Importante:** _Certifique-se de possuir as devidas permissões ao utilizar esta ferramenta. Não me responsabilizo por qualquer uso indevido da mesma. Utilize-a de forma ética e legal, em conformidade com as leis e regulamentações locais_
