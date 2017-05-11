#include "statekeeper.h"

bool StateKeeper::contains(const uint &key){
    return m_states.contains(key);
}

void StateKeeper::insert(const uint &key, const uint &value){
    m_states.insert(key,value);
}

int StateKeeper::remove(const uint &key){
    return m_states.remove(key);
}

uint StateKeeper::get(const uint &key){
    return m_states.value(key);
}

void StateKeeper::clear(){
    m_states.clear();
}
