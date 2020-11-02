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

char trackerip1[15];
char trackerip2[15];
int trackerport1;
int trackerport2;
char trackerip[15];
int trackerport;
char buffer1[256], buffer2[256];
int islogin=0;
unordered_map<string,string> login_id_pass;
unordered_map<string,string> grp_admin;
unordered_map<string,vector<string>> grp_member_list;
unordered_map<string,vector<string>> join_req;
//pair<string,int> userinfo;
set<pair<string,int>> userinfoset;
unordered_map<string,set<pair<string,int>>> fileinfo;
//set<unordered_map<string,set<pair<string,int>>>> fileinfoset;
unordered_map<string,unordered_map<string,set<pair<string,int>>>> grp_file_info;

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
        }
        else
        {
            temp+=cmd[i];
        }
    }
    res.push_back(temp);
    return res;
}


void islg(string cmd,int a)
{
    cout<<"tracker ka islg";
    if(cmd[0]=='l' && cmd[1]=='i' && cmd[2]=='s' && cmd[3]=='t' && cmd[4]=='_' && cmd[5]=='g')
    {
        string res="";
        res+="The list of all groups is : ";
        unordered_map<string,string>::iterator it;
        it=grp_admin.begin();
        //for(it=grp_admin.begin();it!=grp_admin.end();it++)
        for(auto i:grp_admin)
        {
            res+="\n";
            res+=i.first;
            cout<<i.first.length()<<" ";
        }
        cout<<res<<endl;
        bzero(buffer1,256);
        strcpy(buffer1,res.c_str());
        send(a,buffer1,256,0);
    }
}


void* sendrecv(void* arg)
{
    int a=*(int*)arg;
    free(arg);
    while(true)
        {
            /*recv(a,buffer2,256,0);
            cout<<"\nClient : "<<buffer2;
            cout<<"\nYou : ";
            fgets(buffer1,256,stdin);
            send(a,buffer1,256,0);*/
            bzero(buffer2,256);
            recv(a,buffer2,256,0);
            string req(buffer2);
            vector<string> tokens=parser(req,' ');
            string cmd=tokens[0].substr(0,tokens[0].length());
            cout<<"\n"<<cmd<<"***"<<cmd.length();
            islg(tokens[0],a);
            if(tokens[0]=="create_user")
            {
                string usernm=tokens[1];
                string pass=tokens[2];
                if(login_id_pass.find(usernm)==login_id_pass.end())
                {
                    login_id_pass[usernm]=pass;
                    //char msg[256];
                    string msg="User Created Successfully";
                    //cout<<"\n"<<buffer2<<"    "<<login_id_pass[usernm]<<"  "<<msg;
                    //cout<<"\nA new user has been created ";
                    
                    bzero(buffer1,256);
                    //cout<<msg;
                    strcpy(buffer1,msg.c_str());
                    //cout<<msg;
                    send(a,buffer1,256,0);
                }
                else
                {
                    strcpy(buffer1,"User already exists");
                    send(a,buffer1,256,0);
                } 
            }


            if(tokens[0]=="login")
            {
                string usernm=tokens[1];
                string pass=tokens[2];
                if(login_id_pass.find(usernm)==login_id_pass.end())
                {
                    bzero(buffer1,256);
                    strcpy(buffer1,"User doesn't exists");
                    send(a,buffer1,256,0);
                }
                else
                {
                    if(pass==login_id_pass[usernm])
                    {
                        string msg="User Loged In Successfully";
                        bzero(buffer1,256);
                        strcpy(buffer1,msg.c_str());
                        //cout<<msg;
                        send(a,buffer1,256,0);
                    }
                    else
                    {
                        bzero(buffer1,256);
                        strcpy(buffer1,"Password Extered is incorect");
                        send(a,buffer1,256,0);
                    }
                }
            }

            if(tokens[0]=="create_group")
            {
                string usernm=tokens[2];
                //string pass=tokens[3];
                string gname=tokens[1];
                if(grp_admin.find(gname)!=grp_admin.end())
                {
                    bzero(buffer1,256);
                    strcpy(buffer1,"Group Already Exists");
                    send(a,buffer1,256,0);
                }
                else
                {
                    //vector<string> v;
                    //v.push_back(usernm);
                    //grp_admin.insert({gname,v});
                    grp_admin[gname]=usernm;
                    vector<string> v;
                    v.push_back(usernm);
                    grp_member_list.insert({gname,v});
                    bzero(buffer1,256);
                    strcpy(buffer1,"Group is created");
                    send(a,buffer1,256,0);
                }
            }

            if(tokens[0]=="join_group")
            {
                string usernm=tokens[2];
                string gname=tokens[1];
                int f1=0,f2=0;
                if(grp_admin.find(gname)==grp_admin.end())
                {
                    bzero(buffer1,256);
                    strcpy(buffer1,"Group Doesn't Exists");
                    send(a,buffer1,256,0);
                }
                else
                {
                    if(grp_admin[gname]==usernm)
                    {
                        bzero(buffer1,256);
                        strcpy(buffer1,"You are the admin of the group....Already in the Group");
                        send(a,buffer1,256,0);
                    }
                    else
                    {
                        vector<string> member=grp_member_list[gname];
                        int s=member.size();
                        for(int i=0;i<s;i++)
                        {
                            if(member[i]==usernm)
                            {
                                f1=1;
                                bzero(buffer1,256);
                                strcpy(buffer1,"Already in the Group");
                                send(a,buffer1,256,0);
                            }
                        }
                        if(f1==0)
                        {
                            if(join_req.empty()==true)
                            {
                                vector<string> v;
                                v.push_back(usernm);
                                join_req.insert({gname,v});
                                //join_req[gname].push_back(usernm);
                                bzero(buffer1,256);
                                strcpy(buffer1,"Join Request sent to the group Admin");
                                send(a,buffer1,256,0);
                            }
                            else
                            {
                                if(join_req.find(gname)==join_req.end())
                                {
                                    vector<string> v;
                                    v.push_back(usernm);
                                    join_req.insert({gname,v});
                                    bzero(buffer1,256);
                                    strcpy(buffer1,"Join Request sent to the group Admin");
                                    send(a,buffer1,256,0);
                                }
                                
                                /*if(join_req[gname].empty()==true)
                                {
                                    vector<string> v;
                                    v.push_back(usernm);
                                    join_req.insert({gname,v});
                                    //join_req[gname].push_back(usernm);
                                    bzero(buffer1,256);
                                    strcpy(buffer1,"Join Request sent to the group Admin");
                                    send(a,buffer1,256,0);
                                }*/
                                else
                                {
                                    vector<string> temp=join_req[gname];
                                    int s=temp.size();
                                    for(int i=0;i<s;i++)
                                    {
                                        if(temp[i]==usernm)
                                        {
                                            f2=1;
                                            bzero(buffer1,256);
                                            strcpy(buffer1,"Join Request is already sent .....wait for admin to add you ");
                                            send(a,buffer1,256,0);
                                        }
                                    }
                                    if(f2==0)
                                    {
                                        join_req[gname].push_back(usernm);
                                        bzero(buffer1,256);
                                        strcpy(buffer1,"Join Request sent to the group Admin");
                                        send(a,buffer1,256,0);
                                    }
                                }
                            }
                        } 
                    }
                }
            }

            if(tokens[0]=="leave_group")
            {
                cout<<"inside leave grp"<<endl;
                string usernm=tokens[2];
                string gname=tokens[1];
                int f1=0;
                if(grp_admin.find(gname)==grp_admin.end())
                {
                    bzero(buffer1,256);
                    strcpy(buffer1,"Group Doesn't Exists");
                    send(a,buffer1,256,0);
                }
                else
                {
                    if(grp_admin[gname]==usernm)
                    {
                        bzero(buffer1,256);
                        strcpy(buffer1,"Admin Cannot Leave the Group");
                        send(a,buffer1,256,0);
                    }
                    else
                    {
                        //cout<<"else ke andar"<<endl;
                        //vector<string> temp=grp_member_list[gname];
                        //cout<<"vector bna diya "<<endl;
                        //int s=temp.size();
                        vector<string>::iterator it;
                        //it=temp.begin();
                        for(it=grp_member_list[gname].begin();it!=grp_member_list[gname].end();it++)
                        {
                            //cout<<"for ke andar"<<endl;
                            if(*it==usernm)
                            {
                                //cout<<"temp se erase krne wla hu"<<endl;
                                //cout<<"***********"<<endl;
                                //temp.erase(it+i);
                                //cout<<"map se erase krne wla hu"<<endl;
                                cout<<"***********"<<endl;
                                grp_member_list[gname].erase(it);
                                //cout<<"erase krne ke baad"<<endl;
                                cout<<"***********"<<endl;
                                f1=1;
                                bzero(buffer1,256);
                                strcpy(buffer1,"You left the group");
                                send(a,buffer1,256,0);
                                break;
                            }
                        }
                        if(f1==0)
                        {
                            bzero(buffer1,256);
                            strcpy(buffer1,"You are not a member of the group");
                            send(a,buffer1,256,0);
                        }
                    }
                }
            }


            if(tokens[0]=="requests" && tokens[1]=="list_requests")
            {
                string usernm=tokens[3];
                string gname=tokens[2];
                if(grp_admin.find(gname)==grp_admin.end())
                {
                    bzero(buffer1,256);
                    strcpy(buffer1,"Group Doesn't Exists");
                    send(a,buffer1,256,0);
                }
                else
                {
                    if(grp_admin[gname]==usernm)
                    {
                        string res="";
                        res+="The list is : ";
                        vector<string> temp=join_req[gname];
                        int s=temp.size();
                        for(int i=0;i<s;i++)
                        {
                            res+="\n";
                            res+=temp[i];
                        }
                        bzero(buffer1,256);
                        strcpy(buffer1,res.c_str());
                        send(a,buffer1,256,0);
                    }
                    else
                    {
                        bzero(buffer1,256);
                        strcpy(buffer1,"Only group admin can see the join request");
                        send(a,buffer1,256,0);
                    }
                }
            }


            if(tokens[0][0]=='a')
            {
                //cout<<"accept ke andar\n";
                string usernm2=tokens[3];//grp admin rahega
                string usernm=tokens[2];//jiski req accept krni hai
                cout<<"user is = "<<usernm<<endl;
                string gname=tokens[1];
                
                int f1=0;
                if(grp_admin.find(gname)==grp_admin.end())
                {
                    bzero(buffer1,256);
                    strcpy(buffer1,"Group Doesn't Exists");
                    send(a,buffer1,256,0);
                }
                else
                {
                    if(grp_admin[gname]==usernm2)
                    {
                        //cout<<"admin hi hai wo\n";
                        vector<string> req=join_req[gname];
                        //int s=req.size();
                        vector<string>::iterator it;
                        for(it=join_req[gname].begin();it!=join_req[gname].end();it++)
                        {
                            if(*it==usernm)
                            {
                                f1=1;
                                //it=it+i;
                                //cout<<"req[it] = "<<*it<<endl;
                                join_req[gname].erase(it);
                                //req.erase(it);
                                cout<<"\n**********"<<endl;
                                //cout<<"after erase"<<endl;
                                grp_member_list[gname].push_back(usernm);
                                //cout<<"member list me dalwa diya user"<<endl;
                                bzero(buffer1,256);
                                strcpy(buffer1,"The user has been added to the group");
                                send(a,buffer1,256,0);
                                //cout<<"if ke abdar"<<endl;
                                break;
                            }
                        }
                        if(f1==0)
                        {
                            bzero(buffer1,256);
                            strcpy(buffer1,"There is no join request from the user");
                            send(a,buffer1,256,0);
                        }
                        /*else
                        {
                            //cout<<"else ke andar"<<endl;
                            join_req[gname].erase(it);
                            cout<<"**********"<<endl;
                            //cout<<"after erase"<<endl;
                            grp_member_list[gname].push_back(usernm);
                            //cout<<"member list me dalwa diya user"<<endl;
                            bzero(buffer1,256);
                            strcpy(buffer1,"The user has been added to the group");
                            send(a,buffer1,256,0);
                        }*/
                        
                    }
                    else
                    {
                        bzero(buffer1,256);
                        strcpy(buffer1,"Only group admin can accpet the join request");
                        send(a,buffer1,256,0);
                    }
                }
                
            }

            /*if(tokens[0]=="list_groups")
            {
                string res="";
                res+="The list of all groups is : ";
                unordered_map<string,string>::iterator it;
                it=grp_admin.begin();
                //for(it=grp_admin.begin();it!=grp_admin.end();it++)
                for(auto i:grp_admin)
                {
                    res+="\n";
                    res+=i.first;
                    cout<<i.first.length()<<" ";
                }
                cout<<res<<endl;
                bzero(buffer1,256);
                strcpy(buffer1,res.c_str());
                send(a,buffer1,256,0);
            }*/


            if(tokens[0]=="upload_file")
            {
                string usernm=tokens[3];
                string gname=tokens[2];
                string filename=tokens[1];
                string sip=tokens[4];
                int sport=stoi(tokens[5]);
                vector<string>::iterator it;
                int f1=0;
                //pair<string,int> ipport(sip,sport);
                pair<string,int> userinfo(usernm,sport);
                //set<pair<string,int>> userinfoset;
                //unordered_map<string,set<pair<string,int>>> fileinfo;
                //unordered_map<string,unordered_map<string,set<pair<string,pair<string,int>>>>>

                if(grp_admin.find(gname)==grp_admin.end())
                {
                    bzero(buffer1,256);
                    strcpy(buffer1,"Group Doesn't Exists");
                    send(a,buffer1,256,0);
                }
                else
                {
                    for(it=grp_member_list[gname].begin();it!=grp_member_list[gname].end();it++)
                    {
                        
                        if(*it==usernm)
                        {
                            f1=1;
                            if(grp_file_info.find(gname)==grp_file_info.end())
                            {
                                userinfoset.insert(userinfo);
                                fileinfo.insert({filename,userinfoset});
                                //fileinfoset.insert(fileinfo);
                                grp_file_info.insert({gname,fileinfo});
                                bzero(buffer1,256);
                                strcpy(buffer1,"File Uploaded Succesfully");
                                send(a,buffer1,256,0);
                            }
                            else
                            {
                                if((grp_file_info[gname]).find(filename)==grp_file_info[gname].end())
                                {
                                    userinfoset.insert(userinfo);
                                    fileinfo.insert({filename,userinfoset});
                                    grp_file_info[gname].insert({filename,userinfoset});
                                    bzero(buffer1,256);
                                    strcpy(buffer1,"File Uploaded Succesfully");
                                    send(a,buffer1,256,0);
                                }
                                else
                                {
                                    userinfoset.insert(userinfo);
                                    grp_file_info[gname][filename].insert(userinfo);
                                    bzero(buffer1,256);
                                    strcpy(buffer1,"File Uploaded Succesfully");
                                    send(a,buffer1,256,0);
                                }      
                            }
                            //unordered_map<string,set<pair<string,int>>> temp=grp_file_info[gname];
                            /*if(grp_file_info[gname].find(filename)==grp_file_info[gname].end())
                            {
                                userinfoset.insert(userinfo);
                                fileinfo.insert({filename,userinfoset});
                                grp_file_info.insert({gname,fileinfo});
                                bzero(buffer1,256);
                                strcpy(buffer1,"File Uploaded Succesfully");
                                send(a,buffer1,256,0);
                            }
                            else
                            {
                                if(userinfoset.find(userinfo)==userinfoset.end())
                                {
                                    userinfoset.insert(userinfo);
                                    fileinfo.insert({filename,userinfoset});
                                    grp_file_info.insert({gname,fileinfo});
                                    bzero(buffer1,256);
                                    strcpy(buffer1,"File Uploaded Succesfully");
                                    send(a,buffer1,256,0);
                                }
                                else
                                {
                                    bzero(buffer1,256);
                                    strcpy(buffer1,"The File is already uploaded");
                                    send(a,buffer1,256,0);
                                }
                                
                            }*/
                            
                            /*userinfoset.insert(userinfo);
                            fileinfo.insert({filename,userinfoset});
                            grp_file_info.insert({gname,fileinfo});
                            bzero(buffer1,256);
                            strcpy(buffer1,"File Uploaded Succesfully");
                            send(a,buffer1,256,0);*/
                        }
                    }
                    if(f1==0)
                    {
                        bzero(buffer1,256);
                        strcpy(buffer1,"You are not a member of the group");
                        send(a,buffer1,256,0);
                    }
                }
            }

            if(tokens[0]=="list_files")
            {
                string gname=tokens[1];
                string usernm=tokens[2];
                int f1=0;
                vector<string>::iterator it;
                string res="";
                if(grp_admin.find(gname)==grp_admin.end())
                {
                    bzero(buffer1,256);
                    strcpy(buffer1,"Group Doesn't Exists");
                    send(a,buffer1,256,0);
                }
                else
                {
                    for(it=grp_member_list[gname].begin();it!=grp_member_list[gname].end();it++)
                    {
                        if(*it==usernm)
                        {
                            f1=1;
                            /*if(grp_file_info[gname].empty()==true)
                            {
                                bzero(buffer1,256);
                                strcpy(buffer1,"There is no files uploaded to the group");
                                send(a,buffer1,256,0);
                            }
                            else
                            {*/
                                unordered_map<string,set<pair<string,int>>> temp=grp_file_info[gname];
                                res+="The list of Files in the group is :- ";
                                for(auto i:temp)
                                {
                                    res+="\n";
                                    res+=i.first;
                                }
                                bzero(buffer1,256);
                                strcpy(buffer1,res.c_str());
                                send(a,buffer1,256,0);
                            //}
                            
                        }
                    }
                    if(f1==0)
                    {
                        bzero(buffer1,256);
                        strcpy(buffer1,"You are not a member of the group");
                        send(a,buffer1,256,0);
                    }
                }
            }
        }
}


//void* start_server(void* arg)
void start_server()
{
    char* ip;
    struct sockaddr_in server_add,peer_add;  
    int server_fd=socket(AF_INET, SOCK_STREAM, 0);
    cout<<"Hello"<<endl;
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
    //string iptr(trackerip);
    //inet_pton(AF_INET,trackerip , &server_add.sin_addr); 
    server_add.sin_addr.s_addr = inet_addr(trackerip);
    server_add.sin_port = htons(trackerport);
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
    server_add_size = sizeof(struct sockaddr);
    while(true)
    {
        int a=accept(server_fd,(struct sockaddr*)&peer_add,&server_add_size);
        char ip[INET_ADDRSTRLEN]; 
        if(a<0)
        {
            cout<<"\nConnection Failed";
        }
        else cout<<"\nConnection Established Successfully";
        //cout<<"\n";
        inet_ntop(AF_INET,&(peer_add.sin_addr),ip,INET_ADDRSTRLEN);
        cout<<"\n**********************************8";
        cout<<"\nIP = "<<ip<<"  Port no = "<<ntohs(peer_add.sin_port);
        pthread_t tid;
        int *fd=(int*)malloc(sizeof(int));
        *fd=a;
        if(pthread_create(&tid,NULL,&sendrecv,fd)!=0)
        {
            cout<<"failed";
        }
        //pthread_join(tid,NULL);
    }

}



int main(int argc, char* argv[])
{
    FILE* f=fopen(argv[1],"r");
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

    if(stoi(argv[2])==1)
    {
        strcpy(trackerip,trackerip1);
        trackerport=trackerport1;
    }
    else if(stoi(argv[2])==2)
    {
        strcpy(trackerip,trackerip2);
        trackerport=trackerport2;
    }
    cout<<"****************"<<endl;
    cout<<"IP = "<<trackerip<<"  "<<"port = "<<trackerport<<endl;
    pthread_t cthrd,sthrd;
    start_server();
    /*if(pthread_create(&sthrd,NULL,&start_server,NULL)!=0)
    {
        cout<<"Error : Server Thread cannot be Created \n";
        exit(1);
    }*/
   /* if(pthread_create(&cthrd,NULL,&start_client,NULL)!=0)
    {
        cout<<"Error : Client Thread cannot be Created \n";
        exit(1);
    }*/
    cout<<"In main"<<endl;
    //pthread_create(&sthrd,NULL,&start_server,NULL);
    //pthread_create(&cthrd,NULL,&start_client,NULL);
    //pthread_join(sthrd,NULL);
}
