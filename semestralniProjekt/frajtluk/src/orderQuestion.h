//
// Created by Lukáš Frajt on 13/05/2018.
//

#ifndef SEMESTRALNIPROJEKT_ORDERQUESTION_H
#define SEMESTRALNIPROJEKT_ORDERQUESTION_H

#include "baseQuestion.h"
#include "quizManager.h"

#define LIST_COUNT 4
#define ORDER_TYPE 4


/**
 * 
 */
class COrderQuestion : public CBaseQuestion {

    public:
    /**
     * @brief Construct a new COrderQuestion object
     * 
     * @param questionText 
     * @param textsToOrder 
     * @param rigthOrder 
     * @param ID 
     * @param page 
     * @param quizID 
     * @param points 
     * @param dependOnLastAnswer 
     * @param lastQuestionID 
     * @param lastQuestionAnswer 
     */
    COrderQuestion(string questionText,vector<string> &textsToOrder, vector<int> &rigthOrder, int ID, int page, int quizID, int points, bool dependOnLastAnswer=false,int lastQuestionID=0, 
    int lastQuestionAnswer=0);
    /**
     * @brief Construct a new COrderQuestion object
     * 
     * @param questionText 
     * @param textsToOrder 
     * @param rigthOrder 
     * @param page 
     * @param quizID 
     * @param points 
     * @param dependOnLastAnswer 
     * @param lastQuestionID 
     * @param lastQuestionAnswer 
     */
    COrderQuestion(string questionText,vector<string> &textsToOrder, vector<int> &rigthOrder, int page, int quizID, int points, bool dependOnLastAnswer=false,int lastQuestionID=0, 
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
     * @brief return's clone as pointer -> for vector of all questions.
     * 
     * @return CBaseQuestion* 
     */
    virtual shared_ptr<CBaseQuestion> Clone() const;
    /**
     * @brief read answer from the user.
     *   
     */
    virtual void ReadAnswer(ostream& os, istream& is);
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
     * @brief Destroy the COrderQuestion object
     * 
     */
    ~COrderQuestion();
    /**
     * @brief static function to create Question from user input.
     * 
     * @param quiz 
     * @return CMultiTextAnswerQuestion created question
     */
    static COrderQuestion PrintQuestionBuilder(ostream &os, istream &is,CQuizManager &quiz);
    /**
     * @brief Create a Question From CSV and clone it for vector of pointers
     * 
     * @param questionCSV csv question from the file
     * @return CBaseQuestion* pointer to the new question
     */
    static shared_ptr<CBaseQuestion> CreateQuestionFromCSV(string questionCSV);

    private:

    vector<string> m_TextsToOrder;
    vector<int> m_Answer;
    vector<int> m_UserAnswer;

};

#endif //SEMESTRALNIPROJEKT_ORDERQUESTION_H
