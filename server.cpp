#include<stdio.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<sys/un.h> 
#include<string.h> 
#include<netdb.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<string.h>
#include<iostream> 
#include<pthread.h>

using namespace std;
char buffer1[256], buffer2[256];
//int a;
void* sendrecv(void* p_newsockfd)
{
    
    int a = * (int*)p_newsockfd;
    free(p_newsockfd);
    while(true)
        {
            recv(a,buffer2,256,0);
            cout<<"\nClient : "<<buffer2;
            cout<<"\nYou : ";
            fgets(buffer1,256,stdin);
            send(a,buffer1,256,0);
        }
}

int main(int argc, char* argv[])
{
    
    char* ip;
    struct sockaddr_in server_add,peer_add;  
    int server_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd<0)
    {
        cout<<"\nSocket Failed";
        exit(1);
    }
    else
    {
        cout<<"\nServer Created";
    }
    server_add.sin_family = AF_INET; 
    server_add.sin_addr.s_addr = INADDR_ANY;
    server_add.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_add.sin_port = htons(12000);
    int b=bind(server_fd,(struct sockaddr*)&server_add,sizeof(server_add));
    if(b==0)
    {
        cout<<"\nCorrectly binded to the port";
    }
    else
    {
        cout<<"\nBinding Failed";
        exit(1);
    }

    int l=listen(server_fd,10);
    if(l<0)
    {
        cout<<"\nListening Failed";
    }
    else
    {
        cout<<"\nListening......";
    }
    
    socklen_t server_add_size; 
    server_add_size = sizeof(struct sockaddr_in);
    while(true)
    {
        int a=accept(server_fd,(struct sockaddr*)&peer_add,&server_add_size);
        char ip[INET_ADDRSTRLEN]; 
        if(a<0)
        {
            cout<<"\nConnection Failed";
        }
        else cout<<"\nConnection Established Successfully";
        cout<<"\n";
        inet_ntop(AF_INET,&(peer_add.sin_addr),ip,INET_ADDRSTRLEN);
        cout<<"\nIP = "<<ip<<"  Port no = "<<ntohs(peer_add.sin_port);
        pthread_t tid;
        int *pclient = (int*)malloc(sizeof(int));
        *pclient = a;
        if(pthread_create(&tid,NULL,&sendrecv,pclient)!=0)
        {
            cout<<"failed";
        }
        //pthread_join(tid,NULL);
    }
}