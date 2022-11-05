#include "FbxLoader.h"


FbxLoader::FbxLoader(D3DDevice *Device)
{
	this->device=Device;
}


FbxLoader::~FbxLoader(void)
{
}

bool FbxLoader::LoadFbx(LPCSTR filename)
{
	EngineFbxManager* loader=NULL;
	loader=new EngineFbxManager();
	loader->InitializeSdkManagerAndScene();
	loader->LoadFBXScene(filename);
	//////////////////////////////////////
	bool bl=this->LoadContent(loader->gScene,(FbxNode *)loader->GetRootNode());
	/////////////////////////////////////
	return bl;
}
bool FbxLoader::LoadContent(FbxScene *lScene, FbxNode *pNode)
{
	bool bl=false;
	bool blMesh=true;
	bool blLight=true;
	int count=pNode->GetChildCount();
	FbxNode *Child=NULL;
	for(int i=0;i<count;i++)
	{
		Child=pNode->GetChild(i);
		if(Child->GetNodeAttribute()->GetAttributeType()==FbxNodeAttribute::eMesh)
			//blMesh=LoadMesh(Child);
			blMesh=LoadMesh(Child,true);
		if(Child->GetNodeAttribute()->GetAttributeType()==FbxNodeAttribute::eLight)
			blLight=LoadLight(Child);
		LoadContent(lScene,Child);
	}
	bl=blLight & blMesh;
	return bl;
}
bool FbxLoader::LoadMesh(FbxNode *Node)
{
	bool bl=true;
	Object *object;
	object=new Object;
	/////
	KFbxMesh * pMesh=(KFbxMesh *)Node->GetNodeAttribute();
	int lPolygonCount = pMesh->GetPolygonCount();
	BaseMesh *mesh = new BaseMesh(this->device);
	object->mesh = mesh;
	int VetexCount=pMesh->GetControlPointsCount();
	int UvCount=pMesh->GetTextureUVCount();
	mesh->vertices = new D3DXVECTOR3[VetexCount];
	mesh->normals = new D3DXVECTOR3[VetexCount];
	mesh->UVs = new D3DXVECTOR3[UvCount];
	mesh->numVertices = pMesh->GetControlPointsCount();
	KFbxVector4 *lControlPoints = pMesh->GetControlPoints();
	LPCSTR textureName;
	D3DMATERIAL9 *material;
	//////
	material=new D3DMATERIAL9();
	////////////////////////////////get Texture And Material
	int MaterialCount=Node->GetMaterialCount();
	//cout<<"Material Count="<<MaterialCount<<endl;
	FbxTexture* t=NULL;
	for(int p=0;p<MaterialCount;p++)
		{
			FbxSurfaceMaterial* material=Node->GetMaterial(p);
		    int lTextureIndex;
			bool lDisplayHeader = true;
			FbxProperty lProperty;
            FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
            {
                lProperty = material->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
                t=FindAndDisplayTextureInfoByProperty(lProperty, lDisplayHeader, p); 
				if(t!=NULL)
					break;
            }
		}
	if(t!=NULL)
	{
	FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(t);
	if (lFileTexture)
	{
		textureName=(char *) lFileTexture->GetFileName();
	}
	}
	///////////////////////////////////////////////////////////
	////get vertices
	for(int i=0; i<mesh->numVertices; i++)
	{
		mesh->vertices[i] = D3DXVECTOR3((float)lControlPoints[i][0], (float)lControlPoints[i][1], (float)lControlPoints[i][2]);
	}

	/////get Normals
	KFbxLayerElementNormal *lNormals = pMesh->GetLayer(0)->GetNormals();
	if(lNormals && lNormals->GetMappingMode() == KFbxLayerElement::eByControlPoint)
	{
		for(int i=0; i<mesh->numVertices; i++)
		{
			KFbxVector4 lVector;
			if(lNormals->GetReferenceMode() == KFbxLayerElement::eDirect )
			{
				lVector = lNormals->GetDirectArray().GetAt(i);
			}
			else
			{
				int index = lNormals->GetIndexArray().GetAt(i);
				lVector =  lNormals->GetDirectArray().GetAt(index);
			}
				mesh->normals[i] = D3DXVECTOR3((float)lVector[0], (float)lVector[1],(float) lVector[2]);
		}//for
	}//end if

	////get Uv
	FbxLayerElementArrayTemplate<KFbxVector2> *UVs = NULL;
	if(pMesh->GetTextureUV(&UVs,KFbxLayerElement::eTextureDiffuse))
	{
		if(pMesh->GetLayer(0)->GetUVs()->GetMappingMode() == KFbxLayerElement::eByControlPoint)
		{
			for(int i=0; i<pMesh->GetTextureUVCount(); i++)
			{
				mesh->UVs[i] = D3DXVECTOR3((float)UVs->GetAt(i).mData[0], (float)UVs->GetAt(i).mData[1], 1.0f);
			}
		}
	}

	////faces
	for(int i=0; i<lPolygonCount; i++)
	{
		int lPolygonSize = pMesh->GetPolygonSize(i);		
		// Triangulation Code
		for(int j = 1; j < lPolygonSize - 1; j++)
			{
				int index1 = pMesh->GetPolygonVertex(i,0);
				int index2 = pMesh->GetPolygonVertex(i,j);
				int index3 = pMesh->GetPolygonVertex(i,j+1);

				int uv1 = pMesh->GetTextureUVIndex(i,0);
				int uv2 = pMesh->GetTextureUVIndex(i,j);
				int uv3 = pMesh->GetTextureUVIndex(i,j+1);
				mesh->AddFace(index1, index2, index3, index1, index2, index3, uv1, uv2, uv3);
			}
	}
	mesh->AddMaterialAndTexture(textureName,material);
	mesh->InitVertexBuffer();
	///
	Objects.push_back(object);
	bl=true;
	return bl;
}
bool FbxLoader::LoadLight(FbxNode *pNode)
{
	bool bl=true;
	return bl;
}
D3DXVECTOR3 FbxLoader::GetCenter()
{
	D3DXVECTOR3 center=D3DXVECTOR3(0,0,0);
	int vertexCount=0;
	for(int i=0;i<this->Objects.size();i++)
	{
		for(int j=0;j<Objects[i]->mesh->numVertices;j++)
		{
			D3DXVECTOR3 cur=Objects[i]->mesh->vertices[j];
			center.x=cur.x;
			center.y=cur.y;
			center.z=cur.z;
			vertexCount++;
		}
	}
	center=center/(float)vertexCount;
	return center;
}
FbxTexture* FbxLoader::FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex){

    if( pProperty.IsValid() )
    {
		int lTextureCount = pProperty.GetSrcObjectCount<FbxTexture>();

		for (int j = 0; j < lTextureCount; ++j)
		{
			//Here we have to check if it's layeredtextures, or just textures:
			FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
			if (lLayeredTexture)
			{
                //cout<<"Layered Texture: "<< j<<endl;
                FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
                int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
                for(int k =0; k<lNbTextures; ++k)
                {
                    FbxTexture* lTexture = lLayeredTexture->GetSrcObject<FbxTexture>(k);
                    if(lTexture)
                    {

                        if(pDisplayHeader){                    
                            //cout<<"    Textures connected to Material "<< pMaterialIndex<<endl;
                            pDisplayHeader = false;
                        }

                        //NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
                        //Why is that?  because one texture can be shared on different layered textures and might
                        //have different blend modes.

                        FbxLayeredTexture::EBlendMode lBlendMode;
                        lLayeredTexture->GetTextureBlendMode(k, lBlendMode);
                        //cout<<"    Textures for "<< pProperty.GetName()<<endl;
                        //cout<<"        Texture "<< k<<endl;  
                        //DisplayTextureInfo(lTexture, (int) lBlendMode); 
						return lTexture;
                    }

                }
            }
			else
			{
				//no layered texture simply get on the property
                FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
                if(lTexture)
                {
                    //display connected Material header only at the first time
                    if(pDisplayHeader){                    
                        //cout<<"    Textures connected to Material "<< pMaterialIndex<<endl;
                        pDisplayHeader = false;
                    }             

                    //cout<<"    Textures for "<< pProperty.GetName()<<endl;
                    //cout<<"        Texture "<< j<<endl;  
                    //DisplayTextureInfo(lTexture, -1);
					return lTexture;
                }
            }
        }
    }//end if pProperty

	return NULL;

}
void FbxLoader::Render()
{
	for(int i=0;i<Objects.size();i++)
	{
		tmp=Objects[i];
		tmp->mesh->DrawFace();
		//for(int j=0;j<tmp->mesh->Faces.size();j++)
		//{
		//	tmp->mesh->DrawFace(j);
		//}
	}

}
bool FbxLoader::LoadMesh(FbxNode *Node,bool Bl)
{
	bool bl=true;
	Object *object;
	object=new Object;
	/////
	KFbxMesh * pMesh=(KFbxMesh *)Node->GetNodeAttribute();
	int lPolygonCount = pMesh->GetPolygonCount();
	BaseMesh *mesh = new BaseMesh(this->device);

	int VetexCount=pMesh->GetControlPointsCount();
	mesh->vertices = new D3DXVECTOR3[VetexCount];

	mesh->numVertices = pMesh->GetControlPointsCount();
	KFbxVector4 *lControlPoints = pMesh->GetControlPoints();

	LPCSTR textureName=NULL;
	D3DMATERIAL9 *material;
	material=new D3DMATERIAL9();

	////////////////////////////////get Texture And Material
	int MaterialCount=Node->GetMaterialCount();
	//cout<<"Material Count="<<MaterialCount<<endl;
	FbxTexture* t=NULL;
	for(int p=0;p<MaterialCount;p++)
		{
			FbxSurfaceMaterial* material=Node->GetMaterial(p);
		    int lTextureIndex;
			bool lDisplayHeader = true;
			FbxProperty lProperty;
            FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
            {
                lProperty = material->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
                t=FindAndDisplayTextureInfoByProperty(lProperty, lDisplayHeader, p); 
				if(t!=NULL)
					break;
            }
		}
	if(t!=NULL)
	{
	FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(t);
	if (lFileTexture)
	{
		textureName=(char *) lFileTexture->GetFileName();
	}
	}
	///////////////////////////////////////////////////////////
	////get vertices
	for(int i=0; i<mesh->numVertices; i++)
	{
		mesh->vertices[i] = D3DXVECTOR3((float)lControlPoints[i][0], (float)lControlPoints[i][1], (float)lControlPoints[i][2]);
	}

	////////////////////////////Get Normals
		FbxGeometryElementNormal* lGeometryElementNormal=pMesh->GetElementNormal();
		//mesh->normals = new D3DXVECTOR3[VetexCount];
		if(lGeometryElementNormal)
		{

			///////get Normals by control points
			if(lGeometryElementNormal->GetMappingMode()==FbxLayerElement::EMappingMode::eByControlPoint)
			{
				mesh->normals = new D3DXVECTOR3[VetexCount];
				for(int q=0;q<VetexCount;q++)
				{
				FbxVector4 Vec;
				if(lGeometryElementNormal->GetReferenceMode()==FbxLayerElement::EReferenceMode::eDirect)
				{
					Vec=lGeometryElementNormal->GetDirectArray().GetAt(q);
				}
				else if(lGeometryElementNormal->GetReferenceMode()==FbxLayerElement::EReferenceMode::eIndexToDirect)
				{
					int ind=lGeometryElementNormal->GetIndexArray().GetAt(q);
					Vec=lGeometryElementNormal->GetDirectArray().GetAt(ind);
				}
				mesh->normals[q].x=(float)Vec.mData[0];
				mesh->normals[q].y=(float)Vec.mData[1];
				mesh->normals[q].z=(float)Vec.mData[2];

				}//for
			}

			///////get Normals by polygun vertex
			if(lGeometryElementNormal->GetMappingMode()==FbxLayerElement::EMappingMode::eByPolygonVertex)
			{
				int PolygonVertexCount=pMesh->GetPolygonVertexCount();
				mesh->normals = new D3DXVECTOR3[PolygonVertexCount];
				for(int q=0;q<PolygonVertexCount;q++)
				{
					FbxVector4 Vec;
				if(lGeometryElementNormal->GetReferenceMode()==FbxLayerElement::EReferenceMode::eDirect)
				{
					Vec=lGeometryElementNormal->GetDirectArray().GetAt(q);
				}
				else if(lGeometryElementNormal->GetReferenceMode()==FbxLayerElement::EReferenceMode::eIndexToDirect)
				{
					int ind=lGeometryElementNormal->GetIndexArray().GetAt(q);
					Vec=lGeometryElementNormal->GetDirectArray().GetAt(ind);
				}
				mesh->normals[q].x=(float)Vec.mData[0];
				mesh->normals[q].y=(float)Vec.mData[1];
				mesh->normals[q].z=(float)Vec.mData[2];

				}//for
			}
		}
	////////////////////////////
	////get Uv
	//FbxLayerElementArrayTemplate<KFbxVector2> *UVs = NULL;
	//if(pMesh->GetTextureUV(&UVs,KFbxLayerElement::eTextureDiffuse))
	//{
	//	if(pMesh->GetLayer(0)->GetUVs()->GetMappingMode() == KFbxLayerElement::eByControlPoint)
	//	{
	//		for(int i=0; i<pMesh->GetTextureUVCount(); i++)
	//		{
	//			mesh->UVs[i] = D3DXVECTOR3((float)UVs->GetAt(i).mData[0], (float)UVs->GetAt(i).mData[1], 1.0f);
	//		}
	//	}
	//}

	//////////////////Get Uvs
	int UvLayerCount=pMesh->GetElementUVCount();
	for(int t=0;t<UvLayerCount;t++)
	{
	FbxGeometryElementUV* UvElement=pMesh->GetElementUV(t);
	if(UvElement)
	{
		//get uvs by control points
		if(UvElement->GetMappingMode()==FbxLayerElement::EMappingMode::eByControlPoint)
		{
			mesh->UVs = new D3DXVECTOR3[VetexCount];
			for(int q=0;q<VetexCount;q++)
			{
			FbxVector4 Vec;
			if(UvElement->GetReferenceMode()==FbxLayerElement::EReferenceMode::eDirect)
			{
				Vec=UvElement->GetDirectArray().GetAt(q);
			}
			else if(UvElement->GetReferenceMode()==FbxLayerElement::EReferenceMode::eIndexToDirect)
			{
				int ind=UvElement->GetIndexArray().GetAt(q);
				Vec=UvElement->GetDirectArray().GetAt(ind);
			}
			mesh->UVs[q].x=(float)Vec.mData[0];
			mesh->UVs[q].y=(float)Vec.mData[1];
			mesh->UVs[q].z=(float)Vec.mData[2];
			}//for 

		}
		////get uvs by polugon vertex
		else if(UvElement->GetMappingMode()==FbxLayerElement::EMappingMode::eByPolygonVertex)
		{
			int PolygonVertexCount=pMesh->GetPolygonVertexCount();
			mesh->UVs = new D3DXVECTOR3[PolygonVertexCount];
			for(int q=0;q<PolygonVertexCount;q++)
			{
			FbxVector4 Vec;
			if(UvElement->GetReferenceMode()==FbxLayerElement::EReferenceMode::eDirect)
			{
				Vec=UvElement->GetDirectArray().GetAt(q);
			}
			else if(UvElement->GetReferenceMode()==FbxLayerElement::EReferenceMode::eIndexToDirect)
			{
				int ind=UvElement->GetIndexArray().GetAt(q);
				Vec=UvElement->GetDirectArray().GetAt(ind);
			}
			mesh->UVs[q].x=(float)Vec.mData[0];
			mesh->UVs[q].y=(float)Vec.mData[1];
			mesh->UVs[q].z=(float)Vec.mData[2];
			}//for
		}//else if
	}

	}//for uvLayerCount
	//////////////////

	////faces
	for(int i=0; i<lPolygonCount; i++)
	{
		int lPolygonSize = pMesh->GetPolygonSize(i);		
		// Triangulation Code
		//for(int j = 1; j < lPolygonSize - 1; j++)
		//	{
				int index1 = pMesh->GetPolygonVertex(i,0);
				int index2 = pMesh->GetPolygonVertex(i,1);
				int index3 = pMesh->GetPolygonVertex(i,2);

				int uv1 = pMesh->GetTextureUVIndex(i,0);
				int uv2 = pMesh->GetTextureUVIndex(i,1);
				int uv3 = pMesh->GetTextureUVIndex(i,2);

					//KFbxVector4 fbxNormal;
					//pMesh->GetPolygonVertexNormal(i, 0, fbxNormal);
					//fbxNormal.Normalize();
				mesh->AddFace(index1,index2,index3, index1,index2, index3, uv1, uv2, uv3);
		//	}
	}
	////
	FbxVector4 s=Node->LclScaling;
	FbxVector4 r=Node->LclRotation;
	FbxVector4 tt=Node->LclTranslation;
	mesh->matScale=D3DXVECTOR3(s[0],s[1],s[2]);
	mesh->matRotation=D3DXVECTOR3(r[0],r[1],r[2]);
	mesh->matTrans=D3DXVECTOR3(tt[0],tt[1],tt[2]);
	mesh->AddMaterialAndTexture(textureName,material);
	mesh->InitVertexBuffer();
	///
	object->mesh = mesh;
	//
	Objects.push_back(object);
	bl=true;
	return bl;
}