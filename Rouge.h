#include<string>
#include <vector>

#pragma once

class CRouge
{
public:
	CRouge(void);
	~CRouge(void);

	double Rouge_N(std::wstring wstrCandidate,std::vector<std::wstring> vecReference, int nGram=1,BOOL bSeg=FALSE);
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
	void GetWLCS(long &wlcs,long &mR,long &nC,std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	void GetSKIP2(long &skip2,long &mR,long &nC,std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	void PrintLCS(std::vector<std::wstring> vectorWords,int nRow,int nColumn,std::vector<std::wstring> &vectorLCS);
	int f(int k) {return k*k;}
	double inversef(double k){ return pow(k,0.5);}

private:
	int **pTable,**prev,**pweight;
};

