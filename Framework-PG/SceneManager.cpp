#include "SceneManager.h"
#include <cstdlib>
#include <ctime>
#include <random>


//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static bool mouseClick = false;
static bool mouseReleased = false;
static const int QTY_OF_IMG_FILTERS = 7;
vector <string> stickerImgs = {"../stickers/astronaut.png","../stickers/charming-bart.png","../stickers/jake-the-dog-coffee.png","../stickers/lol-pepe.png","../stickers/fabulous.png","../stickers/sad-pepe.png"};
static const int QTY_OF_STICKERS = stickerImgs.size();

Sprite* img;
Sprite* background;


string imgFilePath; // C:\Users\I530715\Documents\ProcessamentoGrafico\TrabGB\imgs\ricknMorty.png

static GLuint width, height;


SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h, string filePath)
{
	imgFilePath = filePath;
	width = w;
	height = h;
	
	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Fotoxop", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");
	addShader("../shaders/transformations.vs", "../shaders/BinaryFilter.frag");
	addShader("../shaders/transformations.vs", "../shaders/RedFilter.frag");
	addShader("../shaders/transformations.vs", "../shaders/BlueFilter.frag");
	addShader("../shaders/transformations.vs", "../shaders/ColorFilter.frag");
	addShader("../shaders/transformations.vs", "../shaders/GrayScaleFilter.frag");
	addShader("../shaders/transformations.vs", "../shaders/InverseAndColorFilter.frag");
	addShader("../shaders/transformations.vs", "../shaders/InverseColorFilter.frag");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shaders.push_back(new Shader (vFilename.c_str(), fFilename.c_str()));
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouseClick = true;
		mouseReleased = false;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mouseClick = false;
		mouseReleased = true;
	}
}

void SceneManager::detectStickerSelection()
{
	if (mouseClick == true)
	{
		bool isClicked = false;
		for (int i = 0; i < stickers.size(); i++)
		{
			isClicked = detectClickOnObject(window, stickers[i]);
			if (isClicked) {
				
				//unsigned int stickerTexId = loadTexture(stickerImgs[i]);
				//Sprite* stickerToBePlaced = new Sprite;
				//stickerToBePlaced->setPosition(stickers[i]->pos);
				//stickerToBePlaced->setDimension(stickers[i]->scale);
				//stickerToBePlaced->transform = stickers[i]->transform;
				//stickerToBePlaced->setTexture(stickerTexId);
				//stickerToBePlaced->setShader(shaders[0]);

				do 
				{
					glfwPollEvents();
					double cursorXPos, cursorYPos;
					glfwGetCursorPos(window, &cursorXPos, &cursorYPos);
					stickers[i]->setPosition(glm::vec3(cursorXPos,height - cursorYPos,0.0));
					render();
				} while (!mouseReleased);

				return;
			}
		}
	}
}

void SceneManager::detectFilterSelection()
{
	if (mouseClick == true)
	{
		bool isClicked = false;
		for (int i = 0; i < filteredImgs.size(); i++)
		{
			isClicked = detectClickOnObject(window, filteredImgs[i]);
			if (isClicked) {

				img->setShader(filteredImgs[i]->shader);
				render();

				return;
			}
		}
	}
}

bool SceneManager::	detectClickOnObject(GLFWwindow* window, Sprite* object)
{
	double cursorXPos, cursorYPos;
	glfwGetCursorPos(window, &cursorXPos, &cursorYPos);
	

	glm::vec3 objectPos = object->pos;
	glm::vec3 objectDimensions = object->scale;
	GLfloat objectLeftCorner, objectRightCorner, objectUpperCorner, objectBottomCorner;
	objectLeftCorner = object->pos.x - object->scale.x / 2;
	objectRightCorner = object->pos.x + object->scale.x / 2;
	objectUpperCorner = object->pos.y + object->scale.y / 2;
	objectBottomCorner = object->pos.y - object->scale.y / 2;

	if(cursorXPos > objectLeftCorner && cursorXPos < objectRightCorner
		&& height-cursorYPos > objectBottomCorner && height-cursorYPos < objectUpperCorner)
	{
		return true;
	}
	return false;
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	background->shader->Use();
	background->update();
	background->draw();

	img->shader->Use();
	img->update();
	img->draw();

	for (int i = 0; i < filteredImgs.size(); i++)
	{
		filteredImgs[i]->setShader(shaders[i+1]);
		filteredImgs[i]->update();
		filteredImgs[i]->draw();
	}

	for (int i = 0; i < stickers.size(); i++)
	{
		stickers[i]->setShader(shaders[0]);
		stickers[i]->update();
		stickers[i]->draw();
	}
	glfwSwapBuffers(window);
	
}

void SceneManager::run()
{
	//Render scene
	render();

	// Swap the screen buffers
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		if (mouseClick)
		{
			detectFilterSelection();
			detectStickerSelection();
		}
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	unsigned int bgTexId = loadTexture("../imgs/grey-background.png");

	background = new Sprite;
	background->setPosition(glm::vec3(width/2, height/2, 0.0));
	background->setShader(shaders[0]);
	background->setDimension(glm::vec3(width, height, 1.0f));
	background->setTexture(bgTexId);

	unsigned int imgTexId = loadTexture(imgFilePath);

	img = new Sprite;
	img->setPosition(glm::vec3(width/2, height/2, 0.0));
	img->setShader(shaders[0]);
	img->setDimension(glm::vec3(500.0f, 500.0f, 1.0f));
	img->setTexture(imgTexId);

	Sprite* filteredImg;
	GLfloat xPositioning = width / (QTY_OF_IMG_FILTERS + 1);
	for (int i = 0; i < QTY_OF_IMG_FILTERS; i++)
	{
		filteredImg = new Sprite;
		filteredImg->setPosition(glm::vec3(xPositioning + i*xPositioning, 150.0f, 1.0));
		filteredImg->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
		filteredImg->setTexture(imgTexId);
		filteredImgs.push_back(filteredImg);
	}

	Sprite* sticker;
	xPositioning = width / (QTY_OF_STICKERS + 1);
	for (int i = 0; i < stickerImgs.size(); i++)
	{
		unsigned int stickerTexId = loadTexture(stickerImgs[i]);
		sticker = new Sprite;
		sticker->setPosition(glm::vec3(xPositioning + i * xPositioning, 800.0f, 1.0));
		sticker->setDimension(glm::vec3(70.0f, 70.0f, 1.0f));
		sticker->setTexture(stickerTexId);
		stickers.push_back(sticker);
	}
	

	//Definindo a janela do mundo (ortho2D)
	ortho2D[0] = 0.0f; //xMin
	ortho2D[1] = width; //xMax
	ortho2D[2] = 0.0f; //yMin
	ortho2D[3] = height; //yMax

	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void SceneManager::setupCamera2D() //TO DO: parametrizar aqui
{
	float zNear = -1.0, zFar = 1.0; //estão fixos porque não precisamos mudar

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);

	for (int i = 0; i < shaders.size(); i++) {
		shaders[i]->Use();
		//Obtendo o identificador da matriz de projeção para enviar para o shader
		GLint projLoc = glGetUniformLocation(shaders[i]->ID, "projection");
		//Enviando a matriz de projeção para o shader
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}
}

unsigned int SceneManager::loadTexture(string filename)
{
	unsigned int texture;

	// load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	


	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	return texture;
}
