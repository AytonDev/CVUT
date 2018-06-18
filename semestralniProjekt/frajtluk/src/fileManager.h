//
// Created by Lukáš Frajt on 13/05/2018.
//

#ifndef SEMESTRALNIPROJEKT_FILEMANAGER_H
#define SEMESTRALNIPROJEKT_FILEMANAGER_H
#include "player.h"
#include "textQuestion.h"
#include "multiTextQuestion.h"
#include "trueFalseQuestion.h"
#include "orderQuestion.h"
#include "pickOneQuestion.h"
#include "pickManyQuestion.h"
#include <string>
#include <cstdio>
#include <vector>
#include <iostream>
#include <fstream>

#define PATH_TO_FILE "./examples/"

using namespace std;

/*
 * Comment 
 */
class CFileManager {

    public:
    /**
     * @brief Construct a new CFileManager object
     * 
     * @param quizesFileName name of the quizzesFile
     */
    CFileManager(string quizesFileName);
    /**
     * @brief Save quiz to the CSV file
     * 
     * @param quiz CQuizManager object
     */
    virtual void SaveQuiz(const CQuizManager &quiz) const = 0;
    /**
     * @brief loads quizzes from the file. IF something fail returns false, else returns true. 
     * Push quizzes to the vector
     * 
     * @param quizzes vector of quizzes
     * @return true if everything is ok
     * @return false if something fail
     */
    virtual bool LoadQuizzes(vector<CQuizManager> &quizzes) const = 0;
    /**
     * @brief save question to the file.
     * 
     * @param question returns
     * @param fileName 
     */
    virtual void SaveQuestion(const CBaseQuestion &question, string fileName) const = 0;
    /**
     * @brief loads question to the file.
     * 
     * @param fileName 
     * @param quiz 
     * @return true if everything is ok
     * @return false if something fail
     */
    virtual bool LoadQuestions(string fileName, CQuizManager &quiz) const = 0;

    protected:

    string m_QuizesFileName;
    string quizFile;
    string scoreFile;

};

#endif //SEMESTRALNIPROJEKT_FILEMANGER_H
