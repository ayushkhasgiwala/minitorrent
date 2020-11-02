#include<stdio.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<sys/un.h> 
#include<string.h> 
#include<netdb.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<stdlib.h>
#include<iostream>

using namespace std;

int main(int argc, char* argv[])
{
    char buffer1[256],buffer2[256];
    struct sockaddr_in server_add, client_add;
    socklen_t addr_size = sizeof server_add; 
    int client_fd=socket(AF_INET,SOCK_STREAM,0);
    if(client_fd<0)
    {
        cout<<"\nClient Failed";
        exit(1);
    }
    else
    {
        cout<<"\nClient Created";
    }
    //server_add.sin_addr.s_addr=INADDR_ANY;
    server_add.sin_family=AF_INET;   
    server_add.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_add.sin_port=htons(12000);
    //client_add.sin_addr.s_addr = INADDR_ANY; 
    client_add.sin_family=AF_INET; 
    client_add.sin_port = htons(atoi(argv[1]));
    client_add.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    int b=bind(client_fd,(struct sockaddr*)&client_add,sizeof(struct sockaddr_in));
    if(b<0)
    {
        cout<<"\nBinding Failed";
        exit(1);
    }
    else
    {
        cout<<"\nSuccessfully Binded";
    }
    int connection=connect(client_fd,(struct sockaddr*)&server_add,sizeof(server_add));
    if(connection<0)
    {
        cout<<"\nConnection Failed";
        exit(1);
    }
    else
    {
        cout<<"\nClient Connection Established Successfully";
    }
    cout<<"\n";
    while(true)
    {
        cout<<"\nYou : ";
        fgets(buffer2,256,stdin);
        send(client_fd,buffer2,256,0);
        recv(client_fd,buffer1,256,0);
        cout<<"\nServer : "<<buffer1;
    }
    
}