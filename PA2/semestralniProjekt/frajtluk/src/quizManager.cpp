//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "quizManager.h"



CQuizManager::CQuizManager(string name, int ID, string desc,string fileName): m_Name(name), m_ID(ID), m_Desc(desc), m_QuestionsFileName(fileName) {

}


CQuizManager::CQuizManager(string name, string desc): m_Name(name), m_Desc(desc)
 {
     srand(time(NULL));
     m_ID =rand() % 100000;
     m_QuestionsFileName = m_Name+ to_string(m_ID)+ CSV;
}

void CQuizManager::PrintQuiz(ostream &os) const {
    os << m_Name <<" - " << m_Desc <<endl;
}

 std::ostream& CQuizManager::PrintAll(ostream &os) const {
    unsigned int count = m_Questions.size();
    os << m_Name <<" - " << m_Desc  << " ID:" << m_ID<<endl;

    for(unsigned int i=0; i < count;++i){
         m_Questions[i]->PrintAllAboutQuestion(os);
    }

    os<< endl;

    return os;
}

bool CQuizManager::AddQuestion(const CBaseQuestion &question) {
    shared_ptr<CBaseQuestion> newQuestion = question.Clone();
    m_Questions.push_back(newQuestion);
    return true;

}


string CQuizManager::ToStringCSV() const {
    string output =  m_Name +";" + m_Desc +";"+to_string(m_ID) + ";"+m_QuestionsFileName;
    return output;
}

ofstream& CQuizManager::SaveToPrint(ofstream &outfile, bool results) const {
    for(unsigned int i =0; i < m_Questions.size(); ++i){
        if(results && m_Questions[i]->GetPrintStatus())
            outfile <<"  " <<i+1 << ") " <<m_Questions[i]->ToPrintResultsTXT();
        else if(!results)
            outfile <<"  " <<i+1 << ") " <<m_Questions[i]->ToPrintTXT();

    }
    return outfile;
}

int CQuizManager::GetQuizID() {
    return m_ID;
}

string CQuizManager::GetQuestionsFile() {
    return m_QuestionsFileName;
}

string CQuizManager::GetQuizName() {
    return m_Name;
}

void CQuizManager::GetQuestionsOnPage(vector<shared_ptr<CBaseQuestion> >& questions, int page, std::map<int,int>& answers, int &maxPages) const{
    int maxPage =0;
    for(unsigned int i=0; i < m_Questions.size(); ++i){
        if(maxPages < m_Questions[i]->GetPage())
            maxPages = m_Questions[i]->GetPage(); 
        if(m_Questions[i]->GetPage() == page){
            if(!m_Questions[i]->GetDependency())
                questions.push_back(m_Questions[i]);
            else if(m_Questions[i]->GetDependency() && m_Questions[i]->CheckDependency(answers))
                questions.push_back(m_Questions[i]); 
        }  
    }
}

void CQuizManager::ClearQuestions() {
    /*for(auto i = m_Questions.begin(); i !=m_Questions.end();++i)
    { 
            delete *i;
    }*/
    m_Questions.clear();
}

CQuizManager::~CQuizManager()
{
	/*for(auto i = m_Questions.begin(); i !=m_Questions.end();++i)
    { 
        delete *i;
    }*/
}
