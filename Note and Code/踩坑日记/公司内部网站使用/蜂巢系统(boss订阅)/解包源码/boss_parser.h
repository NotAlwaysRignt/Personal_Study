#ifndef _BOSS_PARSER_H_
#define _BOSS_PARSER_H_
#include"boss_bus.h"
#include "tool.h"
#include <string>
#include <vector>
#include <sstream>
#include <stdint.h>
using std::string;
using std::stringstream;
using std::string;
using std::vector;


namespace BossProtocalParser
{
    const int BOSS_PACKET_MIN_LEN = 23;
    const int BOSS_HEADER_VALUE = 0x55;
    const int BOSS_TAIL_VALUE = 0xAA;

    //参照<<网站部CS协议>>
    typedef struct BossPacket
    {
        uint8_t web_header;
        uint32_t packet_len;
        uint16_t bussiness_type;
        uint8_t version;
        uint32_t seq;
        uint8_t ext_len;
        string ext_content;
        string body;
        int32_t body_len;
        uint32_t boss_id;
        uint32_t time;
        uint8_t web_tail;
        
    }BossPacket;


    class Parser
    {
        public:
            virtual int ParseBossPacket(const unsigned int shmkey,BossPacket& boss_packet,string& err_message)=0; 
            virtual int GetBossPacketBody(const unsigned int shmkey,vector<string>& packet_body,string& err_message)=0;
    };


    class BossParser: public Parser
    {
        public:
            /*
             * ParserBossPacket 是对 Boss packet 的解包
             * 第一个参数:共享内存key, 第二个参数是解包后的结果
             * 返回值: 0 正常 -1  共享内存 key值非法  -2 共享内存错误,如果出错, err_message被赋值,可通过其获得错误信息
             */ 
            virtual int ParseBossPacket(const unsigned int shmkey,BossPacket& boss_packet,string& err_message);

            /*
             * GetBossPacketBody 解包后可以取到解包后body 的每一个字段
             * 第一个元素得到boss id,第二个元素得到时间(字段ftime),从第三个元素开始根据具体需求而定
             * 返回值: 0 正常 -1  共享内存 key值非法  -2 共享内存错误,如果出错, err_message被赋值,可通过其获得错误信息
             * */
            virtual int GetBossPacketBody(const unsigned int shmkey, vector<string>& packet_body,string& err_message);
        private:
            int split(const string& str, vector<string>& ret_, const string& sep);
    };
};
#endif
