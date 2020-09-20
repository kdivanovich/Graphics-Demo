/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 4.0a 30/01/2016
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "CCatmullRom.h"
#include "Cube.h"
#include "UI.h"
#include "Diamond.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	m_pBarrelMesh = NULL;
	m_pHorseMesh = NULL;
	m_pTigerMesh = NULL;
	m_pRockMesh = NULL;
	m_pStatueMesh = NULL;
	m_pTankMesh = NULL;
	m_pAircraftMesh = NULL;
	m_pSphere = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pCatmullRom = NULL;
	m_pHouse = NULL;
	m_pUI = NULL;
	m_pDiamond = NULL;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;

	m_currentDistance = 0.0f;
	m_cameraMovSpeed = 0.08f;

	distAhead = 0.0f;
	debugVal = 0.0f;

}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pTigerMesh;
	delete m_pRockMesh;
	delete m_pStatueMesh;
	delete m_pTankMesh;
	delete m_pAircraftMesh;
	delete m_pSphere;
	delete m_pAudio;
	delete m_pCatmullRom;
	delete m_pHouse;
	delete m_pUI;
	delete m_pDiamond;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pTigerMesh = new COpenAssetImportMesh;
	m_pRockMesh = new COpenAssetImportMesh;
	m_pStatueMesh = new COpenAssetImportMesh;
	m_pTankMesh = new COpenAssetImportMesh;
	m_pAircraftMesh = new COpenAssetImportMesh;

	m_pSphere = new CSphere;
	m_pAudio = new CAudio;
	m_pCatmullRom = new CCatmullRom;
	
	m_pHouse = new CCube;
	m_pUI = new CUI;
	m_pDiamond = new CDiamond;

	normalShaderUsed = true;
	toonShaderUsed = false;
	UIon = false;


	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("sphereShader.vert");
	sShaderFileNames.push_back("sphereShader.frag");
	sShaderFileNames.push_back("toonShader.vert");
	sShaderFileNames.push_back("toonShader.frag");


	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);

	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// Create the sphere shader program
	CShaderProgram* pSphereProgram = new CShaderProgram;
	pSphereProgram->CreateProgram();
	pSphereProgram->AddShaderToProgram(&shShaders[4]);
	pSphereProgram->AddShaderToProgram(&shShaders[5]);
	pSphereProgram->LinkProgram();
	m_pShaderPrograms->push_back(pSphereProgram);

	// Create the main shader program
	CShaderProgram* pMainProgram2 = new CShaderProgram;
	pMainProgram2->CreateProgram();
	pMainProgram2->AddShaderToProgram(&shShaders[6]);
	pMainProgram2->AddShaderToProgram(&shShaders[7]);
	pMainProgram2->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram2);

	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);
	
	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "moon.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013

	m_pTigerMesh->Load("resources\\models\\Tiger\\12230_smilodon_v1_l2.obj");	// All models downloaded from https://free3d.com/ -- searched by "royalty-free low-poly"
	m_pRockMesh->Load("resources\\models\\Rock\\Rock1.obj");					// translate Y = -
	m_pStatueMesh->Load("resources\\models\\Statue\\12328_Statue_v1_L2.obj");
	m_pTankMesh->Load("resources\\models\\Tank\\m26.obj");

	m_pAircraftMesh->Load("resources\\models\\Air\\E 45 Aircraft_obj.obj");		// Player model

	m_pHouse->Create("resources\\textures\\house.png");							//wall texture: https://cdnb.artstation.com/p/assets/images/images/021/993/219/large/spencer-hanigan-brick-wall-2119712.jpg?1573701871
																				//window texture: https://i.pinimg.com/originals/a2/6a/db/a26adbc6b6b971c993f91b080774bb9b.jpg
																				//door texure: https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcQ0yyjpP1pB9bMVgkJ24GaL1EAIIL3TJkzPikOXPK-ayQpa3YUN&usqp=CAU
	   
	m_pDiamond->Create("resources\\textures\\pyramid.jpg");

	m_pUI->Create("resources\\textures\\ui.png");		// UI Texture from https://opengameart.org/sites/default/files/preview_512.png


	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	// Initialise audio and play background music
	m_pAudio->Initialise();
	m_pAudio->LoadEventSound("Resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	m_pAudio->LoadMusicStream("Resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	m_pAudio->PlayMusicStream();

	glm::vec3 p0 = glm::vec3(-500, 10, -200);
	glm::vec3 p1 = glm::vec3(0, 10, -200);
	glm::vec3 p2 = glm::vec3(0, 10, 200);
	glm::vec3 p3 = glm::vec3(-500, 10, 200);	
	//m_pCatmullRom->CreatePath(p0, p1, p2, p3);

	m_pCatmullRom->CreateCentreline();

	m_pCatmullRom->CreateOffsetCurves();

	m_pCatmullRom->CreateTrack();
}

// Render method runs repeatedly in a loop
void Game::Render() 
{

	if (!toonShaderUsed)
	{
		
		// Clear the buffers and enable depth testing (z-buffering)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// Set up a matrix stack
		glutil::MatrixStack modelViewMatrixStack;
		modelViewMatrixStack.SetIdentity();

		// Use the main shader program 
		CShaderProgram* pMainProgram = (*m_pShaderPrograms)[0];
		pMainProgram->UseProgram();
		pMainProgram->SetUniform("bUseTexture", true);
		pMainProgram->SetUniform("sampler0", 0);
		pMainProgram->SetUniform("CubeMapTex", 1);

		// Set the projection matrix
		pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

		// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
		// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
		modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
		glm::mat4 viewMatrix = modelViewMatrixStack.Top();
		glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

		
		// Set light and materials in main shader program
		glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
		glm::vec4 lightPosition2(-80, 16.5f, 20, 1); // Position of light source *in world coordinates*

		if (normalShaderUsed)
		{
			pMainProgram->SetUniform("light1.position", viewMatrix * lightPosition1); // Position of light source *in eye coordinates*
			pMainProgram->SetUniform("light1.La", glm::vec3(1.0f, 1.0f, 1.0f));		// Ambient colour of light
			pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f, 1.0f));		// Diffuse colour of light
			pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f, 1.0f));		// Specular colour of light
			pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
			pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
			pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
			pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
		}
		else
		{
			// use night time shader
			pMainProgram->SetUniform("light1.position", viewMatrix * lightPosition1); // Position of light source *in eye coordinates*
			pMainProgram->SetUniform("light1.La", glm::vec3(0.2f, 0.2f, 0.8f));		// Ambient colour of light
			pMainProgram->SetUniform("light1.Ld", glm::vec3(0.2f, 0.2f, 0.8f));		// Diffuse colour of light
			pMainProgram->SetUniform("light1.Ls", glm::vec3(0.2f, 0.2f, 0.8f));		// Specular colour of light
			pMainProgram->SetUniform("material1.Ma", glm::vec3(.5f));	// Ambient material reflectance
			pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
			pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
			pMainProgram->SetUniform("material1.shininess", 10.0f);		// Shininess material property
		}


		pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
		pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
		pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property


		// Render the skybox and terrain with full ambient reflectance 
		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render();
		pMainProgram->SetUniform("renderSkybox", false);
		modelViewMatrixStack.Pop();

		// Render the planar terrain
		modelViewMatrixStack.Push();
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pPlanarTerrain->Render();
		modelViewMatrixStack.Pop();


		// Turn on diffuse + specular materials
		pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
		pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
		pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	


		// ------------------------------------------------- Render the addition objects: ---------------------------------------------------------------------------

		// Render the Tiger 
		modelViewMatrixStack.Push();
		{
			modelViewMatrixStack.Translate(glm::vec3(-90, 0.0f, -90));
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);	
			modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), 4.7f );
			modelViewMatrixStack.Scale(0.12f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pTigerMesh->Render();
		}	
		modelViewMatrixStack.Pop();
		
		for (int i = 0; i < 40; i += 8)
		{
			for (int j = 0; j < 60; j += 10)
			{
				// Render the Rock 
				modelViewMatrixStack.Push();
				{
					modelViewMatrixStack.Translate(glm::vec3(-30 + i, -0.5f, 30 + j));
					modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), (double)i/ (double)j);		// trick to make them rotated differently for a bit more realistm
					modelViewMatrixStack.Scale(2.0f);
					pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
					pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
					m_pRockMesh->Render();
				}
				modelViewMatrixStack.Pop();
			}		
		}

		// Render the Statue 
		modelViewMatrixStack.Push();
		{
			modelViewMatrixStack.Translate(glm::vec3(30, -0.3f, -40));
			modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), 4.75f);	
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 0.0f, 1.0f), 2.4f);
			modelViewMatrixStack.Scale(0.1f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pStatueMesh->Render();
		}
		modelViewMatrixStack.Pop();

		// Render the Tank 
		modelViewMatrixStack.Push();
		{
			modelViewMatrixStack.Translate(glm::vec3(-70, 2.9f, 20));
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 4.7f);
			modelViewMatrixStack.Scale(4.5f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pTankMesh->Render();
		}
		modelViewMatrixStack.Pop();
		
		// Render the Aircraft 
		modelViewMatrixStack.Push();											// Needs more adjustments
		{
			modelViewMatrixStack.ApplyMatrix(playerModelMatrix);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pAircraftMesh->Render();
		}
		modelViewMatrixStack.Pop();

		// UI
		modelViewMatrixStack.Push();
		{
			double camx = m_pCamera->GetView().x;
			double camy = m_pCamera->GetView().y;
			double camz = m_pCamera->GetView().z;
			//modelViewMatrixStack.Translate(glm::vec3(0, 5, 0));
			modelViewMatrixStack.Translate(glm::vec3(camx, camy, camz ));
			//modelViewMatrixStack.Translate(m_pCamera->GetPosition());
			modelViewMatrixStack.Scale(4.5f);
			//modelViewMatrixStack.ApplyMatrix(playerModelMatrix);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			if (UIon) 
				m_pUI->Render();
		}
		modelViewMatrixStack.Pop();

		// Render the diamond
		modelViewMatrixStack.Push();
		{
			modelViewMatrixStack.Translate(glm::vec3(0, 0, 0));
			modelViewMatrixStack.Scale(43.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pDiamond->Render();
		}
		modelViewMatrixStack.Pop();

		// Render the house
		modelViewMatrixStack.Push(); 
		{
			modelViewMatrixStack.Translate(glm::vec3(-0, 3, 0));
			modelViewMatrixStack.Scale(3.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			//m_pHouse->Render();
		}	
		modelViewMatrixStack.Pop();
		
		modelViewMatrixStack.Push();
		{
			modelViewMatrixStack.Translate(glm::vec3(25, 3, -115));
			modelViewMatrixStack.Scale(3.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pHouse->Render();
		}
		modelViewMatrixStack.Pop();
		
		// Render the house
		modelViewMatrixStack.Push();
		{
			modelViewMatrixStack.Translate(glm::vec3(48, 3, 48));
			modelViewMatrixStack.Scale(3.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pHouse->Render();
		}
		modelViewMatrixStack.Pop();
		
		// Render the house
		modelViewMatrixStack.Push();
		{
			modelViewMatrixStack.Translate(glm::vec3(58, 3, 68));
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 4.7f);
			modelViewMatrixStack.Scale(3.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pHouse->Render();
		}
		modelViewMatrixStack.Pop();

		// Render the house
		modelViewMatrixStack.Push();
		{
			modelViewMatrixStack.Translate(glm::vec3(38, 3, 88));
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.7f);
			modelViewMatrixStack.Scale(3.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pHouse->Render();
		}
		modelViewMatrixStack.Pop();
		
		// Render the house
		modelViewMatrixStack.Push();
		{
			modelViewMatrixStack.Translate(glm::vec3(28, 3, 98));
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.7f);
			modelViewMatrixStack.Scale(3.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pHouse->Render();
		}
		modelViewMatrixStack.Pop();

		// Render the house
		modelViewMatrixStack.Push();
		{
			modelViewMatrixStack.Translate(glm::vec3(65, 3, 28));
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), .5f);
			modelViewMatrixStack.Scale(3.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pHouse->Render();
		}
		modelViewMatrixStack.Pop();

		//----------------------------------------------------------------------------------------------------------------------------------------------------------

		// Render the spline
		modelViewMatrixStack.Push();
			pMainProgram->SetUniform("bUseTexture", true);						// use the road texture
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix",
			m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			//m_pCatmullRom->RenderPath();
			m_pCatmullRom->RenderCentreline();
			m_pCatmullRom->RenderOffsetCurves();
		// Render your object here
		modelViewMatrixStack.Pop();
		
		// Render the track
		modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pCatmullRom->RenderTrack();
		modelViewMatrixStack.Pop();


			// Draw the 2D graphics after the 3D graphics
		DisplayFrameRate();

		// Swap buffers to show the rendered image
		SwapBuffers(m_gameWindow.Hdc());
	} 
	else
	{
		// Clear the buffers and enable depth testing (z-buffering)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// Set up a matrix stack
		glutil::MatrixStack modelViewMatrixStack;
		modelViewMatrixStack.SetIdentity();

		
		// Toon shader
		CShaderProgram* pMainProgram2 = (*m_pShaderPrograms)[3];
		pMainProgram2->UseProgram();
		pMainProgram2->SetUniform("bUseTexture", true);
		pMainProgram2->SetUniform("sampler0", 0);
		pMainProgram2->SetUniform("CubeMapTex", 1);
		
		// Set the projection matrix
		pMainProgram2->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());


		// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
		// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
		modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
		glm::mat4 viewMatrix = modelViewMatrixStack.Top();
		glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);
		
		
		// Set light and materials in main shader program
		glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
		glm::vec4 lightPosition2(-80, 16.5f, 20, 1); // Position of light source *in world coordinates*

		if (normalShaderUsed)
		{
			pMainProgram2->SetUniform("light1.position", viewMatrix * lightPosition1); // Position of light source *in eye coordinates*
			pMainProgram2->SetUniform("light1.La", glm::vec3(1.0f, 1.0f, 1.0f));		// Ambient colour of light
			pMainProgram2->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f, 1.0f));		// Diffuse colour of light
			pMainProgram2->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f, 1.0f));		// Specular colour of light
			pMainProgram2->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
			pMainProgram2->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
			pMainProgram2->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
			pMainProgram2->SetUniform("material1.shininess", 15.0f);		// Shininess material property
		}
		else
		{
			pMainProgram2->SetUniform("light1.position", viewMatrix * lightPosition1); // Position of light source *in eye coordinates*
			pMainProgram2->SetUniform("light1.La", glm::vec3(0.2f, 0.2f, 0.8f));		// Ambient colour of light
			pMainProgram2->SetUniform("light1.Ld", glm::vec3(0.2f, 0.2f, 0.8f));		// Diffuse colour of light
			pMainProgram2->SetUniform("light1.Ls", glm::vec3(0.2f, 0.2f, 0.8f));		// Specular colour of light
			pMainProgram2->SetUniform("material1.Ma", glm::vec3(.5f));	// Ambient material reflectance
			pMainProgram2->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
			pMainProgram2->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
			pMainProgram2->SetUniform("material1.shininess", 10.0f);		// Shininess material property
		}

		pMainProgram2->SetUniform("light2.position", viewMatrix * lightPosition2);
		pMainProgram2->SetUniform("light2.La", glm::vec3(1.0f, 0.0f, 0.0f));
		pMainProgram2->SetUniform("light2.Ld", glm::vec3(1.0f, 0.0f, 0.0f));
		pMainProgram2->SetUniform("light2.Ls", glm::vec3(1.0f, 0.0f, 0.0f));
		pMainProgram2->SetUniform("light2.direction", glm::normalize(viewNormalMatrix * glm::vec3(0, -1, 0)));
		pMainProgram2->SetUniform("light2.exponent", 20.0f);
		pMainProgram2->SetUniform("light2.cutoff", 30.0f);
		pMainProgram2->SetUniform("material2.Md", glm::vec3(0.0f));	// Diffuse material reflectance
		pMainProgram2->SetUniform("material2.Ms", glm::vec3(0.0f));	// Specular material reflectance
		pMainProgram2->SetUniform("material2.shininess", 15.0f);		// Shininess material property

		// Render the skybox and terrain with full ambient reflectance 
		modelViewMatrixStack.Push();
		pMainProgram2->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram2->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));

		m_pSkybox->Render();
		pMainProgram2->SetUniform("renderSkybox", false);
		modelViewMatrixStack.Pop();

		// Render the planar terrain
		modelViewMatrixStack.Push();
			pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pPlanarTerrain->Render();
		modelViewMatrixStack.Pop();

		// Turn on diffuse + specular materials
		pMainProgram2->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
		pMainProgram2->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
		pMainProgram2->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	



	// ------------------------------------------------- Render the addition objects: ---------------------------------------------------------------------------

	// Render the Tiger 
	modelViewMatrixStack.Push();
	{
		modelViewMatrixStack.Translate(glm::vec3(-90, 0.0f, -90));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);	
		modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), 4.7f );
		modelViewMatrixStack.Scale(0.12f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pTigerMesh->Render();
	}	
	modelViewMatrixStack.Pop();
	
	for (int i = 0; i < 40; i += 8)
	{
		for (int j = 0; j < 60; j += 10)
		{
			// Render the Rock 
			modelViewMatrixStack.Push();
			{
				modelViewMatrixStack.Translate(glm::vec3(-30 + i, -0.5f, 30 + j));
				modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), (double)i/ (double)j);		// trick to make them rotated differently for a bit more realistm
				modelViewMatrixStack.Scale(2.0f);
				pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				m_pRockMesh->Render();
			}
			modelViewMatrixStack.Pop();
		}		
	}

	// Render the Statue 
	modelViewMatrixStack.Push();
	{
		modelViewMatrixStack.Translate(glm::vec3(30, -0.3f, -40));
		modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), 4.75f);	
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 0.0f, 1.0f), 2.4f);
		modelViewMatrixStack.Scale(0.1f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pStatueMesh->Render();
	}
	modelViewMatrixStack.Pop();

	// Render the Tank 
	modelViewMatrixStack.Push();
	{
		modelViewMatrixStack.Translate(glm::vec3(-70, 2.9f, 20));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 4.7f);
		modelViewMatrixStack.Scale(4.5f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pTankMesh->Render();
	}
	modelViewMatrixStack.Pop();
	
	// Render the Aircraft 
	modelViewMatrixStack.Push();											// Needs more adjustments
	{
		modelViewMatrixStack.ApplyMatrix(playerModelMatrix);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pAircraftMesh->Render();
	}
	modelViewMatrixStack.Pop();

	// Render the diamond
	modelViewMatrixStack.Push();
	{
		modelViewMatrixStack.Translate(glm::vec3(0, 0, 0));
		modelViewMatrixStack.Scale(43.0f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pDiamond->Render();
	}
	modelViewMatrixStack.Pop();

	// Render the house
	modelViewMatrixStack.Push(); 
	{
		modelViewMatrixStack.Translate(glm::vec3(-0, 3, 0));
		modelViewMatrixStack.Scale(3.0f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		//m_pHouse->Render();
	}	
	modelViewMatrixStack.Pop();
	
	modelViewMatrixStack.Push();
	{
		modelViewMatrixStack.Translate(glm::vec3(25, 3, -115));
		modelViewMatrixStack.Scale(3.0f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHouse->Render();
	}
	modelViewMatrixStack.Pop();

	// Render the house
	modelViewMatrixStack.Push();
	{
		modelViewMatrixStack.Translate(glm::vec3(48, 3, 48));
		modelViewMatrixStack.Scale(3.0f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHouse->Render();
	}
	modelViewMatrixStack.Pop();
	
	// Render the house
	modelViewMatrixStack.Push();
	{
		modelViewMatrixStack.Translate(glm::vec3(58, 3, 68));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 4.7f);
		modelViewMatrixStack.Scale(3.0f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHouse->Render();
	}
	modelViewMatrixStack.Pop();

	// Render the house
	modelViewMatrixStack.Push();
	{
		modelViewMatrixStack.Translate(glm::vec3(38, 3, 88));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.7f);
		modelViewMatrixStack.Scale(3.0f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHouse->Render();
	}
	modelViewMatrixStack.Pop();
	
	// Render the house
	modelViewMatrixStack.Push();
	{
		modelViewMatrixStack.Translate(glm::vec3(28, 3, 98));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.7f);
		modelViewMatrixStack.Scale(3.0f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHouse->Render();
	}
	modelViewMatrixStack.Pop();

	// Render the house
	modelViewMatrixStack.Push();
	{
		modelViewMatrixStack.Translate(glm::vec3(65, 3, 28));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), .5f);
		modelViewMatrixStack.Scale(3.0f);
		pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHouse->Render();
	}
	modelViewMatrixStack.Pop();

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------

		// Render the spline
		modelViewMatrixStack.Push();
			pMainProgram2->SetUniform("bUseTexture", true);						// use the road texture
			pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram2->SetUniform("matrices.normalMatrix",
			m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			//m_pCatmullRom->RenderPath();
			m_pCatmullRom->RenderCentreline();
			m_pCatmullRom->RenderOffsetCurves();
		// Render your object here
		modelViewMatrixStack.Pop();
		
		// Render the track
		modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
			pMainProgram2->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram2->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pCatmullRom->RenderTrack();
		modelViewMatrixStack.Pop();




		// Draw the 2D graphics after the 3D graphics
		DisplayFrameRate();

		// Swap buffers to show the rendered image
		SwapBuffers(m_gameWindow.Hdc());
	}

}


// Update method runs repeatedly with the Render method
void Game::Update()
{
	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	glm::vec3 cam_veiwpoint = glm::vec3(0, 0, 0);
	glm::vec3 cam_up_vector = glm::vec3(0, 1, 0);
	m_pCamera->Update(m_dt * m_cameraMovSpeed);
	m_currentDistance += 0.1f;													// speed of moving between control points
	
	m_pCatmullRom->Sample(m_currentDistance, p, game_cam_up);					// set a point on the spline at a dist = m_currentDistance		
	//m_pCatmullRom->Sample(m_currentDistance + 1.0f, pNext, game_cam_up);		// find the NEXT point ahead
	

	if (GetAsyncKeyState('W') < 0) {
		m_currentDistance += 0.1f;		// go faster
	}
	if (GetAsyncKeyState('S') < 0) {
		m_currentDistance -= 0.1f;		// stop
	}


	// normalShaderUsed = !normalShaderUsed; oroginally but keys are too sensitive and require an external framework to work properly so K and L instead
	if (GetAsyncKeyState('K') < 0) {
		normalShaderUsed = true;
	}
	if (GetAsyncKeyState('L') < 0) {
		normalShaderUsed = false; 
	}

	if (GetAsyncKeyState('M') < 0) {
		toonShaderUsed = true;
	}
	if (GetAsyncKeyState('N') < 0) {
		toonShaderUsed = false;
	}

	if (GetAsyncKeyState('V') < 0) {
		UIon = true;
	}
	if (GetAsyncKeyState('B') < 0) {
		UIon = false;
	}
	   
	if (GetAsyncKeyState('U') < 0)												// activate unlocked camera
	{
		firstPersonCameraOn = false;
		thirdPersonCameraOn = false;
		topDownCameraOn = false;
		unlockedCameraOn = true;
	}
	else if (GetAsyncKeyState('F') < 0)											// activate first person camera
	{
		thirdPersonCameraOn = false;
		unlockedCameraOn = false;
		topDownCameraOn = false;
		firstPersonCameraOn = true;
	}
	else if (GetAsyncKeyState('T') < 0)											// activate third person camera
	{
		unlockedCameraOn = false;
		firstPersonCameraOn = false;
		topDownCameraOn = false;
		thirdPersonCameraOn = true;
	} 
	else if (GetAsyncKeyState('O') < 0)											// activate top down camera
	{
		unlockedCameraOn = false;
		firstPersonCameraOn = false;		
		thirdPersonCameraOn = false;
		topDownCameraOn = true;
	}


	if (thirdPersonCameraOn)
	{
		distAhead = 4.0f;
		cameraAdditionalHeight = 2.0f;
		m_pCatmullRom->Sample(m_currentDistance + distAhead, pNext, game_cam_up);
	}
	else if (firstPersonCameraOn)
	{
		distAhead = -10.2f;
		cameraAdditionalHeight = -0.5f;
		m_pCatmullRom->Sample(m_currentDistance + distAhead, pNext, game_cam_up);
	}
	else if (topDownCameraOn)
	{
		distAhead = 4.5f;
		cameraAdditionalHeight = 19.0f;
		m_pCatmullRom->Sample(m_currentDistance + distAhead, pNext, game_cam_up);
	}

	   
	glm::vec3 splineCamera = glm::normalize(pNext - p);							// normalised vector that points  p -> pNext
	
	glm::vec3 T = glm::normalize(playerLookAt - pNext);							// TNB frame vectors
	glm::vec3 N = glm::normalize(glm::cross(T, glm::vec3(0, 1, 0)));
	glm::vec3 B = glm::normalize(glm::cross(T, N));

	playerForward = glm::mat4(glm::mat3(T, B, N));

	playerModelMatrix = glm::mat4();											// calculate the airship model position
	playerModelMatrix = glm::translate(pNext);	
	playerModelMatrix = glm::scale(playerModelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
	playerModelMatrix *= playerForward;


	pNext.y += 1.0f + cameraAdditionalHeight;									// set the camera slighlty higher so it overlooks more of the path

	//m_pCatmullRom->Sample(m_currentDistance, playerLookAt, cam_up_vector);	// sample towards horse
	//m_pCamera->Set(p, cam_veiwpoint, cam_up_vector);							// specify the camera view towrds horse	
	m_pCatmullRom->Sample(m_currentDistance + 10.0f, splineCamera, cam_up_vector);	// sample towards centreline
	   
	if (!unlockedCameraOn)
		m_pCamera->Set(pNext, splineCamera, cam_up_vector);						// set the camera to follow the spline
					
	m_pAudio->Update();
}




void Game::DisplayFrameRate()
{

	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d, %d", m_framesPerSecond, debugVal);
	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
	

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case '1':
			m_pAudio->PlayEventSound();
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
