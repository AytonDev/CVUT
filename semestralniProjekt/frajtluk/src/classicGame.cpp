//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "classicGame.h"

CClassicGame::CClassicGame(const CPlayer& player, const CQuizManager& quiz): CBaseGame( quiz), m_Player(player), m_CurrentPage(0), m_MaxPage(1) {
   //TODO player points to zero 
}

void CClassicGame::StartGame(ostream& os, istream& is) {
    if(!NextPage())
        EndGame(os);
    else {
        QuestionsGenerator(os,is);
        StartGame(os,is);
    }     
       
}

void CClassicGame::QuestionsGenerator(ostream& os, istream& is) {
  
    system("clear");
    cout.flush();
    os << "Strana: " << m_CurrentPage << endl;
    os << "Aktualní počet bodů: "<< m_Points << endl;
    m_Quiz.GetQuestionsOnPage(m_PageQuestions, m_CurrentPage,m_Answers, m_MaxPage);
    os << "----------------"<<endl;
    os << "OTÁZKY:"<<endl;
    os << "----------------"<<endl<<endl;
    for(unsigned int i =0; i < m_PageQuestions.size(); i++){
        cout<< "otázka č."<< i+1<<" "<< *m_PageQuestions[i] <<endl;
    }
    os << "----------------"<<endl;
    os << "ODPOVĚDI:"<<endl;
    os << "----------------"<<endl;
    for(unsigned int i =0; i < m_PageQuestions.size(); i++){
        os<< "otázka č."<< i+1<< " "; 
        m_PageQuestions[i]->ReadAnswer(os, is);
        m_PageQuestions[i]->CheckAnswer(m_Points, m_Answers);
        os<<endl;
    }
    m_PageQuestions.clear();
}

bool CClassicGame::NextPage(){
    m_CurrentPage++;
    if(m_CurrentPage > m_MaxPage)
        return false;
   
    return true;    

}

void CClassicGame::EndGame(ostream& os){
    string input;
    system("clear");
    cout.flush();
    os << endl;
    os << "----------------"<<endl;
    os << "KONEC HRY"<<endl;
    os << "----------------"<<endl<<endl;
    os<< "Gratulujeme " << m_Player.GetPlayerName() << " získal jsi "<< m_Points << "bodů" <<endl;
    os<< "Chcete výsledek uložit do souboru?"<<endl;
    std::getline(cin >> std::ws, input);
    if(input == "ANO" || input=="ano" || input =="A" || input =="a") {
        os<< "Napište název souboru (výsledný soubor naleznete ve složce \"results\""<<endl<< "Název: ";
        std::getline(cin >> std::ws, input);
        CFileManagerTXT manager = CFileManagerTXT(input);
        manager.SaveQuiz(m_Quiz, true, &m_Player, m_Points);
    }
    else
        return;
   
}

CClassicGame::~CClassicGame() {
  /*  for(auto i = m_PageQuestions.begin(); i != m_PageQuestions.end(); i++){
    delete *i;
    }*/
}
