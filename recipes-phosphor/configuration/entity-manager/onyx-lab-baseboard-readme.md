# Entity-Manager Configuration Explanation

 Markdown  `onyx-lab-baseboard.json` （ BIOS  BMC ）：

## Configuration file parsing

Entity Manager (EM) defines various hardware components on the system (FRU, Board, Sensor, etc.) through JSON configuration files.
，EM  `Probe` ，，（Expose） D-Bus 。

### Probe (Detection conditions)
```json
"Probe": "TRUE"
```
「」。， `Probe: "FOUND('xyz.openbmc_project.FruDevice', 'BOARD_PRODUCT_NAME', 'MyBoard')"`， FRU ，，/。

### Exposes (Published hardware and sensors)
```json
{
  "Name": "Lab_Inlet_Temp",
  "Type": "TMP75",
  "Bus": 3,
  "Address": "0x48",
  ...
}
```
* **Name**: The Sensor name defined on D-Bus.
* **Type**:  dbus-sensors ，`TMP75`  `hwmontempsensor` 。
* **Bus & Address**: Used to match the hardware I2C address. Entity Manager will tell dbus-sensors where to find the data.

### Thresholds
```json
"Thresholds": [
  {"Direction": "greater than", "Name": "upper critical", "Severity": 1, "Value": 70},
  {"Direction": "greater than", "Name": "upper non critical", "Severity": 0, "Value": 60}
]
```
The warning and critical temperature thresholds are defined here.
* When the temperature is greater than 60 degrees, it triggers "upper non critical" (Warning).
* When the temperature is greater than 70 degrees, it triggers "upper critical" (Critical).

dbus-sensors ， Sensor  D-Bus  `xyz.openbmc_project.Sensor.Threshold.Warning`  `Critical` ，， D-Bus  (Signal)，、 SEL 。
