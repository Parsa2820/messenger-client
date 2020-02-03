# Messenger Client Version 1.0

## Table of contents
* [Prerequisites](#Prerequisites)
* [Functions](#Functions)
    * [connectSocket](#connectSocket)
    * [sendAndRecv](#sendAndRecv)
    * [readJson](#readJson)
    * [mainMenu](#mainMenu)
    * [signup](#signup)
    * [login](#login)
    * [about](#about)
    * [dashboard](#dashboard)
    * [creatChannel](#creatChannel)
    * [joinChannel](#joinChannel)
    * [logout](#logout)
    * [chatMenu](#chatMenu)
    * [sendMessage](#sendMessage)
    * [refreshChannel](#refreshChannel)
    * [channelMembers](#channelMembers)
    * [leaveChannel](#leaveChannel)
    * [readMessage](#readMessage)
    * [readMembers](#readMembers)
    * [searchMembers](#searchMembers)
* [Defualt port and IP address](#Defualt-port-and-IP-address)


## Prerequisites
First of all you need standard C libraries listed below
1. stdio and stdlib
2. string
3. arpa/inet
4. stdbool
5. sys/socket

you also need [JSONc](https://github.com/parsa2820/JSONc) (See the link for docs)

compiled with gcc 9.2.1 on linux (Windows needs different libraries)

## Functions

### connectSocket
```c
void connectSocket();
```
This function designed to connect to server throght a socket. It connect to defualt IP and port (as It explained at the end of this document).

### sendAndRecv
```c
void sendAndRecv();
```
This function send buffer to server and fill buffer again with server respond.

### readJson
```c
void readJson();
```
This function parse server respond json from buffer and put type and content into variables named `respondType` and `respondContent`. Mind that these are global variables which can be used in another functions.

### mainMenu
```c
int mainMenu();
```
Main menu includes functionalities listed below
1. Signup
2. Login
3. Quit

Each of them call suitable function and Quit return from function to main function.

### signup
```c
void signup();
```
Function designed to signup new user. It's ask for username and password.

### login
```c
void login();
```
Function designed to login user. It store authentication token received from server to send request to server in future. After successful login, it will call dashboard.

### about
```c
void about();
```
Just print version and details.

### dashboard
```c
void dashboard();
```
This is menu after login and it includes this functionalities
1. Create a channel
2. Join a channel
3. Logout

### creatChannel
```c
void creatChannel();
```
This function take a channel name from user and send request to server for creating a new channel.

### joinChannel
```c
void joinChannel();
```
This function used for joining to already existed channel. It just takes channel name from user.

### logout
```c
void logout();
```
This function used to logout from account.

### chatMenu
```c
void chatMenu();
```
This menu contains this entries
1. Send message
2. Refresh messages
3. Channel members
4. Leave channel
5. Search members

### sendMessage
```c
void sendMessage();
```
This function designed for sending message to the current channel. It just takes message content from user as a text.

### refreshChannel
```c
void refreshChannel();
```
This function receive unseen messages from server and call `readMessage` to print them to console.

### channelMembers
```c
void channelMembers();
```
This function receive channel members array from server and call `readMembers` to print them to console.

### leaveChannel
```c
void leaveChannel();
```
This function remove user from current channel and goes back to dashboard.

### readMessage
```c
void readMessage();
```
This function print messages from json array of objects to console.

### readMembers
```c
void readMembers();
```
This function print member from json array of strings to console.

### searchMembers
```c
void searchMembers();
```
This function get keysername from user and send request for matching channel members username of user current channel. Then print the result.

## Defualt port and IP address
For this client defualts are as listed below
- IP : 127.0.0.1
- Port : 12345

It's obvious that you can change them.`(main.c Ln 11, 56)`
