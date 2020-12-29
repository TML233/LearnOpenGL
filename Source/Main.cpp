#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

float factor = 0;
void OnWindowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void ProcessKey(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		factor += 0.05;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		factor -= 0.05;
	}
	if (factor > 1) {
		factor = 1;
	}
	if (factor < 0) {
		factor = 0;
	}
}

int main() {
#pragma region Init
	//Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Test", NULL, NULL);
	if (window == nullptr) {
		std::cout << "glfwCreateWindow failed!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "gladLoadGLLoader failed!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Prepare viewport and size change callback
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, OnWindowResize);
#pragma endregion

#pragma region Vertex array object
	//Templates that records your following operatings.
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
#pragma endregion

#pragma region Vertex buffer object
	float zoom = 0.f;
	float vertices[] = {
		//Pos               Color             TexCoord
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f + zoom, 1.0f - zoom,//¨I
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f - zoom, 1.0f - zoom,//¨J
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f + zoom, 0.0f + zoom,//¨L
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f - zoom, 0.0f + zoom//¨K
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion

#pragma region Element buffer object
	unsigned int order[] = {
		0,1,3,
		0,2,3
	};
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(order), order, GL_STATIC_DRAW);
#pragma endregion

#pragma region Vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
#pragma endregion

#pragma region Texture
	stbi_set_flip_vertically_on_load(true);
	//Container
	int widthContainer;
	int heightContainer;
	int channelsContainer;
	unsigned char* imgContainer = stbi_load("container.png", &widthContainer, &heightContainer, &channelsContainer, 0);

	unsigned int texContainer;
	glGenTextures(1, &texContainer);
	glBindTexture(GL_TEXTURE_2D, texContainer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthContainer, heightContainer, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgContainer);
	glGenerateMipmap(GL_TEXTURE_2D);
	//Set wrap mode -- S=X, T=Y, R=Z
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//Set filter & mipmap mode -- DO NOT USE MIPMAP FOR MAGNIFYING
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(imgContainer);
	std::cout << "Container image loaded!" << std::endl;

	//Face
	int widthFace;
	int heightFace;
	int channelsFace;
	unsigned char* imgFace = stbi_load("face.png", &widthFace, &heightFace, &channelsFace, 0);

	unsigned int texFace;
	glGenTextures(1, &texFace);
	glBindTexture(GL_TEXTURE_2D, texFace);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthFace, heightFace, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgFace);
	glGenerateMipmap(GL_TEXTURE_2D);
	//Set wrap mode -- S=X, T=Y, R=Z
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//Set filter & mipmap mode -- DO NOT USE MIPMAP FOR MAGNIFYING
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(imgFace);
	std::cout << "Face image loaded!" << std::endl;

#pragma endregion

#pragma region Vertex shader
	//Prepare vertex shader code
	GLchar vertexShaderCode[] = "#version 330 core\nlayout(location = 0) in vec3 inPos;\nlayout(location = 1) in vec3 inColor;\nlayout(location = 2) in vec2 inCoord;\nout vec3 ourColor;\nout vec2 texCoord;\nvoid main() {\ngl_Position = vec4(inPos, 1.0f);\nourColor=inColor;\ntexCoord=inCoord;\n}";
	const GLchar* vertextShaderPtr = vertexShaderCode;

	//Create, input, compile vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertextShaderPtr, nullptr);
	glCompileShader(vertexShader);

	//Check if vertex shader is compiled successfully
	int succeed;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succeed);
	if (!succeed) {
		char info[512];
		glGetShaderInfoLog(vertexShader, sizeof(info), NULL, info);
		std::cout << "Vertex shader failed to compile: " << info << std::endl;
	}
#pragma endregion

#pragma region Fragment shader
	//Prepare fragment shader code
	GLchar fragmentShaderCode[] = 
"#version 330 core\n\
in vec3 ourColor;\n\
in vec2 texCoord;\n\
out vec4 FragColor;\n\
uniform sampler2D mTex1;\n\
uniform sampler2D mTex2;\n\
uniform float factor;\n\
void main() {\n\
	FragColor = mix(texture(mTex1, texCoord),texture(mTex2, texCoord),factor);\n\
	//FragColor = texture(mTex2, texCoord);\n\
}";
	const GLchar* fragmentShaderPtr = fragmentShaderCode;

	//Create, input, compile fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderPtr, nullptr);
	glCompileShader(fragmentShader);

	//Check if fragment shader is compiled successfully
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succeed);
	if (!succeed) {
		char info[512];
		glGetShaderInfoLog(fragmentShader, sizeof(info), NULL, info);
		std::cout << "Fragment shader failed to compile: " << info << std::endl;
	}
#pragma endregion

#pragma region Shader program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//Check if shader program is linked successfully
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &succeed);
	if (!succeed) {
		char info[512];
		glGetProgramInfoLog(shaderProgram, sizeof(info), NULL, info);
		std::cout << "Shader program failed to compile: " << info << std::endl;
	}

	//After linking shader program, the shader is not useful for drawing
	//Delete the shaders if they are no longer used (YES YES YES)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#pragma endregion
	glBindVertexArray(0);

	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "mTex1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "mTex2"), 1);
	int factorLoc = glGetUniformLocation(shaderProgram, "factor");

	//Loop
	while (!glfwWindowShouldClose(window)) {
		ProcessKey(window);

		//Clear screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glUniform1f(factorLoc, factor);
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texContainer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texFace);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		
		std::this_thread::sleep_for(std::chrono::duration<float>(1 / 60.f));
	}

	//Cleanup
	glfwTerminate();
	return 0;
}