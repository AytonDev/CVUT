//
// Created by Lukáš Frajt on 13/05/2018.
//

#ifndef SEMESTRALNIPROJEKT_PLAYER_H
#define SEMESTRALNIPROJEKT_PLAYER_H
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <stdlib.h>

using namespace std;

class CPlayer {

    public:
    /**
     * @brief Construct a new CPlayer object
     * 
     * @param name 
     */
    CPlayer(string name);
    /**
     * @brief Construct a new CPlayer object
     * 
     * @param name 
     * @param id 
     * @param score 
     */
    CPlayer(string name, int id, int score);
    /**
     * @brief 
     * 
     * @param os 
     */
    void PrintPlayer(ostream & os) const;
    /**
     * @brief 
     * 
     * @param os 
     * @param is 
     * @return CPlayer 
     */
    static CPlayer PrintPlayerBuilder();
    /**
     * @brief Get the Player Name object
     * 
     * @return string 
     */
    string GetPlayerName() const;
    /**
     * @brief 
     * 
     * @param os 
     * @param player 
     * @return std::ostream& 
     */
    friend std::ostream & operator<<(std::ostream &os, const CPlayer &player)
    {
        player.PrintPlayer(os); return os; 
    }  

    private:
    
    string m_Name;
    int m_ID;
    int m_Score;
  
};

#endif //SEMESTRALNIPROJEKT_PLAYER_H
