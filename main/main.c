#include <stdio.h>
#include <stdlib.h>
#include "wifi_connect.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "mdns.h"
#include "toggle_led.h"
#include <cJSON.h>
#include "push_btn.h"
#include <string.h>
#include "sensor_read.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"


static const char * TAG = "Internet CLIENT";
static httpd_handle_t server = NULL;
static const char *BASE_PATH = "/store";



void start_mdns_service()
{
  mdns_init();
 ESP_ERROR_CHECK(mdns_hostname_set("my-esp32"));
 ESP_ERROR_CHECK(mdns_instance_name_set("learn"));
}


static esp_err_t ondefault_url(httpd_req_t *req)
{
   ESP_LOGI(TAG,"Req : %s",req->uri);
   char path[540] = "/store";
   sprintf(path,"/store%s",req->uri);
   //strcat(path,req->uri);
   printf("The path is %s \n",path);

   char *ext = strrchr(req->uri,'.');
   if(ext)
   {
     if(strcmp(ext,".css")== 0) httpd_resp_set_type(req,"text/css");
     if(strcmp(ext,".js")== 0) httpd_resp_set_type(req,"text/javascript");
     if(strcmp(ext,".svg")== 0) httpd_resp_set_type(req,"image/svg+xml");
     if(strcmp(ext,".png")== 0) httpd_resp_set_type(req,"image/png");
     if(strcmp(ext,".jpg")== 0) httpd_resp_set_type(req,"image/jpg");
     if(strcmp(ext,".ico")== 0) httpd_resp_set_type(req,"image/vnd.microsoft.icon");
   }
  
  
    FILE *file;
    file = fopen(path,"r");
    // file = fopen("/store/index.html","r");
    if(file == NULL)
    {
      file = fopen("/store/index.html","r");
      //ESP_LOGE(TAG,"/store/index.html not found");
      if(file == NULL)
      {
          httpd_resp_send_404(req);
      }
    }   

  char buffer[1024];
  int bytes_read = 0;
  while((bytes_read = fread(buffer,sizeof(char),sizeof(buffer),file)) > 0)
  {
    httpd_resp_send_chunk(req,buffer,bytes_read);
  }

  fclose(file);
  httpd_resp_send_chunk(req,NULL,0);
  return ESP_OK;
}

  static esp_err_t  toggle_led_url(httpd_req_t *req)
 {
  char buffer[100];
  memset(&buffer,0, sizeof(buffer));
  httpd_req_recv(req,buffer,req->content_len);
  cJSON *payload = cJSON_Parse(buffer);
  cJSON *is_on_json = cJSON_GetObjectItem(payload,"is_on");
  bool is_on = cJSON_IsTrue(is_on_json);
  cJSON_Delete(payload);
  printf("Your boolean variable is: %s \n", is_on ? "true" : "false");
  toggle_bool(is_on);
  httpd_resp_set_status(req,"204 NO CONTENT");
  httpd_resp_send(req,"RECIEVED",HTTPD_RESP_USE_STRLEN);
  return ESP_OK;

 }

/*********** Web Socket ***************/

 #define WS_MAX_SIZE 1024
 static int client_session_id;

 esp_err_t send_ws_message(char *message)
{
  if (!client_session_id)
  {
    ESP_LOGE(TAG, "no client_session_id");
    return -1;
  }
  httpd_ws_frame_t ws_message = {
      .final = true,
      .fragmented = false,
      .len = strlen(message),
      .payload = (uint8_t *)message,
      .type = HTTPD_WS_TYPE_TEXT};
  return httpd_ws_send_frame_async(server, client_session_id, &ws_message);
}


 static esp_err_t  on_web_socket_url(httpd_req_t *req)
 {
   client_session_id = httpd_req_to_sockfd(req);
   if(req->method == HTTP_GET)
    return ESP_OK;
   
   httpd_ws_frame_t ws_pkt;
   memset(&ws_pkt,0,sizeof(httpd_ws_frame_t));
   ws_pkt.type = HTTPD_WS_TYPE_TEXT;
   ws_pkt.payload = malloc(WS_MAX_SIZE);

   httpd_ws_recv_frame(req,&ws_pkt,WS_MAX_SIZE);
   printf("WS Payload is %.*s \n",ws_pkt.len,ws_pkt.payload);

   free(ws_pkt.payload); 

    char *response = "connected OK 😊";
    
    
   httpd_ws_frame_t ws_responce = {
      .final = true,
      .fragmented = false,
      .type = HTTPD_WS_TYPE_TEXT,
      .payload = (uint8_t *)response,
      .len = strlen(response)};
  return httpd_ws_send_frame(req, &ws_responce);
}

/**************************************/
void mount_fs(void)
{
  esp_vfs_fat_mount_config_t esp_vfs_fat_mount_config = {
    .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
    .max_files = 5,
    .format_if_mount_failed = true,
  };

  esp_vfs_fat_spiflash_mount_ro(BASE_PATH, "storage", &esp_vfs_fat_mount_config);

}

//Initialising different wildcards for the webserver
static void init_server()
{

    httpd_config_t httpd_config = HTTPD_DEFAULT_CONFIG();
    httpd_config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_ERROR_CHECK(httpd_start(&server,&httpd_config));

    httpd_uri_t toggleled_url = 
    {
        .uri = "/api/toggle-led/",
        .method = HTTP_POST,
        .handler = toggle_led_url
    };
    httpd_register_uri_handler(server,&toggleled_url);

     httpd_uri_t web_socket_url = 
 {
     .uri = "/ws/*",
     .method = HTTP_GET,
     .handler = on_web_socket_url,
     .is_websocket = true
 };
 httpd_register_uri_handler(server,&web_socket_url);

     httpd_uri_t default_url = 
    {
        .uri = "/*",
        .method = HTTP_GET,
       .handler = ondefault_url
    };
  httpd_register_uri_handler(server,&default_url);

}

void app_main(void)
{
  ESP_ERROR_CHECK(nvs_flash_init());
  init_led();
  init_btn();
  read_pot_value();
  wifi_connect_init();
  //Type in your wifi cid and password.
  wifi_connect_sta("Itsawifi","ciqr7484",10000);
  start_mdns_service();
  mount_fs();

  init_server();

}

