//
// Created by Lukáš Frajt on 13/05/2018.
//

#include <fstream>
#include <iostream>
#include "fileManagerTXT.h"


CFileManagerTXT::CFileManagerTXT(string fileName) : m_FileName(fileName+TXT) {

}
void CFileManagerTXT::SaveQuiz(const CQuizManager& quiz, bool results, CPlayer *player, int points) const {
    ofstream outfile;
    string path = PATH_TO_FILE;
    path += PATH_PRINT;
    outfile.open(path+m_FileName, ios::out | ios::trunc );
    outfile << endl<<endl;
    if(results){
        outfile << "  Jméno: " <<player->GetPlayerName()<<endl;
        outfile << "  Počet bodů: " <<points<<endl<<endl;
        quiz.SaveToPrint(outfile, results);
    }
        
    else
        quiz.SaveToPrint(outfile);
    outfile.close();
}
