//
// Created by Lukáš Frajt on 13/05/2018.
//

#ifndef SEMESTRALNIPROJEKT_FILEMANAGERCSV_H
#define SEMESTRALNIPROJEKT_FILEMANAGERCSV_H

#include "fileManager.h"
#include "quizManager.h"
#include "baseQuestion.h"

/**
 * 
 */
class CFileMangerCSV : public CFileManager {
    
    public:
    /**
     * @brief Construct a new CFileMangerCSV object
     * 
     * @param quizes 
     * @param delimenter 
     */
    CFileMangerCSV(string quizes, string delimenter);
    /**
     * @brief Save quiz to the CSV file
     * 
     * @param quiz CQuizManager object
     */
	virtual void SaveQuiz(const CQuizManager &quiz) const;
    /**
     * @brief loads quizzes from the CSV file. IF something fail returns false, else returns true. 
     * Push quizzes to the vector
     * 
     * @param quizzes vector of quizzes
     * @return true if ok
     * @return false if something fail
     */
    virtual bool LoadQuizzes(vector<CQuizManager> &quizzes) const;
    /**
     * @brief save question to the file.
     * 
     * @param question returns
     * @param fileName 
     */
    virtual void SaveQuestion(const CBaseQuestion &question, string fileName) const;
    /**
     * @brief loads question to the CSV file.
     * 
     * @param fileName 
     * @param quiz 
     * @return true if everything is ok
     * @return false if something fail
     */
    virtual bool LoadQuestions(string fileName, CQuizManager &quiz) const;

    private:

    string m_Delimeter;
	
};

#endif //SEMESTRALNIPROJEKT_FILEMANGERCSV_H
