#include "terrain_manager.h"
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>


void TerrainManager::ShowInterface()
{
	float dispFactor, scaleFactor, frequency, grassCoverage, tessMultiplier, fogFalloff, power;
	int octaves;
	glm::vec3 rockColor;


	rockColor = glm::vec4(120, 105, 75, 255) * 1.5f / 255.f;
	power = 3.0;
	octaves = 13;
	frequency = 0.01;
	grassCoverage = 0.65;
	tessMultiplier = 1.0;
	dispFactor = 20.0;
	fogFalloff = 1.5;

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // render your GUI
	ImGui::Begin("Terrain controls: ");
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Terrain Controls");
	//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//ImGui::Checkbox("Clouds PostProc + God Rays", this->getPostProcPointer());
	ImGui::SliderInt("Octaves", &octaves, 1, 20);
	ImGui::SliderFloat("Frequency", &frequency, 0.0f, 0.05f);
	ImGui::SliderFloat("Displacement factor", &dispFactor, 0.0f, std::pow(32.f * 32.f * 32.f, 1 / power));
	ImGui::SliderFloat("Grass coverage", &grassCoverage, 0.0f, 1.f);
	ImGui::SliderFloat("Tessellation multiplier", &tessMultiplier, 0.1f, 5.f);
	ImGui::SliderFloat("Fog fall-off", &fogFalloff, 0.0f, 10.);
	ImGui::SliderFloat("Power", &power, 0.0f, 10.);

	//glm::vec3 * cloudBottomColor = this->getCloudColorBottomPtr();
	//ImGui::ColorEdit3("Cloud color", (float*)cloudBottomColor); // Edit 3 floats representing a color

	//ImGui::TextColored(ImVec4(1, 1, 0, 1), "Sky controls");
	ImGui::ColorEdit3("Rock color", (float*)&rockColor[0]); // Edit 3 floats representing a color
	//ImGui::ColorEdit3("Sky bottom color", (float*)this->getSkyBottomColorPtr()); // Edit 3 floats representing a color
    ImGui::End();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}