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
	double ComputeFlcs_Sentence(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	double ComputeFlcs_Summary(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	double ComputeFlcs_NormalizedPairwiseLCS(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	std::vector<std::wstring> GetLCS(int &lcs,int &mR,int &nC,std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg=FALSE);
	void PrintLCS(std::vector<std::wstring> vectorWords,int nRow,int nColumn,std::vector<std::wstring> &vectorLCS);

private:
	int **pTable,**prev;
};

