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
