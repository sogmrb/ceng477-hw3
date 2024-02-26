#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>   // The GL Header File
#include <GL/gl.h>   // The GL Header File
#include <GLFW/glfw3.h> // The GLFW header
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <random>
#define BUFFER_OFFSET(i) ((char*)NULL + (i))

using namespace std;

GLuint gProgram[3];
GLint gIntensityLoc;
float gIntensity = 1000;
int gWidth = 1000, gHeight = 800;
int width = 1000, height = 800;
struct Vertex
{
    Vertex(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
    GLfloat x, y, z;
};

struct Texture
{
    Texture(GLfloat inU, GLfloat inV) : u(inU), v(inV) { }
    GLfloat u, v;
};

struct Normal
{
    Normal(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
    GLfloat x, y, z;
};

struct Face
{
	Face(int v[], int t[], int n[]) {
		vIndex[0] = v[0];
		vIndex[1] = v[1];
		vIndex[2] = v[2];
		tIndex[0] = t[0];
		tIndex[1] = t[1];
		tIndex[2] = t[2];
		nIndex[0] = n[0];
		nIndex[1] = n[1];
		nIndex[2] = n[2];
	}
    GLuint vIndex[3], tIndex[3], nIndex[3];
};

vector<Vertex> gVertices;
vector<Texture> gTextures;
vector<Normal> gNormals;
vector<Face> gFaces;

GLuint gVertexAttribBuffer,  gIndexBuffer;
GLint gInVertexLoc, gInNormalLoc;
int gVertexDataSizeInBytes, gNormalDataSizeInBytes;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};



GLuint quadProgram; GLuint bunnyProgram;
GLuint textProgram;
std::map<GLchar, Character> Characters;
GLint modelingMatrixLoc;
GLint viewingMatrixLoc;
GLint projectionMatrixLoc;
GLint eyePosLoc;
GLint lightPositionLoc;
GLuint offsetLoc; 
GLuint scaleLoc; 
GLuint color1Loc; 
GLuint color2Loc; 
GLuint gTextVBO;

GLint modelingMatrixLoc_bunny ;
GLint viewingMatrixLoc_bunny ;
GLint projectionMatrixLoc_bunny ;
GLint eyePosLoc_bunny ;
GLint lightPositionLoc_bunny ;

glm::mat4 bunnyModelingMatrix;


glm::mat4 projectionMatrix;
glm::mat4 viewingMatrix;
glm::mat4 modelingMatrix;
glm::vec3 eyePos(0, 0, 0);
glm::vec3 lightPos(5,5,5);
glm::vec3 yellowText(1,1,0);
glm::vec3 redText(1,0,0);
int activeProgramIndex = 0;
float change = 0.0;
float speed_change = -0.07;
float bunny_x_change = 0.02;
float hop_speed = 0.01;
float rotate_speed = 7.0;
bool hop_down = 0;
float bunny_x = 0;
float cam = -0.3;
bool d_pressed = 0;
bool a_pressed = 0;
bool r_pressed = 0;
float bunny_z = -1.5;
float bunny_y = -0.9;
float quad_z = 8.0;
float cube_z = cam - 8.0;
float angle_bunny = 0;
float yellow_x0, yellow_x1, yellow_z0, yellow_z1;
float red1_x0, red1_x1, red1_z0, red1_z1;
float red2_x0, red2_x1, red2_z0, red2_z1;
float dead = 0;

bool hasCollidedYellow = 0;
bool hasCollidedRed = 0;
bool collidedRedR = 0;
bool collidedRedL = 0;
bool eatYellow = 0;

string scoreText = "Score: ";
int currentScore = 0;
float scoreChange = 0.5; 



vector<Vertex> quadVertices;
vector<Texture> quadTextures;
vector<Normal> quadNormals;
vector<Face> quadFaces;

vector<Vertex> bunnyVertices;
vector<Texture> bunnyTextures;
vector<Normal> bunnyNormals;
vector<Face> bunnyFaces;


GLuint quadVertexAttribBuffer, quadIndexBuffer;
GLint quadInVertexLoc, quadInNormalLoc;
int quadVertexDataSizeInBytes, quadNormalDataSizeInBytes;



GLuint bunnyVertexAttribBuffer, bunnyIndexBuffer;
GLint bunnyInVertexLoc, bunnyInNormalLoc;
int bunnyVertexDataSizeInBytes, bunnyNormalDataSizeInBytes;

GLuint cubeProgram_1, cubeProgram_2, cubeProgram_3;



GLint modelingMatrixLoc_cube_1 ;
GLint viewingMatrixLoc_cube_1 ;
GLint projectionMatrixLoc_cube_1 ;
GLint eyePosLoc_cube_1 ;
GLint lightPositionLoc_cube_1 ;

GLint modelingMatrixLoc_cube_2 ;
GLint viewingMatrixLoc_cube_2 ;
GLint projectionMatrixLoc_cube_2 ;
GLint eyePosLoc_cube_2 ;
GLint lightPositionLoc_cube_2 ;

GLint modelingMatrixLoc_cube_3 ;
GLint viewingMatrixLoc_cube_3 ;
GLint projectionMatrixLoc_cube_3 ;
GLint eyePosLoc_cube_3 ;
GLint lightPositionLoc_cube_3 ;


glm::mat4 modelingMatrix_1;
glm::mat4 modelingMatrix_2;
glm::mat4 modelingMatrix_3;


vector<Vertex> cubeVertices;
vector<Texture> cubeTextures;
vector<Normal> cubeNormals;
vector<Face> cubeFaces;


GLuint cubeVertexAttribBuffer, cubeIndexBuffer;
GLint cubeInVertexLoc, cubeInNormalLoc;
int cubeVertexDataSizeInBytes, cubeNormalDataSizeInBytes;

std::random_device rd;

std::mt19937 gen(rd());
std::uniform_int_distribution<int> distribution(1, 3);
static int isYellow = distribution(gen);





bool ReadDataFromFile(
	const string& fileName, ///< [in]  Name of the shader file
	string& data)     ///< [out] The contents of the file
{
	fstream myfile;

	// Open the input 
	myfile.open(fileName.c_str(), std::ios::in);

	if (myfile.is_open())
	{
		string curLine;

		while (getline(myfile, curLine))
		{
			data += curLine;
			if (!myfile.eof())
			{
				data += "\n";
			}
		}

		myfile.close();
	}
	else
	{
		return false;
	}

	return true;
}



bool ParseCubeObj(const string& fileName)
{
	fstream myfile;

	// Open the input 
	myfile.open(fileName.c_str(), std::ios::in);

	if (myfile.is_open())
	{
		string curLine;

		while (getline(myfile, curLine))
		{
			stringstream str(curLine);
			GLfloat c1, c2, c3;
			GLuint index[9];
			string tmp;

			if (curLine.length() >= 2)
			{
				if (curLine[0] == 'v')
				{
					if (curLine[1] == 't') // texture
					{
						str >> tmp; // consume "vt"
						str >> c1 >> c2;
						cubeTextures.push_back(Texture(c1, c2));
					}
					else if (curLine[1] == 'n') // normal
					{
						str >> tmp; // consume "vn"
						str >> c1 >> c2 >> c3;
						cubeNormals.push_back(Normal(c1, c2, c3));
					}
					else // vertex
					{
						str >> tmp; // consume "v"
						str >> c1 >> c2 >> c3;
						cubeVertices.push_back(Vertex(c1, c2, c3));
					}
				}
				else if (curLine[0] == 'f') // face
				{
					str >> tmp; // consume "f"
					char c;
					int vIndex[3], nIndex[3], tIndex[3];
					str >> vIndex[0]; str >> c >> c; // consume "//"
					str >> nIndex[0];
					str >> vIndex[1]; str >> c >> c; // consume "//"
					str >> nIndex[1];
					str >> vIndex[2]; str >> c >> c; // consume "//"
					str >> nIndex[2];

					assert(vIndex[0] == nIndex[0] &&
						vIndex[1] == nIndex[1] &&
						vIndex[2] == nIndex[2]); // a limitation for now

					// make indices start from 0
					for (int c = 0; c < 3; ++c)
					{
						vIndex[c] -= 1;
						nIndex[c] -= 1;
						tIndex[c] -= 1;
					}

					cubeFaces.push_back(Face(vIndex, tIndex, nIndex));
				}
				else
				{
					cout << "Ignoring unidentified line in obj file: " << curLine << endl;
				}
			}

			//data += curLine;
			if (!myfile.eof())
			{
				//data += "\n";
			}
		}

		myfile.close();
	}
	else
	{
		return false;
	}
	
	assert(cubeVertices.size() == cubeNormals.size());

	return true;
}

// stores the attributes vertex, texture, normals into structures from a file


bool ParseBunnyObj(const string& fileName)
{
	fstream myfile;

	// Open the input 
	myfile.open(fileName.c_str(), std::ios::in);

	if (myfile.is_open())
	{
		string curLine;

		while (getline(myfile, curLine))
		{
			stringstream str(curLine);
			GLfloat c1, c2, c3;
			GLuint index[9];
			string tmp;

			if (curLine.length() >= 2)
			{
				if (curLine[0] == 'v')
				{
					if (curLine[1] == 't') // texture
					{
						str >> tmp; // consume "vt"
						str >> c1 >> c2;
						bunnyTextures.push_back(Texture(c1, c2));
					}
					else if (curLine[1] == 'n') // normal
					{
						str >> tmp; // consume "vn"
						str >> c1 >> c2 >> c3;
						bunnyNormals.push_back(Normal(c1, c2, c3));
					}
					else // vertex
					{
						str >> tmp; // consume "v"
						str >> c1 >> c2 >> c3;
						bunnyVertices.push_back(Vertex(c1, c2, c3));
					}
				}
				else if (curLine[0] == 'f') // face
				{
					str >> tmp; // consume "f"
					char c;
					int vIndex[3], nIndex[3], tIndex[3];
					str >> vIndex[0]; str >> c >> c; // consume "//"
					str >> nIndex[0];
					str >> vIndex[1]; str >> c >> c; // consume "//"
					str >> nIndex[1];
					str >> vIndex[2]; str >> c >> c; // consume "//"
					str >> nIndex[2];

					assert(vIndex[0] == nIndex[0] &&
						vIndex[1] == nIndex[1] &&
						vIndex[2] == nIndex[2]); // a limitation for now

					// make indices start from 0
					for (int c = 0; c < 3; ++c)
					{
						vIndex[c] -= 1;
						nIndex[c] -= 1;
						tIndex[c] -= 1;
					}

					bunnyFaces.push_back(Face(vIndex, tIndex, nIndex));
				}
				else
				{
					cout << "Ignoring unidentified line in obj file: " << curLine << endl;
				}
			}

			//data += curLine;
			if (!myfile.eof())
			{
				//data += "\n";
			}
		}

		myfile.close();
	}
	else
	{
		return false;
	}
	
	assert(bunnyVertices.size() == bunnyNormals.size());

	return true;
}

// stores the attributes vertex, texture, normals into structures from a file
bool ParseQuadObj(const string& fileName)
{
	fstream myfile;

	// Open the input 
	myfile.open(fileName.c_str(), std::ios::in);

	if (myfile.is_open())
	{
		string curLine;

		while (getline(myfile, curLine))
		{
			stringstream str(curLine);
			GLfloat c1, c2, c3;
			GLuint index[9];
			string tmp;

			if (curLine.length() >= 2)
			{
				if (curLine[0] == 'v')
				{
					if (curLine[1] == 't') // texture
					{
						str >> tmp; // consume "vt"
						str >> c1 >> c2;
						quadTextures.push_back(Texture(c1, c2));
					}
					else if (curLine[1] == 'n') // normal
					{
						str >> tmp; // consume "vn"
						str >> c1 >> c2 >> c3;
						quadNormals.push_back(Normal(c1, c2, c3));
					}
					else // vertex
					{
						str >> tmp; // consume "v"
						str >> c1 >> c2 >> c3;
						quadVertices.push_back(Vertex(c1, c2, c3));
					}
				}
				else if (curLine[0] == 'f') // face
				{
					str >> tmp; // consume "f"
					char c;
					int vIndex[3], nIndex[3], tIndex[3];
					str >> vIndex[0]; str >> c >> c; // consume "//"
					str >> nIndex[0];
					str >> vIndex[1]; str >> c >> c; // consume "//"
					str >> nIndex[1];
					str >> vIndex[2]; str >> c >> c; // consume "//"
					str >> nIndex[2];

					assert(vIndex[0] == nIndex[0] &&
						vIndex[1] == nIndex[1] &&
						vIndex[2] == nIndex[2]); // a limitation for now

					// make indices start from 0
					for (int c = 0; c < 3; ++c)
					{
						vIndex[c] -= 1;
						nIndex[c] -= 1;
						tIndex[c] -= 1;
					}

					quadFaces.push_back(Face(vIndex, tIndex, nIndex));
				}
				else
				{
					cout << "Ignoring unidentified line in obj file: " << curLine << endl;
				}
			}

			//data += curLine;
			if (!myfile.eof())
			{
				//data += "\n";
			}
		}

		myfile.close();
	}
	else
	{
		return false;
	}
	
	assert(quadVertices.size() == quadNormals.size());

	return true;
}



GLuint initVertexShader(const char* shaderName)
{
	string shaderSource;

	string filename(shaderName);
	if (!ReadDataFromFile(filename, shaderSource))
	{
		cout << "Cannot find file name: " + filename << endl;
		exit(-1);
	}

	GLint length = shaderSource.length();
	const GLchar* shader = (const GLchar*)shaderSource.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &shader, &length);
	glCompileShader(vs);

	char output[1024] = { 0 };
	glGetShaderInfoLog(vs, 1024, &length, output);
	printf("VS compile log: %s\n", output);

	return vs;
}

GLuint initFragmentShader(const char* shaderName)
{
	string shaderSource;

	string filename(shaderName);
	if (!ReadDataFromFile(filename, shaderSource))
	{
		cout << "Cannot find file name: " + filename << endl;
		exit(-1);
	}

	GLint length = shaderSource.length();
	const GLchar* shader = (const GLchar*)shaderSource.c_str();

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &shader, &length);
	glCompileShader(fs);

	char output[1024] = { 0 };
	glGetShaderInfoLog(fs, 1024, &length, output);
	printf("FS compile log: %s\n", output);

	return fs;
}

void initCubeVertexBufferObjects()
{
	GLuint CubeVao;
	glGenVertexArrays(1, &CubeVao);
	assert(CubeVao > 0);
	glBindVertexArray(CubeVao);
	// cout << "Cube vao = " << CubeVao << endl;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NONE);

	glGenBuffers(1, &cubeVertexAttribBuffer);
	glGenBuffers(1, &cubeIndexBuffer);

	assert(cubeVertexAttribBuffer > 0 && cubeIndexBuffer > 0);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexAttribBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBuffer);

	cubeVertexDataSizeInBytes = cubeVertices.size() * 3 * sizeof(GLfloat);
	cubeNormalDataSizeInBytes = cubeNormals.size() * 3 * sizeof(GLfloat);
	int indexDataSizeInBytes = cubeFaces.size() * 3 * sizeof(GLuint);
	GLfloat* vertexData = new GLfloat[cubeVertices.size() * 3];
	GLfloat* normalData = new GLfloat[cubeNormals.size() * 3];
	GLuint* indexData = new GLuint[cubeFaces.size() * 3];

	float minX = 1e6, maxX = -1e6;
	float minY = 1e6, maxY = -1e6;
	float minZ = 1e6, maxZ = -1e6;

	for (int i = 0; i < cubeVertices.size(); ++i)
	{
		vertexData[3 * i] = cubeVertices[i].x;
		vertexData[3 * i + 1] = cubeVertices[i].y;
		vertexData[3 * i + 2] = cubeVertices[i].z;

		minX = std::min(minX, cubeVertices[i].x);
		maxX = std::max(maxX, cubeVertices[i].x);
		minY = std::min(minY, cubeVertices[i].y);
		maxY = std::max(maxY, cubeVertices[i].y);
		minZ = std::min(minZ, cubeVertices[i].z);
		maxZ = std::max(maxZ, cubeVertices[i].z);
	}

	// std::cout << "minX = " << minX << std::endl;
	// std::cout << "maxX = " << maxX << std::endl;
	// std::cout << "minY = " << minY << std::endl;
	// std::cout << "maxY = " << maxY << std::endl;
	// std::cout << "minZ = " << minZ << std::endl;
	// std::cout << "maxZ = " << maxZ << std::endl;

	for (int i = 0; i < cubeNormals.size(); ++i)
	{
		normalData[3 * i] = cubeNormals[i].x;
		normalData[3 * i + 1] = cubeNormals[i].y;
		normalData[3 * i + 2] = cubeNormals[i].z;
	}

	for (int i = 0; i < cubeFaces.size(); ++i)
	{
		indexData[3 * i] = cubeFaces[i].vIndex[0];
		indexData[3 * i + 1] = cubeFaces[i].vIndex[1];
		indexData[3 * i + 2] = cubeFaces[i].vIndex[2];
	}


	glBufferData(GL_ARRAY_BUFFER, cubeVertexDataSizeInBytes + cubeNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, cubeVertexDataSizeInBytes, vertexData);
	glBufferSubData(GL_ARRAY_BUFFER, cubeVertexDataSizeInBytes, cubeNormalDataSizeInBytes, normalData);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indexData, GL_STATIC_DRAW);

	// done copying to GPU memory; can free now from CPU memory
	delete[] vertexData;
	delete[] normalData;
	delete[] indexData;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(cubeVertexDataSizeInBytes));
}



void initBunnyVertexBufferObjects()
{
	GLuint BunnyVao;
	glGenVertexArrays(1, &BunnyVao);
	assert(BunnyVao > 0);
	glBindVertexArray(BunnyVao);
	// cout << "Bunny vao = " << BunnyVao << endl;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NONE);

	glGenBuffers(1, &bunnyVertexAttribBuffer);
	glGenBuffers(1, &bunnyIndexBuffer);

	assert(bunnyVertexAttribBuffer > 0 && bunnyIndexBuffer > 0);

	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexAttribBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBuffer);

	bunnyVertexDataSizeInBytes = bunnyVertices.size() * 3 * sizeof(GLfloat);
	bunnyNormalDataSizeInBytes = bunnyNormals.size() * 3 * sizeof(GLfloat);
	int indexDataSizeInBytes = bunnyFaces.size() * 3 * sizeof(GLuint);
	GLfloat* vertexData = new GLfloat[bunnyVertices.size() * 3];
	GLfloat* normalData = new GLfloat[bunnyNormals.size() * 3];
	GLuint* indexData = new GLuint[bunnyFaces.size() * 3];

	float minX = 1e6, maxX = -1e6;
	float minY = 1e6, maxY = -1e6;
	float minZ = 1e6, maxZ = -1e6;

	for (int i = 0; i < bunnyVertices.size(); ++i)
	{
		vertexData[3 * i] = bunnyVertices[i].x;
		vertexData[3 * i + 1] = bunnyVertices[i].y;
		vertexData[3 * i + 2] = bunnyVertices[i].z;

		minX = std::min(minX, bunnyVertices[i].x);
		maxX = std::max(maxX, bunnyVertices[i].x);
		minY = std::min(minY, bunnyVertices[i].y);
		maxY = std::max(maxY, bunnyVertices[i].y);
		minZ = std::min(minZ, bunnyVertices[i].z);
		maxZ = std::max(maxZ, bunnyVertices[i].z);
	}

	std::cout << "minX = " << minX << std::endl;
	std::cout << "maxX = " << maxX << std::endl;
	std::cout << "minY = " << minY << std::endl;
	std::cout << "maxY = " << maxY << std::endl;
	std::cout << "minZ = " << minZ << std::endl;
	std::cout << "maxZ = " << maxZ << std::endl;

	for (int i = 0; i < bunnyNormals.size(); ++i)
	{
		normalData[3 * i] = bunnyNormals[i].x;
		normalData[3 * i + 1] = bunnyNormals[i].y;
		normalData[3 * i + 2] = bunnyNormals[i].z;
	}

	for (int i = 0; i < bunnyFaces.size(); ++i)
	{
		indexData[3 * i] = bunnyFaces[i].vIndex[0];
		indexData[3 * i + 1] = bunnyFaces[i].vIndex[1];
		indexData[3 * i + 2] = bunnyFaces[i].vIndex[2];
	}


	glBufferData(GL_ARRAY_BUFFER, bunnyVertexDataSizeInBytes + bunnyNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bunnyVertexDataSizeInBytes, vertexData);
	glBufferSubData(GL_ARRAY_BUFFER, bunnyVertexDataSizeInBytes, bunnyNormalDataSizeInBytes, normalData);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indexData, GL_STATIC_DRAW);

	// done copying to GPU memory; can free now from CPU memory
	delete[] vertexData;
	delete[] normalData;
	delete[] indexData;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bunnyVertexDataSizeInBytes));
}


// stores the vertex, normals and indeces into the graphics card
void initVertexBufferObjects()
{
	GLuint QuadVao;
	glGenVertexArrays(1, &QuadVao);
	assert(QuadVao > 0);
	glBindVertexArray(QuadVao);
	cout << "Quad vao = " << QuadVao << endl;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NONE);

	glGenBuffers(1, &quadVertexAttribBuffer);
	glGenBuffers(1, &quadIndexBuffer);

	assert(quadVertexAttribBuffer > 0 && quadIndexBuffer > 0);

	glBindBuffer(GL_ARRAY_BUFFER, quadVertexAttribBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndexBuffer);

	quadVertexDataSizeInBytes = quadVertices.size() * 3 * sizeof(GLfloat);
	quadNormalDataSizeInBytes = quadNormals.size() * 3 * sizeof(GLfloat);
	int indexDataSizeInBytes = quadFaces.size() * 3 * sizeof(GLuint);
	GLfloat* vertexData = new GLfloat[quadVertices.size() * 3];
	GLfloat* normalData = new GLfloat[quadNormals.size() * 3];
	GLuint* indexData = new GLuint[quadFaces.size() * 3];

	float minX = 1e6, maxX = -1e6;
	float minY = 1e6, maxY = -1e6;
	float minZ = 1e6, maxZ = -1e6;

	for (int i = 0; i < quadVertices.size(); ++i)
	{
		vertexData[3 * i] = quadVertices[i].x;
		vertexData[3 * i + 1] = quadVertices[i].y;
		vertexData[3 * i + 2] = quadVertices[i].z;

		minX = std::min(minX, quadVertices[i].x);
		maxX = std::max(maxX, quadVertices[i].x);
		minY = std::min(minY, quadVertices[i].y);
		maxY = std::max(maxY, quadVertices[i].y);
		minZ = std::min(minZ, quadVertices[i].z);
		maxZ = std::max(maxZ, quadVertices[i].z);
	}

	std::cout << "minX = " << minX << std::endl;
	std::cout << "maxX = " << maxX << std::endl;
	std::cout << "minY = " << minY << std::endl;
	std::cout << "maxY = " << maxY << std::endl;
	std::cout << "minZ = " << minZ << std::endl;
	std::cout << "maxZ = " << maxZ << std::endl;

	for (int i = 0; i < quadNormals.size(); ++i)
	{
		normalData[3 * i] = quadNormals[i].x;
		normalData[3 * i + 1] = quadNormals[i].y;
		normalData[3 * i + 2] = quadNormals[i].z;
	}

	for (int i = 0; i < quadFaces.size(); ++i)
	{
		indexData[3 * i] = quadFaces[i].vIndex[0];
		indexData[3 * i + 1] = quadFaces[i].vIndex[1];
		indexData[3 * i + 2] = quadFaces[i].vIndex[2];
	}


	glBufferData(GL_ARRAY_BUFFER, quadVertexDataSizeInBytes + quadNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, quadVertexDataSizeInBytes, vertexData);
	glBufferSubData(GL_ARRAY_BUFFER, quadVertexDataSizeInBytes, quadNormalDataSizeInBytes, normalData);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indexData, GL_STATIC_DRAW);

	// done copying to GPU memory; can free now from CPU memory
	delete[] vertexData;
	delete[] normalData;
	delete[] indexData;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(quadVertexDataSizeInBytes));
}


GLuint initTextShader(const char* vertexShaderName, const char* fragmentShaderName){
		// Create the programs

	GLuint idProgramShader = glCreateProgram();

	GLuint idVertexShader   = initVertexShader(vertexShaderName);
	GLuint idFragmentShader = initFragmentShader(fragmentShaderName);

	// Attach the shaders to the programs

	glAttachShader(idProgramShader, idVertexShader);
	glAttachShader(idProgramShader, idFragmentShader);
	glBindAttribLocation(idProgramShader, 2, "vertex");

	// Link the programs

	glLinkProgram(idProgramShader);
	GLint status;
	glGetProgramiv(idProgramShader, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}
	return idProgramShader;

}
GLuint initShadersBunny(const char* vertexShaderName, const char* fragmentShaderName)
{
	// Create the programs

	GLuint idProgramShader = glCreateProgram();

	GLuint idVertexShader   = initVertexShader(vertexShaderName);
	GLuint idFragmentShader = initFragmentShader(fragmentShaderName);

	// Attach the shaders to the programs

	glAttachShader(idProgramShader, idVertexShader);
	glAttachShader(idProgramShader, idFragmentShader);


	// Link the programs

	glLinkProgram(idProgramShader);
	GLint status;
	glGetProgramiv(idProgramShader, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}


	// Get the locations of the uniform variables from both programs

	modelingMatrixLoc_bunny = glGetUniformLocation(idProgramShader, "modelingMatrix");
	viewingMatrixLoc_bunny = glGetUniformLocation(idProgramShader, "viewingMatrix");
	projectionMatrixLoc_bunny = glGetUniformLocation(idProgramShader, "projectionMatrix");
	eyePosLoc_bunny = glGetUniformLocation(idProgramShader, "eyePos");
    
	lightPositionLoc_bunny = glGetUniformLocation(idProgramShader, "lightPos");
	
	return idProgramShader;
}



GLuint initShaders(const char* vertexShaderName, const char* fragmentShaderName)
{
	// Create the programs

	GLuint idProgramShader = glCreateProgram();

	// Create the shaders for both programs

	//GLuint vs1 = createVS("vert.glsl");
	//GLuint fs1 = createFS("frag.glsl");
	GLuint idVertexShader   = initVertexShader(vertexShaderName);
	GLuint idFragmentShader = initFragmentShader(fragmentShaderName);

	// Attach the shaders to the programs

	glAttachShader(idProgramShader, idVertexShader);
	glAttachShader(idProgramShader, idFragmentShader);


	// Link the programs

	glLinkProgram(idProgramShader);
	GLint status;
	glGetProgramiv(idProgramShader, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}


	// Get the locations of the uniform variables from both programs

	modelingMatrixLoc = glGetUniformLocation(idProgramShader, "modelingMatrix");
	viewingMatrixLoc = glGetUniformLocation(idProgramShader, "viewingMatrix");
	projectionMatrixLoc = glGetUniformLocation(idProgramShader, "projectionMatrix");
	eyePosLoc = glGetUniformLocation(idProgramShader, "eyePos");
	lightPositionLoc = glGetUniformLocation(idProgramShader, "lightPos");
	offsetLoc = glGetUniformLocation(idProgramShader, "offset");
	scaleLoc = glGetUniformLocation(idProgramShader, "scale");
	color1Loc = glGetUniformLocation(idProgramShader, "color1");
	color2Loc = glGetUniformLocation(idProgramShader, "color2");
	
	return idProgramShader;
}

GLuint initShadersCube_1(const char* vertexShaderName, const char* fragmentShaderName)
{
	// Create the programs

	GLuint idProgramShader = glCreateProgram();

	// Create the shaders for both programs

	//GLuint vs1 = createVS("vert.glsl");
	//GLuint fs1 = createFS("frag.glsl");
	GLuint idVertexShader   = initVertexShader(vertexShaderName);
	GLuint idFragmentShader = initFragmentShader(fragmentShaderName);

	// Attach the shaders to the programs

	glAttachShader(idProgramShader, idVertexShader);
	glAttachShader(idProgramShader, idFragmentShader);


	// Link the programs

	glLinkProgram(idProgramShader);
	GLint status;
	glGetProgramiv(idProgramShader, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}


	// Get the locations of the uniform variables from both programs

	modelingMatrixLoc_cube_1 = glGetUniformLocation(idProgramShader, "modelingMatrix");
	viewingMatrixLoc_cube_1 = glGetUniformLocation(idProgramShader, "viewingMatrix");
	projectionMatrixLoc_cube_1 = glGetUniformLocation(idProgramShader, "projectionMatrix");
	eyePosLoc_cube_1 = glGetUniformLocation(idProgramShader, "eyePos");
	lightPositionLoc_cube_1 = glGetUniformLocation(idProgramShader, "lightPos");
	
	return idProgramShader;
}


GLuint initShadersCube_2(const char* vertexShaderName, const char* fragmentShaderName)
{
	// Create the programs

	GLuint idProgramShader = glCreateProgram();

	// Create the shaders for both programs

	//GLuint vs1 = createVS("vert.glsl");
	//GLuint fs1 = createFS("frag.glsl");
	GLuint idVertexShader   = initVertexShader(vertexShaderName);
	GLuint idFragmentShader = initFragmentShader(fragmentShaderName);

	// Attach the shaders to the programs

	glAttachShader(idProgramShader, idVertexShader);
	glAttachShader(idProgramShader, idFragmentShader);


	// Link the programs

	glLinkProgram(idProgramShader);
	GLint status;
	glGetProgramiv(idProgramShader, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}


	// Get the locations of the uniform variables from both programs

	modelingMatrixLoc_cube_2 = glGetUniformLocation(idProgramShader, "modelingMatrix");
	viewingMatrixLoc_cube_2 = glGetUniformLocation(idProgramShader, "viewingMatrix");
	projectionMatrixLoc_cube_2 = glGetUniformLocation(idProgramShader, "projectionMatrix");
	eyePosLoc_cube_2 = glGetUniformLocation(idProgramShader, "eyePos");
	lightPositionLoc_cube_2 = glGetUniformLocation(idProgramShader, "lightPos");
	
	return idProgramShader;
}

GLuint initShadersCube_3(const char* vertexShaderName, const char* fragmentShaderName)
{
	// Create the programs

	GLuint idProgramShader = glCreateProgram();

	// Create the shaders for both programs

	//GLuint vs1 = createVS("vert.glsl");
	//GLuint fs1 = createFS("frag.glsl");
	GLuint idVertexShader   = initVertexShader(vertexShaderName);
	GLuint idFragmentShader = initFragmentShader(fragmentShaderName);

	// Attach the shaders to the programs

	glAttachShader(idProgramShader, idVertexShader);
	glAttachShader(idProgramShader, idFragmentShader);


	// Link the programs

	glLinkProgram(idProgramShader);
	GLint status;
	glGetProgramiv(idProgramShader, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}


	// Get the locations of the uniform variables from both programs

	modelingMatrixLoc_cube_3 = glGetUniformLocation(idProgramShader, "modelingMatrix");
	viewingMatrixLoc_cube_3 = glGetUniformLocation(idProgramShader, "viewingMatrix");
	projectionMatrixLoc_cube_3 = glGetUniformLocation(idProgramShader, "projectionMatrix");
	eyePosLoc_cube_3 = glGetUniformLocation(idProgramShader, "eyePos");
	lightPositionLoc_cube_3 = glGetUniformLocation(idProgramShader, "lightPos");
	
	return idProgramShader;
}


void initCube(){
	ParseCubeObj("cube.obj");
    glEnable(GL_DEPTH_TEST);
	cubeProgram_1 = initShadersCube_1("vert_red.glsl", "frag_red.glsl");
    cubeProgram_2 = initShadersCube_2("vert_red.glsl", "frag_red.glsl");
    cubeProgram_3 = initShadersCube_3("vert_yellow.glsl", "frag_yellow.glsl");
	initCubeVertexBufferObjects();
}



void initQuad(){
	ParseQuadObj("quad.obj");
	
	glEnable(GL_DEPTH_TEST);
	quadProgram = initShaders("vert_quad.glsl", "frag_quad.glsl");
	initVertexBufferObjects();
	
	
}

void initBunny(){
	ParseBunnyObj("bunny.obj");
	glEnable(GL_DEPTH_TEST);
	bunnyProgram = initShadersBunny("vert.glsl", "frag.glsl");

	initBunnyVertexBufferObjects();
}



void drawQuadModel()
{
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexAttribBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(quadVertexDataSizeInBytes));

	glDrawElements(GL_TRIANGLES, quadFaces.size() * 3, GL_UNSIGNED_INT, 0);
}

void drawBunnyModel()
{
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexAttribBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bunnyVertexDataSizeInBytes));

	glDrawElements(GL_TRIANGLES, bunnyFaces.size() * 3, GL_UNSIGNED_INT, 0);
}
void drawCubeModel()
{
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexAttribBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(cubeVertexDataSizeInBytes));

	glDrawElements(GL_TRIANGLES, cubeFaces.size() * 3, GL_UNSIGNED_INT, 0);
}

void reset_state(){
	r_pressed = 0;
	hasCollidedRed = 0;
	collidedRedL = 0;
	collidedRedR = 0;
	eatYellow = 0;
	cam = -0.3;
	bunny_z = -1.5;
	bunny_y = -0.9;
	change = 0;
	speed_change = -0.07;
	hop_speed = 0.01;
	rotate_speed = 7.0;
	hop_down = 0;
	bunny_x = 0;
	quad_z = 8.0;
	cube_z = cam - 8.0;
	angle_bunny = 0;
	dead = 0;
    bunny_x_change = 0.02;
    currentScore = 0;
    scoreChange = 0.5; 


}



void reshape(GLFWwindow* window, int w, int h)
{
	w = w < 1 ? 1 : w;
	h = h < 1 ? 1 : h;

	gWidth = w;
	gHeight = h;

	glViewport(0, 0, w, h);

	// Use perspective projection
	float fovyRad = (float)(95.0 / 180.0) * M_PI;
	projectionMatrix = glm::perspective(fovyRad, w / (float)h, 1.0f, 100.0f);

	viewingMatrix = glm::lookAt(glm::vec3(0, -0.3, 0.0), glm::vec3(0, 0, 0) + glm::vec3(0, -0.3, -1.5), glm::vec3(0, 1, 0));

}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		activeProgramIndex = 0;
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		activeProgramIndex = 1;
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		glShadeModel(GL_FLAT);
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		glShadeModel(GL_SMOOTH);
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS){
		d_pressed = 1;
		
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS){
		a_pressed = 1;
	}
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE){
		d_pressed = 0;
		
	}
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE){
		a_pressed = 0;
	}
	else if(key == GLFW_KEY_R && action == GLFW_PRESS ){
		r_pressed = 1;
        
	}
}


void displayCube()
{

	
	
	static float offset = 0.2;
	static float cube_y = -0.8;
	//viewingMatrix = glm::lookAt(glm::vec3(0, 1, cameraPos), glm::vec3(0, 1, cameraPos) + glm::vec3(0, 0, -3), glm::vec3(0, 1, 0));

	// Compute the modeling matrix 
	glm::mat4 matT1; 
	glm::mat4 matT2; 
	glm::mat4 matT3; 

	if (isYellow == 1){
		matT1 = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, cube_y, cube_z));
		matT2 = glm::translate(glm::mat4(1.0), glm::vec3(-0.7f, cube_y, cube_z));
		matT3 = glm::translate(glm::mat4(1.0), glm::vec3(0.7f, cube_y, cube_z));

		yellow_x0 = 0.7 - offset, yellow_x1 = 0.7 + offset, yellow_z0 = cube_z - offset, yellow_z1 = cube_z + offset;
		red1_x0 = 0.0 - offset, red1_x1 = 0.0 + offset, red1_z0  = cube_z - offset, red1_z1 = cube_z + offset;
		red2_x0 = -0.7 - offset, red2_x1 = -0.7 + offset, red2_z0  = cube_z - offset, red2_z1 = cube_z + offset;

		
	}
	else if (isYellow==2){
		matT1 = glm::translate(glm::mat4(1.0), glm::vec3(0.7f, cube_y, cube_z));
		matT2 = glm::translate(glm::mat4(1.0), glm::vec3(-0.7f, cube_y, cube_z));
		matT3 = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, cube_y, cube_z));

		yellow_x0 = 0.0 - offset, yellow_x1 = 0.0 + offset, yellow_z0 = cube_z - offset, yellow_z1 = cube_z + offset;
		red1_x0 = 0.7 - offset, red1_x1 = 0.7 + offset, red1_z0  = cube_z - offset, red1_z1 = cube_z + offset;
		red2_x0 = -0.7 - offset, red2_x1 = -0.7 + offset, red2_z0  = cube_z - offset, red2_z1 = cube_z + offset;
	}
	else{
		matT1 = glm::translate(glm::mat4(1.0), glm::vec3(0.7f, cube_y, cube_z));
		matT2 = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, cube_y, cube_z));
		matT3 = glm::translate(glm::mat4(1.0), glm::vec3(-0.7f, cube_y, cube_z));

		yellow_x0 = -0.7 - offset, yellow_x1 = -0.7 + offset, yellow_z0 = cube_z - offset, yellow_z1 = cube_z + offset;
		red1_x0 = 0.7  - offset, red1_x1 = 0.7 + offset, red1_z0  = cube_z - offset, red1_z1 = cube_z + offset;
		red2_x0 = 0.0 - offset, red2_x1 = 0.0 + offset, red2_z0  = cube_z - offset, red2_z1 = cube_z + offset;
	}

	glm::mat4 matS = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.2, 0.1));
	glm::mat4 matR = glm::rotate<float>(glm::mat4(1.0), (5. / 180.) * M_PI, glm::vec3(1.0, 0.0, 0.0));
	modelingMatrix_1 = matT1 * matR*  matS;
    if(!collidedRedL){
		glUseProgram(cubeProgram_1);
		glUniformMatrix4fv(projectionMatrixLoc_cube_1, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(viewingMatrixLoc_cube_1, 1, GL_FALSE, glm::value_ptr(viewingMatrix));
		glUniformMatrix4fv(modelingMatrixLoc_cube_1, 1, GL_FALSE, glm::value_ptr(modelingMatrix_1));
		glUniform3fv(eyePosLoc_cube_1, 1, glm::value_ptr(eyePos));
		glUniform3fv(lightPositionLoc_cube_1, 1, glm::value_ptr(lightPos));
		drawCubeModel();
	}
	
	if(!collidedRedR){
		modelingMatrix_2 =  matT2 * matR*  matS;
		glUseProgram(cubeProgram_2);
		glUniformMatrix4fv(projectionMatrixLoc_cube_2, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(viewingMatrixLoc_cube_2, 1, GL_FALSE, glm::value_ptr(viewingMatrix));
		glUniformMatrix4fv(modelingMatrixLoc_cube_2, 1, GL_FALSE, glm::value_ptr(modelingMatrix_2));
		glUniform3fv(eyePosLoc_cube_2, 1, glm::value_ptr(eyePos));
		glUniform3fv(lightPositionLoc_cube_2, 1, glm::value_ptr(lightPos));
		drawCubeModel();
	}
	
	if(!eatYellow){
		modelingMatrix_3 =  matT3 * matR*  matS;

		glUseProgram(cubeProgram_3);
		glUniformMatrix4fv(projectionMatrixLoc_cube_3, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(viewingMatrixLoc_cube_3, 1, GL_FALSE, glm::value_ptr(viewingMatrix));
		glUniformMatrix4fv(modelingMatrixLoc_cube_3, 1, GL_FALSE, glm::value_ptr(modelingMatrix_3));
		glUniform3fv(eyePosLoc_cube_3, 1, glm::value_ptr(eyePos));
		glUniform3fv(lightPositionLoc_cube_3, 1, glm::value_ptr(lightPos));
	}
	



	if(cube_z >= cam-0.1) {
		isYellow = distribution(gen);
		cube_z = cam - 8;
		eatYellow = 0;
		// bunny_y = y0;
	}



	// Draw the scene
	drawCubeModel();





    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
    fprintf(stderr, "OpenGL Error: %d\n", error);
}




}




void displayBunny()
{

	if (hasCollidedYellow){
		angle_bunny += 5.0;
		if (angle_bunny >= 360){
			hasCollidedYellow = 0;
			angle_bunny = 0;
		}
		
	}


	
	
	glm::mat4 matT = glm::translate(glm::mat4(1.0), glm::vec3(bunny_x, bunny_y, bunny_z));
	glm::mat4 matS = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1));
	//glm::mat4 matT_2 = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 3.0f, -2.7f));
	glm::mat4 matR = glm::rotate<float>(glm::mat4(1.0), (-90. / 180.) * M_PI, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 matR2 = glm::rotate<float>(glm::mat4(1.0),  glm::radians(angle_bunny), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 matR3 = glm::rotate<float>(glm::mat4(1.0),  glm::radians(dead), glm::vec3(0.0, 0.0, 1.0));

	bunnyModelingMatrix=  matT * matR3 * matR2 * matR * matS ;

	viewingMatrix = glm::lookAt(glm::vec3(0, -0.3, cam), glm::vec3(0, 0, 0) + glm::vec3(0, -0.3, bunny_z), glm::vec3(0, 1, 0));
	glUseProgram(bunnyProgram);
	glUniformMatrix4fv(projectionMatrixLoc_bunny, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(viewingMatrixLoc_bunny, 1, GL_FALSE, glm::value_ptr(viewingMatrix));
	glUniformMatrix4fv(modelingMatrixLoc_bunny, 1, GL_FALSE, glm::value_ptr(bunnyModelingMatrix));
	glUniform3fv(eyePosLoc_bunny, 1, glm::value_ptr(eyePos));
	glUniform3fv(lightPositionLoc_bunny, 1, glm::value_ptr(lightPos));

	// Draw the scene
	drawBunnyModel();
	assert(glGetError() == GL_NO_ERROR);


    assert(glGetError() == GL_NO_ERROR);

	if (!hasCollidedRed){
		float t1 = bunny_z;
		bunny_z += speed_change;
		speed_change -= 0.00006;
        currentScore += scoreChange;
        scoreChange += 0.02;
		change = bunny_z - t1;
		cam += change;
		
		if (hop_down){
			bunny_y -= hop_speed;
			if (bunny_y <= -0.9) hop_down = 0;
		}
		else{
			bunny_y += hop_speed;
			if (bunny_y >= -0.8) hop_down = 1;
		}
		hop_speed += 0.000008;
		rotate_speed += 0.5;
        
		static int i = 0;
		static int j = 0;
		if(d_pressed){
			if (bunny_x < 0.8){
				
				if (i < 10){
					bunny_x += bunny_x_change;
					i++;
				}
				else{
					i = 0;
				}
			}
		}
		if (a_pressed){
			if (bunny_x > -0.8){
				
				if (j < 10){
					bunny_x -= bunny_x_change;
					j++;
				}
				else{
					j = 0;
				}
			}
		}
        
        bool increaseScore = 0;
        static bool alreadyIncreased = 0;
        

		if (bunny_x >= yellow_x0 && bunny_x <= yellow_x1 && bunny_z >= yellow_z0 && bunny_z <= yellow_z1){
            increaseScore = 1;
			hasCollidedYellow = 1;
            
			eatYellow = 1;
		}
        else alreadyIncreased = 0;
		
		if (bunny_x >= red1_x0 && bunny_x <= red1_x1 && bunny_z >= red1_z0 && bunny_z <= red1_z1){
			hasCollidedRed = 1;
			collidedRedL = 1;
		}
		if(bunny_x >= red2_x0 && bunny_x <= red2_x1 && bunny_z >= red2_z0 && bunny_z <= red2_z1){
			hasCollidedRed = 1;
			collidedRedR = 1;
		}
        if (increaseScore && !alreadyIncreased){
            currentScore+=1000;
            alreadyIncreased = 1;
        }
        bunny_x_change += 0.00002;


	}
	else{

		dead = -90;
		bunny_y = -0.8;
	}
	
}




void displayQuad()
{
	glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	static float angle = 90;
	float angleRad = (float)(angle / 180.0) * M_PI;

	// Compute the modeling matrix 
	glm::mat4 matT = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 0.0f, -4.0f));
	glm::mat4 matS = glm::scale(glm::mat4(1.0), glm::vec3(1.0, 25.0, 1.0));
	glm::mat4 matT_2 = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 3.0f, quad_z));

	glm::mat4 matR_2 = glm::rotate<float>(glm::mat4(1.0), -angleRad, glm::vec3(1.0f, 0.0f, 0.0f));
	modelingMatrix = matT_2* matR_2* matT * matS;
	int width = 1000, height = 800;
	// float fovyRad = (float)(95.0 / 180.0) * M_PI;
	// projectionMatrix = glm::perspective(fovyRad, width / (float)height, 1.0f, 100.0f);
	// Set the active program and the values of its uniform variables
	glm::vec3 offsetValue(1.0f, 0.0f, 0.0f); // Set your desired offset
	float scaleValue = 2.0f; // Set your desired scale
	glm::vec3 color1Value(0.5f, 0.67f, 0.95f); // Set your desired color1
	glm::vec3 color2Value(0.14f, 0.21f, 0.33f); // Set your desired color2
	
	glUseProgram(quadProgram);
	glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(viewingMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewingMatrix));
	glUniformMatrix4fv(modelingMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelingMatrix));
	glUniform3fv(eyePosLoc, 1, glm::value_ptr(eyePos));
	glUniform3fv(lightPositionLoc, 1, glm::value_ptr(lightPos));
	glUniform3fv(offsetLoc, 1, glm::value_ptr(offsetValue));
	glUniform1f(scaleLoc, scaleValue);
	glUniform3fv(color1Loc, 1, glm::value_ptr(color1Value));
	glUniform3fv(color2Loc, 1, glm::value_ptr(color2Value));

	// Draw the scene
	drawQuadModel();
	if(!hasCollidedRed) quad_z += change;
	//angle += 0.5;
	

}





bool ParseObj(const string& fileName)
{
    fstream myfile;

    // Open the input
    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open())
    {
        string curLine;

        while (getline(myfile, curLine))
        {
            stringstream str(curLine);
            GLfloat c1, c2, c3;
            GLuint index[9];
            string tmp;

            if (curLine.length() >= 2)
            {
                if (curLine[0] == '#') // comment
                {
                    continue;
                }
                else if (curLine[0] == 'v')
                {
                    if (curLine[1] == 't') // texture
                    {
                        str >> tmp; // consume "vt"
                        str >> c1 >> c2;
                        gTextures.push_back(Texture(c1, c2));
                    }
                    else if (curLine[1] == 'n') // normal
                    {
                        str >> tmp; // consume "vn"
                        str >> c1 >> c2 >> c3;
                        gNormals.push_back(Normal(c1, c2, c3));
                    }
                    else // vertex
                    {
                        str >> tmp; // consume "v"
                        str >> c1 >> c2 >> c3;
                        gVertices.push_back(Vertex(c1, c2, c3));
                    }
                }
                else if (curLine[0] == 'f') // face
                {
                    str >> tmp; // consume "f"
					char c;
					int vIndex[3],  nIndex[3], tIndex[3];
					str >> vIndex[0]; str >> c >> c; // consume "//"
					str >> nIndex[0];
					str >> vIndex[1]; str >> c >> c; // consume "//"
					str >> nIndex[1];
					str >> vIndex[2]; str >> c >> c; // consume "//"
					str >> nIndex[2];

					assert(vIndex[0] == nIndex[0] &&
						   vIndex[1] == nIndex[1] &&
						   vIndex[2] == nIndex[2]); // a limitation for now

					// make indices start from 0
					for (int c = 0; c < 3; ++c)
					{
						vIndex[c] -= 1;
						nIndex[c] -= 1;
						tIndex[c] -= 1;
					}

                    gFaces.push_back(Face(vIndex, tIndex, nIndex));
                }
                else
                {
                    cout << "Ignoring unidentified line in obj file: " << curLine << endl;
                }
            }

            //data += curLine;
            if (!myfile.eof())
            {
                //data += "\n";
            }
        }

        myfile.close();
    }
    else
    {
        return false;
    }

	/*
	for (int i = 0; i < gVertices.size(); ++i)
	{
		Vector3 n;

		for (int j = 0; j < gFaces.size(); ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				if (gFaces[j].vIndex[k] == i)
				{
					// face j contains vertex i
					Vector3 a(gVertices[gFaces[j].vIndex[0]].x,
							  gVertices[gFaces[j].vIndex[0]].y,
							  gVertices[gFaces[j].vIndex[0]].z);

					Vector3 b(gVertices[gFaces[j].vIndex[1]].x,
							  gVertices[gFaces[j].vIndex[1]].y,
							  gVertices[gFaces[j].vIndex[1]].z);

					Vector3 c(gVertices[gFaces[j].vIndex[2]].x,
							  gVertices[gFaces[j].vIndex[2]].y,
							  gVertices[gFaces[j].vIndex[2]].z);

					Vector3 ab = b - a;
					Vector3 ac = c - a;
					Vector3 normalFromThisFace = (ab.cross(ac)).getNormalized();
					n += normalFromThisFace;
				}

			}
		}

		n.normalize();

		gNormals.push_back(Normal(n.x, n.y, n.z));
	}
	*/

	assert(gVertices.size() == gNormals.size());

    return true;
}



void createVS(GLuint& program, const string& filename)
{
    string shaderSource;

    if (!ReadDataFromFile(filename, shaderSource))
    {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &shader, &length);
    glCompileShader(vs);

    char output[1024] = {0};
    glGetShaderInfoLog(vs, 1024, &length, output);
    printf("VS compile log: %s\n", output);

    glAttachShader(program, vs);
}

void createFS(GLuint& program, const string& filename)
{
    string shaderSource;

    if (!ReadDataFromFile(filename, shaderSource))
    {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &shader, &length);
    glCompileShader(fs);

    char output[1024] = {0};
    glGetShaderInfoLog(fs, 1024, &length, output);
    printf("FS compile log: %s\n", output);

    glAttachShader(program, fs);
}

void initShaders_text()
{
    // gProgram[0] = glCreateProgram();
    // gProgram[1] = glCreateProgram();
    gProgram[2] = glCreateProgram();


    createVS(gProgram[2], "vert_text.glsl");
    createFS(gProgram[2], "frag_text.glsl");

    
    glBindAttribLocation(gProgram[2], 2, "vertex");

    glLinkProgram(gProgram[2]);

}

void initVBO()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    assert(glGetError() == GL_NONE);

    glGenBuffers(1, &gVertexAttribBuffer);
    glGenBuffers(1, &gIndexBuffer);

    assert(gVertexAttribBuffer > 0 && gIndexBuffer > 0);

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

    gVertexDataSizeInBytes = gVertices.size() * 3 * sizeof(GLfloat);
    gNormalDataSizeInBytes = gNormals.size() * 3 * sizeof(GLfloat);
    int indexDataSizeInBytes = gFaces.size() * 3 * sizeof(GLuint);
    GLfloat* vertexData = new GLfloat [gVertices.size() * 3];
    GLfloat* normalData = new GLfloat [gNormals.size() * 3];
    GLuint* indexData = new GLuint [gFaces.size() * 3];

    float minX = 1e6, maxX = -1e6;
    float minY = 1e6, maxY = -1e6;
    float minZ = 1e6, maxZ = -1e6;

    for (int i = 0; i < gVertices.size(); ++i)
    {
        vertexData[3*i] = gVertices[i].x;
        vertexData[3*i+1] = gVertices[i].y;
        vertexData[3*i+2] = gVertices[i].z;

        minX = std::min(minX, gVertices[i].x);
        maxX = std::max(maxX, gVertices[i].x);
        minY = std::min(minY, gVertices[i].y);
        maxY = std::max(maxY, gVertices[i].y);
        minZ = std::min(minZ, gVertices[i].z);
        maxZ = std::max(maxZ, gVertices[i].z);
    }

    std::cout << "minX = " << minX << std::endl;
    std::cout << "maxX = " << maxX << std::endl;
    std::cout << "minY = " << minY << std::endl;
    std::cout << "maxY = " << maxY << std::endl;
    std::cout << "minZ = " << minZ << std::endl;
    std::cout << "maxZ = " << maxZ << std::endl;

    for (int i = 0; i < gNormals.size(); ++i)
    {
        normalData[3*i] = gNormals[i].x;
        normalData[3*i+1] = gNormals[i].y;
        normalData[3*i+2] = gNormals[i].z;
    }

    for (int i = 0; i < gFaces.size(); ++i)
    {
        indexData[3*i] = gFaces[i].vIndex[0];
        indexData[3*i+1] = gFaces[i].vIndex[1];
        indexData[3*i+2] = gFaces[i].vIndex[2];
    }


    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, vertexData);
    glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, normalData);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indexData, GL_STATIC_DRAW);

    // done copying; can free now
    delete[] vertexData;
    delete[] normalData;
    delete[] indexData;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));

}

void initFonts(int windowWidth, int windowHeight)
{
    // Set OpenGL options
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(windowWidth), 0.0f, static_cast<GLfloat>(windowHeight));
    glUseProgram(gProgram[2]);
    glUniformMatrix4fv(glGetUniformLocation(gProgram[2], "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationSerif-Italic.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
                );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    //
    // Configure VBO for texture quads
    //
    glGenBuffers(1, &gTextVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gTextVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void init()
{
	//ParseObj("armadillo.obj");
	// ParseObj("bunny.obj");

    // glEnable(GL_DEPTH_TEST);
    initShaders_text();
    initFonts(gWidth, gHeight);
    // initVBO();
}

void renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state
    glUseProgram(gProgram[2]);
    glUniform3f(glGetUniformLocation(gProgram[2], "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, gTextVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)

        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}


void display()
{
    string score_string = scoreText + to_string(currentScore);
    if(!hasCollidedRed) renderText(score_string, 0, 730, 0.5, yellowText);
    else renderText(score_string, 0, 730, 0.5, redText);
    

    assert(glGetError() == GL_NO_ERROR);
}

void mainLoop(GLFWwindow* window)

{
    while (!glfwWindowShouldClose(window))
	{
		if (r_pressed){
			reset_state();
		}
		displayQuad();
		displayBunny();
		displayCube();
        display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

int main(int argc, char** argv)   // Create Main Function For Bringing It All Together
{
	GLFWwindow* window;
	if (!glfwInit())
	{
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Simple Example", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	char rendererInfo[512] = { 0 };
	strcpy(rendererInfo, (const char*)glGetString(GL_RENDERER)); // Use strcpy_s on Windows, strcpy on Linux
	strcat(rendererInfo, " - "); // Use strcpy_s on Windows, strcpy on Linux
	strcat(rendererInfo, (const char*)glGetString(GL_VERSION)); // Use strcpy_s on Windows, strcpy on Linux
	glfwSetWindowTitle(window, rendererInfo);
    initQuad();
	initBunny();
	initCube();
    init();

    glfwSetKeyCallback(window, keyboard);
    glfwSetWindowSizeCallback(window, reshape);

    reshape(window, gWidth, gHeight); // need to call this once ourselves
    mainLoop(window); // this does not return unless the window is closed

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

