#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow* window);

const unsigned int Width = 800;
const unsigned int Height = 800;

const char *vertexshadersource = "#version 330 core\n"
								"layout (location = 0) in vec3 aPos;\n"
								"void main(){gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);}\n";

const char* fragmentshadersource = "#version 330 core\n"
								"out vec4 FragColor;\n"
								"void main(){ FragColor = vec4(1.0f,0.5f,0.2f,1.0f);}\n";

const char* fragmentshaderssource2 = "#version 330 core\n"
									"out vec4 FragColor2;\n"
									"void main(){ FragColor2 = vec4(1.0f,1.0f,0.0f,1.0f);}\n";

int main() {

	//Intializing the glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	//Creating Window ....
	GLFWwindow* window = glfwCreateWindow(Width, Height, "Master | PRESS 'Q' to Quit!", NULL, NULL);

	if (window == NULL) {
		std::cout << "Window Creation Failed To Execute!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Make Current window as context
	glfwMakeContextCurrent(window);

	//Window resize frame buffer callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initalize GLAD and load OpenGL Function Pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed To Load GLAD!" << std::endl;
		return -1;
	}

	//Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexshadersource,NULL);
	glCompileShader(vertexShader);

	int success;
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "Failed To Compile Vertex Shader!" << infolog<<std::endl;
		return -1;
	}

	//Fragment Shader 
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentshadersource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "Failed To Compile Fragment Shader!" << std::endl;
		return -1;
	}

	//Second Fragment shader
	unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentshaderssource2,NULL);
	glCompileShader(fragmentShader2);

	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infolog);
		std::cout << "Failde To Compile Fragment Shader 2!" << std::endl;
		return -1;
	}

	//Link Shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cout << "Failed to Link the Shader Programs" << std::endl;
		return -1;
	}

	//Link Shaders2
	unsigned int shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infolog);
		std::cout << "Failed to Link the Shader Programs" << std::endl;
		return -1;
	}

	//Remove shader!
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);
	
	//Vertices data
	float vertices1[] = {
		0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.3f, 0.0f,  // bottom right
		//-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,  // top left
		/*0.1f,-0.1f,0.0f,
		0.1f,-0.3f,0.0f,
		-0.1f,-0.3f,0.0f*/
	};

	float vertices2[] = {
		0.5f,-0.3f,0.0f,
		0.5f,-0.7f,0.0f,
		-0.5f,-0.7f,0.0f
	};

	//Creating Indices for elements.
	unsigned int indices[] = {
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	//Creating Objects

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind Vertex Array and configure Vertux Buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3* sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Creating One more VAO and VBO to draw One more triangle!

	unsigned int VBO1, VAO1;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);

	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Build Render Engine
	while (!glfwWindowShouldClose(window)) {
		
		//Input Data!
		processInput(window);

		//render
		glClearColor(0.1f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw a geometry!
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width,int heigth) {
	glViewport(0, 0, width, heigth);
}

void processInput(GLFWwindow* window) {

	//Close Window.
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window,true);
	}

	//Polygon Mode.
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {

		GLint polygonMode [2];
		glGetIntegerv(GL_POLYGON_MODE, polygonMode);
		/*std::cout << polygonMode[0] << polygonMode[1];
		if (polygonMode[0] == GL_FILL && polygonMode[1] == GL_FILL) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}*/
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//Polygon Mode Fill.
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_RELEASE) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}