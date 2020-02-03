#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include "JSONc.c"
#include "getch.c"
#define MAX 101 //This is size of strings
#define altMAX 100001 //This is alternative MAX for string size
#define PORT 12345 //Server listener port
#define SA struct sockaddr 

char buffer[altMAX], username[MAX], password[MAX], authToken[MAX], 
respondType[MAX], respondContent[MAX], channelName[MAX], keyPress;
int clientSocket, command;
bool logedIn = false, inChannel = false; //Status booleans

//Functions prototypes
void connectSocket();//Connect to given IP and port
void sendAndRecv();//Send buffer to server and fill buffer with server respond json
void readJson();//Read respond json from buffer and put it in respondType & respondContent
int mainMenu();
void signup();
void login();
void about();
void dashboard();
void creatChannel();
void joinChannel();
void logout();
void chatMenu();
void sendMessage();
void refreshChannel();
void channelMembers();
void leaveChannel();
void readMessage();//Read messages json and print it to terminal
void readMembers();//Read members json and print it to terminal
void searchMembers();
void pointerCheck(int optionNum);

int main(){
    if (!mainMenu()) return 0;
    return 0;
}

void connectSocket() {
    struct sockaddr_in servaddr; 
	// Create and verify socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
	if (clientSocket == -1) { 
		printf("Socket creation failed...\n"); 
		exit(0);
	}
	// Assign IP and port
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 
	// Connect the client socket to server socket 
	if (connect(clientSocket, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("Connection to the server failed...\n"); 
		exit(0); 
	} 
}

void sendAndRecv(){
    connectSocket();
    send(clientSocket, buffer, strlen(buffer), 0);//Send buffer
    memset(buffer, 0, sizeof(buffer));//Clear buffer
    recv(clientSocket, buffer, sizeof(buffer), 0);//Receive buffer
}

void readJson(){
    JSONc *respond = JSONc_parse(buffer);
    memset(respondType, 0, sizeof(respondType));//Clearing respondType & respondContent
    memset(respondContent, 0, sizeof(respondContent));
    strcat(respondType, JSONc_getObjectItem(respond, "type") -> stringVal);
    strcat(respondContent, JSONc_getObjectItem(respond, "content") -> stringVal);
    JSONc_delete(respond);
}

int mainMenu() {
    while (true){
        command = 1;
        system("clear");
        while (true)
        {
            printf("\n---Main menu---\n\n");
            pointerCheck(1);
            printf("Signup\n");
            pointerCheck(2);
            printf("Login\n");
            pointerCheck(3);
            printf("Quit\n");
            pointerCheck(4);
            printf("About\n\n");
            pointerCheck(-1);
            keyPress = getch();
            if (keyPress == 115) command++;
            if (keyPress == 119) command--;
            if (keyPress == '\n') break;
            system("clear");
        }
        switch (command) {
            case 1:
                signup();
                break;
            case 2:
                login();
                break;
            case 3:
                return 0;
                break;
            case 4:
                about();
                break;
            default:
                printf("Please enter a valid command\n");
                break;
        }
    }
}

void signup() {
    printf("\nEnter your username : ");
    scanf("%s%*c", username);
    printf("Enter your password : ");
    scanf("%s%*c", password);
    //Making request
    memset(buffer, 0, sizeof(buffer));//Clearing buffer
    strcat(buffer, "register ");
    strcat(buffer, username);
    strcat(buffer, ", ");
    strcat(buffer, password);
    strcat(buffer, "\n");
    //End of making request
    sendAndRecv();
    readJson();
    if(!strcmp(respondType, "Error"))
        printf("\nError : %s\n", respondContent);
    if (!strcmp(respondType, "Successful"))
        printf("\nYou successfully signed up\n");
    getchar();
}

void login() {
    printf("\nEnter your username : ");
    scanf("%s%*c", username);
    printf("Enter your password : ");
    scanf("%s%*c", password);
    //Making request
    memset(buffer, 0, sizeof(buffer));//Clearing buffer
    strcat(buffer, "login ");
    strcat(buffer, username);
    strcat(buffer, ", ");
    strcat(buffer, password);
    strcat(buffer, "\n");
    //End of making request
    sendAndRecv();
    readJson();
    if (!strcmp(respondType, "Error")){
        printf("\nError : %s\n", respondContent);
        getchar();
    }
    else if (!strcmp(respondType, "AuthToken")){
	    memset(authToken, 0, sizeof(authToken));
        strcat(authToken, respondContent);
        printf("\nYou successfully loged in\n");
        getchar();
        logedIn = true;//Change logedIn state
        dashboard();
    }
}

void about(){
    printf("\nMessenger version 1.00 client\n");
    printf("Written in C by Parsa Mohammadian\n");
    printf("https://github.com/Parsa2820\n");
    getchar();
}

void dashboard(){
    while(logedIn){
command = 1;
        system("clear");
        while(true)
        {
            printf("\n---User dashboard---\n\n");
            pointerCheck(1);
            printf("Create a channel\n");
            pointerCheck(2);
            printf("Join a channel\n");
            pointerCheck(3);
            printf("Logout\n");
            pointerCheck(-1);
            keyPress = getch();
            if (keyPress == 115) command++;
            if (keyPress == 119) command--;
            if (keyPress == '\n') break;
            system("clear");
        }
        switch (command)
        {
        case 1:
            creatChannel();
            break;
        case 2:
            joinChannel();
            break;
        case 3:
            logout();
            break;
        default:
            printf("\nPlease enter a valid command\n");
            break;
        }
    }
}

void creatChannel(){
    printf("\nEnter channel name : ");
    scanf("%s%*c", channelName);
    //Making request
    memset(buffer, 0, sizeof(buffer));//Clear buffer
    strcat(buffer, "create channel ");
    strcat(buffer, channelName);
    strcat(buffer, ", ");
    strcat(buffer, authToken);
    strcat(buffer, "\n");
    //End of making request
    sendAndRecv();
    readJson();
    if (!strcmp(respondType, "Error")){
        printf("\nError : %s\n", respondContent);
        getchar();
    }
    else if (!strcmp(respondType, "Successful")){
        printf("\nChannel created successfully\n");
        inChannel = true;//Change inChannel state
        getchar();
        chatMenu();
    }
}

void joinChannel(){
    printf("\nEnter channel name : ");
    scanf("%s%*c", channelName);
    //Making request
    memset(buffer, 0, sizeof(buffer));//Clearing buffer
    strcat(buffer, "join channel ");
    strcat(buffer, channelName);
    strcat(buffer, ", ");
    strcat(buffer, authToken);
    strcat(buffer, "\n");
    //End of making request
    sendAndRecv();
    readJson();
    if (!strcmp(respondType, "Error")){
        printf("\nError : %s\n", respondContent);
        getchar();
    }
    else if (!strcmp(respondType, "Successful")){
        printf("\nYou joined channel successfully\n");
        inChannel = true;//Change inChannel state
        getchar();
        chatMenu();
    }
}

void logout(){
    //Making request
    memset(buffer, 0, sizeof(buffer));//Clearing buffer
    strcat(buffer, "logout ");
    strcat(buffer, authToken);
    strcat(buffer, "\n");
    //End of making request
    sendAndRecv();
    readJson();
    if (!strcmp(respondType, "Error"))
        printf("Error : %s", respondContent);
    else if (!strcmp(respondType, "Successful")){
        printf("You successfully loged out\n");
        logedIn = false;//Change logedIn state
    }
    getchar();
}

void chatMenu(){
    while(inChannel){
command = 1;
        system("clear");
        while (true)
        {
            printf("\n---Chat menu---\n\n");
            pointerCheck(1);
            printf("Send message\n");
            pointerCheck(2);
            printf("Refresh messages\n");
            pointerCheck(3);
            printf("Channel members\n");
            pointerCheck(4);
            printf("Leave channel\n");
            pointerCheck(5);
            printf("Search members\n");
            pointerCheck(-1);
            keyPress = getch();
            if (keyPress == 115) command++;
            if (keyPress == 119) command--;
            if (keyPress == '\n') break;
            system("clear");
        }
        switch (command)
        {
        case 1:
            sendMessage();
            break;
        case 2:
            refreshChannel();
            break;
        case 3:
            channelMembers();
            break;
        case 4:
            leaveChannel();
            break;
        case 5:
            searchMembers();
            break;
        default:
            printf("Enter a valid command\n");
            break;
        }
    }
}

void sendMessage(){
    char message[altMAX];
    printf("\nEnter your message and press enter : ");
    scanf("%[^\n]%*c", message);
    //Making request
    memset(buffer, 0, sizeof(buffer));//Clearing buffer
    strcat(buffer, "send ");
    strcat(buffer, message);
    strcat(buffer, ", ");
    strcat(buffer, authToken);
    strcat(buffer, "\n");
    //End of making request
    sendAndRecv();
    readJson();
    if (!strcmp(respondType, "Error")){
        printf("\nError : %s\n", respondContent);
        getchar();
    }
}

void refreshChannel(){
    //Making request
    memset(buffer, 0, sizeof(buffer));//Clearing buffer
    strcat(buffer, "refresh ");
    strcat(buffer, authToken);
    strcat(buffer, "\n");
    //End of making request
    sendAndRecv();
    readMessage();
}

void channelMembers(){
    //Making request
    memset(buffer, 0, sizeof(buffer));//Clearing buffer
    strcat(buffer, "channel members ");
    strcat(buffer, authToken);
    strcat(buffer, "\n");
    //End of making request
    sendAndRecv();
    readMembers();
}

void leaveChannel(){
    //Making request
    memset(buffer, 0, sizeof(buffer));//Clearing buffer
    strcat(buffer, "leave ");
    strcat(buffer, authToken);
    strcat(buffer, "\n");
    //End of making requset
    sendAndRecv();
    readJson();
    if (!strcmp(respondType, "Error"))
        printf("Error : %s", respondContent);
    else if (!strcmp(respondType, "Successful")){
        printf("You successfully leaved channel\n");
        inChannel = false;//Change inChannel state
    }
    getchar();
}

void readMessage(){
    JSONc *respond = JSONc_parse(buffer);
    if (!strcmp(JSONc_getObjectItem(respond, "type")->stringVal, "Error")){
        printf("\nError : %s\n", JSONc_getObjectItem(respond, "content")->stringVal);
        return;
    }
    JSONc *messages = JSONc_getObjectItem(respond, "content");
    JSONc *message;
    int sizeMessages = JSONc_getArraySize(messages);
    printf("\n\t%s\n", channelName);
    for (int i = 0; i < sizeMessages; i++){
        message = JSONc_getArrayItem(messages, i);
        printf("**%s : %s\n",
        JSONc_getObjectItem(message, "sender")->stringVal,
        JSONc_getObjectItem(message, "content")->stringVal);//So long. nah?:green:
    }
    JSONc_delete(respond);
    getchar();
}

void readMembers(){
    JSONc *respond = JSONc_parse(buffer);
    if (!strcmp(JSONc_getObjectItem(respond, "type")->stringVal, "Error")){
        printf("\nError : %s\n", JSONc_getObjectItem(respond, "content")->stringVal);
        return;
    }
    JSONc *membersList = JSONc_getObjectItem(respond, "content");
    int size = JSONc_getArraySize(membersList);
    printf("\n%s members :\n", channelName);
    for (int i = 0; i < size; i++)
        printf("\t%d - %s\n", i+1, JSONc_getArrayItem(membersList, i)->stringVal);
    JSONc_delete(respond);
    getchar();
}

void searchMembers()
{
    char keyUsename[MAX];
    printf("\nEnter usename : ");
    scanf("%s", keyUsename);
    //Making request
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "search members ");
    strcat(buffer, keyUsename);
    strcat(buffer, ", ");
    strcat(buffer, authToken);
    strcat(buffer, "\n");
    //End of making request
    sendAndRecv();
    readJson();
    if (!strcmp(respondType, "Error"))
        printf("Error : %s", respondContent);
    else if (!strcmp(respondType, "Successful"))
    {
        if (!strcmp(respondContent, "True"))
            printf("\nThis user exist in channel\n");
        else 
            printf("\nThis user is not in this channel\n");
    }
    getchar();
}

void pointerCheck(int optionNum)
{
    if (command == optionNum)
        printf("\033[1;31m");
    else
        printf("\033[0m");
}
