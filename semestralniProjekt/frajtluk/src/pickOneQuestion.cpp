//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "pickOneQuestion.h"




CPickOneQuestion::CPickOneQuestion(string questionText,vector<string>& answerList, int righAnswer, int ID,int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer): CBaseQuestion(PICK_ONE_TYPE, questionText,ID,page, quizID, points,dependOnLastAnswer, lastQuestionID, lastQuestionAnswer) ,
    m_AnswersList(answerList), m_RightAnswer(righAnswer) {

} 

CPickOneQuestion::CPickOneQuestion(string questionText,vector<string>& answerList, int righAnswer, int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer): CBaseQuestion(PICK_ONE_TYPE, questionText,page, quizID, points,dependOnLastAnswer, lastQuestionID, lastQuestionAnswer) ,
    m_AnswersList(answerList), m_RightAnswer(righAnswer) {

} 

bool CPickOneQuestion::CheckAnswer(int &points, std::map<int,int> &answers) {
    m_Printed = true;
    answers.insert(pair<int, int> (m_ID, m_UserAnswer));
    if(m_RightAnswer == m_UserAnswer){
        points += m_Points;
        return true;
    }
    return false;
}

void CPickOneQuestion::ReadAnswer(ostream& os, istream& is) {
    os << "Napiš číslo správné odpovědi a stiskni Enter"<<endl;
    is >> m_UserAnswer;
}

void CPickOneQuestion::PrintQuestion(ostream &os) const {
    os << m_QuestionText <<endl;
    for (unsigned int i=0; i < m_AnswersList.size(); ++i){
        cout << i+1 <<". " << m_AnswersList[i]<<endl;
    }
}

void CPickOneQuestion::PrintAllAboutQuestion(ostream &os) const {
    os << "ID:" <<m_ID << " Text: " << m_QuestionText << endl;
    for(unsigned int i =0; i < m_AnswersList.size(); ++i){
         os << "\t"  << i+1 <<") " << m_AnswersList[i] << endl;
    }
    os << endl; 
}


shared_ptr<CBaseQuestion> CPickOneQuestion::Clone() const {
    return shared_ptr<CBaseQuestion>( new CPickOneQuestion(*this));
}

CPickOneQuestion CPickOneQuestion::PrintQuestionBuilder(ostream &os, istream &is,CQuizManager &quiz) {
    string questionText, input;
    vector<string> answerList;
    int page, points, righAnswer, lastquestionID, lastquestionAnswer; //answerID;
    bool dependOnLastAnswer=false;
    os<< "Zadejte text otázky"<<endl;
    std::getline(is >> std::ws, questionText);
    for(int i =0; i < LIST_COUNT; ++i){
        os << "Zadejte možnost odpovědi " << i+1  << "/4"  <<endl;
        std::getline(is >> std::ws, input);
        answerList.push_back(input);
    }
    os << "Zadejte číslo správné odpovědi"<< endl;
    is >> righAnswer;
    os << "Zadejte počet bodů za správnou odpověď"<<endl;
    is >> points;
    os << "Zadejte na jaké stránce kvízu se má otázka nacházet?"<<endl;
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
            return CPickOneQuestion(questionText,answerList,righAnswer,page, quiz.GetQuizID(), points, dependOnLastAnswer, lastquestionID, lastquestionAnswer);
        }
    }
    return CPickOneQuestion(questionText,answerList,righAnswer,page, quiz.GetQuizID(), points); 
}

string CPickOneQuestion::ToStringCSV() const{
   string s = to_string(m_QuizID)+";"+ to_string(m_QuestionType) +";" + to_string(m_ID) + ";" + m_QuestionText + ";"+ to_string
    (m_AnswersList.size()) +";";
    for(unsigned int i=0; i < m_AnswersList.size();++i){
        s+= m_AnswersList[i] + ";";
    }
    s+=to_string(m_RightAnswer)+";"+ to_string(m_Points) + ";" + to_string(m_Page) + ";" + to_string(m_DependOnLastAnswer)+
            ";" + to_string(m_LastQuestionID) + ";" + to_string(m_LastQuestionAnswer);
    return s;
}

string CPickOneQuestion::ToPrintTXT() const {
    string s = m_QuestionText + " (zaškrtněte pouze jednu odpověď)\n";
    for(unsigned int i = 0;i < m_AnswersList.size(); ++i){
        s += "\t  "+ to_string(i+1) + ") " + m_AnswersList[i] + "\n";
    }
    s+="\n\n";
    return s;
}

string CPickOneQuestion::ToPrintResultsTXT() const {
    string s = m_QuestionText + " (zaškrtněte pouze jednu odpověď)\n";
    for(unsigned int i = 0;i < m_AnswersList.size(); ++i){
        if(m_UserAnswer == i+1)
            s += "\tX  "+ to_string(i+1) + ") " + m_AnswersList[i] + "\n";
        else
            s += "\t   "+ to_string(i+1) + ") " + m_AnswersList[i] + "\n";
    }
    s+="\n\n";
    return s;
}

shared_ptr<CBaseQuestion> CPickOneQuestion::CreateQuestionFromCSV(string questionCSV) {
    string questionText, countOfAnswers, answer, ID, page,  quizID, points,questionType, dependOnLastAnswer, lastQuestID, lastAnswer;
    vector<string> answerList;
    bool onLastAnswer;
    try {
        quizID = questionCSV.substr(0, questionCSV.find(";"));
        questionType = questionCSV.substr(0, questionCSV.erase(0, quizID.length()+1).find(";"));
        ID = questionCSV.substr(0, questionCSV.erase(0, questionType.length()+1).find(";"));

        questionText = questionCSV.substr(0, questionCSV.erase(0, ID.length()+1).find(";"));
        countOfAnswers = questionCSV.substr(0, questionCSV.erase(0, questionText.length()+1).find(";"));
        answer = countOfAnswers;
        for(int i=0; i < stoi(countOfAnswers); ++i){
            answer = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
            answerList.push_back(answer);
            
        }
        answer = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
        points = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
        page = questionCSV.substr(0, questionCSV.erase(0, points.length()+1).find(";"));
        dependOnLastAnswer = questionCSV.substr(0, questionCSV.erase(0, page.length()+1).find(";"));
        lastQuestID = questionCSV.substr(0, questionCSV.erase(0, dependOnLastAnswer.length()+1).find(";"));
        lastAnswer = questionCSV.substr(0, questionCSV.erase(0, lastQuestID.length()+1).find(";"));
        onLastAnswer = dependOnLastAnswer == "0" ? false : true; 
    
        CPickOneQuestion question = CPickOneQuestion(questionText,answerList,stoi(answer),stoi(ID),stoi(page), stoi(quizID),stoi(points),onLastAnswer,stoi(lastQuestID), stoi(lastAnswer));
        return question.Clone();
    }
    catch( const std::invalid_argument& e ) {
        cout << "Chyba v souboru" << endl;
        throw;
    }
}

CPickOneQuestion::~CPickOneQuestion() {

}
