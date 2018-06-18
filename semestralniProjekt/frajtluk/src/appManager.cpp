//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "appManager.h"


CAppManager::CAppManager() : m_CSVManager(CFileMangerCSV("quizzes.csv",";")) {
    m_MainMenu.push_back("Nová hra");m_MainMenu.push_back("Spravovat Kvízy");m_MainMenu.push_back("Export");m_MainMenu.push_back("Konec");  
    m_QuizManager.push_back("Seznam Kvízů");m_QuizManager.push_back("Přidat otázku do kvízu");m_QuizManager.push_back("Přidat Kvíz");m_QuizManager.push_back("Zpět");
    m_QuestionTypes.push_back("Textová odpověď (jedna správná odpověď)");m_QuestionTypes.push_back("Textová odpověď (více spravných odpovědí)");
    m_QuestionTypes.push_back("ANO/NE otázka");m_QuestionTypes.push_back("Seřaďte texty ve správném pořadí");m_QuestionTypes.push_back( "Vyberte právě jednu správnou odpověď");
    m_QuestionTypes.push_back( "Vyberte jednu nebo více správných odpovědí");m_QuestionTypes.push_back("Zpět");
}

void  CAppManager::PrintMain(ostream &os, istream &is) {
    os.flush(); // Flush the output stream
    system("clear"); // Clear the console with the "system" function
    os <<APPNAME<<endl;
    for(unsigned int i =0; i < m_MainMenu.size(); ++i){
        os << i+1 << ". " << m_MainMenu[i] <<endl;
    }
    int input; 
    do { 
        is >> input;
        if(input == 1){
             PrintNewGame(os,is);
             break;
        } 
        else if(input == 2){
            PrintQuizManager(os,is);
            break;
        }      
        else if(input == 3) {
            PrintExportManager(os,is);
            break;
        }     
        else if(input == 4) {
            return; 
        }  
        else
            os << "Zadejte platné číslo" << endl;        
    }while(true);

    PrintMain(os,is); 
 }

 //QUIZ MANAGER
void  CAppManager::PrintQuizManager(ostream &os, istream &is) {
    m_Quizes.clear();
    m_CSVManager.LoadQuizzes(m_Quizes);
    os.flush(); // Flush the output stream
    system("clear"); // Clear the console with the "system" function
    os <<APPNAME<<endl;
    for(unsigned int i =0; i < m_QuizManager.size(); ++i){
       os << i+1 << ". " << m_QuizManager[i] <<endl;
    }
    int input;
    do{    
        is >> input;
        if(input == 1){
            PrintQuizzes(os);
            is.ignore();
            is.get();
            break;
        }
        else if(input ==2) {
            PrintQuestionBuilder(os,is);
            break;
        }     
        else if(input == 3){
            PrintQuizBuilder(os,is);
            break;
        }      
        else if(input == 4)
            return;
        else 
            os << "Zadejte platné číslo" << endl;            
    }while(true);

    PrintQuizManager(os,is);
}

void CAppManager::PrintQuizBuilder(ostream &os, istream &is) {
    os.flush(); // Flush the output stream
    system("clear"); // Clear the console with the "system" function
    string name, desc,input;
    os<< "Napište název kvízu"<<endl<< "Název: ";
    std::getline(is >> std::ws, name);
    os<< "Napište popis kvízu"<<endl<< "Popis: ";
    std::getline(is >> std::ws, desc);
    os<< "Pro potvrzení vytvoření kvízu napište A, pro zrušení napište N"<<endl<<"Název: " << name<<endl <<"Popis:" <<desc<<endl;
    is>>input;
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    if(input == "ano" || input =="a"){
        CQuizManager quiz = CQuizManager(name, desc);
        m_CSVManager.SaveQuiz(quiz);
        os << "Kvíz vytvořen"<<endl;
        is.ignore();
        is.get();
    }else {
        PrintQuizManager(os,is);
        return;
    }
    return;
 }

void CAppManager::PrintQuizzes(ostream &os) {
    for(unsigned int i =0; i < m_Quizes.size(); ++i){
       os << i+1 <<") " << m_Quizes[i];
    }
    return;
}

void CAppManager::PrintQuestionBuilder(ostream &os, istream &is) {
    unsigned int quizID, input, i, size = m_Quizes.size();
    os<<"Vyberte kvíz do kterého chcete přidat otázku:"<<endl;
    for(i =0; i < size; ++i){
       os << i+1 << ") " <<m_Quizes[i];
    }
    os << i+1 << ") " <<"Zpět" <<endl;
    do {  
        is >> quizID;
        --quizID;
        if(quizID == size)
            return;
        if(quizID >= size)
            os << "Zadejte platné číslo" << endl;
        else 
            break;    
    }while(true);
    
    os.flush(); // Flush the output stream
    system("clear"); // Clear the console with the "system" function

    if(!m_CSVManager.LoadQuestions(m_Quizes[quizID].GetQuestionsFile(), m_Quizes[quizID]))
        return;
    m_Quizes[quizID].PrintAll(os);
    for(unsigned int i =0; i < m_QuestionTypes.size(); ++i){
        os << i+1 << ". " << m_QuestionTypes[i] <<endl;
    }
    while(true){
        is >> input;
        if(input == TEXT_TYPE){
            CTextQuestion question = CTextQuestion::PrintQuestionBuilder(os,is, m_Quizes[quizID]);
            m_CSVManager.SaveQuestion(question, m_Quizes[quizID].GetQuestionsFile());
            break;
        }
        else if(input == MULTI_TEXT_TYPE){
            CMultiTextAnswerQuestion question2 = CMultiTextAnswerQuestion::PrintQuestionBuilder(os,is,  m_Quizes[quizID]);
            m_CSVManager.SaveQuestion(question2, m_Quizes[quizID].GetQuestionsFile());
            break;
        }
        else if(input == TRUE_FALSE_TYPE){
            CTrueFalseQuestion question3 = CTrueFalseQuestion::PrintQuestionBuilder(os,is, m_Quizes[quizID]);
            m_CSVManager.SaveQuestion(question3, m_Quizes[quizID].GetQuestionsFile());
            break;
        }
        else if(input == ORDER_TYPE){
            COrderQuestion question4 = COrderQuestion::PrintQuestionBuilder(os,is, m_Quizes[quizID]);
            m_CSVManager.SaveQuestion(question4, m_Quizes[quizID].GetQuestionsFile());
            break;
        }
        else if(input == PICK_ONE_TYPE){
            CPickOneQuestion question5 = CPickOneQuestion::PrintQuestionBuilder(os,is, m_Quizes[quizID]);
            m_CSVManager.SaveQuestion(question5, m_Quizes[quizID].GetQuestionsFile());
            break;
        }
        else if(input == PICK_MANY_TYPE){
            CPickManyQuestion question6 = CPickManyQuestion::PrintQuestionBuilder(os,is, m_Quizes[quizID]);
            m_CSVManager.SaveQuestion(question6, m_Quizes[quizID].GetQuestionsFile());
            break;
        }
        else if(input == 7) {
            return;
        }
        else
            os << "Zadej platné čislo"<<endl;
    }
    return;
}

//GAME
void CAppManager::PrintNewGame(ostream &os, istream &is) {
    m_Quizes.clear();
    m_CSVManager.LoadQuizzes(m_Quizes);
    CQuizManager* selectedQuiz;
    os <<"Vyberte si jeden z kvízů:"<<endl;
    unsigned int quizNumber;
    unsigned int i;
    for(i =0; i < m_Quizes.size(); ++i){
       os << i+1 << ") " <<m_Quizes[i];
    }
    do { 
        os << i+1 << ") " <<"Zpět" <<endl;
        is >> quizNumber;
        --quizNumber;
        if(quizNumber == m_Quizes.size())
            return;
        if(quizNumber >= m_Quizes.size())
            os << "Zadejte platné číslo" << endl;
        else 
            break;    
    }while(true);
    
    selectedQuiz = &m_Quizes[quizNumber];
    
    if(!m_CSVManager.LoadQuestions(selectedQuiz->GetQuestionsFile(), *selectedQuiz)){
        is.ignore();
        is.get();
        return;
    }
       
   // m_Quizes[quizNumber].PrintAll(os);
    os<<"Vybraný kvíz: " << *selectedQuiz;
    CPlayer player = CPlayer::PrintPlayerBuilder();
    CClassicGame game = CClassicGame(player, *selectedQuiz);
    game.StartGame(os,is);
    return ;

}

void CAppManager::PrintExportManager(ostream &os, istream &is) {
    CQuizManager* selectedQuiz;
    os <<"Vyberte kvíz, který chcete exportovat" << endl;
    unsigned int quizNumber;
    unsigned int i;
    for(i =0; i < m_Quizes.size(); ++i){
       os << i+1 << ") " <<m_Quizes[i];
    }
    do { 
        os << i+1 << ") " <<"Zpět" <<endl;
        is >> quizNumber;
        --quizNumber;
        if(quizNumber == m_Quizes.size())
            return;
        if(quizNumber >= m_Quizes.size())
            os << "Zadejte platné číslo" << endl;
        else 
            break;    
    }while(true);
    
    selectedQuiz = &m_Quizes[quizNumber];
    
    if(!m_CSVManager.LoadQuestions(selectedQuiz->GetQuestionsFile(), *selectedQuiz)){
        is.ignore();
        is.get();
        return;
    }
    string file = "print";
    os<< "Napište název souboru (výsledný soubor naleznete ve složce \"exports\""<<endl<< "Název: ";
    std::getline(is >> std::ws, file);
    CFileManagerTXT txtManager = CFileManagerTXT(file);
    txtManager.SaveQuiz(*selectedQuiz);

    return;
    
}
