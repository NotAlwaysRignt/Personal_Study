#include "boss_parser.h"
#include<arpa/inet.h>

using namespace BossProtocalParser;


int BossParser::ParseBossPacket(const unsigned int shmkey,BossPacket& boss_packet,string& err_message){
    const char* packet_buf = NULL;
    unsigned int packet_len = 0;
    int error =0;
    error = boss_bus_read(shmkey,&packet_buf,&packet_len);

    if((0 == error)&& (NULL != packet_buf) && (packet_len > 0)){
        int prefix_count = 0;
        if(packet_buf[0] == BOSS_HEADER_VALUE && packet_len > BOSS_PACKET_MIN_LEN){
            // web header
            boss_packet.web_header = *packet_buf;
            packet_buf += sizeof(boss_packet.web_header);
            prefix_count += sizeof(boss_packet.web_header);

            //packet len
            boss_packet.packet_len = htonl(*((uint32_t *)packet_buf));
            packet_buf += sizeof(boss_packet.packet_len);
            prefix_count += sizeof(boss_packet.packet_len);
            if( packet_len < boss_packet.packet_len){
                err_message = "data decode error,the actual boss_packet len is "+ strtool::IntToString(packet_len) +",but the len in data is" + strtool::IntToString(boss_packet.packet_len);
                return 1;
                    
            }

            //bussiness type
            boss_packet.bussiness_type = htons(*((uint16_t *)packet_buf));
            packet_buf += sizeof(boss_packet.bussiness_type);
            prefix_count += sizeof(boss_packet.bussiness_type);

            //version
            boss_packet.version = *packet_buf;
            packet_buf += sizeof(boss_packet.version);
            prefix_count += sizeof(boss_packet.version);

            //sequence
            boss_packet.seq = htonl(*((uint32_t *)packet_buf));
            packet_buf += sizeof(boss_packet.seq);
            prefix_count += sizeof(boss_packet.seq);

            //extention length
            boss_packet.ext_len = *packet_buf;
            packet_buf += sizeof(boss_packet.ext_len);
            prefix_count += sizeof(boss_packet.ext_len);

            //extention content
            boss_packet.ext_content.append(packet_buf,boss_packet.ext_len);
            packet_buf += (uint32_t)boss_packet.ext_len;
            prefix_count += (int)boss_packet.ext_len;

            //Body
            int32_t body_len = (int32_t)boss_packet.packet_len-prefix_count;

            if(body_len <=0){
                err_message = "packet len error, all packet len:"+strtool::IntToString(boss_packet.packet_len)+" packet body len"+strtool::IntToString(body_len);
                return 1;
            }
            
            //4 bytes Boss_ID , 4 bytes Time ,then  Content
            if(body_len <=8){
                err_message = "body len:"+strtool::IntToString(body_len)+" <=8";
                return 1;
            }
            
            //boss id
            boss_packet.boss_id = htonl(*((uint32_t *)packet_buf));

            packet_buf += sizeof(boss_packet.boss_id);

            //time
            boss_packet.time = htonl(*((uint32_t *)packet_buf));
            packet_buf += sizeof(boss_packet.time);

            stringstream body_temp;
            body_temp<<boss_packet.boss_id<<","<<boss_packet.time<<","; // boss_id 和 time 的解析

            //将body部分组装好
            boss_packet.body = body_temp.str();
            boss_packet.body.append(packet_buf,body_len-8);

        }

    }
    
        //读共享内存失败
        else if(error == -1)
        {
            err_message = "the sharememory key is illegal:"+ strtool::IntToString(shmkey);
            
        }
        //共享内存内部错误
        else if( error == -2 ){
            err_message = "there is error in share memory,share memory key:"+strtool::IntToString(shmkey);
        }

        return error;
}


int BossParser::GetBossPacketBody(const unsigned int shmkey, vector<string>& packet_body,string& err_message){
    BossPacket packet;
    packet_body.clear();
    int error = ParseBossPacket(shmkey,packet,err_message);
    //boss 是以逗号分隔字段的
    split(packet.body,packet_body,",");
    return error;
}


int BossParser::split(const string& str, vector<string>& ret_, const string& sep)
{
	if (str.empty())
	{
		return 0;
	}

	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

        //即使某个字段为空也会加入,这样索引才能对齐
        ret_.push_back(tmp);
        tmp.clear();
	}
	return 0;
}
