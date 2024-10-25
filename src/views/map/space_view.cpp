#include "dibuligs/views/map/space_view.hpp"

SpaceView::SpaceView() {
	rotationMatrix = glm::mat4(1.0f);
}

SpaceView::~SpaceView() {}

void SpaceView::pre_view() {
  std::vector<float> vertices;
	int rows = 10, cols = 10;

	float rowStep = 10.0f / rows;
	float colStep = 10.0f / cols;

	for (int i = 0; i <= rows; ++i) {
		float y = -5.0f + i * rowStep;
		vertices.push_back(-5.0f);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(5.0f);
		vertices.push_back(y);
		vertices.push_back(0.0f);
	}

	for (int j = 0; j <= cols; ++j) {
		float x = -5.0f + j * colStep;
		vertices.push_back(x);
		vertices.push_back(-5.0f);
		vertices.push_back(0.0f);
		vertices.push_back(x);
		vertices.push_back(5.0f);
		vertices.push_back(0.0f);
	}

	const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\0";

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

	glUseProgram(shaderProgram);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);  
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	glBindVertexArray(VBO);

	glGenFramebuffers(1, &fb);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glViewport(0, 0, 800, 600);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer is not complete!" << std::endl;
	}

	// Draw the 3D view of the points
	model = glm::mat4(1.0f);
	cameraPosition = glm::vec3(0.0, cameraViewSphereRadius, cameraViewSphereRadius);
	cameraCenter = glm::vec3(0.0, 0.0, 0.0);
	upAxis = glm::vec3(0.0, 1.0, 0.0);
	projection = glm::perspective(glm::radians(50.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	
	glBindVertexArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SpaceView::view() {
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();
	float scale = 0.8;
	ImGui::BeginChild("SpaceView", ImVec2(vMax.x * scale, vMax.y * scale));
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	vMax = ImGui::GetWindowContentRegionMax();

	vMin.x += ImGui::GetWindowPos().x;
	vMin.y += ImGui::GetWindowPos().y;
	vMax.x += ImGui::GetWindowPos().x;
	vMax.y += ImGui::GetWindowPos().y;

	ImGuiIO& io = ImGui::GetIO();
	int mouseX = io.MousePos.x - vMin.x;
	int mouseY = io.MousePos.y - vMin.y;

	glm::vec3 rayDirection;
	bool mouseInSpaceview = false;
	
	if (ImGui::IsMousePosValid(&io.MousePos) && 
		io.MousePos.x >= vMin.x && io.MousePos.x <= vMax.x &&
		io.MousePos.y >= vMin.y && io.MousePos.y <= vMax.y) {
			mouseInSpaceview = true;
			// rotation actions
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
				mousePositionXRightMouse = io.MousePos.x;
				mousePositionYRightMouse = io.MousePos.y;
				isRightMouseDown = false;
			} else if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
				if (!isRightMouseDown) {
					mousePositionXRightMouse = io.MousePos.x;
					mousePositionYRightMouse = io.MousePos.y;
					isRightMouseDown = true;
				}
				float offsetX = 0.001 * (io.MousePos.x - mousePositionXRightMouse);
				float offsetY = 0.001 * (io.MousePos.y - mousePositionYRightMouse);

				// first get the camera direction vector
				glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraCenter);
				glm::vec3 cameraLeftAxis = glm::cross(upAxis, cameraDirection);

				glm::mat4 rotation1 = glm::rotate(rotationMatrix, offsetY, cameraLeftAxis);
				rotationMatrix = glm::rotate(rotation1, offsetX, upAxis);

				upAxis = glm::vec3(rotationMatrix[1]);
				cameraPosition = cameraCenter + glm::vec3(rotationMatrix[2]) * cameraViewSphereRadius;

				mousePositionXRightMouse = io.MousePos.x;
				mousePositionYRightMouse = io.MousePos.y;
			} if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
				isLeftMouseDown = false;
			}else if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				isRightMouseDown = false;
				if (!isLeftMouseDown) {
					isLeftMouseDown = true;
					mousePositionXLeftMouse = io.MousePos.x;
					mousePositionYLeftMouse = io.MousePos.y;

					originalCameraPosition = cameraPosition;
					originalCameraCenter = cameraCenter;
				}

				float offsetX = (io.MousePos.x - mousePositionXLeftMouse) * 0.01f;
				float offsetY = (io.MousePos.y - mousePositionYLeftMouse) * 0.01f;

				// first get the camera direction vector
				glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraCenter);
				glm::vec3 cameraRightAxis = glm::cross(upAxis, cameraDirection);

				glm::vec3 offsetXVec = cameraRightAxis * offsetX;
				glm::vec3 offsetYVec = upAxis * offsetY;

				cameraPosition = originalCameraPosition - offsetXVec;
				cameraPosition = cameraPosition - offsetYVec;

				cameraCenter = originalCameraCenter - offsetXVec;
				cameraCenter = cameraCenter - offsetYVec;
			} else if (io.MouseWheel != 0.0f) {
        cameraViewSphereRadius += io.MouseWheel * 0.1f;
				cameraViewSphereRadius = std::fmax(0.1f, cameraViewSphereRadius);
				cameraPosition = cameraCenter + cameraViewSphereRadius * glm::normalize(cameraPosition - cameraCenter);
    	}

			viewMat = glm::lookAt(cameraPosition, cameraCenter, upAxis);
			
			// check if there is any point pointed by the mouse
			glm::vec4 viewport = glm::vec4(0.0f, 0.0f, vMax.x - vMin.x, vMax.y - vMin.y);

			glm::vec3 nearWorld = glm::unProject(glm::vec3(mouseX, mouseY, .0f), viewMat, projection, viewport);
			glm::vec3 farWorld = glm::unProject(glm::vec3(mouseX, mouseY, 0.01f), viewMat, projection, viewport);

			rayDirection = glm::normalize(farWorld - nearWorld);
	}
	

  GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
	GLint pLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glViewport(0, 0, 800, 600);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(VAO);
	glColor3f(1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_LINES, 0, 22 * 2);
	glBindVertexArray(0);

	Eigen::Vector3d eigenCameraPosition(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	Eigen::Vector3d eigenRayDirection(rayDirection.x, rayDirection.y, rayDirection.z);

	double cloestDistance = 888888888888888.0f;
	int cloestPointId = -1;

	for (int pointIdx = 0; pointIdx < pointsVAO.size(); pointIdx++) {
    glGenVertexArrays(1, &pointsVAO[pointIdx]);
    glGenBuffers(1, &pointsVBO[pointIdx]);
    glGenBuffers(1, &pointsEBO[pointIdx]);

    glBindVertexArray(pointsVAO[pointIdx]);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO[pointIdx]);
    glBufferData(GL_ARRAY_BUFFER, pointVertices[pointIdx].size() * sizeof(float), pointVertices[pointIdx].data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointsEBO[pointIdx]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointIndices[pointIdx].size() * sizeof(unsigned int), pointIndices[pointIdx].data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(pointsVAO[pointIdx]);
		glDrawElements(GL_TRIANGLE_STRIP, pointIndices[pointIdx].size(), GL_UNSIGNED_INT, 0);

		// find the closest point
		if (mouseInSpaceview) {
			Eigen::Vector3d p = points[pointIdx];
			Eigen::Vector3d originToP = eigenCameraPosition - p;
			double distance = originToP.transpose() * originToP;

			double a = eigenRayDirection.transpose() * eigenRayDirection;
			double b = 2 * eigenRayDirection.transpose() * originToP;
			double c = distance - glCameraSphereRadius * glCameraSphereRadius;

			double discriminant = b * b - 4.0f * a * c;

			if (distance < cloestDistance && discriminant >= 0) {
				cloestPointId = pointIdx;
				cloestDistance = distance;
			}
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.f, 0.f, 0.f, 1.0f);

  ImGui::Image((void*)(intptr_t)textureColorbuffer, ImVec2(vMax.x - vMin.x, vMax.y - vMin.y), ImVec2(0, 0), ImVec2(1, 1));
	
	ImGui::EndChild();

	if (cloestPointId != -1) {
		ImGui::Text("Cloest point id: %d", cloestPointId);
	}
	ImGui::Text("mouseX: %d", mouseX);
	ImGui::Text("mouseY: %d", mouseY);
}

void SpaceView::after_first_view() {}

void SpaceView::post_view() {}

std::string SpaceView::get_name() {
  return "SpaceView";
}

void SpaceView::assignPointsPositions(std::vector<Eigen::Vector3d> points) {
	this->points = points;
	for (int idx = 0; idx < pointsVAO.size(); idx++) {
		glDeleteVertexArrays(1, &pointsVAO[idx]);
		glDeleteBuffers(1, &pointsVBO[idx]);
		glDeleteBuffers(1, &pointsEBO[idx]);
	}
	pointsVAO.clear();
	pointsVBO.clear();
	pointsEBO.clear();
	pointsVAO.resize(points.size());
	pointsVBO.resize(points.size());
	pointsEBO.resize(points.size());

	pointVertices.clear();
	pointIndices.clear();
	pointVertices.resize(points.size());
	pointIndices.resize(points.size());

	for (int pointIdx = 0; pointIdx < points.size(); pointIdx++) {
		GraphicUtils::getSphereVerticeData(points[pointIdx], glCameraSphereRadius, 10, 10, pointVertices[pointIdx], pointIndices[pointIdx]);
	}
}