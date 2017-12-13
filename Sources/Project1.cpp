/*
	Project 1 Submission for CMPSC458
    Name: Joe Smith
	psu id: xyz123
*/
#include <string.h>
#include <map>

#include <Project1.hpp>

// globals 
	// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



	// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

	// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

	// for reading
const int Positive = 1;
const int Negative = -1;
const int Rotate = 0;
const int Scale = 1;
const int Translation = 2;
const int X = 0;
const int Y = 1;
const int Z = 2;

	//for box control
const float base = 0.001f;	//a default value, need to be smaller than 0.5
int interaction[3] = { 0, 0, -1 };	//for controling the box, [axies, P or N, way]
float boxes[3][3] = { {0,0,0},{1,1,1},{0,0,0} };	//all box history, initial value for box. 2nd is scale need to be 1/base	
float rotation[3] = { 0,0,0 }; //for speed of the rotation

	//for texture
const int TEXTURE = 8;
unsigned int textures[TEXTURE];
const std::string path[] = { 
	"../CMPSC458_Projects/Media/textures/container.jpg",
	"../CMPSC458_Projects/Media/textures/awesomeface.png",
	"../CMPSC458_Projects/Media/skybox/front.jpg",
	"../CMPSC458_Projects/Media/skybox/left.jpg",
	"../CMPSC458_Projects/Media/skybox/back.jpg",
	"../CMPSC458_Projects/Media/skybox/right.jpg",
	"../CMPSC458_Projects/Media/skybox/top.jpg",
	"../CMPSC458_Projects/Media/skybox/bottom.jpg",
};

	//for skybox
float angel[] = { 0.0f, -90.0f, -90.0f, -90.0f };

	//for moving along height map
bool walking = false;
bool sway = true;

	//for keyboard clicking
std::map<char,bool> key;

// init heightmap (defined in heightmap.hpp)
//Heightmap heightmap("../CMPSC458_Projects/Media/heightmaps/hflab4.jpg");

int Project1::Project1()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// for anti aliasing
	glfwWindowHint(GLFW_SAMPLES, 16);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X (left here as legacy, more would need to change)
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project 1: Heightmap", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, Project1::framebuffer_size_callback);
	glfwSetCursorPosCallback(window, Project1::mouse_callback);
	glfwSetScrollCallback(window, Project1::scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	// build and compile our shader program (defined in shader.hpp)
	// ------------------------------------
	Shader ourShader("../CMPSC458_Projects/Shaders/shader.vert", "../CMPSC458_Projects/Shaders/shader.frag");

	// set up vertex data (and buffer(s)) and configure vertex attributes for boxes
	// ------------------------------------------------------------------

	//   3?D Coordinates   | Texture Coordinates
	//    x     y     z   |  s     t  
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	//  1.  Create ID / Generate Buffers and for Vertex Buffer Object (VBO), 
	//      Vertex Array Buffer (VAO), and the Element Buffer Objects (EBO)
	unsigned int VBO, VAO, EBO;

	// 2. Bind Vertex Array Object
	glGenVertexArrays(1, &VAO);

	//  Bind the Vertex Buffer
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	// 3. Copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 4. Copy our indices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 5.  Position attribute for the 3D Position Coordinates and link to position 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 6.  TexCoord attribute for the 2d Texture Coordinates and link to position 2
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// load and create a texture 
	// -------------------------

	for (int i = 0; i < TEXTURE; i++)
	{
		glGenTextures(1, &textures[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
												// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		unsigned char *data = stbi_load(path[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			if (i==1)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(data);
	}
	
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
					 // either set it manually like so:
	//Setting it using the texture class
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// init heightmap (defined in heightmap.hpp)
	Heightmap heightmap("../CMPSC458_Projects/Media/heightmaps/hflab4.jpg");	
	

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		//for walking
		if (walking)
		{
			int x = int((camera.Map_Position.x / 200.0f + 0.5f)* float(heightmap.width));
			int y = int((camera.Map_Position.z / 200.0f + 0.5f)* float(heightmap.height));
			float temp_h = heightmap.findHeight(x, y);
			if (temp_h - camera.Map_Position.y > 0.01f)
			{
				if (sway)
					camera.Sway(LEFT);
				else
					camera.Sway(RIGHT);
				sway = !sway;
			}
			camera.Map_Position.y = temp_h;
			camera.Position.y = temp_h;
		}


		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		Project1::processInput(window);

		// render
		// ------
		// Set background color (shouldn't need it in the end b/c the box should fully cover everything
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.3f, 0.0f);

		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units in fragment shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		

		// activate shader
		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		
		// render boxes
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;

			
			// Translate the model to the cube starting position
			model = glm::translate(model, cubePositions[i]);


			// for interaction of the box
			if (interaction[1] != 0 && interaction[2] >=0) {
				boxes[interaction[2]][interaction[0]] += base*interaction[1];
			}

			
			//scale
			model = glm::scale(model, glm::vec3(boxes[Scale][X], boxes[Scale][Y], boxes[Scale][Z]));
			

			//translation
			model = glm::translate(model, glm::vec3(boxes[Translation][X], boxes[Translation][Y], boxes[Translation][Z]));
			
			//rotation
			for (int i = 0; i < 3; i++)
				rotation[i] += boxes[Rotate][i];
			model = glm::rotate(model, glm::radians(rotation[X]), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation[Y]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation[Z]), glm::vec3(0.0f, 0.0f, 1.0f));


			// Rotate the cube by an angle
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			// Set model in shader
			ourShader.setMat4("model", model);

			// Draw the box with triangles
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		

		// Make the model for one wall and shift/scale it
		glm::mat4 model;

		model = glm::translate(model, camera.Position);
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));

		for (int i = 2; i < TEXTURE-2; i++)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			model = glm::rotate(model, glm::radians(angel[i-2]), glm::vec3(0.0f, 1.0f, 0.0f));

			// Set model in shader
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		//for top
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[6]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[6]);

		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// Set model in shader
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//for bottom
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[7]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[7]);

		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// Set model in shader
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
				

		// Draw the heightmap (defined in heightmap.hpp)  Similar to above but you have to write it.
		heightmap.Draw(ourShader, textures[7]);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Project1::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// reset interaction 
	memset(interaction, 0, sizeof(interaction));
	//receive more input
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		interaction[2] = Scale;
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		interaction[2] = Translation;
	else
		interaction[2] = Rotate;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		interaction[0] = X;
		interaction[1] = Positive;
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		interaction[0] = X;
		interaction[1] = Negative;
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		interaction[0] = Y;
		interaction[1] = Positive;
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		interaction[0] = Y;
		interaction[1] = Negative;
	}

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		interaction[0] = Z;
		interaction[1] = Positive;
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		interaction[0] = Z;
		interaction[1] = Negative;
	}

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		key['v'] = true;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) {
		if (key['v']) walking = !walking;
		key['v'] = false;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Project1::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Project1::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Project1::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}






