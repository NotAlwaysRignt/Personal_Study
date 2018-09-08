#include<iostream>
#include<vector>
#include"boss_parser.h"
#include "SingletonHolder.h"
#include "tool.cpp"
#include"boss_parser.cpp"

using namespace std;
using namespace BossProtocalParser;



int main(int argc, char *argv[])
{
    const unsigned int shmkey = 25731;
    const unsigned int shmkey_6364 = 25931;
    int count = 5;
    string err_message;
    vector<string> body;
    Parser* p  = SingletonHolder<BossParser>::Instance();
    int error;
    for(int i=0;i<count;i++){
        error =  p->GetBossPacketBody(shmkey,body,err_message);
        if(error == 0) {
            cout<<"--------------------------"<<endl;
            cout<<"字段数量: "<<body.size()<<endl;
            cout<<"boss id: "<<body[0]<<endl; //boss id
            cout<<"ftime: "<<body[1]<<endl; //ftime
            cout<<"skinid: "<<body[2]<<endl; //skinid
            cout<<"qq: "<<body[3]<<endl; //qq
            cout<<"cmd: "<<body[4]<<endl; //cmd
            cout<<"skinVersion: "<<body[5]<<endl; 
            cout<<"guid: "<<body[6]<<endl; 
            cout<<"skinVersion: "<<body[body.size()-1]<<endl;//videostorePluginVersion
            cout<<"\n\n"<<endl;
            }
        }
        

    for(int i = 0; i<count; i++){  //这里用一个没有任何数据推过来的 共享内存做示例,证明了 boss_buss_read 是非阻塞的,如果没有数据推过来 error 一样会返回0
        cout<<"----------------------------------------"<<endl;
        error =  p->GetBossPacketBody(shmkey_6364,body,err_message);
        if(error == 0) {
            cout<<"recv message"<<endl;
            cout<<body.size()<<endl;

        }
        else{
            cout<<"I don't recv message,err_message:"<<err_message<<"error number:"<<error<<endl;
        }
    }
    return 0;
}
