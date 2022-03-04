# OMC WIFI

Proyecto de dispositivo medidor/protector conectado a una red a través de Wi-Fi y monitoreado con un servidor inicialmente interno a la red.

## Dispositivo medidor

### Hardware
Versión actual: 1.0\
Estado: Diseño PCB a la espera de enviar a fábrica.

**Listo**
- Diseño Esquemático KiCAD
- Diseño PCB KiCAD
- GERBERS

**Por hacer**
- Soldar componentes a PCB
- Realizar pruebas con software

Rango voltaje: 100 - 240 V RMS\
Máxima corriente: 20 A RMS - Limitado a 15 A con fusible

### Software
Versión actual: -\
Estado: Creación de códigos separados para posterior unificación.

#### Mediciones voltaje y corriente
Estado: A la espera de dispositivos que consuman mayor potencia para realizar más pruebas.

##### Listo
- Código medición voltaje y corriente.
- Realizar pruebas con distintos niveles de voltaje.

##### Por hacer
- Realizar pruebas con dispositivos que consuman mayor potencia (> 25W).

Rango voltaje: 70 - 240 V RMS\
Rango corriente: 0.2 - 15 A RMS\
*Nota:* Los mímimos en las lecturas se deben al ruido en la conversión AD. Es probable que se reduzca el ruido en PCB.

#### Accionamiento de relay
Estado: En proceso de desarrollo y pruebas

##### Listo
- Código de accionamiento de relay de acuerdo con nivel de voltaje.
- Realizar pruebas con valores de voltaje enviados por comunicación serial.
- Histéresis.

##### Por hacer
- Accionar relay cuando haya sobrecorriente
- Realizar pruebas con mediciones de voltaje real.
- Añadir inicio retardado para caso multidispositivo.

#### Configuración inicial - Conexión Dispositivo-Wi-Fi (AutoConnect)
Estado: En proceso de desarrollo y pruebas.

##### Listo
Por redactar

##### Por hacer
Por redactar

#### Conexión con servidor
Estado: No se ha iniciado.

##### Por hacer
- Implementación de MQTT

#### Integración de códigos
Estado: No se ha iniciado.
##### Por hacer
- Buscar forma de integrar usando FreeRTOS
- Comprobar funcionamiento de códigos integrados evitando interrupciones mutuas.


## Servidor
Estado: No se ha iniciado

**Por hacer**
- Implementación de MQTT en Raspberry Pi









