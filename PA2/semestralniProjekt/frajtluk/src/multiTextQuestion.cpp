//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "multiTextQuestion.h"


CMultiTextAnswerQuestion::CMultiTextAnswerQuestion(string questionText,vector<string>& answers, int ID,int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer) :CBaseQuestion(MULTI_TEXT_TYPE, questionText,ID,page,quizID,points,dependOnLastAnswer,lastQuestionID,lastQuestionAnswer),
     m_Answers(answers) {

}

CMultiTextAnswerQuestion::CMultiTextAnswerQuestion(string questionText,vector<string>& answers,int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer) :CBaseQuestion(MULTI_TEXT_TYPE, questionText,page,quizID,points,dependOnLastAnswer,lastQuestionID,lastQuestionAnswer),
     m_Answers(answers) {

}

void CMultiTextAnswerQuestion::PrintQuestion(ostream &os) const {
     os << m_QuestionText << endl;
}

 void CMultiTextAnswerQuestion::PrintAllAboutQuestion(ostream &os) const {
    os << "ID:" << m_ID << " Text: " << m_QuestionText << endl;
    for(unsigned int i =0; i < m_Answers.size(); ++i){
         os << "\t"  << i+1 <<") " << m_Answers[i] << endl;
    } 
    os << endl;
}

void CMultiTextAnswerQuestion::ReadAnswer(ostream& os, istream& is) {
    os << "Napište odpověď a stiskněte enter" <<endl;
    std::getline(is>> std::ws, m_UserAnswer);
}

bool CMultiTextAnswerQuestion::CheckAnswer( int &points, std::map<int,int>& answers){
    vector<string>::iterator it;
    m_Printed = true;
    int answerID =0;
    if ((it = find(m_Answers.begin(), m_Answers.end(), m_UserAnswer)) != m_Answers.end())
    {
        ptrdiff_t index = distance(m_Answers.begin(), it)+1;
        answerID = (int)index;
        points += m_Points;
        return true;
    }
    answers.insert(pair <int, int>(m_ID, answerID));
    return false;

}


shared_ptr<CBaseQuestion> CMultiTextAnswerQuestion::Clone() const { 
    return  shared_ptr<CBaseQuestion>(new CMultiTextAnswerQuestion(*this));
}

CMultiTextAnswerQuestion CMultiTextAnswerQuestion::PrintQuestionBuilder(ostream &os, istream &is, CQuizManager& quiz) {
    string questionText, rightAnswer, input;
    vector<string> rightAnswers;
    int page, points, lastquestionID,lastquestionAnswer; //answerID;
    bool dependOnLastAnswer = false;
    os<< "Zadejte text otázky"<<endl;
    std::getline(std::cin >> std::ws, questionText);
    do {
        os << "Zadejte text správné odpovědi" <<endl;
        std::getline(std::cin >> std::ws, rightAnswer);
        rightAnswers.push_back(rightAnswer);
        os << "Chcete zadat další spravnou odpověď A/N" <<endl;
        std::getline(std::cin >> std::ws, input);
    }
    while(input =="A" || input=="a");

    os << "Zadejte počet bodů za správnou odpověď"<<endl;
    is >> points;
    os << "Zadejte na jaké stránce kvízu se má otázka nacházet?"<<endl;
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
            return CMultiTextAnswerQuestion(questionText,rightAnswers,page,quiz.GetQuizID(), points,dependOnLastAnswer, lastquestionID, lastquestionAnswer);
        }
    }
    return CMultiTextAnswerQuestion(questionText,rightAnswers,page,quiz.GetQuizID(), points);
}

string CMultiTextAnswerQuestion::ToStringCSV() const {
    string s = to_string(m_QuizID)+";"+ to_string(m_QuestionType)+ ";" + to_string(m_ID) +";" + m_QuestionText + "; "+ to_string
    (m_Answers.size()) +";";
    for(unsigned int i=0; i < m_Answers.size();++i){
        s+= m_Answers[i] + ";";
    }
    
    s+=  to_string(m_Points) + ";" + to_string(m_Page)+ ";" + to_string(m_DependOnLastAnswer) + ";" + to_string(m_LastQuestionID)
    + ";" + to_string(m_LastQuestionAnswer);
    return s;
}

string CMultiTextAnswerQuestion::ToPrintTXT() const {
    string s = m_QuestionText + "\n" + "\t  " + "Odpověď:  _____________\n\n\n";
    return s;
}

string CMultiTextAnswerQuestion::ToPrintResultsTXT() const {
    string s = m_QuestionText + "\n" + "\t  " + "Odpověď: "+ m_UserAnswer+"\n\n\n";
    return s;
}

CMultiTextAnswerQuestion::~CMultiTextAnswerQuestion() {

}

shared_ptr<CBaseQuestion> CMultiTextAnswerQuestion::CreateQuestionFromCSV(string questionCSV) {
    string questionText, countOfAnswers, answer, ID, page,  quizID, points,questionType, dependOn,lastQuestID, lastAnswer ;
    vector<string> answers;
    bool depend;
    try {
        quizID = questionCSV.substr(0, questionCSV.find(";"));
        questionType = questionCSV.substr(0, questionCSV.erase(0, quizID.length()+1).find(";"));
        ID = questionCSV.substr(0, questionCSV.erase(0, questionType.length()+1).find(";"));
        questionText = questionCSV.substr(0, questionCSV.erase(0, ID.length()+1).find(";"));
        countOfAnswers = questionCSV.substr(0, questionCSV.erase(0, questionText.length()+1).find(";"));
        answer = countOfAnswers;
        for(int i=0; i < stoi(countOfAnswers); ++i){
            answer = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
            answers.push_back(answer);
        }
        points = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
        page = questionCSV.substr(0, questionCSV.erase(0, points.length()+1).find(";"));
        dependOn = questionCSV.substr(0, questionCSV.erase(0, page.length()+1).find(";"));
        lastQuestID = questionCSV.substr(0, questionCSV.erase(0, dependOn.length()+1).find(";"));
        lastAnswer = questionCSV.substr(0, questionCSV.erase(0, lastQuestID.length()+1).find(";"));
        depend = dependOn == "0" ? false : true; 
        CMultiTextAnswerQuestion question = CMultiTextAnswerQuestion(questionText,answers,stoi(ID),stoi(page), stoi(quizID),stoi(points), depend, stoi(lastQuestID),stoi(lastAnswer));
        return question.Clone();
    }
    catch( const std::invalid_argument& e ) {
        cout << "Chyba v souboru" << endl;
        throw;
    }
}
