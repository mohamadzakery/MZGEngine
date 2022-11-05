#pragma once
#include <fbxsdk.h>
class EngineFbxManager
{
private:
FbxManager* gSdkManager;
public:
	FbxScene* gScene;
	EngineFbxManager(void);
	~EngineFbxManager(void);
	bool LoadFBXScene(const char *pFbxFilePath);
	void InitializeSdkManagerAndScene();
	void DestroySdkObjects(FbxManager* pSdkManager,bool pExitStatus);
	const char *GetReaderOFNFilters();
	bool LoadScene(FbxManager *pSdkManager, FbxScene *pScene, const char *pFbxFilePath);
	const FbxNode* GetRootNode();
	const char * GetRootNodeName();
	FbxString GetNodeNameAndAttributeTypeName(const FbxNode* pNode);
	FbxString GetDefaultTranslationInfo(const FbxNode* pNode);
	FbxString GetNodeVisibility(const FbxNode* pNode);
};

