//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "baseQuestion.h"


CBaseQuestion::CBaseQuestion(int questionType,string questionText, int page, int quizID, int points, bool dependOnLastAnswer,
    int lastQuestionID, int lastQuestionAnswer):m_QuestionType(questionType), m_QuestionText(questionText), m_Page(page), m_QuizID(quizID),
    m_Points(points), m_DependOnLastAnswer(dependOnLastAnswer), m_LastQuestionID(lastQuestionID), 
    m_LastQuestionAnswer(lastQuestionAnswer), m_Printed(false) {
    srand(time(NULL));
     m_ID =rand() % 100000;
}

CBaseQuestion::CBaseQuestion(int questionType,string questionText, int ID, int page, int quizID, int points, bool dependOnLastAnswer,
    int lastQuestionID, int lastQuestionAnswer):m_QuestionType(questionType), m_QuestionText(questionText), m_ID(ID), m_Page(page), m_QuizID(quizID),
    m_Points(points), m_DependOnLastAnswer(dependOnLastAnswer), m_LastQuestionID(lastQuestionID), 
    m_LastQuestionAnswer(lastQuestionAnswer), m_Printed(false) {
   
}

CBaseQuestion::~CBaseQuestion() {}

void CBaseQuestion::PrintQuestion(ostream &os) const {
   os << m_QuestionText;
}

int CBaseQuestion::GetPage() const {
    return m_Page;
}

bool CBaseQuestion::GetDependency() const {
    return m_DependOnLastAnswer;
}

bool CBaseQuestion::GetPrintStatus() const {
     return m_Printed;
}

bool CBaseQuestion::CheckDependency(std::map<int,int> &answers) const {
    if(m_DependOnLastAnswer) {
        map <int, int> :: iterator itr;
        for (itr = answers.begin(); itr != answers.end(); ++itr)
        {
            if(m_LastQuestionID == itr->first && m_LastQuestionAnswer == itr->second)
            return true;
        }
    }
    return false;
}
