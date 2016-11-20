#define WIN32_LEAN_AND_MEAN 
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>
#include <cstdlib>

#include "glad/glad.h"

#include "SampleApplication.h"
#include"FixedFunctionPrimitives.h"

static SampleApplication debugInstance("Sample Application", 800, 600);

// TODO: REMOVE
#include <iostream>
Mesh meshObject;
Model modelObject;
AABB ground;
Scene* scene;
// END TODO

float SampleApplication::random(float min, float max) {
	if (max < min) {
		float t = min;
		min = max;
		max = t;
	}

	float random = ((float)rand()) / (float)RAND_MAX;
	
	float range = max - min;
	return (random*range) + min;
}

void SampleApplication::OnInitialize() {
	GLWindow::OnInitialize();

	camera.Perspective(60.0f, (float)m_nWidth / (float)m_nHeight, 0.01f, 1000.0f);

	cameraDist = -10.0f;
	//camera.LookAt(vec3(cameraPos.x, cameraPos.y, cameraDist), vec3(), vec3(0.0f, 1.0f, 0.0f));
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);

	glPointSize(3.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);
	val[0] = 0.0f; val[1] = 1.0f; val[2] = 0.0f; val[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_AMBIENT, val);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, val);
	val[0] = 1.0f; val[2] = 1.0f;
	glLightfv(GL_LIGHT0, GL_SPECULAR, val);


	
	LoadMesh("Assets/suzane.mdl", &meshObject);
	modelObject.SetContent(&meshObject);
	modelObject.position = vec3(0, 0, 0.0);
	modelObject.rotation = vec3(0.0f, 0.0f, 0.0f);
	ground = FromMinMax(vec3(-5, -2, -5), vec3(5, -1, 5));

	scene = new Scene();
	scene->Accelerate(vec3(), 500);
}

void SampleApplication::OnShutdown() {
	delete scene;
	FreeMesh(&meshObject);
	//FreeBVHNode(meshObject.accelerator);
	//delete meshObject.accelerator;
}

void SampleApplication::OnResize(int width, int height) {
	GLWindow::OnResize(width, height);

	mat4 projection = camera.GetProjectionMatrix();
	mat4 view = camera.GetViewMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection.asArray);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(view.asArray);
}

void SampleApplication::OnRender() {
	GLWindow::OnRender();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.GetViewMatrix().asArray);

	mat4 debugMat = LookAt(vec3(0.0f, 0.0f, -15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//glLoadMatrixf(debugMat.asArray);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.0f, 0.0f, 1.0f);
	float val[] = {0, 1, 0, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, val);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, val);
	Render(modelObject);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0f, 0.0f, 0.0f);
	val[0] = 1; val[1] = 0;
	glLightfv(GL_LIGHT0, GL_AMBIENT, val);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, val);
	Render(GetOBB(modelObject));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.0f, 0.0f, 1.0f);
	val[0] = 0; val[2] = 1;
	glLightfv(GL_LIGHT0, GL_AMBIENT, val);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, val);
	Render(ground);


	/*glColor3f(1.0f, 0.0f, 0.0f);
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Render(*meshObject.accelerator);
	glEnable(GL_LIGHTING);*/

	FixedFunctionOrigin(true, false);
}

void SampleApplication::OnUpdate(float deltaTime) {
	GLWindow::OnUpdate(deltaTime);

	if (KeyDown(KEY_ONE)) {
		cameraDist = -10.0f;
		cameraPos = vec2();
	}
	else if (KeyDown(KEY_TWO)) {
		cameraPos = vec2(14, 14);
		cameraDist = -14.0f;
	}
	else if (KeyDown(KEY_THREE)) {
		float size = 2.0f;
		float aspect = (float)m_nWidth / (float)m_nHeight;
		camera.Orthographic(size * aspect, size, 100, -100);
	}
	else if (KeyDown(KEY_FOUR)) {
		camera.Perspective(60.0f, camera.GetAspect(), 0.01f, 1000.0f);
	}

	if (MouseButonDown(MOUSE_LEFT)) {
		cameraDist += 3.0f * deltaTime;
	} else if (MouseButonDown(MOUSE_RIGHT)) {
		cameraDist -= 3.0f * deltaTime;
	}

	if (KeyDown(KEY_DOWN_ARROW)) {
		cameraPos.y += -5.0f * deltaTime;
	}
	else if (KeyDown(KEY_UP_ARROW)) {
		cameraPos.y += 5.0f * deltaTime;
	}

	if (KeyDown(KEY_LEFT_ARROW)) {
		cameraPos.x += -5.0f * deltaTime;
	}
	else if (KeyDown(KEY_RIGHT_ARROW)) {
		cameraPos.x += 5.0f * deltaTime;
	}

	//camera.LookAt(vec3(cameraPos.x, cameraPos.y, cameraDist), vec3(), vec3(0.0f, 1.0f, 0.0f));
	//camera.position = vec3(cameraPos.x, cameraPos.y, cameraDist);
	// camera.rotation

	camera.Update(deltaTime);
}