
#include "utils.h"

Utils Utils::m_instance = Utils();
Utils::Utils()
{
}

Utils& Utils::getInstance(){
    return m_instance;
}
