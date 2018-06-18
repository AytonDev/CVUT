//
// Created by Lukáš Frajt on 13/05/2018.
//

#include "fileManagerCSV.h"

#define  PATH_TO_QUIZZES "quizzes/"
#define  PATH_TO_QUESTIONS "questions/"
CFileMangerCSV::CFileMangerCSV(string queizes, string delimeter) : CFileManager(queizes), m_Delimeter(delimeter){

}

void CFileMangerCSV::SaveQuiz(const CQuizManager& quiz) const{
    string path = PATH_TO_FILE; 
	path +=PATH_TO_QUIZZES;
	cout << path << endl;
	fstream file;
	// Open the file in truncate mode if first line else in Append Mode
	file.open(path + m_QuizesFileName, std::ios::out |  std::ios::app);
 
	// Iterate over the range and add each lement to file seperated by delimeter.
    file << quiz.ToStringCSV();
	file << "\n";
	//m_LinesCount++;
 
	// Close the file
	file.close();

}

bool CFileMangerCSV::LoadQuizzes(vector<CQuizManager>& quizzes ) const {
	string path = PATH_TO_FILE; 
	path +=PATH_TO_QUIZZES;
	ifstream file(path + m_QuizesFileName);
	string name,desc,id, lastQuestionID, fileName;
	// Check if object is valid
	while (getline(file, name, ';'))
	{
		getline(file, desc, ';');
		getline(file, id, ';');
		getline(file, fileName);
		CQuizManager quiz = CQuizManager(name, stoi(id),desc, fileName);
		quizzes.push_back(quiz);
	}
	// Close the File
	file.close();
	return true;

}

void CFileMangerCSV::SaveQuestion(const CBaseQuestion& question, string fileName) const{
    string path = PATH_TO_FILE; 
	path +=PATH_TO_QUESTIONS;
	fstream file;
	// Open the file in truncate mode if first line else in Append Mode
	file.open(path+fileName, std::ios::out |  std::ios::app);
 
	// Iterate over the range and add each lement to file seperated by delimeter.
    file << question.ToStringCSV();
	cout <<  question.ToStringCSV() <<endl;
	file << "\n";
	//m_LinesCount++;
 
	// Close the file
	file.close();

}

bool CFileMangerCSV::LoadQuestions(string fileName, CQuizManager & quiz ) const {
	string path = PATH_TO_FILE; 
	path +=PATH_TO_QUESTIONS;
	quiz.ClearQuestions();
	ifstream file(path+fileName);
	string original,question, quizId,questionType ;
	// Open the file in truncate mode if first line else in Append Mode
	try {
		while(getline(file,question)){
			original = question;
			quizId = question.substr(0, question.find(";"));
			questionType = question.substr(0, question.erase(0, quizId.length()+1).find(";"));
			if(stoi(questionType) == TEXT_TYPE)
				quiz.AddQuestion( *CTextQuestion::CreateQuestionFromCSV(original));
			else if(stoi(questionType) == MULTI_TEXT_TYPE) {
				quiz.AddQuestion(*CMultiTextAnswerQuestion::CreateQuestionFromCSV(original));
			}
			else if(stoi(questionType) == TRUE_FALSE_TYPE){
				quiz.AddQuestion(*CTrueFalseQuestion::CreateQuestionFromCSV(original));
			}
			else if(stoi(questionType) == ORDER_TYPE) {
				quiz.AddQuestion(*COrderQuestion::CreateQuestionFromCSV(original));
			}
			else if(stoi(questionType) == PICK_ONE_TYPE){
				quiz.AddQuestion(*CPickOneQuestion::CreateQuestionFromCSV(original));

			}else if(stoi(questionType) == PICK_MANY_TYPE){
				quiz.AddQuestion(*CPickManyQuestion::CreateQuestionFromCSV(original));

			}
		}	
		file.close();
	}
	catch(...){
		return false;
	}
	return true;
}
