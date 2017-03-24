#include "ramiProtocol.h"

RamiProtocol RamiProtocol::m_instance = RamiProtocol();
RamiProtocol::RamiProtocol(){
}

RamiProtocol::~RamiProtocol(){

}

RamiProtocol& RamiProtocol::instance(){
   return m_instance;
}

RamiProtocol::Data RamiProtocol::encrypt(char adress, char line, char column, bool state, char ack){
    RamiProtocol::Data result;
    //we make a binary mask '1111'
    char mask = (1 << 5)-1;
    result.adressColum=(adress & mask) << 4;
    result.adressColum+=(column & mask);
    result.lineState=(line & mask) << 4;
    result.lineState+=state;
    result.ack=ack;
    return result;
}

RamiProtocol::Data RamiProtocol::encrypt(const Params& params){
    return RamiProtocol::encrypt(params.adress,params.line,params.column,params.state,params.ack);
}

void RamiProtocol::print(const Data& data){
    std::bitset<8> adressColumn(data.adressColum), lineState(data.lineState), ack(data.ack);
    std::cout << "adressColumn:" << adressColumn << std::endl
                 << "lineState:" << lineState << std::endl
                 << "ack:" << ack << std::endl;
}

void RamiProtocol::print(const Params& params){
    std::cout << "adress:" << params.adress << std::endl
              << "column:" << params.column << std::endl
              << "line:" << params.line << std::endl
              << "state:" << params.state << std::endl
              << "ack:" << params.ack << std::endl;
}

RamiProtocol::Params RamiProtocol::decrypt(const Data& data){
    std::string str;
    str.append(1,data.adressColum);
    str.append(1,data.lineState);
    str.append(1,data.ack);
    return RamiProtocol::decrypt(str);
}

RamiProtocol::Params RamiProtocol::decrypt(const std::string& str){
    RamiProtocol::Params result;
    std::cmatch cm;
    //TODO: create regex in case we increase number of columns possible
    std::regex_match(str,cm,std::regex("[\x81\x82\x83\x84].!"));
    result.column=cm[0]-0x81;
    result.line=cm[1]/16-1;
    result.ack=cm[3];
    result.adress=cm[0]/16;
    result.state=(cm[1]<<4)>>4;
    return result;
}
