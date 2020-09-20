#include "CCatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>


CCatmullRom::CCatmullRom()
{
	m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3);

	return a + b * t + c * t2 + d * t3;
}


void CCatmullRom::SetControlPoints()
{
	//m_controlPoints.push_back(glm::vec3(0, 0.05f, 100));
	//m_controlPoints.push_back(glm::vec3(0, 0.05f, -100));
	//m_controlPoints.push_back(glm::vec3(10, 0.05f, -130));


	// Set control points (m_controlPoints) here, or load from disk
	m_controlPoints.push_back(glm::vec3(-100, 0.5f, 0));
	m_controlPoints.push_back(glm::vec3(-80, 6.5f, 20));
	m_controlPoints.push_back(glm::vec3(0, 2.5f, 100));
	m_controlPoints.push_back(glm::vec3(100, 0.5f, 0));
	m_controlPoints.push_back(glm::vec3(90, 2.5f, -20));
	m_controlPoints.push_back(glm::vec3(40, 11.5f, -40));
	m_controlPoints.push_back(glm::vec3(20, 3.5f, -80));

	m_controlPoints.push_back(glm::vec3(0, 0.5f, -140));
	m_controlPoints.push_back(glm::vec3(-40, 0.5f, -60));
	m_controlPoints.push_back(glm::vec3(-60, 2.5f, -80));
	m_controlPoints.push_back(glm::vec3(-80, 0.5f, -100));
	m_controlPoints.push_back(glm::vec3(-100, 2.5f, -120));
	m_controlPoints.push_back(glm::vec3(-120, 0.5f, -90));
	m_controlPoints.push_back(glm::vec3(-110, 4.5f, -40));

	//m_controlPoints.push_back(glm::vec3(, 0.05f, ));

	//// >> Added here from tutorial - circle
	//m_controlPoints.push_back(glm::vec3(120, 0.05f, 0));
	//m_controlPoints.push_back(glm::vec3(71, 5.05f, 71));
	//m_controlPoints.push_back(glm::vec3(0, 10.05f, 100));
	//m_controlPoints.push_back(glm::vec3(-71, 3.05f, 71));

	//m_controlPoints.push_back(glm::vec3(-100, 0.05f, 0));
	//m_controlPoints.push_back(glm::vec3(-71, 0.05f, -71));
	//m_controlPoints.push_back(glm::vec3(0, 0.05f, -100));
	//m_controlPoints.push_back(glm::vec3(71, 0.05f, -71));


	//// Stright line Test track:
	//m_controlPoints.push_back(glm::vec3(-100, 0.05f, 100));
	//m_controlPoints.push_back(glm::vec3(100, 0.05f, 100));
	//m_controlPoints.push_back(glm::vec3(100, 0.05f, 0));
	//m_controlPoints.push_back(glm::vec3(-100, 0.05f, 0));
	//

	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)
	//m_controlUpVectors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	//m_controlUpVectors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	//m_controlUpVectors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	//m_controlUpVectors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

	//m_controlUpVectors.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); 
	//m_controlUpVectors.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); 
	//m_controlUpVectors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	//m_controlUpVectors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int)m_controlPoints.size();
	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);

	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
//glm::vec3 up = glm::vec3(0, 0, 0);
bool CCatmullRom::Sample(float d, glm::vec3& p, glm::vec3& up)
{
	if (d < 0)
		return false;

	int M = (int)m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size(); i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;

	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));


	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}

	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}

}


void CCatmullRom::CreateCentreline()
{
	// Set Control Points
	SetControlPoints();

	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(500);

	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);

	// Create the VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	int M = (int)m_controlPoints.size();
	for (int i = 0; i < M; i++)
	{
		float t = (float)i / M;
		glm::vec3 v = m_controlPoints[i];
		vbo.AddData(&v, sizeof(glm::vec3));				// add interpolated vector
		vbo.AddData(&texCoord, sizeof(glm::vec2));		// add texture coordinates
		vbo.AddData(&normal, sizeof(glm::vec3));		// add normal 
	}

	// Upload to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations -- i.e. all sizes summed up of every elem of a vertex
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);								// enable or disable a generic vertex attribute array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);	// define an array of generic vertex attribute data

	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));

	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::CreateOffsetCurves()
{
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.
	int points_size = m_centrelinePoints.size();

	for (int i = 0; i < points_size - 1; ++i)					// size - 1 to avoid overflow
	{
		glm::vec3 T = glm::normalize(m_centrelinePoints[i + 1] - m_centrelinePoints[i]);
		glm::vec3 N = glm::cross(T, glm::vec3(0, 1, 0));
		//glm::vec3 B = glm::normalize(glm::cross(T, N));		// not needed, removed to reduce computation

		m_leftOffsetPoints.push_back(m_centrelinePoints[i] - (m_pathWidth / 2) * N);
		m_rightOffsetPoints.push_back(m_centrelinePoints[i] + (m_pathWidth / 2) * N);
	}

	glGenVertexArrays(1, &m_vaoLeftOffsetCurve);				// left VAO/VBO code
	glBindVertexArray(m_vaoLeftOffsetCurve);

	CVertexBufferObject vbo_left;									
	vbo_left.Create();
	vbo_left.Bind();

	glm::vec2 texCoord_left(0.0f, 0.0f);
	glm::vec3 normal_left(0.0f, 1.0f, 0.0f);
		
	int M_left = (int)m_leftOffsetPoints.size();
	for (int i = 0; i < M_left; i++)
	{
		glm::vec3 v_left = m_leftOffsetPoints[i];
		vbo_left.AddData(&v_left, sizeof(glm::vec3));
		vbo_left.AddData(&texCoord_left, sizeof(glm::vec2));
		vbo_left.AddData(&normal_left, sizeof(glm::vec3));
	}

	vbo_left.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei stride_left = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_left, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride_left, (void*)sizeof(glm::vec3));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride_left, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));


	
	glGenVertexArrays(1, &m_vaoRightOffsetCurve);				// rightt VAO/VBO code
	glBindVertexArray(m_vaoRightOffsetCurve);

	CVertexBufferObject vbo_right;
	vbo_right.Create();
	vbo_right.Bind();

	glm::vec2 texCoord_right(0.0f, 0.0f);
	glm::vec3 normal_right(0.0f, 1.0f, 0.0f);

	int M_right = (int)m_rightOffsetPoints.size();
	for (int i = 0; i < M_right; i++)
	{
		glm::vec3 v_right = m_rightOffsetPoints[i];
		vbo_right.AddData(&v_right, sizeof(glm::vec3));
		vbo_right.AddData(&texCoord_right, sizeof(glm::vec2));
		vbo_right.AddData(&normal_right, sizeof(glm::vec3));
	}

	vbo_right.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei stride_right = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_right, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride_right, (void*)sizeof(glm::vec3));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride_right, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::CreateTrack()
{
	m_texture.Load("resources\\textures\\road.png", true);						// Load road texture
																				// texture taken from https://www.cgbookcase.com/textures/Street/Four_lane_road_wet_01/Four_lane_road_wet_01_2K_Base_Color.png
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Set parameters for texturing using sampler object
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);					// Taken from Plane.cpp
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);


	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card
	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);

	CVertexBufferObject vao_spline;
	vao_spline.Create();
	vao_spline.Bind();

	/*
	// ------------------------------------------------------------------------------------------------------------------------
	glm::vec2 texCoord00(0.0f, 0.0f);											// Specify tex coords for every 4 vertices
	glm::vec2 texCoord01(0.0f, 1.0f);
	glm::vec2 texCoord10(1.0f, 0.0f);
	glm::vec2 texCoord11(1.0f, 1.0f);

	for (int i = 0; i < m_vertexCount; ++i)										
	{																			
		m_splineVertices.push_back(m_leftOffsetPoints[i]);
		m_splineTexCoords.push_back(texCoord00);

		m_splineVertices.push_back(m_rightOffsetPoints[i]);
		m_splineTexCoords.push_back(texCoord01);

		m_splineVertices.push_back(m_rightOffsetPoints[i + m_splineVertexOffset]);
		m_splineTexCoords.push_back(texCoord11);


		m_splineVertices.push_back(m_leftOffsetPoints[i + m_splineVertexOffset]);
		m_splineTexCoords.push_back(texCoord10);

		m_splineVertices.push_back(m_leftOffsetPoints[i + m_splineVertexOffset]);
		m_splineTexCoords.push_back(texCoord10);

		m_splineVertices.push_back(m_rightOffsetPoints[i]);						
		m_splineTexCoords.push_back(texCoord01);	
	}
	*/
	// ------------------------------------------------------------------------------------------------------------------------
	 //Napishi path-a s novi tochki i dali kato se izdiga raboti

	glm::vec2 texCoord00(0.0f, 0.0f);											// Specify tex coords for every 4 vertices
	glm::vec2 texCoord01(0.0f, 1);
	glm::vec2 texCoord10(0.2f, 0);												// Values above 0.2 make the road too blurry, with more time 
	glm::vec2 texCoord11(0.2f, 1);												// a better texure PNG can be made as this cuts it too much
																				// but since it's very stretched looks nice and realistic enough this way
	m_vertexCount = (int)m_rightOffsetPoints.size() - m_splineVertexOffset;

	for (int i = 0; i < m_vertexCount; ++i)										// Push the coordinates and tex coords for the triangle that form the rectangle
	{																			// Order counter-clockwise for correct faces
		m_splineVertices.push_back(m_leftOffsetPoints[i]);
		m_splineTexCoords.push_back(texCoord00);

		m_splineVertices.push_back(m_rightOffsetPoints[i]);
		m_splineTexCoords.push_back(texCoord01);		

		m_splineVertices.push_back(m_leftOffsetPoints[i+ m_splineVertexOffset]);
		m_splineTexCoords.push_back(texCoord10);


		m_splineVertices.push_back(m_leftOffsetPoints[i + m_splineVertexOffset]);
		m_splineTexCoords.push_back(texCoord10);

		m_splineVertices.push_back(m_rightOffsetPoints[i]);						
		m_splineTexCoords.push_back(texCoord01);								

		m_splineVertices.push_back(m_rightOffsetPoints[i + m_splineVertexOffset]);
		m_splineTexCoords.push_back(texCoord11);
	}

	m_vertexCount = (int)m_splineVertices.size();
	glm::vec3 normalVector(0.0f, 1.0f, 0.0f);

	for (unsigned int i = 0; i < m_vertexCount; ++i)							// Add the vertex attributes into the VBO
	{
		vao_spline.AddData(&m_splineVertices[i], sizeof(glm::vec3));
		vao_spline.AddData(&m_splineTexCoords[i], sizeof(glm::vec2));
		vao_spline.AddData(&normalVector, sizeof(glm::vec3));
	}

	vao_spline.UploadDataToGPU(GL_STATIC_DRAW);									// Upload VBO to GPU
	
	GLsizei istride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);				// Set stride size
		
	glEnableVertexAttribArray(0);												// Vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);

	glEnableVertexAttribArray(1);												// Texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));

	glEnableVertexAttribArray(2);												// Normal vectors
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	
}


void CCatmullRom::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render it
	int M = (int)m_controlPoints.size();
	glLineWidth(1.0f);
	glBindVertexArray(m_vaoCentreline);

	//glDrawArrays(GL_LINE_STRIP, 0, M);	// 500 ctrl pts?
	glDrawArrays(GL_POINTS, 0, M);
	glDrawArrays(GL_LINE_LOOP, 0, M);
}


void CCatmullRom::RenderOffsetCurves()
{
	// Bind the VAO m_vaoLeftOffsetCurve and render it
	int M_left = (int)m_leftOffsetPoints.size();
	glPointSize(1.0f);
	glBindVertexArray(m_vaoLeftOffsetCurve);
	
	glDrawArrays(GL_POINTS, 0, M_left);
	glDrawArrays(GL_LINE_STRIP, 0, M_left);
	
	// Bind the VAO m_vaoRightOffsetCurve and render it
	int M_right = (int)m_rightOffsetPoints.size();
	glPointSize(1.0f);
	glBindVertexArray(m_vaoRightOffsetCurve);
	
	glDrawArrays(GL_POINTS, 0, M_right);
	glDrawArrays(GL_LINE_STRIP, 0, M_right);

}


void CCatmullRom::RenderTrack()
{
	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoTrack);
	m_texture.Bind();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);
	glDrawArrays(GL_LINE_STRIP, 0, m_vertexCount);
}


int CCatmullRom::CurrentLap(float d)
{
	return (int)(d / m_distances.back());
}