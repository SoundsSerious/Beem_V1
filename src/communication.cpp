#include "globals.h"

void handleWiFiEvent(WiFiEvent_t event){
    switch(event) {
        case SYSTEM_EVENT_AP_START:
            if (PRINT_COM) {beem.com.log("AP Started",true);}
            WiFi.softAPsetHostname(SSID_AP);
            break;
        case SYSTEM_EVENT_AP_STOP:
              if (PRINT_COM) {beem.com.log("AP Stopped",true);}
            break;
        case SYSTEM_EVENT_STA_START:
              if (PRINT_COM) {beem.com.log("STA Started",true);}
            WiFi.setHostname(SSID_AP);
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
              if (PRINT_COM) {beem.com.log("STA Connected",true);}
            WiFi.enableIpV6();
            break;
        case SYSTEM_EVENT_AP_STA_GOT_IP6:
              if (PRINT_COM) {beem.com.log("STA IPv6: "+WiFi.localIPv6().toString(),true);}
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
              if (PRINT_COM) {beem.com.log("STA IPv4: "+WiFi.localIP().toString(),true);}
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
              if (PRINT_COM) {beem.com.log("STA Disconnected",true);}
            break;
        case SYSTEM_EVENT_STA_STOP:
              if (PRINT_COM) {beem.com.log("STA Stopped",true);}
            break;
        default:
            break;
    }
}

void onRequest(AsyncWebServerRequest *request){
  //Handle Unknown Request
    if (PRINT_COM) {beem.com.log("Unknown Page: "+request -> url(),true);}
  request->send(404);
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  //Handle body
    if (PRINT_COM) {beem.com.log("Handle Body Message",true);}
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  //Handle upload
    if (PRINT_COM) {beem.com.log("Upload",true);}
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  //Handle WebSocket event
  if(type == WS_EVT_CONNECT){
    //client connected
    //os_printf("ws[%s][%u] connect\n", server->url(), client->id());
    if (PRINT_COM) {client->printf("Hello Client %u :)", client->id());}
    if (PRINT_COM) {beem.com.log( "ws connect C:" +String(client->id()),true);}
    client->ping();
  } else if(type == WS_EVT_DISCONNECT){
    //client disconnected
    if (PRINT_COM) {beem.com.log( "ws disconnect S: "+ String(server->url()) +"C:" +String(client->id()),true) ;}
  } else if(type == WS_EVT_ERROR){
    //error was received from the other end
    if (PRINT_COM) {beem.com.log( "ws error S: "+ String(server->url()) +"C:" +String(client->id()),true) ;}
  } else if(type == WS_EVT_PONG){
    if (PRINT_COM) {beem.com.log( "ws pong S: "+ String(server->url()) +"C:" +String(client->id()),true) ;}
    //pong message was received (in response to a ping request maybe)
  } else if(type == WS_EVT_DATA){
    //data packet
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      //os_printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
      if(info->opcode == WS_TEXT){
        data[len] = 0;
        if (PRINT_COM) {beem.com.log( "msg S: "+ String(server->url()) +"C:" +String(client->id()) + String((char*)data),true) ;}
        //os_printf("%s\n", (char*)data);
      } else {
        for(size_t i=0; i < info->len; i++){
          //os_printf("%02x ", data[i]);
        }
        //os_printf("\n");
      }
      if(info->opcode == WS_TEXT)
        if (PRINT_COM) {client->text("I got your text message");}
      else
        if (PRINT_COM) {client->binary("I got your binary message");}
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0){}//os_printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        //os_printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }
      //os_printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      if(info->message_opcode == WS_TEXT){
        data[len] = 0;
        //os_printf("%s\n", (char*)data);
        if (PRINT_COM) {beem.com.log( "msg S: "+ String(server->url()) +"C:" +String(client->id()) + String((char*)data),true) ;}
      } else {
        for(size_t i=0; i < len; i++){
          //os_printf("%02x ", data[i]);
        }
        //os_printf("\n");
      }

      if((info->index + len) == info->len){
        //os_printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          //os_printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT)
            if (PRINT_COM) {client->text("I got your text message");}
          else
            if (PRINT_COM) {client->binary("I got your binary message");}
        }
      }
    }
  }
}


void COM::initialize(){
    if (LOG_DEBUG){Serial.begin( 115200 );} //Open Serial...Mmm breakfast
    initialize_server();
    //setup_ble_beacon();
}

void COM::setup_mesh(){
  //setup_mesh();
  mesh.init(SSID_AP, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION);

  // mesh.onReceive(&mesh_receivedCallback);
  // mesh.onNewConnection(&mesh_newConnectionCallback);
  // mesh.onChangedConnections(&mesh_changedConnectionCallback);
  // mesh.onNodeTimeAdjusted(&mesh_nodeTimeAdjustedCallback);
  // mesh.onNodeDelayReceived(&mesh_delayReceivedCallback);


  //Task taskSendMessage( TASK_SECOND * 1, TASK_FOREVER, &sendMessage );
  //userScheduler.addTask( taskSendMessage );
  //taskSendMessage.enable();

  // blinkNoNodes.set(BLINK_PERIOD, (mesh.getNodeList().size() + 1) * 2, []() {
  //   // If on, switch off, else switch on
  //   if (onFlag)
  //     onFlag = false;
  //   else
  //     onFlag = true;
  //   blinkNoNodes.delay(BLINK_DURATION);
  //
  //   if (blinkNoNodes.isLastIteration()) {
  //     // Finished blinking. Reset task for next run
  //     // blink number of nodes (including this node) times
  //     blinkNoNodes.setIterations((this -> mesh.getNodeList().size() + 1) * 2);
  //     // Calculate delay based on current mesh time and BLINK_PERIOD
  //     // This results in blinks between nodes being synced
  //     blinkNoNodes.enableDelayed(BLINK_PERIOD -
  //         (this -> mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
  //   }
  //   });
  // userScheduler.addTask(blinkNoNodes);
  // blinkNoNodes.enable();
}

void COM::initialize_server(){
  //setup_wifi();

  setup_mesh();

  // attach AsyncWebSocket
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  // attach AsyncEventSource
  server.addHandler(&events);

  //MDNS.addService("beem","tcp",BEEMO_PORT);

  // respond to GET requests on URL /heap
  server.on("/heap", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (PRINT_COM) {this -> log("GET HEAP",true);}
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  // upload a file to /upload
  server.on("/upload", HTTP_POST, [this](AsyncWebServerRequest *request){
    if (PRINT_COM) {this -> log("POST UPLOAD",true);}
    request->send(200);
  }, onUpload);

  server.on("/lights/on", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (PRINT_COM) {this -> log("Lights ON",true);}
    beem.lights._on = true;
    request->send(200);
  });

  server.on("/lights/off", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (PRINT_COM) {this -> log("Lights OFF",true);}
    beem.lights._on = false;
    request->send(200);
  });

  // send a file when /index is requested
  server.on("/index", HTTP_ANY, [this](AsyncWebServerRequest *request){
    //request->send(SPIFFS, "/index.htm");
    if (PRINT_COM) {this -> log("INDEX",true);}
    request->send(200, "/index.htm");
  });

  server.on("/lights/settings/set", HTTP_GET, [this](AsyncWebServerRequest *request){
    int params = request->params();
    int i;
    uint8_t input;
    for(i=0;i<params;i++){
      AsyncWebParameter* h = request->getParam(i);
      String name = String(h->name().c_str());
      String value = String(h->value().c_str());
      if (name == "power"){
          int val = value.toInt();
          if (val == 0){ beem.lights._on = false;}
          if (val == 1){ beem.lights._on = true;}
        }
      if (name == "temp"){  beem.lights.setTemerature(value.toInt()); }
      if (name == "brightness"){ beem.lights.setBrightness(value.toInt()); }
    }

    request->send(200);
  });

  server.on("/lights/settings/get", HTTP_GET, [this](AsyncWebServerRequest *request){
    AsyncResponseStream *response = request->beginResponseStream("text/json");
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject &settings = jsonBuffer.createObject();
    settings["temp"] = beem.lights.temperature;
    settings["power"] = beem.lights._on;
    settings["brightness"] = beem.lights.brightness_multiplier;
    settings.printTo(*response);
    request->send(response);
  });

  // send a file when /index is requested
  server.on("/mode/color/set", HTTP_GET, [this](AsyncWebServerRequest *request){
    //this -> log("CLIENT POSITION",true);
    int params = request->params();
    int i;
    uint8_t input [4] = {0,0,0,0};
    for(i=0;i<params;i++){
      AsyncWebParameter* h = request->getParam(i);
      String name = String(h->name().c_str());
      String value = String(h->value().c_str());
      if (name == "I"){ input[0] = (uint8_t)(value.toInt()); }
      if (name == "H"){ input[1] = (uint8_t)(value.toInt()); }
      if (name == "S"){ input[2] = (uint8_t)(value.toInt()); }
      if (name == "V"){ input[3] = (uint8_t)(value.toInt()); }
    }

   ModeColorEv * comEvent = new ModeColorEv(input[0],input[1],input[2],input[3]);
   beem.event_queue.addEvent( comEvent );

    request->send(200);
  });

  // send a file when /index is requested
  server.on("/mode/palette/set", HTTP_GET, [this](AsyncWebServerRequest *request){

    int params = request->params();
    int i;
    uint8_t input;
    for(i=0;i<params;i++){
      AsyncWebParameter* h = request->getParam(i);
      String name = String(h->name().c_str());
      String value = String(h->value().c_str());
      if (name == "I"){ input = (uint8_t)(value.toInt()); }
    }
   ModePalletteEv * comEvent = new ModePalletteEv(input);
   beem.event_queue.addEvent( comEvent );

    request->send(200);
  });

  // send a file when /index is requested
  server.on("/mode/settings/set", HTTP_GET, [this](AsyncWebServerRequest *request){

    int params = request->params();
    int i;
    String input[4]; //Index,Value,Min,Max
    for(i=0;i<params;i++){
      AsyncWebParameter* h = request->getParam(i);
      String name = String(h->name().c_str());
      String value = String(h->value().c_str());
      if (name == "INX"){ input[0] = value; }
      if (name == "SET"){ input[1] = value; }
      if (name == "MAX"){ input[3] = value; }
      if (name == "MIN"){ input[2] = value; }
    }
    if ( input[2].length() == 0 && input[3].length() == 0){ //Direct SEtting
      ModeSettingEv * comEvent = new ModeSettingEv( ((uint8_t) input[0].toInt()),
                                                    ((uint8_t) input[1].toInt()));
      beem.event_queue.addEvent( comEvent );
    }
    //Check if min and max bounds were created


    request->send(200);
  });

  // send a file when /index is requested
  server.on("/mode/select", HTTP_GET, [this](AsyncWebServerRequest *request){
    //this -> log("CLIENT POSITION",true);
    int params = request->params();
    int i;
    for(i=0;i<params;i++){
      AsyncWebParameter* h = request->getParam(i);
      String name = String(h->name().c_str());
      String value = String(h->value());
      if (name == "INDEX"){
        int gamesel = value.toInt();
        if (gamesel >=0 ){
          unsigned int selectedMode = (unsigned int)(gamesel);
          beem.games_manager.switchToMode(selectedMode);
        }
      }
    }
    request->send(200);
  });

  // HTTP basic authentication
  // server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
  //   this -> log("GET LOGIN",true);
  //   if(!request->authenticate(http_username, http_password))
  //       return request->requestAuthentication();
  //   request->send(200, "text/plain", "Login Success!");
  // });

  server.onNotFound(onRequest);
  server.onFileUpload(onUpload);
  server.onRequestBody(onBody);

  server.begin();
  //if (LOG_DEBUG){ startTelemetryServer();}
} //End initialize_server


void COM::setup_wifi(){
  wifi_on();
  WiFi.onEvent(handleWiFiEvent);
  scanWifiNetworks();
}

void COM::wifi_on(){
  log("Turning Wifi ON..",true);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID_AP);
  is_wifi_on = true;
}

void COM::wifi_off(){
  log("Turning Wifi OFF..",true);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  is_wifi_on = false;
}




void COM::mesh_receivedCallback(uint32_t from, String & msg) {
  log("startHere: Received from"+String(from)+"msg=" + msg.c_str());
}

void COM::mesh_newConnectionCallback(uint32_t nodeId) {
  // Reset blink task
  //onFlag = false;
  //blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
  //blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);

  log("--> startHere: New Connection, nodeId: " +String(nodeId));
}

void COM::mesh_changedConnectionCallback() {
  log("Changed connections: " + String(mesh.subConnectionJson().c_str()));
  // Reset blink task
  //onFlag = false;
  //blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
  //blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);

  nodes = mesh.getNodeList();

  log("Num nodes: " + String(nodes.size()));
  log("Connection list:");

  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end()) {
    log(String( *node) );
    node++;
  }
  //calc_delay = true;
}

void COM::mesh_nodeTimeAdjustedCallback(int32_t offset) {
  log("Adjusted time "+String(mesh.getNodeTime())+ " Offset =" + String(offset) );
}

void COM::mesh_delayReceivedCallback(uint32_t from, int32_t delay) {
  log("Delay to node"+ String(from) +"is" + String(delay) );
}


void COM::flush()
{
    if (LOG_DEBUG){Serial.flush();}
}

void COM::scanWifiNetworks(){
  log("scan start "+String(micros()));

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  log("scan done");
  if (n == 0) {
      log("no networks found");
  } else {
      log(String(n)+" networks found");
      for (int i = 0; i < 10; ++i) {
          // Print SSID and RSSI for each network found
          log( "SSID:\t" + String(WiFi.SSID(i)));
          log( "RSSI:\t" + String(WiFi.RSSI(i)));
          //delay(10);
      }
  }
  log("scan end "+String(micros()));
}

void COM::open(){
  //Determine Number Of Clients
  clientCount = 0; // Needs To Be Zero For Loop
  for (int i=lastActive; i<MAX_CLIENTS+lastActive; i++){
    _loopClient = ws.hasClient(i);
    activeClients[i-1] = _loopClient; //Diffferent Indexing For Array
    if (_loopClient){clientCount++;}
    if(!clientsActive && _loopClient) {
        firstActive = i;
        lastActive =  i;
    }
    if(clientCount > 0){clientsActive = true;}
  }
  if (PRINT_COM) {log("FirstActive: "+String(firstActive));}
  if (PRINT_COM) {log("LastActive: "+String(lastActive));}
  if (PRINT_COM) {log("Active WSClients: "+String(ws.count()));}
  if (PRINT_COM) {log("Active Clients: "+String(clientCount));}
}

void COM::close(){
  clientsActive = false;
  firstActive = 0;
}

void COM::tick(){
  _tick += 1;
  _telk += 1;
  //log("TICK: --> "+String( micros()-startLoop));
  //log("TOCK: --> "+String( beem.mpu.cycle_count ));
  //log("CPS: --> "+String( beem.mpu.cycle_count /(micros()/1E6) ));
  if (_tick == tickCount){
    writeNow = true;
  }
  else{
    writeNow = false;
  }
  if (_tick > tickCount){
    _tick = 0;
  }
  //Telemtery Counting
  if (_telk == telCount){
    telNow = true;
  }
  else{
    telNow = false;
  }
  if (_telk > telCount){
    _telk = 0;
  }
}

void COM::start_cycle()
{
  startLoop = micros();
}

void COM::update(){
  if (PRINT_COM) {log("Update COM");}
  //serveTelemetry();
  //str_send_telemetry();
}

void COM::broadcastClients(String msg)
{
  if (clientsActive){ws.textAll(msg);}
}

void COM::broadcastPrimary(String msg)
{ //Get This Guy Going At A Clip
  //Assume We Have An Active First Client - ID Starts at 1 baby
  if (clientsActive){ws.text(firstActive,msg);}
  //ws.binary(firstActive,(char*) msg.c_str());
  //AsyncWebSocketClient *c = ws.client(lastActive);
  //AsyncClient *cl = c -> client();
  //cl -> write( (char*) msg.c_str() );

}


void COM::log(String message, bool force){
  //Super Debug Mode Will Try Both Serial And WiFi-zle if it's turn
  //We will default to serial always for zeee robust debugging
  if ( writeNow || force){
    if (LOG_DEBUG){Serial.println( "LOG:\t"+message  );}
    broadcastPrimary( "LOG:\t"+message );
  }
}
