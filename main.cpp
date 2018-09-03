#include <iostream>
#include <unistd.h> //contains various constants
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include <string.h>
#include <arpa/inet.h> // defines in_addr structure


using namespace std;


void server() {
    //Create Socket
    int sockDesc = socket(AF_INET, SOCK_STREAM, 0);
    cout << "Desc:" << sockDesc << endl;
    if (sockDesc == -1) {
        cout << "Err Creating oscket";
        return;
    }

    //Bind socket to port & aceept connection from any IP of computer
    struct sockaddr_in ipOfServer;
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY);
    ipOfServer.sin_port = htons(2017);  //port no.
    int bindRetVal = bind(sockDesc, (struct sockaddr *) &ipOfServer, sizeof(ipOfServer));
    if (bindRetVal == -1) {
        cout << "Error binding socket\n";
        return;
    }
    //Can queue upto 20 clients
    listen(sockDesc, 20);

    //Descriptor for clients
    int clientDesc;
    char *data = "hello";
    char buffer[2000];
    string tosend;

    //Outer loop compulsory for accepting more than one clients
    while (true) {
        cout << "<Server Running >" << endl;

        //Accept connection and store in descriptor variable
        clientDesc = accept(sockDesc, (struct sockaddr *) NULL, NULL);

        //This loop for continuous conversation. You can have a simple read& write here then close connection
        while (1) {
            int bytesrec = read(clientDesc, buffer, 2000-1);
            if(bytesrec == 0){
                cout<<"< Connection Terminated abruptly >\n";
                break;
            }
            buffer[bytesrec] = '\0';
            cout << "Client:" << buffer<<endl;
            if(strcmp(buffer,"close") == 0) {
                cout<<"< Client Terminated Connection >\n\n";
                break;
            }
            cout<<"Server:";
            getline(cin,tosend);
//            cout<<"Read done, sending to client now...\n";
            write(clientDesc, tosend.c_str(), tosend.length());
        }

        //Close connection to client
        close(clientDesc);
        //To avoid too much pressure, poll for connection at second intervals
        sleep(1);
    }
}

#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void client() {
    //Create socket ,sockDes is socket descriptor
    int sockDes = socket(AF_INET, SOCK_STREAM, 0);
    cout << "Desc:" << sockDes << endl;
    if (sockDes == -1) {
        cout << "Err Creating socket";
    }

    // Set server info (IP & PORT)
    struct sockaddr_in ipOfServer;
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_port = htons(2017);
    ipOfServer.sin_addr.s_addr = inet_addr("127.0.0.1");

    char *data = "ClientData";
    char buffer[2000];

    //Initiate connection to server
    connect(sockDes, (struct sockaddr *) &ipOfServer, sizeof(ipOfServer));

    //Write some data
    write(sockDes, data, strlen(data));

    string tosend;
    while (true) {
        int bytesrec = read(sockDes, buffer, 2000);
        if(bytesrec == 0){
            cout<<"< Connection Terminated >\n";
            break;
        }
        buffer[bytesrec] = '\0';
        cout << "Server:" << buffer<<endl;
        cout << "Client:";
        getline(cin,tosend);
        write(sockDes,tosend.c_str(), tosend.length());
    }

}

#pragma clang diagnostic pop

int main() {
    int opt;
    cout<<"== Chat Program ==\n";
    cout<<"1. Run As server\t2. Run As Client\nChoice: ";
    cin >> opt;
    cin.ignore();

    if (opt == 1) server();
    else if(opt == 2) client();
    else {
        main();
    }
}