//
// Created by Lukáš Frajt on 13/05/2018.
//
#pragma once

#ifndef SEMESTRALNIPROJEKT_CLASSICGAME_H
#define SEMESTRALNIPROJEKT_CLASSICGAME_H

#include "baseGame.h"
#include <memory>
#include <map>


class CClassicGame: public CBaseGame {
  public:
  /**
   * @brief Construct a new CClassicGame object
   * 
   * @param player 
   * @param quiz 
   */
  CClassicGame(const CPlayer &player, const CQuizManager &quiz);
  /**
   * @brief Prepare game
   * 
   * @param os 
   * @param is 
   */
  virtual void StartGame(ostream &os, istream& is);
  /**
   * @brief  generate questions
   * 
   */
  virtual void QuestionsGenerator(ostream& os, istream& is);
  /**
   * @brief increase page number if page number is higher than max Page return false
   * 
   * @return true if the page number is lower than max page number
   * @return false 
   */
  virtual bool NextPage();
  /**
   * @brief print results of the game. Ask user if wants to save results to txt.
   * 
   * @param os 
   */
  virtual void EndGame(ostream &os);

    ~CClassicGame();

  private:

  CPlayer m_Player;
  std::map<int,int> m_Answers;
  vector<shared_ptr<CBaseQuestion> > m_PageQuestions;
  int m_CurrentPage;
  int m_MaxPage;

};

#endif //SEMESTRALNIPROJEKT_CLASSICGAME_H
