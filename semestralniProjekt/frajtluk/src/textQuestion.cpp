//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "textQuestion.h"

CTextQuestion::CTextQuestion(string questionText, string righAnswer, int ID,int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer): CBaseQuestion(TEXT_TYPE,questionText,ID,page, quizID, points,dependOnLastAnswer, lastQuestionID, lastQuestionAnswer) ,
     m_RightAnswer(righAnswer) {

} 

CTextQuestion::CTextQuestion(string questionText, string righAnswer, int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer): CBaseQuestion(TEXT_TYPE,questionText,page, quizID, points,dependOnLastAnswer, lastQuestionID, lastQuestionAnswer) ,
     m_RightAnswer(righAnswer) {

} 

bool CTextQuestion::CheckAnswer(int &points, std::map<int,int> &answers) {
    m_Printed = true;
    if(m_RightAnswer == m_UserAnswer){
       answers.insert(pair<int, int> (m_ID, 1));
        points += m_Points;
        return true;
    }
    answers.insert(pair<int, int> (m_ID, 0));
    
    return false;
}

void CTextQuestion::ReadAnswer(ostream& os, istream& is) {
    os << "Napište správnou odpověď a stiskněte Enter"<<endl;
    std::getline(is >> std::ws, m_UserAnswer);
}


void CTextQuestion::PrintQuestion(ostream &os) const {
    os << m_QuestionText <<endl;
}

void CTextQuestion::PrintAllAboutQuestion(ostream &os) const {
    os << "ID:" <<m_ID << " Text: " << m_QuestionText << endl;
    os << "\t  1) " << m_RightAnswer << endl;
    os << endl;
}

shared_ptr<CBaseQuestion> CTextQuestion::Clone() const {
    return shared_ptr<CBaseQuestion>(new CTextQuestion(*this));
}

CTextQuestion CTextQuestion::PrintQuestionBuilder(ostream &os, istream &is,CQuizManager &quiz){
    string questionText, rightAnswer, input;
    int page, points, lastquestionID, lastquestionAnswer; //answerID;
    bool dependOnLastAnswer=false;
    os<< "Zadej text otázky"<<endl;
    std::getline(is >> std::ws, questionText);
    os << "Zadej text správné odpovědi" <<endl;
    std::getline(is >> std::ws, rightAnswer);
    os << "Počet bodů za správnou odpověď"<<endl;
    is >> points;
    os << "Na jaké stránce kvízu se má otázka nacházet?"<<endl;
    is >> page;
    if(page >1){
        os << "Má být otázka závislá na předchozí odpovědi (otázka se zobrazí, jen pokud uživatel zadal na nějaké předchozí strance  určitou odpověd)"<<endl;
        std::getline(is >> std::ws, input);
        if(input == "ANO" || input=="ano" || input =="A" || input =="a")
            dependOnLastAnswer = true;
        else if (input == "NE" || input=="ne" || input =="N" || input =="n")   
            dependOnLastAnswer = false; 
        if(dependOnLastAnswer){
            os<<"Zadejte číslo (ID) otázky, na které má být tato otázka závislá"<<endl;
            is >> lastquestionID;
            os << "Zadejte index odpovědi dané otázky"<<endl;
            is >> lastquestionAnswer;
            return CTextQuestion(questionText,rightAnswer,page, quiz.GetQuizID(),points,dependOnLastAnswer,lastquestionID,lastquestionAnswer);
        }    
    }
    return CTextQuestion(questionText,rightAnswer,page, quiz.GetQuizID(), points);
}

string CTextQuestion::ToStringCSV() const {
    string s = to_string(m_QuizID)+";"+ to_string(m_QuestionType) +";" + to_string(m_ID)+ ";" + m_QuestionText + ";"+ m_RightAnswer +";" +  to_string(m_Points) + ";" + to_string(m_Page)+
    ";" + to_string(m_DependOnLastAnswer) + ";" + to_string(m_LastQuestionID) + ";" + to_string(m_LastQuestionAnswer);
    return s;
}

string CTextQuestion::ToPrintTXT() const {
    string s = m_QuestionText + "\n" + "\t  " + "Odpověď:  _____________\n\n\n";
    return s;
}

string CTextQuestion::ToPrintResultsTXT() const {
    string s = m_QuestionText + "\n" + "\t   " + "Odpověď: "+  m_UserAnswer +"\n\n\n";
    return s;
}

CTextQuestion::~CTextQuestion() {

} 

shared_ptr<CBaseQuestion> CTextQuestion::CreateQuestionFromCSV(string questionCSV){

    string questionText, righAnswer, ID, page,  quizID, points,questionType, dependOnLastAnswer, lastQuestID, lastAnswer;
    bool answer;
    try {
        quizID = questionCSV.substr(0, questionCSV.find(";"));
        questionType = questionCSV.substr(0, questionCSV.erase(0, quizID.length()+1).find(";"));
        ID = questionCSV.substr(0, questionCSV.erase(0, questionType.length()+1).find(";"));
        questionText = questionCSV.substr(0, questionCSV.erase(0, ID.length()+1).find(";"));
        righAnswer = questionCSV.substr(0, questionCSV.erase(0, questionText.length()+1).find(";"));
        points = questionCSV.substr(0, questionCSV.erase(0, righAnswer.length()+1).find(";"));
        page = questionCSV.substr(0, questionCSV.erase(0, points.length()+1).find(";"));
        dependOnLastAnswer = questionCSV.substr(0, questionCSV.erase(0, page.length()+1).find(";"));
        lastQuestID = questionCSV.substr(0, questionCSV.erase(0, dependOnLastAnswer.length()+1).find(";"));
        lastAnswer = questionCSV.substr(0, questionCSV.erase(0, lastQuestID.length()+1).find(";"));
        answer = dependOnLastAnswer == "0" ? false : true; 
        CTextQuestion question = CTextQuestion(questionText,righAnswer,stoi(ID),stoi(page),stoi(quizID),stoi(points),answer, stoi(lastQuestID), stoi(lastAnswer));
        return question.Clone();
    }
    catch( const std::invalid_argument& e ) {
        cout << "Chyba v souboru" << endl;
        throw;
    }
}
