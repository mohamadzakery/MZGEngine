#include "EngineFbxManager.h"
#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(pSdkManager->GetIOSettings()))
#endif

EngineFbxManager::EngineFbxManager(void)
{
	this->gScene=NULL;
	this->gSdkManager=NULL;
}
EngineFbxManager::~EngineFbxManager(void)
{
	this->DestroySdkObjects(this->gSdkManager,true);
}
bool EngineFbxManager::LoadFBXScene(const char *pFbxFilePath)
{
	if( LoadScene(gSdkManager, gScene, pFbxFilePath ) == false ) 
		return false;
    return true;
}
void EngineFbxManager::InitializeSdkManagerAndScene()
{
	// Create the FBX SDK memory manager object.
    // The SDK Manager allocates and frees memory
    // for almost all the classes in the SDK.
    gSdkManager = FbxManager::Create();
	// create an IOSettings object
	FbxIOSettings * ios = FbxIOSettings::Create(gSdkManager, IOSROOT );
	gSdkManager->SetIOSettings(ios);
	gScene = FbxScene::Create(gSdkManager,"");
}
void EngineFbxManager::DestroySdkObjects(FbxManager* pSdkManager,bool pExitStatus)
{
	// Delete the FBX SDK manager. All the objects that have been allocated 
    // using the FBX SDK manager and that haven't been explicitly destroyed 
    // are automatically destroyed at the same time.
    if( pSdkManager ) pSdkManager->Destroy();
}
const char* EngineFbxManager::GetReaderOFNFilters()
{
	int nbReaders = gSdkManager->GetIOPluginRegistry()->GetReaderFormatCount();;

    FbxString s;
    int i = 0;

    for(i=0; i < nbReaders; i++)
    {
        s += gSdkManager->GetIOPluginRegistry()->GetReaderFormatDescription(i);
        s += "|*.";
        s += gSdkManager->GetIOPluginRegistry()->GetReaderFormatExtension(i);
        s += "|";
    }

    // replace | by \0
    int nbChar   = int(strlen(s.Buffer())) + 1;
    char *filter = new char[ nbChar ];
    memset(filter, 0, nbChar);

    FBXSDK_strcpy(filter, nbChar, s.Buffer());

    for(i=0; i < int(strlen(s.Buffer())); i++)
    {
        if(filter[i] == '|')
        {
            filter[i] = 0;
        }
    }

    // the caller must delete this allocated memory
    return filter;
}
bool EngineFbxManager::LoadScene(FbxManager *pSdkManager, FbxScene *pScene, const char *pFbxFilePath)
{
	    bool lStatus;

    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(pSdkManager,"");

    // Initialize the importer by providing a filename.
    bool lImportStatus = lImporter->Initialize(pFbxFilePath, -1, pSdkManager->GetIOSettings() );

    if( !lImportStatus )
    {
        // Destroy the importer
        lImporter->Destroy();
        return false;
    }

    if (lImporter->IsFBX())
    {
        // Set the import states. By default, the import states are always set to 
        // true. The code below shows how to change these states.
        IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
        IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
        IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
        IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
        IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
        IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
        IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }

    // Import the scene
    lStatus = lImporter->Import(pScene);

    // Destroy the importer
    lImporter->Destroy();

    return lStatus;
}
const FbxNode* EngineFbxManager::GetRootNode()
{
	return gScene->GetRootNode();
}
const char * EngineFbxManager::GetRootNodeName()
{
	return GetRootNode()->GetName();
}
FbxString EngineFbxManager::GetNodeNameAndAttributeTypeName(const FbxNode* pNode)
{
	FbxString s = pNode->GetName();

    FbxNodeAttribute::EType lAttributeType;

    if(pNode->GetNodeAttribute() == NULL)
    {
        s += " (No node attribute type)";
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
        case FbxNodeAttribute::eMarker:                s += " (Marker)";               break;
        case FbxNodeAttribute::eSkeleton:              s += " (Skeleton)";             break;
        case FbxNodeAttribute::eMesh:                  s += " (Mesh)";                 break;
        case FbxNodeAttribute::eCamera:                s += " (Camera)";               break;
        case FbxNodeAttribute::eLight:                 s += " (Light)";                break;
        case FbxNodeAttribute::eBoundary:              s += " (Boundary)";             break;
        case FbxNodeAttribute::eOpticalMarker:        s += " (Optical marker)";       break;
        case FbxNodeAttribute::eOpticalReference:     s += " (Optical reference)";    break;
        case FbxNodeAttribute::eCameraSwitcher:       s += " (Camera switcher)";      break;
        case FbxNodeAttribute::eNull:                  s += " (Null)";                 break;
        case FbxNodeAttribute::ePatch:                 s += " (Patch)";                break;
        case FbxNodeAttribute::eNurbs:                  s += " (NURB)";                 break;
        case FbxNodeAttribute::eNurbsSurface:         s += " (Nurbs surface)";        break;
        case FbxNodeAttribute::eNurbsCurve:           s += " (NURBS curve)";          break;
        case FbxNodeAttribute::eTrimNurbsSurface:    s += " (Trim nurbs surface)";   break;
        case FbxNodeAttribute::eUnknown:          s += " (Unidentified)";         break;
        }   
    }

    return s;
}
FbxString EngineFbxManager::GetDefaultTranslationInfo(const FbxNode* pNode)
{
	FbxVector4 v4;
    v4 = ((FbxNode*)pNode)->LclTranslation.Get();
    return FbxString("Translation (X,Y,Z): ") + FbxString(v4[0]) + ", " + FbxString(v4[1]) + ", " + FbxString(v4[2]);
}
FbxString EngineFbxManager::GetNodeVisibility(const FbxNode* pNode)
{
	return FbxString("Visibility: ") + (pNode->GetVisibility() ? "Yes":"No");
}