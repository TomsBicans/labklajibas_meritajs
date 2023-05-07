# Labklājības mērītājs
Labklājības mērītājs ir projekts, kas izstrādāts robotikas kluba ietvaros, balstoties uz oriģinālu ideju. Projekta mērķis ir izstrādāt unikālu sistēmu, kas varētu tikt izmantota bakalaura darba izstrādē datorzinātnēs. Šī projekta pamatā ir mikrokontrolieris Heltec WiFi LoRa 32 v3, kas nodrošina vairākas iespējas, piemēram, WiFi, Bluetooth, Lora un OLED ekrānu. Šis projekts galvenokārt izpēta iespējas izmantot sistēmas Lora moduļus (Semtech SX1262) ar LoraWAN komunikācijas protokolu lielu distanču, divvirzienu saziņai bez starpniekiem.

## Sākums
Lai sāktu izmantot šo projektu, jums būs nepieciešams:

* Heltec WiFi LoRa 32 v3 mikrokontrolieris (https://nettigo.eu/products/heltec-wifi-lora-32-v3-esp32-lora-oled-0-96)
* Pieejama datora USB pieslēgvieta
## Uzstādīšana
1. Lejupielādējiet un instalējiet Arduino IDE: https://www.arduino.cc/en/software
1. Lejupielādējiet šī projekta pēdējo versiju no GitHub repozitorijas.
1. Atveriet main.ino failu ar Arduino IDE.
1. Arduino IDE, iestatiet pareizos uzstādījumus mikrokontroliera veida un porta izvēlei.
1. Pārliecinieties, ka visas nepieciešamās bibliotēkas ir instalētas. (Skatiet nettigo linka sadaļu "links", kurā ir iekļauti visi mikrokontroliera ražotāja resursi)
## Kā izmantot
Lai palaistu projektu, veiciet šādus soļus:

1. Pievienojiet mikrokontrolieri pie datora, izmantojot USB kabeli.
1. Ielādējiet projektu mikrokontrolierī, izmantojot Arduino IDE.
1. Pēc ielādes pabeigšanas, projekts sāks darboties un varēsiet redzēt ekrāna attēlu.
## Struktūra
Projekts sastāv no šādām galvenajām daļām:

* main.ino: galvenā programmas ieejas punkts un cikla loģika.
* BaseDevice.h, MonitoringDevice.h, AdministratorDevice.h: ierīces loģikas, atkarībā no sistēmas lomas.
* Sensors.h, Operation.h: sensoru un darbību loģikas implementācija.
* TestMeasurement.h, TestTimer.h, TestQueue.h, TestLinkedList.h: testu moduļi.
* Util.h, Print.h, Buzzer.h: palīgmoduļi.
## Mikrokontrolieris
Šajā projektā tiek izmantots Heltec WiFi LoRa 32 v3 mikrokontrolieris, kas aprakstīts zemāk, tādēļ programmatūrā tiks iekļauta dinamisku objektu veidošana, jo atmiņa šim mikrokontrolierim ir salīdzinoši liela - 131 KB statiskai programmas atmiņai, 327KB dinamiska atmiņa izpildlaika vajadzībām.

Specifikācija:

* Ražotājs: Heltec
* Modelis: ZC-153-868-1
* CPU: ESP32-S3
* 240 MHz divkodolu
* WiFi līdz 150Mbps 802.11 b/g/n/e/i
* Bluetooth 4.2 (BLE)
* Atmiņa: 8MB (64bit)
* USB-Seriālās konvertora: CP2102
* Radios: Semtech SX1262
* Antenas savienotājs: IPX (U.FL) (antena iekļauta)
* OLED ekrāns:
    * Izmērs: 0.96”
    * Vadītājs: SSD1306
    * Izšķirtspēja: 128x64 px
* Li-Ion/Li-Po uzlādes shēma
* Baterijas ligzda: 2pin ar 1.25 mm soli (kabelis SH1.25)
* Izmēri: 50.2 x 25.2 x 10.2 mm


## Papildu informācija
Projektā ir divas galvenās ierīces lomas:

* MonitoringDevice: sensoru nolasīšana un datu nosūtīšana uz AdministratorDevice.
* AdministratorDevice: datu saņemšana no MonitoringDevice un to apstrāde un attēlošana. Vadības komandu sūtīšanas iespēja uz monitorēšanas iekārtu.
Lai mainītu sistēmas lomu, mainiet `DEVICE_TYPE` vērtību `main.ino` failā. Ir iespējamas divas opcijas `ADMINISTRATOR_DEVICE` un `MONITORING_DEVICE`.

```
#define DEVICE_TYPE ADMINISTRATOR_DEVICE
```

## Atbalsts un sadarbība
Ja jums rodas jautājumi vai nepieciešams atbalsts, lūdzu, sazinieties ar mums, izmantojot GitHub repozitoriju vai e-pastu. Mēs esam ieinteresēti sadarboties ar citiem entuziastiem, kas vēlas pievienoties šim projektam un kopā attīstīt jaunas idejas un risinājumus.

## Licencēšana
Šis projekts tiek piedāvāts saskaņā ar MIT licenci. Lūdzu, ievērojiet licencēšanas noteikumus, izmantojot vai modificējot šo projektu.