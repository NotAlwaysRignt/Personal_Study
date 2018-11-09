#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "gtest.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;

/* 示例XML的内容
<config>
	<file title="windows" version="8.1">
	<!-- File Fisrt Comment -->
	<!-- File Second Comment -->
		<paths attr="directory">
		<!-- Paths Comment -->
			<pathname title="123">abc</pathname>
			<pathname title="456">efg</pathname>
			<pathname title="789">hij</pathname>
		</paths>
		<paths>
			<pathname title="111">klm</pathname>
			<pathname title="222">nop</pathname>
			<pathname title="333">qrs</pathname>
		</paths>

        <md5>123456789</md5>
        <size value="20.213">20M</size>
	</file>
</config>
 * */

/*
 * property tree 如果运行失败会抛出异常,因此使用时必须要用 try catch
 * ptree_error是所有property tree 异常类的基类,如果我们不想细分,则可以全部采用此类,该类的 what 方法可以打印对应的错误信息
 * */
TEST(test,xml_parse_exception)
{
    using namespace boost::property_tree;
    ptree pt;
    try {
        xml_parser::read_xml("./unkonwnfile.xml",pt); //如果文件不存在或者 XML 格式有误,都会抛出异常
    }catch(const xml_parser_error& e) {
        std::cout << e.what() << std::endl; // ./unkonwnfile.xml: cannot open file
    }
    try {
        xml_parser::read_xml("./unkonwnfile.xml",pt); //如果文件不存在或者 XML 格式有误,都会抛出异常
    }catch(ptree_error& e) { //ptree_error是所有property tree 异常类的基类,如果我们不想细分,则可以全部采用此类
        std::cout << e.what() << std::endl; //./unkonwnfile.xml: cannot open file
    }
    try {
        xml_parser::read_xml("./conf/test1.xml",pt);
        pt.get<string>("unknown_node");
    }catch(const xml_parser_error& e) {
        std::cout << e.what() << std::endl; //./unkonwnfile.xml: cannot open file
        return ;
    }catch(ptree_error& e) { //ptree_error是所有property tree 异常类的基类,如果我们不想细分,则可以全部采用此类
        std::cout << e.what() << std::endl; //No such node (unknown_node)
    }
    
}

TEST(test,xml_parse)
{
    using namespace boost::property_tree;
    ptree pt;
    try {
        xml_parser::read_xml("./conf/test1.xml",pt);
    }catch(const xml_parser_error& e) {
        std::cout << e.what() << std::endl;
        return ;
    }
    ptree& file = pt.get_child("config.file"); //失败会抛异常,因此用建议 try
    //会遍历<file> <paths> <paths>
    for (auto node = file.begin(); node != file.end(); node++) {
        if("<xmlattr>" == node->first) { //当前节点
            //如果获取的节点不存在,则会抛出异常,如果不想要抛出异常,则需要设置默认值
            std::cout << node->second.get<string>("title")  << std::endl;
            std::cout << node->second.get<string>("version")  << std::endl;
            std::cout << node->second.get<string>("noexit","no such node") << std::endl;
        }
        else if ("<xmlcomment>" == node->first) { //注释
            std::cout << node->second.data() << std::endl;
        }
        else { // 下一级节点
            if("md5" == node->first){
                std::cout << node->second.data() << std::endl; //string类型
            }
            else if ("size" == node->first) {
                std::cout << node->second.data() << std::endl; //string类型
                //等效: std::cout << node->second.get<string>("") << std::endl;
                //要获取xml中的属性,不可以直接调用 node->second.get<string>("value","0") ,只能再用 for 遍历
                auto child = node->second.get_child("");
                for(auto it = child.begin(); it != child.end(); it++) {
                    if("<xmlattr>" == it->first) {
                        std::cout << it->second.get<double>("value",0) << std::endl;
                        std::cout << it->second.get<int>("value",0) << std::endl; //如果数据不是double的,这里会打印出0,而不是舍弃精度
                    }
                }
            }
        }
    }
}


//用for循环读XML很麻烦,有时我们想获得指定标签的属性,直接调用 ptree 对象的 get 方法
TEST(test,read_attr)
{
    using namespace boost::property_tree;
    ptree pt;
    try {
        xml_parser::read_xml("./conf/test1.xml",pt);
    }catch(const xml_parser_error& e) {
        std::cout << e.what() << std::endl;
        return ;
    }

    std::cout <<pt.get<double>("config.file.size.<xmlattr>.value",0) << std::endl; //20.213; <size value="20.213"></size>
    /*下面的方法也可以
    ptree& element = pt.get_child("config.file.size");
    std::cout <<element.get<double>("<xmlattr>.value",0) << std::endl; //20.213; <size value="20.213"></size>
    */
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
