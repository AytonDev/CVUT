//
// Created by Lukáš Frajt on 13/05/2018.
//

#ifndef SEMESTRALNIPROJEKT_BASEQUESTION_H
#define SEMESTRALNIPROJEKT_BASEQUESTION_H
#include <string>
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <memory>

using namespace std;

/**
 * @brief abstract base class of the question
 * 
 */
class CBaseQuestion {
    
    public:
    /**
     * @brief Construct a new CBaseQuestion object
     * 
     * @param questionType 
     * @param questionText 
     * @param ID 
     * @param page 
     * @param quizID 
     * @param points 
     * @param dependOnLastAnswer 
     * @param lastQuestionID 
     * @param lastQuestionAnswer 
     */
    CBaseQuestion(int questionType, string questionText, int ID, int page, int quizID, int points, bool dependOnLastAnswer = false,
    int lastQuestionID = 0, int lastQuestionAnswer = 0); 
    /**
     * @brief Construct a new CBaseQuestion object
     * 
     * @param questionType 
     * @param questionText 
     * @param page 
     * @param quizID 
     * @param points 
     * @param dependOnLastAnswer 
     * @param lastQuestionID 
     * @param lastQuestionAnswer 
     */
    CBaseQuestion(int questionType, string questionText, int page, int quizID, int points, bool dependOnLastAnswer = false,
    int lastQuestionID = 0, int lastQuestionAnswer = 0);   
    /**
     * @brief Get the Page which contais this questions
     * 
     * @return int page number
     */
    int GetPage() const;
    /**
     * @brief Get the Print status information for the FILEMANAGER_TXT.
     * 
     * @return true if a question was printed (in game)
     * @return false if a question wasn't printed (in game)
     */
    bool GetPrintStatus() const;
    /**
     * @brief Get the Dependency
     * 
     * @return true if a question depends on user's previus answer.
     * @return false if a question doesn't depend on user's previus answer.
     */
    bool GetDependency() const;
    /**
     * @brief Check answer, add points and return bool value
     * 
     * @param points adds points if answer was right, else doesn't change value
     * @param answers add an ID and an answer index to the map of answers.
     * @return true if user's answer was right
     * @return false  if user's answer wasn't right
     */
    virtual bool CheckAnswer(int &points, std::map<int,int> &answers) = 0;
    /**
     * @brief check if the answer depend's on previous answer.If so check if previous answers contais question's ID an right answer index.
     * 
     * @param answers map of previous user's answers
     * @return true  if the question depends on last answer and answers contains ID a Index
     * @return false if the question doesn't depends on last answer or answers doesn't contains ID a Index
     */
    virtual bool CheckDependency(std::map<int,int> &answers) const;
    /**
     * @brief print question text 
     * 
     * @param os 
     */
    virtual void PrintQuestion(ostream &os) const;
    /**
     * @brief print all question infromation
     * 
     * @param os 
     */
    virtual void PrintAllAboutQuestion(ostream &os) const = 0;
    /**
     * @brief read answer from the user.
     *   
     */
    virtual void ReadAnswer(ostream& os, istream& is) = 0;
    /**
     * @brief return's clone as pointer -> for vector of all questions.
     * 
     * @return CBaseQuestion* 
     */
    virtual shared_ptr<CBaseQuestion> Clone() const = 0;
    /**
     * @brief CSV string format. Delimiter  is ";"
     * 
     * @return the guestion in  CSV string 
     */
    virtual string ToStringCSV() const = 0;
    /**
     * @brief Readable format for the press.
     * 
     * @return string 
     */
    virtual string ToPrintTXT() const = 0;
    /**
    * @brief f Readable format for the press with user's answers
    * 
    * @return string 
    */
    virtual string ToPrintResultsTXT() const = 0;
    /**
     * @brief overload operator << for print question text.
     * 
     * @param os ostream
     * @param question question object
     * @return std::ostream&  ostream
     */
    friend std::ostream & operator<<(std::ostream &os, const CBaseQuestion &question)
    { question.PrintQuestion(os); return os; }  

    virtual ~CBaseQuestion();

    protected:
    
    int m_QuestionType;
    string m_QuestionText;
    int m_ID;
    int m_Page;
    int m_QuizID;
    int m_Points;
    bool m_DependOnLastAnswer;
    int m_LastQuestionID;
    int m_LastQuestionAnswer;
    bool m_Printed;

};


#endif //SEMESTRALNIPROJEKT_BASEQUESTION_H
