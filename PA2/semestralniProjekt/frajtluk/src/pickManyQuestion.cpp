//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "pickManyQuestion.h"



CPickManyQuestion::CPickManyQuestion(string questionText,vector<string>& answerList, vector<int>& righAnswer, int ID,int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer): CBaseQuestion(PICK_MANY_TYPE,questionText,ID,page, quizID, points,dependOnLastAnswer, lastQuestionID, lastQuestionAnswer) ,
    m_AnswersList(answerList), m_RightAnswer(righAnswer) {
} 

CPickManyQuestion::CPickManyQuestion(string questionText,vector<string>& answerList, vector<int>& righAnswer, int page, int quizID, int points,bool dependOnLastAnswer,int lastQuestionID, 
    int lastQuestionAnswer): CBaseQuestion(PICK_MANY_TYPE,questionText,page, quizID, points,dependOnLastAnswer, lastQuestionID, lastQuestionAnswer) ,
    m_AnswersList(answerList), m_RightAnswer(righAnswer) {
}


bool CPickManyQuestion::CheckAnswer(int &points, std::map<int,int> &answers) {
    m_Printed = true;
    if(Compare(m_RightAnswer, m_UserAnswer)){
        points += m_Points;
        return true;
    }
    answers.insert(pair<int,int> (m_ID, 0));
    
    return false;
}

void CPickManyQuestion::ReadAnswer(ostream &os, istream &is) {
    int input;
    os << "Napiš čísla správných odpovědí a stiskni Enter"<<endl;
    while (is >> input ){
        m_UserAnswer.push_back(input);
        if(is.get() == '\n')
                break;
    }
}

void CPickManyQuestion::PrintQuestion(ostream &os) const {
    os << m_QuestionText <<endl;
    for (unsigned int i=0; i < m_AnswersList.size(); ++i){
        cout << i+1 <<". " << m_AnswersList[i]<<endl;
    }
}

void CPickManyQuestion::PrintAllAboutQuestion(ostream &os) const {
    os << "ID:" <<m_ID << " Text: " <<m_QuestionText << endl;
    for(unsigned int i =0; i < m_AnswersList.size(); ++i){
         os << "\t"  << i+1 <<") " << m_AnswersList[i] << endl;
    }
    os << endl;
}

bool CPickManyQuestion::Compare(vector<int> &v1, vector<int> &v2) const
{
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    return v1 == v2;
}

bool CPickManyQuestion::ContaisAnswer(const vector <int> &v1, int number) const{
    if(std::find(v1.begin(), v1.end(), number) != v1.end()) 
    /* v contains x */
        return true;
    else 
        return false;
    /* v does not contain x */
}

shared_ptr<CBaseQuestion> CPickManyQuestion::Clone() const {
    return shared_ptr<CBaseQuestion>( new CPickManyQuestion(*this));
}

CPickManyQuestion CPickManyQuestion::PrintQuestionBuilder(ostream &os, istream &is, CQuizManager &quiz) {
    string questionText, input;
    vector<string> answerList;
    int page, points, rightAnswer, lastquestionID, lastquestionAnswer;  //answerID;
    vector<int> rightAnswers;
     bool dependOnLastAnswer=false;
    os<< "Zadejte text otázky"<<endl;
    std::getline(is >> std::ws, questionText);
    for(int i =0; i < LIST_COUNT; ++i){
        os << "Zadejte možnost odpovědi " << i+1  << "/4"  <<endl;
        std::getline(is >> std::ws, input);
        answerList.push_back(input);
    }
    do {
        os << "Zadejte číslo správné odpovědi"<< endl;
        is >> rightAnswer;
        rightAnswers.push_back(rightAnswer);
        os << "Chcete zadat další spravnou odpověď A/N" <<endl;
        std::getline(is >> std::ws, input);
    }
    while(input =="A" || input=="a");
    os << "Zadejte počet bodů za správnou odpověď"<<endl;
    is >> points;
    os << "Zadejte na jaké stránce kvízu se má otázka nacházet?"<<endl;
    is >> page;
    if(page> 1){
        os << "Má být otázka závislá na předchozí odpovědi (otázka se zobrazí, jen pokud uživatel zadal na nějaké předchozí strance  určitou odpověd)"<<endl;
       getline(is >> std::ws, input);
        if(input == "ANO" || input=="ano" || input =="A" || input =="a")
            dependOnLastAnswer = true;
        else if (input == "NE" || input=="ne" || input =="N" || input =="n")
            dependOnLastAnswer = false;
        if(dependOnLastAnswer){
            os<<"Zadejte číslo (ID) otázky, na které má být tato otázka závislá"<<endl;
            is >> lastquestionID;
            os << "Zadejte index odpovědi dané otázky"<<endl;
            is >> lastquestionAnswer;
            return CPickManyQuestion(questionText,answerList, rightAnswers, page, quiz.GetQuizID(), points, dependOnLastAnswer, lastquestionID, lastquestionAnswer);
        }
    }

    return CPickManyQuestion(questionText,answerList, rightAnswers, page, quiz.GetQuizID(), points);
     
}

string CPickManyQuestion::ToStringCSV() const  {
    string s = to_string(m_QuizID)+";"+ to_string(m_QuestionType) +";" + to_string(m_ID) + ";"  + m_QuestionText + ";"+ to_string
        (m_AnswersList.size()) +";";
    for(unsigned int i=0; i < m_AnswersList.size();++i){
        s+= m_AnswersList[i] + ";";
    }
    s+=to_string(m_RightAnswer.size())+";";
    for(unsigned int i=0; i < m_RightAnswer.size();++i){
        s+= to_string(m_RightAnswer[i]) + ";";
    }
    s+=  to_string(m_Points) + ";" + to_string(m_Page) + ";" + to_string(m_DependOnLastAnswer) + ";" + 
    to_string(m_LastQuestionID) + ";" + to_string(m_LastQuestionAnswer); 
    return s;
}

string CPickManyQuestion::ToPrintTXT() const {
    string s = m_QuestionText + " (můžete zaškrtnout více odpovědí)\n";
    for(unsigned int i = 0;i < m_AnswersList.size(); ++i){
        s += "\t  "+ to_string(i+1) + ") " + m_AnswersList[i] + "\n";
    }
    s+="\n\n";
    return s;
}

string CPickManyQuestion::ToPrintResultsTXT() const {
    string s = m_QuestionText + " (můžete zaškrtnout více odpovědí)\n";
    for(unsigned int i = 0;i < m_AnswersList.size(); ++i){
        if(ContaisAnswer(m_UserAnswer, i+1))
            s += "\tX  "+ to_string(i+1) + ") " + m_AnswersList[i] + "\n";
        else 
            s += "\t   "+ to_string(i+1) + ") " + m_AnswersList[i] + "\n";    
    }
    s+="\n\n";
    return s;
}

shared_ptr<CBaseQuestion> CPickManyQuestion::CreateQuestionFromCSV(string questionCSV) {
    string questionText, countOfAnswers, answer, ID, page,  quizID, points,questionType,dependOnLastAnswer, lastQuestID, lastAnswer;
    vector<string> answerList;
    vector<int> rightAnswers;
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
        countOfAnswers = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
        answer = countOfAnswers;
        for(int i=0; i < stoi(countOfAnswers); ++i){
            answer = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
            rightAnswers.push_back(stoi(answer));
        }
        points = questionCSV.substr(0, questionCSV.erase(0, answer.length()+1).find(";"));
        page = questionCSV.substr(0, questionCSV.erase(0, points.length()+1).find(";"));
        dependOnLastAnswer = questionCSV.substr(0, questionCSV.erase(0, page.length()+1).find(";"));
        lastQuestID = questionCSV.substr(0, questionCSV.erase(0, dependOnLastAnswer.length()+1).find(";"));
        lastAnswer = questionCSV.substr(0, questionCSV.erase(0, lastQuestID.length()+1).find(";"));
        onLastAnswer = dependOnLastAnswer == "0" ? false : true; 

        CPickManyQuestion question = CPickManyQuestion(questionText,answerList,rightAnswers,stoi(ID),stoi(page), stoi(quizID),stoi(points),onLastAnswer,stoi(lastQuestID), stoi(lastAnswer));
        return question.Clone();
    }
    catch( const std::invalid_argument& e ) {
        cout << "Chyba v souboru" << endl;
        throw;
    }
}

CPickManyQuestion::~CPickManyQuestion() {
}
