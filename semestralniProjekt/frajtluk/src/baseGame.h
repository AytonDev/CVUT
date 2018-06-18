//
// Created by Lukáš Frajt on 13/05/2018.
//
#pragma once

#ifndef SEMESTRALNIPROJEKT_BASEGAME_H
#define SEMESTRALNIPROJEKT_BASEGAME_H

#include "vector"
#include "player.h"
#include "baseQuestion.h"
#include "quizManager.h"
#include "fileManagerTXT.h"
#include <algorithm>


/**
 * @brief 
 * Abstract class, which creates new a game. 
 * This class creates a quiz, creates next pages of the quiz. 
 * Mange whole guiz "game".
 */
class CBaseGame {

    public:
    /**
     * @brief Construct a new CBaseGame object
     * 
     * @param player 
     * @param quiz 
     */
    CBaseGame(const CQuizManager &quiz);
    /**
     * @brief Prepare game
     * 
     * @param os 
     * @param is 
     */
    virtual void StartGame(ostream &os, istream &is) = 0;
    /**
     * @brief  generate questions
     * 
     */
    virtual void QuestionsGenerator(ostream &os, istream &is) = 0;
    /**
     * @brief increase page number if page number is higher than max Page return false
     * 
     * @return true if the page number is lower than max page number
     * @return false 
     */
    virtual bool NextPage() = 0;
    /**
     * @brief print results of the game. Ask user if wants to save results to txt.
     * 
     * @param os 
     */
    virtual void EndGame(ostream &os) = 0;
    /**
     * @brief Destroy the CBaseGame object
     * 
    */
    virtual ~CBaseGame();

    protected:

    CQuizManager m_Quiz;
    int m_Points;

};


#endif //SEMESTRALNIPROJEKT_BASEGAME_H
