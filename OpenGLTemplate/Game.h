#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;
class CCube;
class CUI;
class CDiamond;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CPlane *m_pPlanarTerrain;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	COpenAssetImportMesh* m_pTigerMesh;
	COpenAssetImportMesh* m_pRockMesh;
	COpenAssetImportMesh* m_pStatueMesh;
	COpenAssetImportMesh* m_pTankMesh;
	COpenAssetImportMesh* m_pAircraftMesh;

	CSphere *m_pSphere;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CCatmullRom *m_pCatmullRom;

	CCube* m_pHouse;
	CUI* m_pUI;
	CDiamond* m_pDiamond;

	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;

	float m_currentDistance;				// Distance travelled along the control path  
	float m_cameraMovSpeed;					// Used in Update()

	glm::vec3 game_cam_up = glm::vec3(0, 0, 0);		// Copied from CCatmullRom.h with default value

	glm::vec3 p;							// p - vector for the calculated interpolation
	glm::vec3 pNext;						// pNext - point to look at
	glm::vec3 playerLookAt;					// players towards
	glm::mat4 playerForward;				// 4D matrix using TNB
	glm::mat4 playerModelMatrix;			// Aircraft model matrix
	

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;

	bool unlockedCameraOn = false;
	bool thirdPersonCameraOn = false;
	bool firstPersonCameraOn = true;
	bool topDownCameraOn = false;


	float distAhead;
	float cameraAdditionalHeight;

	double debugVal;

	bool normalShaderUsed;
	bool toonShaderUsed;
	bool UIon;
};
