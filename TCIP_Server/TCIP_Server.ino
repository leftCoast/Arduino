
#include <SPI.h>
#include <WiFi101.h>

#define CS  6
#define RST 5
#define IRQ 4

#define PORT 23
#define BUF_SIZE 100   // 1400
#define TEST_RECV_TIMEOUT 100

char testBuffer[BUF_SIZE+1];
int listenSocket;
bool socketList[TCP_SOCK_MAX];

char ssid[] = "The Garage";     //  your network SSID (name)
char pass[] = "ytrewq12";       // network password

void setup(void) {

  struct sockaddr_in destAddr;
  int returnVal;
  int status = WL_IDLE_STATUS;

  for (byte i=0;i<TCP_SOCK_MAX;i++) {
    socketList[i]=false;
  }
  
  //Configure pins for Adafruit ATWINC1500 Breakout
  WiFi.setPins(CS, IRQ, RST);

  Serial.begin(9600);
  while (!Serial);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network.
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  printWiFiStatus();
  
  // Initialize sockets.
  socketInit();

  registerSocketCallback(socketHandeler, NULL);

  // Specify the address family
  destAddr.sin_family = AF_INET;
  destAddr.sin_port = _htons(PORT);
  destAddr.sin_addr.s_addr = 0;     // Listining address, don't care.


  // Create a socket
  listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket == -1) {
    Serial.println("Call to socket failed.");
  } else {
    Serial.print("Created listen Socket:");Serial.println(listenSocket);
  }
  
  // Bind the socket to the port and address at which we wish to receive data
  returnVal = bind(listenSocket, (sockaddr*)(&destAddr), sizeof(destAddr));
  Serial.print("Calling bind returned:");Serial.println(returnVal);
  if (returnVal < 0) {
    Serial.println("Call to bind failed.");
  }

  // Set up the socket as a listening socket.
  returnVal = listen(listenSocket, 0);
  Serial.print("Calling listen returned:");Serial.println(returnVal);
  if (returnVal < 0) {
    Serial.println("Call to listen failed.");
  }
}


void checkForData(int socket) {

  int returnVal;

  returnVal = recv(socket, testBuffer, BUF_SIZE, 0);
  if (returnVal < 0) {
    Serial.println("Call to recv failed.");
  } else if (returnVal == 0) {
    close(socket);
    Serial.println("Connection closed.");
  } else {
    testBuffer[0] = '\0';
    testBuffer[returnVal+1]='\0';
    Serial.print(testBuffer);
  }
}


void socketHandeler(SOCKET sock, uint8 u8Msg, void * pvMsg) {

  int ret = -1;
  
  switch (u8Msg) {
    case SOCKET_MSG_BIND: {
        Serial.printf("Callback SOCKET_MSG_BIND");
        tstrSocketBindMsg *pstrBind = (tstrSocketBindMsg*)pvMsg;
        if (pstrBind != NULL) {
          if (pstrBind->status == 0) {
            ret = listen(sock, 0);
            if (ret < 0)
              Serial.printf("Listen failure! Error = %d\n", ret);
          } else {
            Serial.printf("bind Failure!\n");
            close(sock);
          }
        }
      }
      break;
    case SOCKET_MSG_LISTEN: {
        Serial.printf("Callback SOCKET_MSG_LISTEN");
        tstrSocketListenMsg *pstrListen = (tstrSocketListenMsg*)pvMsg;
        if (pstrListen != NULL) {
          if (pstrListen->status == 0) {
            ret = accept(sock, NULL, 0);
          } else {
            Serial.printf("listen Failure!\n");
            close(sock);
          }
        }
      }
      break;
    case SOCKET_MSG_ACCEPT: {
        Serial.printf("Callback SOCKET_MSG_ACCEPT");
        tstrSocketAcceptMsg *pstrAccept = (tstrSocketAcceptMsg*)pvMsg;
        if (pstrAccept->sock >= 0) {
          socketList[pstrAccept->sock] = true;
          testBuffer[0] = '\0';
          recv(pstrAccept->sock, testBuffer, sizeof(BUF_SIZE), TEST_RECV_TIMEOUT);
        } else {
          Serial.printf("accept failure\n");
        }
      }
      break;

    default:
      Serial.printf("Callback default?");
      break;
  }
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void loop(void) {
  for (byte i=0;i<TCP_SOCK_MAX;i++) {
    if (socketList[i]) {
      checkForData(i);
    }
  }  
}

