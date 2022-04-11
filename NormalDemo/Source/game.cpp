#include "stdafx.h"
#include "Game.h"
#include "Camera.h"
#include "engineMath.h"
#include "Graphics.h"
#include "jsonUtil.h"
#include "Profiler.h"
#include "RenderCube.h"
#include "Shader.h"
#include "SkinnedObj.h"
#include "stringUtil.h"
#include "texture.h"
#include "VertexBuffer.h"
#include "VertexFormats.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include "Components\Character.h"
#include "Components\CollisionComponent.h"
#include "Components\FollowCam.h"
#include "Components\Player.h"
#include "Components\PointLight.h"
#include "Components\SimpleRotate.h"
#include <fstream>
#include <sstream>


Game::Game()
    : mCamera(nullptr)
    , mAssets(&mGraphics)
    , mLightBuffer(nullptr)
{
    mJobs.Begin();
}

Game::~Game()
{
}

void Game::Init(HWND hWnd, float width, float height)
{
    mGraphics.InitD3D(hWnd, width, height);

    {   // load the Simple shader
        D3D11_INPUT_ELEMENT_DESC inputElem[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosColor, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        Shader* pShader = new Shader(&mGraphics);
        pShader->Load(L"Shaders/Mesh.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"Mesh", pShader);
    }
    {   // load the BasicMesh shader
        D3D11_INPUT_ELEMENT_DESC inputElem[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormColorUV, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormColorUV, norm),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosNormColorUV, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, offsetof(VertexPosNormColorUV, uv),    D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        Shader* pShader = new Shader(&mGraphics);
        pShader->Load(L"Shaders/BasicMesh.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"BasicMesh", pShader);
    }
    {   // load the Phong and Unlit shaders
        D3D11_INPUT_ELEMENT_DESC inputElem[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormUV, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormUV, norm),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, offsetof(VertexPosNormUV, uv),    D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        Shader* pShader = new Shader(&mGraphics);
        pShader->Load(L"Shaders/Phong.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"Phong", pShader);
        pShader = new Shader(&mGraphics);
        pShader->Load(L"Shaders/Unlit.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"Unlit", pShader);
    }
    {   // load the Skinned shader
        D3D11_INPUT_ELEMENT_DESC inputElem[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormSkinUV, pos),         D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormSkinUV, norm),        D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BONE",     0, DXGI_FORMAT_R8G8B8A8_UINT,      0, offsetof(VertexPosNormSkinUV, boneIndex),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "WEIGHT",   0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, offsetof(VertexPosNormSkinUV, boneWeight),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, offsetof(VertexPosNormSkinUV, uv),          D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        Shader* pShader = new Shader(&mGraphics);
        pShader->Load(L"Shaders/Skinned.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"Skinned", pShader);
    }
    {   // load the Normal shader
        D3D11_INPUT_ELEMENT_DESC inputElem[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormTanUV, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormTanUV, norm),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormTanUV, tan),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, offsetof(VertexPosNormTanUV, uv),    D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        Shader* pShader = new Shader(&mGraphics);
        pShader->Load(L"Shaders/Normal.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"Normal", pShader);
    }

    mCamera = new Camera(&mGraphics);
    mLightBuffer = mGraphics.CreateGraphicsBuffer(&mLightData, sizeof(mLightData), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

    LoadLevel(L"Assets/Levels/Level10.itplevel");
}

void Game::Shutdown()
{
    mJobs.End();

    delete mCamera;
    for (RenderObj* pObj : mRenderObj)
        delete pObj;

    mLightBuffer->Release();

    mAssets.Clear();

    mGraphics.CleanD3D();
}

void Game::Update(float deltaTime)
{
    for (RenderObj* pObj : mRenderObj)
        pObj->Update(deltaTime);

    {
        PROFILE_SCOPE(WaitForJobs);
        mJobs.WaitForJobs();
    }
}

void Game::RenderFrame()
{
    {
        PROFILE_SCOPE(BeginFrame);
        // Clear the screen to blue
        Graphics::Color4 clearColor(0.0f, 0.2f, 0.4f, 1.0f);
        mGraphics.BeginFrame(clearColor);
    }

    {
        PROFILE_SCOPE(SetLighting);
        mCamera->SetActive();
        mGraphics.UploadBuffer(mLightBuffer, &mLightData, sizeof(mLightData));
        mGraphics.GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_LIGHTING, 1, &mLightBuffer);
    }

    {
        PROFILE_SCOPE(RenderObjects);
        // Draw the objects
        for (RenderObj* pObj : mRenderObj)
            pObj->Draw();
    }

    {
        PROFILE_SCOPE(EndFrame);
        // Done with this frame
        mGraphics.EndFrame();
    }
}

void Game::OnKeyDown(uint32_t key)
{
	m_keyIsHeld[key] = true;
}

void Game::OnKeyUp(uint32_t key)
{
	m_keyIsHeld[key] = false;
}

bool Game::IsKeyHeld(uint32_t key) const
{
	const auto find = m_keyIsHeld.find(key);
	if (find != m_keyIsHeld.end())
		return find->second;
	return false;
}

Game::PointLightData* Game::AllocateLight()
{
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        if (false == mLightData.c_pointLight[i].isEnabled)
        {
            mLightData.c_pointLight[i].isEnabled = true;
            return &mLightData.c_pointLight[i];
        }
    }
    return nullptr;
}

void Game::FreeLight(PointLightData* pLight)
{
    DbgAssert(pLight->isEnabled, "Don't try to free a light that isn't enabled");
    pLight->isEnabled = false;
}

bool Game::LoadLevel(const WCHAR* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itplevel" ||
		ver != 2)
	{
		return false;
	}

    { // Read the camera data
        const rapidjson::Value& camObj = doc["camera"];
        if (camObj.IsObject())
        {
            Vector3 pos = Vector3::Zero;
            GetVectorFromJSON(camObj, "position", pos);
            Quaternion rot = Quaternion::Identity;
            GetQuaternionFromJSON(camObj, "rotation", rot);
            mCamera->mView = Matrix4::CreateFromQuaternion(rot) * Matrix4::CreateTranslation(pos);
            mCamera->mView.Invert();
        }
    }

    { // Read the lighting data
        const rapidjson::Value& lightObj = doc["lightingData"];
        if (lightObj.IsObject())
        {
            Vector3 ambient;
            GetVectorFromJSON(lightObj, "ambient", ambient);
            SetAmbientLight(ambient);
        }
    }

    {   // Load the RenderObjs
        const rapidjson::Value& renderObjects = doc["renderObjects"];
        if (renderObjects.IsArray())
        {
            for (rapidjson::SizeType i = 0; i < renderObjects.Size(); ++i)
            {
                const rapidjson::Value& obj = renderObjects[i];
                Vector3 pos;
                GetVectorFromJSON(obj, "position", pos);
                Quaternion rot;
                GetQuaternionFromJSON(obj, "rotation", rot);
                float scale;
                GetFloatFromJSON(obj, "scale", scale);
                std::wstring mesh;
                GetWStringFromJSON(obj, "mesh", mesh);
                Mesh* pMesh = mAssets.LoadMesh(mesh);
                RenderObj* pObj = nullptr;
                SkinnedObj* pSkin = nullptr;
                if ((nullptr != pMesh) && (pMesh->IsSkinned()))
                {
                    pSkin = new SkinnedObj(&mGraphics, pMesh);
                    pObj = pSkin;
                }
                else
                {
                    pObj = new RenderObj(&mGraphics, pMesh);
                }
                pObj->mObjectData.c_modelToWorld =
                    Matrix4::CreateScale(scale)
                    * Matrix4::CreateFromQuaternion(rot)
                    * Matrix4::CreateTranslation(pos);

                const rapidjson::Value& components = obj["components"];
                if (components.IsArray())
                {
                    for (rapidjson::SizeType j = 0; j < components.Size(); ++j)
                    {
                        Component* pComp = nullptr;
                        const rapidjson::Value& comp = components[j];
                        std::string type;
                        GetStringFromJSON(comp, "type", type);
                        if (type == "PointLight")
                        {
                            pComp = new PointLight(this, pObj);
                        }
                        else if (type == "Character")
                        {
                            pComp = new Character(this, pSkin);
                        }
                        else if (type == "Player")
                        {
                            pComp = new Player(this, pSkin);
                        }
                        else if (type == "FollowCam")
                        {
                            pComp = new FollowCam(this, pObj);
                        }
                        else if (type == "Collision")
                        {
                            pComp = new CollisionComponent(&mPhysics, pObj);
                        }
                        else if (type == "SimpleRotate")
                        {
                            pComp = new SimpleRotate(pObj);
                        }
                        if (nullptr != pComp)
                        {
                            pComp->LoadProperties(comp);
                            pObj->AddComponent(pComp);
                        }
                    }
                }

                mRenderObj.push_back(pObj);
            }
        }
    }

	return true;
}