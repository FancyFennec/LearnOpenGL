#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "Shader.h"
#include "stb_image.h"
#include "Window.h"
#include "Mesh.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

unsigned int loadTexture(char const * path);
void updateDeltaTime();

// camera
Window window(Camera(glm::vec3(0.0f, 0.0f, 3.0f)), SCR_WIDTH, SCR_HEIGHT);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

const char* glsl_version = "#version 130";

//TODO: This is not needed at the moment
void calcAverageNormals(unsigned int * indices, unsigned int indCount, float * vertices, unsigned int vertCount,
	unsigned int rowLength, unsigned int normalOffset);

int main()
{
	window.initialise();

	// Setup Dear ImGui context
	bool err = gladLoadGL() == 0;
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);

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

	// build and compile our shader program
	// ------------------------------------
	Shader lightingShader("Shaders/colorShader.vs", "Shaders/colorShader.fs");
	Shader lampShader("Shaders/lampShader.vs", "Shaders/lampShader.fs");

	 // set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
	
	Mesh cubeMesh(36);
	cubeMesh.CreateMesh(vertices);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO, lightVBO;
	glGenBuffers(1, &lightVAO);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int diffuseMap = loadTexture("../Resources/container2.png");
	unsigned int specularMap = loadTexture("../Resources/container2_specular.png");

	// shader configuration
	// --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// render loop
   // -----------
	while (!window.getShouldClose())
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// per-frame time logic
		// --------------------
		updateDeltaTime();


		// input
		// -----
		window.processInput(deltaTime); //TODO:Mybe we can solve this different, such that we don't have to put in delta time here

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects

		glm::mat4 lsmodel = glm::mat4(1.0f);
		lsmodel = glm::rotate(lsmodel, (float) glfwGetTime(), glm::vec3(0, 1, 0));
		lsmodel = glm::translate(lsmodel, lightPos);
		lsmodel = glm::scale(lsmodel, glm::vec3(0.2f)); // a smaller cube

		lightingShader.use();

		lightingShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
		lightingShader.setVec3("material.diffuse", 0.0f, 0.51f, 0.51f);
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setFloat("material.shininess", 32.0f);

		static float a = 0.0f;
		static float d = 0.0f;
		static float s = 0.0f;
		// light properties
		lightingShader.setVec3("light.ambient", a, a, a); // note that all light colors are set at full intensity
		lightingShader.setVec3("light.diffuse", d, d, d);
		lightingShader.setVec3("light.specular", s, s, s);
		lightingShader.setVec3("light.position", lightPos);
		lightingShader.setVec3("viewPos", window.getCamera().Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(window.getCamera().Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = window.getCamera().GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("lsmodel", lsmodel);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		for (glm::vec3 vec : cubePositions) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, vec);
			lightingShader.setMat4("model", model);

			// render the cube
			cubeMesh.RenderMesh();
		}

		// also draw the lamp object
		lampShader.use();

		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		lampShader.setMat4("model", lsmodel);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			
			ImGui::Begin("Imgui Test Window");                          // Create a window called "Hello, world!" and append into it.
			ImGui::SliderFloat("Ambient", &a, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Diffuse", &d, 0.0f, 1.0f);
			ImGui::SliderFloat("Specular", &s, 0.0f, 1.0f);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.swapBuffers();
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	cubeMesh.~Mesh();
	window.~Window();
	return 0;
}

void updateDeltaTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void calcAverageNormals(unsigned int * indices, unsigned int indCount, float * vertices, unsigned int vertCount,
	unsigned int rowLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indCount; i += 3)
	{
		unsigned int in0 = indices[i] * rowLength;
		unsigned int in1 = indices[i + 1] * rowLength;
		unsigned int in2 = indices[i + 2] * rowLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < vertCount / rowLength; i++)
	{
		unsigned int nOffset = i * rowLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}