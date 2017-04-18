
#include <SPI.h>
#include <WiFi101.h>

#define CS  6
#define RST 5
#define IRQ 4

#define PORT 23
#define TM_BUF_SIZE 100   // 1400

char testBuffer[TM_BUF_SIZE];
int listenSocket;

char ssid[] = "The Garage";      //  your network SSID (name)
char pass[] = "ytrewq12";   // your network password

// missing

unsigned long inet_addr(char* addr) {
  return -1;
}


void setup(void) {


  struct sockaddr_in destAddr;
  int returnVal;
  int status = WL_IDLE_STATUS;
  
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
  }

  // Bind the socket to the port and address at which we wish to receive data
  returnVal = bind(listenSocket, (sockaddr*)(&destAddr), sizeof(destAddr));
  if (returnVal < 0) {
    Serial.println("Call to bind failed.");
  }

  // Set up the socket as a listening socket.
  returnVal = listen(listenSocket, 0);
  if (returnVal < 0) {
    Serial.println("Call to listen failed.");
  }
}


void checkForConnection(void) {

  int newSocket;

  newSocket = accept(listenSocket, NULL, 0);
  if (newSocket < 0) {
    Serial.println("Call to accept failed.");
  } else {

  }
}


void checkForData(int socket) {

  int returnVal;

  returnVal = recv(socket, testBuffer, TM_BUF_SIZE, 0);
  if (returnVal < 0) {
    Serial.println("Call to recv failed.");
  } else if (returnVal == 0) {
    close(socket);
    Serial.println("Connection closed.");
  } else {
    Serial.print(testBuffer);
  }
}


void socketHandeler(SOCKET sock, uint8 u8Msg, void * pvMsg) {

  int ret = -1;

  switch (u8Msg) {
    case SOCKET_MSG_BIND: {
        tstrSocketBindMsg *pstrBind = (tstrSocketBindMsg*)pvMsg;
        if (pstrBind != NULL) {
          if (pstrBind->status == 0) {
            ret = listen(sock, 0);
            if (ret < 0)
              printf("Listen failure! Error = %d\n", ret);
          } else {
            M2M_ERR("bind Failure!\n");
            close(sock);
          }
        }
      }
      break;
    case SOCKET_MSG_LISTEN: {
        tstrSocketListenMsg *pstrListen = (tstrSocketListenMsg*)pvMsg;
        if (pstrListen != NULL) {
          if (pstrListen->status == 0) {
            ret = accept(sock, NULL, 0);
          } else {
            M2M_ERR("listen Failure!\n");
            close(sock);
          }
        }
      }
      break;
    case SOCKET_MSG_ACCEPT: {
        tstrSocketAcceptMsg *pstrAccept = (tstrSocketAcceptMsg*)pvMsg;
        if (pstrAccept->sock >= 0) {
          int TcpNotificationSocket = pstrAccept->sock;   // So like nowhere is TcpNotificationSocket defined. Whatever!
          recv(pstrAccept->sock, gau8RxBuffer, sizeof(gau8RxBuffer), TEST_RECV_TIMEOUT);
        } else {
          M2M_ERR("accept failure\n");
        }
      }
      break;

    default:
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


/* Do this forever... */
void loop(void) {

  checkForConnection();
  checkForData(0);
}

