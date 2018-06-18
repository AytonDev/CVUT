//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "player.h"
CPlayer::CPlayer(string name ): m_Name(name), m_Score(0) {
    srand(time(NULL));
    m_ID =rand() % 100000;
}

CPlayer::CPlayer(string name, int id, int score): m_Name(name),m_ID(id), m_Score(score) {
   
}

string CPlayer::GetPlayerName() const {
   return  m_Name;
}


void  CPlayer::PrintPlayer(ostream &os) const {
    os << m_Name <<  " - " << m_ID  << ":: " << m_Score << endl;
}

CPlayer CPlayer::PrintPlayerBuilder(){
    string name;
    cout << "Zadej jméno hráče" <<endl;
    std::getline(std::cin >> std::ws, name);
    return  CPlayer(name);

}
