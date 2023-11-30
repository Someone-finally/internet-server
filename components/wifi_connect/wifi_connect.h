#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

 typedef int esp_err_t;

 void wifi_connect_init(void);
 esp_err_t wifi_connect_sta(char *ssid, char *pass, int timeout);
 void wifi_connect_ap(const char *ssid, const char *password);
 void wifi_disconnect(void);

#endif