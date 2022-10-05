#include "logBuilder.h"
#include<regex>
#include<map>
#include<conio.h>
#include <filesystem>

//Logger* logBuilder::buildLog(const std::string& conf_file)
//{
//    Logger* logger = new Logger;
//    std::ifstream f(conf_file, std::ifstream::binary);
//    if (!f.is_open())
//        throw std::exception("config file can't be opened");
//
//    nlohmann::json js;
//    nlohmann::json js_streams;
//    f >> js;
//
//    if (!examine_conf(js))
//        throw std::exception("wrong json file configuration");
//    js_streams = js.at("streams");
//
//    // инициализируем список потоков
//    std::string file_name = js_streams[0].at("filename").get<std::string>();
//    SEV sever = js.at("sev_code").at(js_streams[0].at("severity").get<std::string>());
//
//    logger->streams_list = new Stream(sever, file_name);
//    Stream* tmp = logger->streams_list;
//    for (int i = 1; i < js_streams.size(); i++)
//    {
//        std::string file_name = js_streams[i].at("filename").get<std::string>();
//        SEV sever = js.at("sev_code").at(js_streams[i].at("severity").get<std::string>());
//        tmp->next = new Stream(sever, file_name);
//        tmp = tmp->next;
//    }
//    return logger;
//}

Logger* logBuilder::buildLog(const std::string& file_name)
{
    char n;
    SEV sever;
    std::string buf;
    Logger* logger = new Logger;
    /*Stream* tmp = logger->streams_list;*/
    std::filesystem::create_directory("logs");
    logger->streams_list = new Stream(INFORMATION, file_name);
    /*while (1)
    {
        system("cls");
        std::cout << "enter the file name (if you wanna stop print 'stop'): ";
        std::cin >> buf;
        if (buf == "stop")
            break;
        
        while (1)
        {
            std::cout << "\n1.trace\n2.debug\n3.information\n4.warning\n5.error\n6.fatal\nchoose the severity:" << std::endl;
            std::cin >> n;
            if (n == '1')
                sever = TRACE;
            else if (n == '2')
                sever = DEBUG;
            else if (n == '3')
                sever = INFORMATION;
            else if (n == '4')
                sever = WARNING;
            else if (n == '5')
                sever = ERROR;
            else if (n == '6')
                sever = FATAL;
            else
                continue;
            break;
        }
    
        try
        {
            if (logger->streams_list == nullptr)
            {
                logger->streams_list = new Stream(sever, buf);
                tmp = logger->streams_list;
            }
            else
            {
                tmp->next = new Stream(sever, buf);
                tmp = tmp->next;
            }
        }
        catch (const std::exception& ex)
        {
            system("cls");
            std::cout << ex.what() << std::endl;
        }
    }*/
    return logger;
}

//bool logBuilder::examine_conf(const nlohmann::json& j)
//{
//    std::map<std::string, int> mp;
//    try
//    {
//        std::string file_name;
//        j.at("streams");
//        j.at("sev_code");
//        nlohmann::json j2 = j.at("streams");
//        if (j2.empty())
//            throw std::exception("no streams list in json");
//        for (int i = 0; i < j2.size(); i++)
//        {
//            if (j2[i].size() != 2)
//                throw std::exception("wrong stream configs");
//            file_name = j2[i].at("filename").get<std::string>();
//            if (mp.find(file_name) != mp.end())
//                throw std::exception("such stream already excist");
//            mp[file_name] = i;
//            j2[i].at("severity");
//            if (std::regex_match(j2[i].at("severity").get<std::string>(), std::regex("debug|fatal|error|information|warning|trace")) == false)
//                throw std::exception("wrong severity name of stream");
//        }
//    }
//    catch (const std::exception& ex)
//    {
//        return false;
//    }
//    return true;
//}
