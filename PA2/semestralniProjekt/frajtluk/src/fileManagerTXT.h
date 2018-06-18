//
// Created by Lukáš Frajt on 13/05/2018.
//

#ifndef SEMESTRALNIPROJEKT_FILEMANAGERTXT_H
#define SEMESTRALNIPROJEKT_FILEMANAGERTXT_H

#include "fileManager.h"
#include "quizManager.h"
#include "baseQuestion.h"
#include "player.h"

#define TXT ".txt"
#define PATH_PRINT "/exports/"


class CFileManagerTXT {
    
    public:
    /**
     * @brief Construct a new CFileMangerTXT object
     * 
     * @param quizes 
     * @param delimenter 
     */
    CFileManagerTXT(string fileName);
    /**
     * @brief @brief saves the quiz results or readable format of the to the TXT file.
     * 
     * @param quiz to print
     * @param results if true it saves results, else it saves quiz to fill.
     * @param player for printing player's name
     * @param points which player earned
     */
    void SaveQuiz(const CQuizManager& quiz, bool results = false, CPlayer *player = NULL, int points = 0) const;

    private:

    string m_FileName;

};

#endif //SEMESTRALNIPROJEKT_FILEMANGERTXT_H
