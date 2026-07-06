# Q37. Wi-Fi Security for IoT

Wi-Fi connectivity enables IoT devices such as ESP32-based sensors, smart appliances, and industrial controllers to communicate with cloud services and mobile applications. However, wireless communication exposes IoT devices to several security threats that can compromise data confidentiality, device integrity, and system availability.

One major attack vector is the **WPA2 vulnerability**, most notably the **KRACK (Key Reinstallation Attack)**. KRACK exploits weaknesses in the WPA2 four-way handshake process used during Wi-Fi authentication. An attacker within radio range can manipulate encryption keys and intercept or replay traffic between the IoT device and the access point, potentially exposing sensitive information such as sensor data, credentials, or control commands. Although patches exist, many low-cost IoT devices never receive firmware updates, making them long-term targets. ([Wikipedia][1])

Another common threat is the **Evil Twin Attack**, where an attacker creates a fake Wi-Fi access point with the same SSID as a legitimate network. IoT devices may automatically connect to the stronger signal, allowing attackers to perform man-in-the-middle attacks, capture credentials, or inject malicious commands.

To secure ESP32 Wi-Fi connections, several best practices should be followed. Developers should use **WPA3** whenever supported, as it offers stronger authentication and protection mechanisms than WPA2. The ESP32 platform supports WPA3 and certificate-based authentication methods in enterprise environments. ([Espressif Systems][2])

Another important technique is **certificate pinning**, where the ESP32 stores the expected server certificate or public key and verifies it before establishing an HTTPS or MQTT over TLS connection. This prevents attackers from impersonating cloud servers using fraudulent certificates.

Additional security measures include:

* Using HTTPS or MQTT over TLS instead of plain HTTP or MQTT.
* Enabling secure boot and flash encryption.
* Keeping firmware updated with OTA updates.
* Using strong and unique Wi-Fi passwords.
* Segmenting IoT devices on a separate network or VLAN.

Hardcoding Wi-Fi credentials directly into source code is dangerous because anyone with access to the firmware binary, public repository, or physical device can extract the SSID and password from flash memory. If the same credentials are reused across multiple deployments, compromise of one device can lead to compromise of the entire network. Credentials should instead be stored securely using encrypted storage, provisioning methods, or secure elements. ([Hackster][3])

## References

1. Espressif Systems, *ESP32 Wi-Fi Security Documentation*. ([Espressif Systems][2])
2. Mathy Vanhoef and Frank Piessens, *KRACK: Key Reinstallation Attacks Against WPA2*. ([Wikipedia][1])
3. NCC Group, *Hardware Security by Design: ESP32 Guidance*. ([nccgroup.com][4])

[1]: https://en.wikipedia.org/wiki/KRACK?utm_source=chatgpt.com "KRACK"
[2]: https://docs.espressif.com/projects/esp-idf/en/v5.2/esp32/api-guides/wifi-security.html?utm_source=chatgpt.com "Wi-Fi Security - ESP32 - — ESP-IDF Programming Guide ..."
[3]: https://www.hackster.io/sudoyasir/esp32-wifi-security-explained-for-beginners-6050fd?utm_source=chatgpt.com "ESP32 WiFi Security Explained for Beginners"
[4]: https://www.nccgroup.com/research/hardware-security-by-design-esp32-guidance/?utm_source=chatgpt.com "Hardware Security By Design: ESP32 Guidance"
