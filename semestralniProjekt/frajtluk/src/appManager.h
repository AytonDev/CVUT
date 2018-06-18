//
// Created by Lukáš Frajt on 13/05/2018.
//


#ifndef SEMESTRALNIPROJEKT_APPMANAGER_H
#define SEMESTRALNIPROJEKT_APPMANAGER_H

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include "quizManager.h"
#include "fileManagerCSV.h"
#include "fileManagerTXT.h"
#include "textQuestion.h"
#include "multiTextQuestion.h"
#include "orderQuestion.h"
#include "pickManyQuestion.h"
#include "pickOneQuestion.h"
#include "trueFalseQuestion.h"
#include "classicGame.h"

#define APPNAME "KVÍZY"

using namespace std;

/**
 * @brief Whole app manager (app starter)
 * 
 */
class CAppManager {

    public:
    /**
     * @brief Construct a new CAppManager object
     * 
     */
    CAppManager();
    /**
     * @brief prepare game 
     * 
     */
    void StartGame(ostream &os, istream &is);
    /**
     * @brief return to main and close app
     * 
     */
    void EndGame();
    /**
     * @brief  prints main navigation
     * 
     * @return std::ostream& 
     */
    void PrintMain(ostream &os, istream &is);
    /**
     * @brief prints quiz navigation
     * 
     * @return void 
     */
    void PrintQuizManager(ostream &os, istream &is);
    /**
     * @brief prints quiz builder navigation
     * 
     * @return void 
     */
    void PrintQuizBuilder(ostream &os, istream &is);
    /**
     * @brief prints question builder navigation
     * 
     * @return void 
     */
    void PrintQuestionBuilder(ostream &os, istream &is);
    /**
     * @brief prints all quizzes 
     * 
     * @return void 
     */
    void PrintQuizzes(ostream &os);
    /**
     * @brief prints all guizzes and ask user for his name
     * 
     * @return void 
     */
    void PrintNewGame(ostream &os, istream &is);
    /**
     * @brief print export possibilities
     * 
     * @return void 
     */
    void PrintExportManager(ostream &os, istream &is);

    private:

    CFileMangerCSV m_CSVManager;
    vector<CQuizManager> m_Quizes;
    vector<string> m_MainMenu;
    vector<string> m_QuizManager;
    vector<string> m_QuestionTypes;
};

#endif //SEMESTRALNIPROJEKT_GAMEMANAGER_H
