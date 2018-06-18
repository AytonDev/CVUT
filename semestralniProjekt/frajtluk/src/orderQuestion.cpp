//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "orderQuestion.h"



COrderQuestion::COrderQuestion(string questionText,vector<string>& textsToOrder, vector<int> &rigthOrder, int ID,int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer): CBaseQuestion(ORDER_TYPE, questionText,ID,page, quizID, points,dependOnLastAnswer, lastQuestionID, lastQuestionAnswer) ,
    m_TextsToOrder(textsToOrder), m_Answer(rigthOrder) {

} 

COrderQuestion::COrderQuestion(string questionText,vector<string>& textsToOrder, vector<int> &rigthOrder,int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer): CBaseQuestion(ORDER_TYPE, questionText,page, quizID, points,dependOnLastAnswer, lastQuestionID, lastQuestionAnswer) ,
    m_TextsToOrder(textsToOrder), m_Answer(rigthOrder) {

} 

bool COrderQuestion::CheckAnswer(int &points, std::map<int,int> &answers) {
    m_Printed = true;
    if(m_Answer == m_UserAnswer){
        points += m_Points;
        return true;
    }
    answers.insert(pair<int,int> (m_ID, 0));
    return false;
}

void COrderQuestion::ReadAnswer(ostream& os, istream& is) {
    int input;
    os << "Odpověď napiš pořádí odpovědí a stiskni Enter"<<endl;
    while (is >> input ){
        m_UserAnswer.push_back(input);
        if(cin.get() == '\n')
                break;
    }
}


void COrderQuestion::PrintQuestion(ostream &os) const {
    os << m_QuestionText <<endl;
    for (unsigned int i=0; i < m_TextsToOrder.size(); ++i){
        cout << i+1 <<". " << m_TextsToOrder[i]<<endl;
    }
}

void COrderQuestion::PrintAllAboutQuestion(ostream &os) const {
    os << "ID:" << m_ID << " Text: " << m_QuestionText << endl;
    for(unsigned int i =0; i < m_TextsToOrder.size(); ++i){
         os << "\t"  << i+1 <<") " << m_TextsToOrder[i] << endl;
    } 
    os << endl;
}

shared_ptr<CBaseQuestion> COrderQuestion::Clone() const {
    return shared_ptr<CBaseQuestion>(new COrderQuestion(*this));
}

COrderQuestion COrderQuestion::PrintQuestionBuilder(ostream &os, istream &is, CQuizManager &quiz) {
    string questionText, input;
    vector<string> answerList;
    int page, points, rightAnswer, lastquestionID, lastquestionAnswer; //answerID;
    vector<int> rightAnswers;
    bool dependOnLastAnswer=false;
    os<< "Zadejte text otázky"<<endl;
    std::getline(is >> std::ws, questionText);
    for(int i =0; i < LIST_COUNT; ++i){
        os << "Zadejte text seřazení " << i+1  << "/4"  <<endl;
        std::getline(is >> std::ws, input);
        answerList.push_back(input);
    }
    for(int i =0; i < LIST_COUNT; ++i){
        os << "Zadejte číslo spravné odpovědi " << i+1  << "/4"  <<endl;
        is>> rightAnswer;
        rightAnswers.push_back(rightAnswer);
    }
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
            return COrderQuestion(questionText, answerList, rightAnswers, page, quiz.GetQuizID(), points, dependOnLastAnswer, lastquestionID, lastquestionAnswer);
        }
    }

    return COrderQuestion(questionText, answerList, rightAnswers, page, quiz.GetQuizID(), points);
}

string COrderQuestion::ToStringCSV() const{
    string s = to_string(m_QuizID)+";"+ to_string(m_QuestionType) + ";"+ to_string(m_ID)  +";" + m_QuestionText + ";"+ to_string
    (m_TextsToOrder.size()) +";";
    for(unsigned int i=0; i < m_TextsToOrder.size();++i){
        s+= m_TextsToOrder[i] + ";";
    }
    s+=to_string(m_Answer.size())+";";
    for(unsigned int i=0; i < m_Answer.size();++i){
        s+= to_string(m_Answer[i]) + ";";
    }
    s+=  to_string(m_Points) + ";" + to_string(m_Page) + ";" + to_string(m_DependOnLastAnswer) + ";" + 
    to_string(m_LastQuestionID) + ";" + to_string(m_LastQuestionAnswer);
    return s;
}

string COrderQuestion::ToPrintTXT() const {
    string s = m_QuestionText + "\n";
    for(unsigned int i = 0;i < m_TextsToOrder.size(); ++i){
        s += "\t    "+ to_string(i+1) + ") " + m_TextsToOrder[i] + "\n";
    }
    s+="\n";
    for(unsigned int i = 0;i < m_TextsToOrder.size(); ++i){
        s += "\t   "+ to_string(i+1) + ") __\n";
    }
    s+="\n\n";
    return s;
}

string COrderQuestion::ToPrintResultsTXT() const {
    string s = m_QuestionText + "\n";
    for(unsigned int i = 0;i < m_TextsToOrder.size(); ++i){
        s += "\t  "+ to_string(i+1) + ") " + m_TextsToOrder[i] + "\n";
    }
    s+="\n";
    for(unsigned int i = 0;i < m_UserAnswer.size(); ++i){
        s += "\t  "+ to_string(i+1) + ") "+ to_string(m_UserAnswer[i]) +"\n";
    }
    s+="\n\n";
    return s;
}

shared_ptr<CBaseQuestion> COrderQuestion::CreateQuestionFromCSV(string questionCSV) {
    string questionText, countOfAnswers, answer, ID, page,  quizID, points,questionType, dependOnLastAnswer, lastQuestID, lastAnswer;
    vector<string> textToOrder;
    vector<int> answers;
    bool onDependlast;
    try {
        quizID = questionCSV.substr(0, questionCSV.find(";"));
        questionType = questionCSV.substr(0, questionCSV.erase(0, quizID.length()+1).find(";"));
        ID = questionCSV.substr(0, questionCSV.erase(0, questionType.length()+1).find(";"));
        questionText = questionCSV.substr(0, questionCSV.erase(0, ID.length()+1).find(";"));
        countOfAnswers = questionCSV.substr(0, questionCSV.erase(0, questionText.length()+1).find(";"));
        answer = countOfAnswers;
        for(int i=0; i < stoi(countOfAnswers); ++i){
            answer = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
            textToOrder.push_back(answer);
            
        }
        countOfAnswers = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
        answer = countOfAnswers;
        for(int i=0; i < stoi(countOfAnswers); ++i){
            answer = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
            answers.push_back(stoi(answer));
        }
        points = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
        page = questionCSV.substr(0, questionCSV.erase(0, points.length()+1).find(";"));
        dependOnLastAnswer = questionCSV.substr(0, questionCSV.erase(0, page.length()+1).find(";"));
        lastQuestID = questionCSV.substr(0, questionCSV.erase(0, dependOnLastAnswer.length()+1).find(";"));
        lastAnswer = questionCSV.substr(0, questionCSV.erase(0, lastQuestID.length()+1).find(";"));
        
        onDependlast = dependOnLastAnswer == "0" ? false : true; 


        COrderQuestion question = COrderQuestion(questionText,textToOrder,answers,stoi(ID),stoi(page), stoi(quizID),stoi(points),onDependlast,stoi(lastQuestID), stoi(lastAnswer));
        return question.Clone();
    }
    catch( const std::invalid_argument& e ) {
        cout << "Chyba v souboru" << endl;
        throw;
    }
}

COrderQuestion::~COrderQuestion() {

}
