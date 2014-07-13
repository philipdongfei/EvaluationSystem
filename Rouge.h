#include<string>
#include <vector>

#pragma once

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
private:
	int **pTable,**prev,**pweight,**pTable0;
	double    m_dbScoreN1,m_dbScoreN2,m_dbScoreN3,m_dbScoreSN1,m_dbScoreON1;//rouge_n score
	double    m_dbScoreL,m_dbScoreNPL,m_dbScoreW,m_dbTScoreNPL,m_dbTScoreW;
	double    m_dbScoreS,m_dbScoreSU,m_dbTScoreSU;
};

