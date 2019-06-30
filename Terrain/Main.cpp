#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Constants.h"
#include "Headers/Window.h"
#include "Headers/Model.h"
#include "Headers/Light.h"
#include "Headers/MarchingCube.h"
#include "Headers/Terrain.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

void updateDeltaTime();

const char* glsl_version = "#version 130";
// camera
Window window(Camera(glm::vec3(0.0f, 0.0f, 3.0f)), SCR_WIDTH, SCR_HEIGHT);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
	Shader lampShader("Shaders/lampShader.vs", "Shaders/lampShader.fs");
	Shader simpleShader("Shaders/simpleShader.vs", "Shaders/simpleShader.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
   // ------------------------------------------------------------------

	Mesh cubeMesh;
	cubeMesh.CreateCubeMesh();
	Light light(&simpleShader);
	Model lampModel(cubeMesh, lampShader);

	int x = 100, y = 100;
	Terrain terrain(x, y);
	terrain.generateTerrain();

	Mesh terrainMesh;
	terrainMesh.CreateMesh(terrain.vertices.data(), terrain.vertices.size() / 9);
	Model terrainModel(terrainMesh, simpleShader);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 lsmodel = glm::mat4(1.0f);

	static float a = 0.15f;
	static float d = 0.4f;
	static float s = 0.1f;

	// render loop
   // -----------
	while (!window.getShouldClose())
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// per-frame time logic and process input
		// --------------------
		updateDeltaTime();
		window.processInput(deltaTime); //TODO:Mybe we can solve this different, such that we don't have to put in delta time here
		// world transformation
		projection = glm::perspective(glm::radians(window.getCamera().Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = window.getCamera().GetViewMatrix();
		lsmodel = glm::mat4(1.0f);
		// clear screen and buffers
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set Ligh properties
		lsmodel = glm::rotate(lsmodel, (float)glfwGetTime(), glm::vec3(0, 1, 0));
		lsmodel = glm::translate(lsmodel, lightPos);
		lsmodel = glm::scale(lsmodel, glm::vec3(0.2f)); // a smaller cube

		light.setAmbient(glm::vec3(a));
		light.setDiffuse(glm::vec3(d));
		light.setSpecular(glm::vec3(s));
		light.setPosition(lightPos);
		light.setViewPos(window.getCamera().Position);

		//draw the lamp object
		lampModel.shader.useMVP(lsmodel, view, projection);
		lampModel.renderModel();

		/*if (oldIsoLevel != isoLevel) {
			mesh = mc.updateIsoLevel(isoLevel);
			perlinMesh.CreateMesh(mesh.data(), mesh.size() / 9);
		}*/

		
		//Draw simple cube
		terrainModel.shader.useLsMVP(lsmodel, view, projection);
		// light properties
		light.updateShader();
		terrainModel.updateColourShader();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5, -1, -3));
		model = glm::scale(model, glm::vec3(4 / ((float)x), 1, 4 / ((float)y)));
		terrainModel.shader.setMat4("model", model);
		terrainModel.renderModel();

		//Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{

			ImGui::Begin("Chose iso level");                  
			ImGui::SliderFloat("Ambient", &a, 0.0f, 1.0f); 
			ImGui::SliderFloat("Diffuse", &d, 0.0f, 1.0f);
			ImGui::SliderFloat("Specular", &s, 0.0f, 1.0f);

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.swapBuffers();
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	terrainMesh.~Mesh();
	window.~Window();
	return 0;
}

void updateDeltaTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}