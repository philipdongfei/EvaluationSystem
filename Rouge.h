#include<string>
#include <vector>
#include "Similarity.h"

#pragma once

class WordFrequency
{
public:
	WordFrequency()
	{
		wstrWord.empty();
		nFreqCandidate = 0;
		nFreqReference = 0;
		dbWCandidate = 0.0;
		dbWReference = 0.0;
	}
	std::wstring  wstrWord;
	int   nFreqCandidate;//∫Ú—°æ‰÷–¥ ∆µ
	int   nFreqReference;//≤Œøºæ‰÷–¥ ∆µ
	double dbWCandidate;//weight word in the candiate sentence
	double dbWReference;//weight word in the reference sentence
	bool operator==(const WordFrequency& c) const
	{
		return c.wstrWord == wstrWord;
	}
};

class CRouge
{
public:
	CRouge(void);
	~CRouge(void);

	double Rouge_N(std::wstring wstrCandidate,std::vector<std::wstring> vecReference, int nGram=1,BOOL bSeg=FALSE);
	void Rouge_N_All(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg=FALSE);//2014.07.08
	double Rouge_L(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg=FALSE);
	double NormalizedPairwiseLCS(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg=FALSE);
	double Rouge_W(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg=FALSE);
	double Rouge_S(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg=FALSE);
	double ComputeFlcs_Sentence(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	double ComputeFlcs_Summary(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	double ComputeFlcs_NormalizedPairwiseLCS(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	double ComputeFwlcs(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	double ComputeFskip2(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	std::vector<std::wstring> GetLCS(double &lcs,double &mR,double &nC,std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	void GetWLCS(double &wlcs,double &mR,double &nC,std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	void GetSKIP2(long &skip2,long &mR,long &nC,std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	std::vector<std::wstring> GetWLLCS(double &lcs,double &mR,double &nC,double &wlcs,
		std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	void PrintLCS(std::vector<std::wstring> vectorWords,int nRow,int nColumn,std::vector<std::wstring> &vectorLCS);
	int f(int k) {return k*k;}
	double inversef(double k){ return pow(k,0.5);}
	void   GetScroeNAll(double &s1,double &s2, double &s3) { s1 = m_dbScoreN1; s2 = m_dbScoreN2; s3 = m_dbScoreN3;}
	void   GetScoreLAll(double &sL,double &sNPL,double &sW) { sL = m_dbScoreL; sNPL = m_dbScoreNPL;sW = m_dbScoreW;}
	void   GetScoreSSU(double &sS,double &sSU) {sS = m_dbScoreS; sSU = m_dbScoreSU;}
	void   GetScoreC(double &c2, double &c3) {c2 = m_dbScoreC2; c3 = m_dbScoreC3;}
	void   GetScoreNgram(double &N2, double &N3, double &N4){N2 = m_db2gram; N3 = m_db3gram; N4 = m_db4gram;}
	void   GetScoreRL(double &RL, double &RNPL, double &RW){RL = m_dbScoreRL; RNPL = m_dbScoreRNPL; RW =m_dbScoreRW; }
	bool   IsPunctuation(std::wstring  ch);// is punctuation?
	double GetSimCos(){return m_dbSimCos;}
private:
	int **pTable,**prev,**pweight,**pTable0;
	double    m_dbScoreN1,m_dbScoreN2,m_dbScoreN3,m_dbScoreN4,m_dbScoreSN1,m_dbScoreON1;//rouge_n score
	double    m_dbScoreL,m_dbScoreNPL,m_dbScoreW,m_dbTScoreNPL,m_dbTScoreW;
	double    m_dbScoreS,m_dbScoreSU,m_dbTScoreSU;
	double    m_dbScoreC2,m_dbScoreC3;
	double    m_db2gram,m_db3gram,m_db4gram;
	double    m_dbScoreRL,m_dbScoreRNPL,m_dbScoreRW;
	Similarity    m_Similarity;
	std::vector<WordFrequency>   m_vectorCosine,m_vectorFrequencyN2,m_vectorFrequencyN3,m_vectorFrequencyN4;
	std::vector<std::wstring>         m_vectorN1,m_vectorN2,m_vectorN3,m_vectorN4,m_vectorLCS,m_vectorSKIP2;
	double m_dbScoreN1TF,m_dbScoreN2TF,m_dbScoreN3TF,m_dbScoreN4TF;
	double  m_RefTF,m_CanTF;//

	double    m_dbSimCos;  //similar cosine
};

