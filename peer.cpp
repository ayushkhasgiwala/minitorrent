#include<stdio.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<sys/un.h> 
#include<string.h> 
#include<netdb.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<string.h>
#include<bits/stdc++.h> 
#include<pthread.h>

using namespace std;

char serverip[15];
int serverport;
char trackerip1[15];
char trackerip2[15];
int trackerport1;
int trackerport2;
char trackerip[15];
int trackerport;
int a;
int client_fd;
int islogin=0;
string usrname,pass;
int flag=0;
char buffer1[256], buffer2[256];

vector<string> parser(string cmd, char delimeter)
{
    int l=cmd.length();
    vector<string> res;
    string temp="";
    for(int i=0;i<l;i++)
    {
        if(cmd[i]==delimeter)
        {
            res.push_back(temp);
            temp="";
            //flag=1;
        }
        else
        {
            temp+=cmd[i];
        }
    }
    /*if(flag==0)
    {
        res.push_back(cmd);
        return res;

    }
    else{*/
        res.push_back(temp);
        
    //}
    return res;
}



void* serverreq(void* arg)
{
    //yaha change krna hai cheeze
    cout<<"idhar";
    while(true)
        {
            recv(a,buffer2,256,0);
            cout<<"\nClient : "<<buffer2;
            cout<<"\nYou : ";
            fgets(buffer1,256,stdin);
            send(a,buffer1,256,0);
        }
}



void* start_server(void* arg)
//void start_server()
{
    char* ip;
    struct sockaddr_in serverpeer_add,otherpeer_add;  
    int serverpeer_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(serverpeer_fd<0)
    {
        cout<<"\nSocket Failed";
        exit(1);
    }
    else
    {
        cout<<"\nServer Created";
    }
    serverpeer_add.sin_family = AF_INET; 
    serverpeer_add.sin_addr.s_addr = INADDR_ANY;
    //string iptr(trackerip);
    serverpeer_add.sin_addr.s_addr = inet_addr(serverip);
    serverpeer_add.sin_port = htons(serverport);
    int b=bind(serverpeer_fd,(struct sockaddr*)&serverpeer_add,sizeof(serverpeer_add));
    if(b==0)
    {
        cout<<"\nCorrectly binded to the port server me";
    }
    else
    {
        cout<<"\nBinding Failed server me";
        exit(1);
    }

    int l=listen(serverpeer_fd,10);
    if(l<0)
    {
        cout<<"\nListening Failed server me";
    }
    else
    {
        cout<<"\nListening......server me";
    }
    
    socklen_t serverpeer_add_size; 
    serverpeer_add_size = sizeof(struct sockaddr_in);
    while(true)
    {
        a=accept(serverpeer_fd,(struct sockaddr*)&otherpeer_add,&serverpeer_add_size);
        char ip[INET_ADDRSTRLEN]; 
        if(a<0)
        {
            cout<<"\nConnection Failed server me";
        }
        else cout<<"\nConnection Established Successfully server me";
        cout<<"\n";
        inet_ntop(AF_INET,&(otherpeer_add.sin_addr),ip,INET_ADDRSTRLEN);
        cout<<"\nIP = "<<ip<<"  Port no = "<<ntohs(otherpeer_add.sin_port);
        pthread_t tid;
        if(pthread_create(&tid,NULL,&serverreq,NULL)!=0)
        {
            cout<<"failed";
        }
    }

}



//void* client_req(void* arg)
void client_req()
{
    //char reqbuff[4096];
    //string req;
    cout<<"inside client req";
    while(true)
    {
       cout<<"\nYou : ";
       fgets(buffer2,256,stdin);
       //req=string(buffer2);
       send(client_fd,buffer2,256,0);
       cout<<buffer2<<endl;
       //cout<<"\nServer : ";
       recv(client_fd,buffer1,256,0);
       cout<<"\nServer : "<<buffer1;
    }
}

void islg(string cmd)
{
    //cout<<"in islg";
    if(cmd[0]=='l' && cmd[1]=='i' && cmd[2]=='s' && cmd[3]=='t' && cmd[4]=='_' && cmd[5]=='g')
    {
        cout<<"fawn";
        if(islogin==0)
        {
            cout<<"\nServer : Please login before requesting to view the list of all the group";
        }
        else
        {
            //bzero(buffer2,256);
            //strcpy(buffer2,req.c_str());
            cout<<"hello  ";
            
            bzero(buffer2,256);
            strcpy(buffer2,cmd.c_str());
            cout<<buffer2;
            send(client_fd,buffer2,256,0);
            cout<<"bye  ";
            bzero(buffer1,256);
            cout<<"chal ja bc  ";
            recv(client_fd,buffer1,256,0);
            cout<<"\nServer : "<<buffer1<<"  lodu";
        }
        cout<<"\nelse ke bhr";
    } 

}

//void* start_client(void* arg)
void start_client()
{
    //char buffer1[256],buffer2[256];
    struct sockaddr_in server_add, peer_add;
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
    server_add.sin_addr.s_addr=INADDR_ANY;
    server_add.sin_family=AF_INET;   
    server_add.sin_addr.s_addr=inet_addr(trackerip);
    server_add.sin_port=htons(trackerport);
    peer_add.sin_addr.s_addr = INADDR_ANY; 
    peer_add.sin_family=AF_INET; 
    peer_add.sin_port = htons(serverport);
    peer_add.sin_addr.s_addr = inet_addr(serverip); 
    /*int b=bind(client_fd,(struct sockaddr*)&peer_add,sizeof(struct sockaddr_in));
    if(b<0)
    {
        cout<<"\nBinding Failed client";
        exit(1);
    }
    else
    {
        cout<<"\nSuccessfully Binded client ";
    }*/
    int connection=connect(client_fd,(struct sockaddr*)&server_add,sizeof(server_add));
    if(connection<0)
    {
        cout<<"\nConnection Failed client me";
        exit(1);
    }
    else
    {
        cout<<"\nClient Connection Established Successfully";
    }
    cout<<"\n";
    //client_req();
    while(true)
    {
        cout<<"\nCommand : ";
        bzero(buffer2,256);
        fgets(buffer2,256,stdin);
        
        string req(buffer2);
        req=req.substr(0,req.length()-1);
        bzero(buffer2,256);
        strcpy(buffer2,req.c_str());
        //cout<<"Hello  ";
        //cout<<"\n##"<<req<<"**"<<req.length();
        /*vector<string> tokens=parser(req,' ');
        string cmd=tokens[0].substr(0,tokens[0].length()-1);
        cout<<"\n$$$"<<cmd<<"***"<<cmd.length();*/
        
        if(req[0]=='l' && req[1]=='i' && req[2]=='s' && req[3]=='t' && req[4]=='_' && req[5]=='g')
        {
            //cout<<"fawn";
            if(islogin==0)
            {
                cout<<"\nServer : Please login before requesting to view the list of all the group";
            }
            else
            {
                //bzero(buffer2,256);
                //strcpy(buffer2,req.c_str());
                //cout<<"hello  ";
                
                //bzero(buffer2,256);
                //strcpy(buffer2,cmd.c_str());
                //cout<<buffer2;
                send(client_fd,buffer2,256,0);
                //cout<<"bye  ";
                bzero(buffer1,256);
                //cout<<"chal ja bc  ";
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
            //cout<<"\nelse ke bhr";
            continue;
        }
        //cout<<"Bye\n";
        if(req[0]=='a' )//&& req[1]=='c' && req[2]=='c' && req[3]=='e' && req[4]=='p' && req[5]=='t')
        {
            //cout<<"in accept ";
            if(islogin==0)
            {
                cout<<"\nServer : Please login before accepting the request for the group";
            }
            else
            {
                req+=" ";
                req+=usrname;
                //cout<<req<<" ";
                bzero(buffer2,256);
                strcpy(buffer2,req.c_str());
                //cout<<buffer2<<" hello";
                send(client_fd,buffer2,256,0);
                bzero(buffer1,256);
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
            continue;
        }

        vector<string> tokens=parser(req,' ');
        string cmd=tokens[0].substr(0,tokens[0].length()-1);
        //islg(cmd);
        //cout<<"\n$$$"<<cmd<<"***"<<cmd.length();
        

        if(tokens[0]=="create_user")
        {
            send(client_fd,buffer2,256,0);
            bzero(buffer1,256);
            recv(client_fd,buffer1,256,0);
            cout<<"\nServer : "<<buffer1;
        }

        if(tokens[0]=="login")
        {
            if(islogin==1)
            {
                cout<<"\nServer : Some User is already logged in ";
            }
            else
            {
                islogin=1;
                usrname=tokens[1];
                pass=tokens[2];
                send(client_fd,buffer2,256,0);
                bzero(buffer1,256);
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
        }

        if(tokens[0]=="create_group")
        {
            if(islogin==0)
            {
                cout<<"\nServer : You need to log in to create a group ";
            }
            else
            {
                req+=" ";
                req+=usrname;
                //req+=" ";
                //req+=pass;
                bzero(buffer2,256);
                strcpy(buffer2,req.c_str());
                send(client_fd,buffer2,256,0);
                bzero(buffer1,256);
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
        }

        if(tokens[0]=="join_group")
        {
            if(islogin==0)
            {
                cout<<"\nServer : You need to log in to join a group ";
            }
            else
            {
                req+=" ";
                req+=usrname;
                //req+=" ";
                //req+=pass;
                bzero(buffer2,256);
                strcpy(buffer2,req.c_str());
                send(client_fd,buffer2,256,0);
                bzero(buffer1,256);
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
        }

        if(tokens[0]=="leave_group")
        {
            if(islogin==0)
            {
                cout<<"\nServer : Please login before leaving the group ";
            }
            else
            {
                req+=" ";
                req+=usrname;
                bzero(buffer2,256);
                strcpy(buffer2,req.c_str());
                send(client_fd,buffer2,256,0);
                bzero(buffer1,256);
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
            
        }


        if(tokens[0]=="requests" && tokens[1]=="list_requests")
        {
            if(islogin==0)
            {
                cout<<"\nServer : Please login before viewing the request list of the group";
            }
            else
            {
                req+=" ";
                req+=usrname;
                bzero(buffer2,256);
                strcpy(buffer2,req.c_str());
                send(client_fd,buffer2,256,0);
                bzero(buffer1,256);
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
        }


        /*if(tokens[0]=="accept_request")
        {
            cout<<"in accept ";
            if(islogin==0)
            {
                cout<<"\nServer : Please login before accepting the request for the group";
            }
            else
            {
                req+=" ";
                req+=usrname;
                cout<<req<<" ";
                bzero(buffer2,256);
                strcpy(buffer2,req.c_str());
                cout<<buffer2<<" ";
                send(client_fd,buffer2,256,0);
                bzero(buffer1,256);
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
            
        }*/


        /*if(cmd=="list_groups")
        {
            cout<<"fawn";
            if(islogin==0)
            {
                cout<<"\nServer : Please login before requesting to view the list of all the group";
            }
            else
            {
                //bzero(buffer2,256);
                //strcpy(buffer2,req.c_str());
                cout<<"hello";
                send(client_fd,buffer2,256,0);
                cout<<"bye";
                bzero(buffer1,256);
                cout<<"chal ja bc";
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
            cout<<"else ke bhr";
        }*/


        if(tokens[0]=="upload_file")
        {
            if(islogin==0)
            {
                cout<<"\nServer : Please login before uploading the file the group";
            }
            else
            {
                req+=" ";
                req+=usrname;
                req+=" ";
                req+=serverip;
                req+=" ";
                req+=to_string(serverport);
                bzero(buffer2,256);
                strcpy(buffer2,req.c_str());
                //cout<<buffer2<<" ";
                send(client_fd,buffer2,256,0);
                bzero(buffer1,256);
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
        }

        if(tokens[0]=="list_files")
        {
            if(islogin==0)
            {
                cout<<"\nServer : Please login before getting the list of files in the group";
            }
            else
            {
                req+=" ";
                req+=usrname;
                bzero(buffer2,256);
                strcpy(buffer2,req.c_str());
                //cout<<buffer2<<" ";
                send(client_fd,buffer2,256,0);
                bzero(buffer1,256);
                recv(client_fd,buffer1,256,0);
                cout<<"\nServer : "<<buffer1;
            }
            
        }

        if(req=="logout")
        {
            if(islogin==0)
            {
                cout<<"\nServer : No user is logged in.....Cannot Logout ";
            }
            else
            {
                islogin=0;
                cout<<"\nServer : User has been successfully Logged out ";
            } 
        }

        //send(client_fd,buffer2,256,0);
        //cout<<"\nServer : "<<buffer1;
        //recv(client_fd,buffer1,256,0);
        //cout<<"\nServer : "<<buffer1;
        /*pthread_t tid;
        if(pthread_create(&tid,NULL,&client_req,NULL)!=0)
        {
            cout<<"failed";
        }
        client_req();*/
    }
}


int main(int argc, char* argv[])
{
    string ip_port(argv[1]);
    vector<string> serverinfo=parser(ip_port,':');
    //cout<<"IP = "<<serverinfo[0]<<"   Port = "<<serverinfo[1]<<endl;
    strcpy(serverip,serverinfo[0].c_str());
    serverport=stoi(serverinfo[1]);
    cout<<"Client IP = "<<serverip<<"   Client Port = "<<serverport<<endl;

    FILE* f=fopen(argv[2],"r");
    char info[256];
    fread(info,sizeof(char),256,f);
    char* token=strtok(info,":");
    strcpy(trackerip1,token);
    //cout<<trackerip1<<endl;
    token=strtok(NULL,"\n");
    trackerport1=stoi(token);
    //cout<<trackerport1<<"\n";

    token=strtok(NULL,":");
    strcpy(trackerip2,token);
    token=strtok(NULL,"\n");
    trackerport2=stoi(token);
    //cout<<trackerip2<<endl;
    //cout<<trackerport2<<"\n";

    if(stoi(argv[3])==1)
    {
        strcpy(trackerip,trackerip1);
        trackerport=trackerport1;
    }
    else if(stoi(argv[3])==2)
    {
        strcpy(trackerip,trackerip2);
        trackerport=trackerport2;
    }
    cout<<"****************"<<endl;
    cout<<"IP of Tracker = "<<trackerip<<"  "<<"Tracker port = "<<trackerport<<endl;

    pthread_t sthrd,cthrd;
    pthread_create(&sthrd,NULL,&start_server,NULL);
    int r=pthread_detach(sthrd);
    //pthread_create(&cthrd,NULL,&start_client,NULL);
    //start_server();
    start_client();
    
}