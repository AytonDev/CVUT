//
// Created by Lukáš Frajt on 13/05/2018.
//

#ifndef SEMESTRALNIPROJEKT_PICKMANYQUESTION_H
#define SEMESTRALNIPROJEKT_PICKMANYQUESTION_H

#include <algorithm>
#include "baseQuestion.h"
#include "quizManager.h"
#include <memory>


#define LIST_COUNT 4
#define PICK_MANY_TYPE 6


/**
 * 
 */ 
class CPickManyQuestion : public CBaseQuestion {
    public:
    /**
     * @brief Construct a new CPickManyQuestion object
     * 
     * @param questionText 
     * @param answerList 
     * @param rightAnswers 
     * @param ID 
     * @param page 
     * @param quizID 
     * @param points 
     * @param dependOnLastAnswer 
     * @param lastQuestionID 
     * @param lastQuestionAnswer 
     */
    CPickManyQuestion(string questionText,vector<string>& answerList, vector<int> &rightAnswers, int ID,int page, int quizID, int points,bool dependOnLastAnswer=false,int lastQuestionID=0, 
    int lastQuestionAnswer=0);
    /**
     * @brief Construct a new CPickManyQuestion object
     * 
     * @param questionText 
     * @param answerList 
     * @param rightAnswers 
     * @param page 
     * @param quizID 
     * @param points 
     * @param dependOnLastAnswer 
     * @param lastQuestionID 
     * @param lastQuestionAnswer 
     */
    CPickManyQuestion(string questionText,vector<string> &answerList, vector<int> &rightAnswers, int page, int quizID, int points,bool dependOnLastAnswer=false,int lastQuestionID=0, 
    int lastQuestionAnswer=0);
     /**
     * @brief Check answer, add points and return bool value
     * 
     * @param points adds points if answer was right, else doesn't change value
     * @param answers add an ID and an answer index to the map of answers.
     * @return true if user's answer was right
     * @return false  if user's answer wasn't right
     */
    virtual bool CheckAnswer(int &points, std::map<int,int> &answers);
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
    virtual void PrintAllAboutQuestion(ostream &os) const;
    /**
     * @brief read answer from the user.
     *   
     */
    virtual void ReadAnswer(ostream& os, istream& is);
    /**
     * @brief return's clone as pointer -> for vector of all questions.
     * 
     * @return CBaseQuestion* 
     */
    virtual shared_ptr<CBaseQuestion> Clone() const;
    /**
     * @brief CSV string format. Delimiter  is ";"
     * 
     * @return the guestion in  CSV string 
     */
    virtual string ToStringCSV() const;
    /**
     * @brief Readable format for the press.
     * 
     * @return string 
     */
    virtual string ToPrintTXT() const;
    /**
     * @brief f Readable format for the press with user's answers
     * 
     * @return string 
     */
    virtual string ToPrintResultsTXT() const;
    /**
     * @brief Destroy the CPickManyQuestion object
     * 
     */
    ~CPickManyQuestion();
    /**
     * @brief static function to create Question from user input.
     * 
     * @param quiz 
     * @return CMultiTextAnswerQuestion created question
     */
    static CPickManyQuestion PrintQuestionBuilder(ostream &os, istream &is, CQuizManager &quiz);
    /**
     * @brief Create a Question From CSV and clone it for vector of pointers
     * 
     * @param questionCSV csv question from the file
     * @return CBaseQuestion* pointer to the new question
     */
    static shared_ptr<CBaseQuestion> CreateQuestionFromCSV(string questionCSV);

    private:
    /**
     * @brief 
     * 
     * @param v1 
     * @param v2 
     * @return true 
     * @return false 
     */
    bool Compare(vector<int> &v1, vector<int> &v2) const;
    /**
     * @brief 
     * 
     * @param v1 
     * @param number 
     * @return true 
     * @return false 
     */
    bool ContaisAnswer(const vector <int> &v1, int number) const;
    vector <string> m_AnswersList;
    vector <int> m_RightAnswer;
    vector <int> m_UserAnswer;
};


#endif //SEMESTRALNIPROJEKT_PICKMANYQUESTION_H
