//
// Created by Lukáš Frajt on 13/05/2018.
//

#ifndef SEMESTRALNIPROJEKT_QUIZMANAGER_H
#define SEMESTRALNIPROJEKT_QUIZMANAGER_H

#include "baseQuestion.h"
#include <cstdlib>
#include <map>
#include <fstream>
#include <iostream>
#include <memory>

#define CSV ".csv"

class CQuizManager {

    public:
    /**
     * @brief Construct a new CQuizManager object from the file.
     * 
     * @param name of the quiz
     * @param ID id of the quiz
     * @param desc 
     * @param fileName 
     */
    CQuizManager(string name, int ID, string desc,string fileName);
    /**
     * @brief Construct a new CQuizManager object from user's input.
     * 
     * @param name 
     * @param desc 
     */
    CQuizManager(string name, string desc);

    ~CQuizManager();
    /**
     * @brief add new question to the quiz
     * 
     * @param question 
     * @return true 
     * @return false 
     */
    bool AddQuestion(const CBaseQuestion& question);
    /**
     * @brief delete all questions from the vector
     * 
     */
    void ClearQuestions();
    /**
     * @brief Get the Quiz ID of quizz
     * 
     * @return int 
     */
    int GetQuizID();
    /**
     * @brief Get the name of file with all questions.
     * 
     * @return string 
     */
    string GetQuestionsFile();
    /**
     * @brief Get the Quiz Name 
     * 
     * @return string 
     */
    string GetQuizName();
    /**
     * @brief return string CSV format of the quiz. 
     * 
     * @return string 
     */
    string ToStringCSV() const;
    /**
     * @brief add string format of the quiz to the ofstream. If results parameter is true, returns user's results.
     * 
     * @param outfile 
     * @return ofstream& 
     */
    ofstream& SaveToPrint(ofstream &outfile, bool results=false) const;
    /**
     * @brief Get the Questions On Page object
     * 
     * @param questions 
     * @param page 
     */
    void GetQuestionsOnPage( vector<shared_ptr<CBaseQuestion> >& questions, int page, std::map<int,int>& answers, int &maxPages) const;
    /**
     * @brief prints all questions and infromations about the quiz.
     * 
     * @param os 
     * @return std::ostream& 
     */
    std::ostream& PrintAll(ostream& os) const;
    /**
     * @brief overload operator << to print basic info about quiz.
     * 
     * @param os 
     * @param quiz 
     * @return std::ostream& 
     */
    friend std::ostream & operator<<(std::ostream& os, const CQuizManager& quiz)
    { quiz.PrintQuiz(os); return os; }  


    private:
    vector<shared_ptr<CBaseQuestion> > m_Questions;
    string m_Name;
    int m_ID;
    string m_Desc;
    string m_QuestionsFileName;

    void PrintQuiz(ostream& os) const;
};

#endif //SEMESTRALNIPROJEKT_QUIZMANAGER_H

