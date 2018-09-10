
#ifndef COM_H
#define COM_H

#include <Arduino.h>
//#include "MDNS.h"
#include <WiFi.h>
#include <vector>
#include <SimpleBLE.h>
// #include "AsyncJson.h"
#include "ArduinoJson.h"
#include "ESPAsyncWebServer.h"
#include "painlessMesh.h"
//#include <ESPmDNS.h>

#include "system_info.h"

#define MAX_CLIENTS 10
#define WL_MAC_ADDR_LENGTH 6


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void handleWiFiEvent(WiFiEvent_t event);

// Task to blink the number of nodes
// Task blinkNoNodes;
// bool onFlag = false;
// bool calc_delay = false;

//Mesh Functionality
void mesh_receivedCallback(uint32_t from, String & msg);
void mesh_newConnectionCallback(uint32_t nodeId);
void mesh_changedConnectionCallback();
void mesh_nodeTimeAdjustedCallback(int32_t offset);
void mesh_delayReceivedCallback(uint32_t from, int32_t delay);

class COM { //, public Subject{
  //In which we send information

public:
  COM(): server(BEEMO_PORT), events("/events"), ws("/ws") {};
         //,telemetry_server(TELEM_PORT){};

  bool is_wifi_on = false;
  // void wifi_on();
  // void wifi_off();

  //Event Timing
  unsigned long old_time;
  int tickCount = 1000;
  int telCount = 100;
  int _tick = tickCount - 1; //Tick One Less than tickCount will print first time
  int _telk = telCount - 1;

  AsyncWebServer server;
  AsyncEventSource events;
  AsyncWebSocket ws; // access at ws://[esp ip]/ws
  painlessMesh  mesh;
  Scheduler userScheduler;
  SimpleList<uint32_t> nodes;
  //SimpleBLE ble;

  //Direct Telemetry Server For High Bandwith Operations
  bool telemetry_activated = false;
  //WiFiServer telemetry_server;
  //WiFiClient telemetry_client;

  //These Parse The Current Clients For Uses In Our COM Protocol
  int clientCount = 0;
  uint32_t startLoop;
  uint32_t firstActive = 0;
  uint32_t lastActive = 1; //Store The Maximum Active For Relative Client Couting
  bool _loopClient = false;
  bool clientsActive = false;
  bool activeClients[MAX_CLIENTS];

  //Message Parsing
  String lastMsg;
  String unParsedMsg;

  //Boolean Values
  bool mdns_success;
  bool debugMode = true;
  bool writeNow = true; //Tells log to write. True Means First Time will print
  bool telNow = true;

  //Important Functions
  void initialize();
  // void setup_wifi();
  void setup_mesh();
  // void setup_ble_beacon();
  // void update_ble_beacon();
  void scanWifiNetworks();
  void initialize_server();


  bool mesh_get_node_report(uint32_t client_id, IPAddress client_ip, uint32_t root_id);
  void mesh_reply_to_report(uint32_t client_id, String client_ip, uint32_t root_id);
  void send_external( uint32_t client_id, String client_ip, String msg);

  //Custom Loop Logic -> Some Depriciated
  void update();
  void open();
  void close();
  void flush();
  String read();
  void tick();
  void start_cycle();

  //Async Server Stuff
  //void onRequest(AsyncWebServerRequest *request);
  //void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
  //void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
  //void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
  void broadcastClients(String msg);
  void broadcastPrimary(String msg);

  void log(String message,bool force=false);


};


#endif //COM_H
