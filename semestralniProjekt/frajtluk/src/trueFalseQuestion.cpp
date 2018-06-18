//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "trueFalseQuestion.h"

CTrueFalseQuestion::CTrueFalseQuestion(string questionText, bool righAnswer, int ID,int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer): CBaseQuestion(TRUE_FALSE_TYPE,questionText,ID,page, quizID, points,dependOnLastAnswer, lastQuestionID, lastQuestionAnswer) ,
     m_RightAnswer(righAnswer) {

} 


CTrueFalseQuestion::CTrueFalseQuestion(string questionText, bool righAnswer,int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer): CBaseQuestion(TRUE_FALSE_TYPE,questionText,page, quizID, points,dependOnLastAnswer, lastQuestionID, lastQuestionAnswer) ,
     m_RightAnswer(righAnswer) {

} 

bool CTrueFalseQuestion::CheckAnswer(int &points, std::map<int,int> &answers) {
    m_Printed = true; 
    int answer = m_UserAnswer ? 1 :2;
    answers.insert(pair<int, int> (m_ID, answer));
    if(m_RightAnswer == m_UserAnswer){
       
        points += m_Points;
        return true;
    }
    return false;
}

void CTrueFalseQuestion::ReadAnswer(ostream& os, istream& is) {
    string input;
    bool read=true;
    while(read){
        os << "Napiš ANO nebo NE  stiskni Enter"<<endl;
        is >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        if(input == "ano" || input == "a") {
            m_UserAnswer = true;
            break;
        }
        else if(input == "ne" || input == "n") {
            m_UserAnswer = false;
            break;
        }
    }
}

void CTrueFalseQuestion::PrintQuestion(ostream &os) const {
    os << m_QuestionText <<endl;
}

void CTrueFalseQuestion::PrintAllAboutQuestion(ostream &os) const {
    os << "ID:" <<m_ID << " Text: " << m_QuestionText << endl;
    os << "\t 1) ANO" << endl  << "\t 2) NE"<<endl;
    os << endl;
}

shared_ptr<CBaseQuestion> CTrueFalseQuestion::Clone ( ) const {
  return shared_ptr<CBaseQuestion>(new CTrueFalseQuestion(*this));
}

CTrueFalseQuestion CTrueFalseQuestion::PrintQuestionBuilder(ostream &os, istream &is, CQuizManager &quiz) {
    string questionText, input;
    bool answer=true, dependOnLastAnswer=false, read=true;
    int page, points, lastquestionID, lastquestionAnswer; //answerID;
    os<< "Zadej text otázky"<<endl;
    std::getline(is >> std::ws, questionText);
     while(read){
        os << "Zadej zda je správná odpověď ANO nebo NE  (A/N)" <<endl;
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        std::getline(is >> std::ws, input);
        if(input == "ano" || input =="a"){
            answer = true;
            break;
        }
        else if (input=="ne" || input =="n"){
            answer = false; 
            break;
        } 
    }
    os << "Počet bodů za správnou odpověď"<<endl;
    is >> points;
    os << "Na jaké stránce kvízu se má otázka nacházet?"<<endl;
    is >> page;
    if(page > 1){
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
            return CTrueFalseQuestion(questionText, answer, page, quiz.GetQuizID(), points, dependOnLastAnswer, lastquestionID, lastquestionAnswer);
        }
    }

    return CTrueFalseQuestion(questionText, answer, page, quiz.GetQuizID(), points);
}

string CTrueFalseQuestion::ToStringCSV()const {
    string s = to_string(m_QuizID)+";"+ to_string(m_QuestionType) +";" + to_string(m_ID) + ";" + m_QuestionText + ";"+ to_string(m_RightAnswer)
            +";" +  to_string(m_Points) + ";" + to_string(m_Page) + ";" + to_string(m_DependOnLastAnswer) + ";" +
               to_string(m_LastQuestionID) + ";" +  to_string(m_LastQuestionAnswer);
    return s;
}

string CTrueFalseQuestion::ToPrintTXT() const {
    string s = m_QuestionText + "\n\t  1) ANO \n \t  2) NE\n\n\n";
    return s;
}

string CTrueFalseQuestion::ToPrintResultsTXT() const {
    string s = m_QuestionText + "\n\t";
    if(m_UserAnswer)
        s+="X  1) ANO \n \t   2) NE\n\n\n";
    else
        s+="   1) ANO \n \tX  2) NE\n\n\n";

    return s;
}

shared_ptr<CBaseQuestion> CTrueFalseQuestion::CreateQuestionFromCSV(string questionCSV) {
    string questionText, righAnswer, ID, page,  quizID, points,questionType, dependOnLastAnswer,lastQuestID, lastAnswer;
    bool answer, onlastAnswer;
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
        
        answer = righAnswer == "0" ? false : true;  
        onlastAnswer = dependOnLastAnswer == "0" ? false : true;  
        CTrueFalseQuestion question = CTrueFalseQuestion(questionText, answer,stoi(ID),stoi(page), stoi(quizID),stoi(points),onlastAnswer, stoi(lastQuestID), stoi(lastAnswer));
        return question.Clone();
    }
    catch( const std::invalid_argument& e ) {
        cout << "Chyba v souboru" << endl;
        throw;
    }
    
    
}


CTrueFalseQuestion::~CTrueFalseQuestion() {

}
