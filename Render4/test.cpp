
#include"Mesh.h"
#include "Object.h"
#include "PhysicsObject.h"
#include "Shape.h"
#include "QuadTre.h"
#include <list>
#include <array>

#include "ParticleEmitter.h"
#include "ComponentManager.h"
#include "ComponentSystem.h"

int windowWidth = 1280;
int windowHeight = 720;

//Trykk ENTER for aa starte opp ballene fra resting state
//hvis ballene begynner aa gaa for sakte kan du trykke ENTER igjen for aa gi dem ny momentum

GLFWwindow* window;

bool wireframe = false;
bool showLights = true;
bool dotRender = false;
bool start = false;
bool cursorEnabled = false;

void processInput(GLFWwindow* window, Camera& camera) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
		cout << "I have been closed" << endl;
	}
	camera.Inputs(window, cursorEnabled);
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	cout << "width = " << width << " height = " << height << endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
		cursorEnabled = !cursorEnabled;
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		wireframe = !wireframe;
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		showLights = !showLights;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		dotRender = !dotRender;
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		start = true;
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Hello again", NULL, NULL);
	if (window == NULL) {
		cout << "Window was never created" << endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "failed to init GLAD" << endl;
		glfwTerminate();
	}

	Texture textures[]
	{
		Texture("empty_keg.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("empty_keg_spec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	Texture rain[]{
		Texture("", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	QuadTre tre = QuadTre();
	tre.subDivide(1);

	Shader shaderProgram("Resources/Vertexshader.vert", "Resources/FragmentShader.frag");
	Shape cube(Shape::CUBE);
	Shape sph(Shape::SPHERE);
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	std::vector <Texture> raintex(rain, rain + sizeof(rain) / sizeof(Texture));

	Mesh cMesh(cube.getVerts(), cube.getInds(), tex);
	Mesh wMesh(cube.getVerts(), cube.getInds(), tex);
	Mesh sMesh(sph.getVerts(), sph.getInds(), tex);
	Mesh rainMesh(sph.getVerts(), sph.getInds(), raintex);

	Shader lightShader("Resources/LightVert.vert", "Resources/LightFrag.frag");
	Shape lig(Shape::CUBE);
	Mesh lightMesh(lig.getVerts(), lig.getInds(), tex);

	LightObject light(lightMesh, glm::vec3(2.0f, 2.5f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), lightShader, shaderProgram);
	PhysicsObject* spheres[9] = {};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			PhysicsObject* a = new PhysicsObject(sMesh, glm::vec3((float)i - 1, 0.5, (float)j - 1), glm::vec3(0.1, 0.1, 0.1f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, shaderProgram, true, i * 0.2f);
			spheres[(i * 3) + j] = a;
			tre.insert(a);
		}
	}

	PhysicsObject plane(cMesh, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 0.1f, 5.0f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, shaderProgram, false, 1.0f);

	PhysicsObject wall1(wMesh, glm::vec3(0.0f, 0.0f, 2.5f), glm::vec3(2.5f, 2.5f, 0.1f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, shaderProgram, false, 1.0f);
	PhysicsObject wall2(wMesh, glm::vec3(0.0f, 0.0f, -2.5f), glm::vec3(2.5f, 2.5f, 0.1f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, shaderProgram, false, 1.0f);
	PhysicsObject wall3(wMesh, glm::vec3(2.5f, 0.0f, 0.0f), glm::vec3(0.1f, 2.5f, 2.5f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, shaderProgram, false, 1.0f);
	PhysicsObject wall4(wMesh, glm::vec3(-2.5f, 0.0f, 0.0f), glm::vec3(0.1f, 2.5f, 2.5f), Rotator{ glm::vec3(0.0f, 0.0f, 1.0f), 0.0f }, shaderProgram, false, 1.0f);

	glEnable(GL_DEPTH_TEST);

	Camera camera(windowWidth, windowHeight, glm::vec3(1.0f, 2.0f, -5.0));
	camera.orientation = glm::rotate(camera.orientation, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	ParticleEmitter emitter;

	list<Entity> entities;
	ComponentManager<PositionComponent>* positionComponent = new ComponentManager<PositionComponent>();
	ComponentManager<VelocityComponent>* velocityComponent = new ComponentManager<VelocityComponent>();
	ComponentManager<RendringComponent> renderingComponent = ComponentManager<RendringComponent>();
	ComponentManager<GravityComponent> gravityComponent = ComponentManager<GravityComponent>();
	ComponentManager<PhysicsComponent> physicsComponent = ComponentManager<PhysicsComponent>();
	ComponentManager<SizeComponent> sizeComponent = ComponentManager<SizeComponent>();
	ComponentManager<CollideableComponent> collideableComponent = ComponentManager<CollideableComponent>();

	//I apply the components to the entities this way because the components have arrays, meaning that if I wanted to apply for example a value to the 6th entity's z coordinate
	//I'd have to manually input values for every spot in the array before it. Doing it this way means that I can directly apply a value to a specific spot in the array
	for (int i = 0; i < 15; i++) {
		Entity ent{ i };
		Mesh ballMesh(sph.getVerts(), sph.getInds(), tex);
		Mesh cubeMesh(cube.getVerts(), cube.getInds(), tex);
		entities.push_back(ent);
		//plane initialiser
		if (i == 0) {
			SizeComponent size = sizeComponent.getComponent(ent.id);
			size.x[ent.id] = 5.0f;
			size.y[ent.id] = 0.1f;
			size.z[ent.id] = 5.0f;
			sizeComponent.addComponent(ent.id, size);
			positionComponent->addComponent(ent.id, PositionComponent{});
			renderingComponent.addComponent(ent.id, RendringComponent{ cubeMesh});
		}
		//Z axis walls initialiser
		else if (i > 0 && i < 3) {
			SizeComponent size = sizeComponent.getComponent(ent.id);
			size.x[ent.id] = 2.5f;
			size.y[ent.id] = 2.5f;
			size.z[ent.id] = 0.1f;
			sizeComponent.addComponent(ent.id, size);
			PositionComponent pos = positionComponent->getComponent(ent.id);
			pos.x[ent.id] = 0.0;
			pos.y[ent.id] = 0.0;
			pos.z[ent.id] = 2.5 * (i % 2 == 0 ? 1 : -1);
			positionComponent->addComponent(ent.id, pos);
			RendringComponent wallMesh = renderingComponent.getComponent(ent.id);
			wallMesh.meshes[ent.id] = cubeMesh;
			renderingComponent.addComponent(ent.id, wallMesh);
		}
		//X axis walls initialiser
		else if (i > 3 && i < 6) {
			SizeComponent size = sizeComponent.getComponent(ent.id);
			size.x[ent.id] = 0.1f;
			size.y[ent.id] = 2.5f;
			size.z[ent.id] = 2.5f;
			sizeComponent.addComponent(ent.id, size);
			PositionComponent pos = positionComponent->getComponent(ent.id);
			pos.x[ent.id] = 2.5 * (i % 2 == 0 ? 1 : -1);
			pos.y[ent.id] = 0.0;
			pos.z[ent.id] = 0.0;
			positionComponent->addComponent(ent.id, pos);
			RendringComponent wallMesh = renderingComponent.getComponent(ent.id);
			wallMesh.meshes[ent.id] = cubeMesh;
			renderingComponent.addComponent(ent.id, wallMesh);
		}
		//spheres initialiser
		else if (i >= 6) {
			SizeComponent size = sizeComponent.getComponent(ent.id);
			size.x[ent.id] = 0.1;
			size.y[ent.id] = 0.1;
			size.z[ent.id] = 0.1;
			sizeComponent.addComponent(ent.id, size);
			PositionComponent pos = positionComponent->getComponent(ent.id);
			pos.x[ent.id] = 0.25 * i - 2;
			pos.y[ent.id] = 1.5;
			pos.z[ent.id] = 0.25 * i - 2;
			positionComponent->addComponent(ent.id, pos);
			VelocityComponent vel = velocityComponent->getComponent(ent.id);
			vel.x[ent.id] = 0.05 * (rand() * 0.00005);
			vel.y[ent.id] = 0.04;
			vel.z[ent.id] = 0.03 * (rand() * 0.00005);
			velocityComponent->addComponent(ent.id, vel);
			RendringComponent sphereMesh = renderingComponent.getComponent(ent.id);
			sphereMesh.meshes[ent.id] = ballMesh;
			renderingComponent.addComponent(ent.id, sphereMesh);
			gravityComponent.addComponent(ent.id, GravityComponent{});
			PhysicsComponent mass = physicsComponent.getComponent(ent.id);
			mass.mass[ent.id] = 0.2 * (i - 6);
			physicsComponent.addComponent(ent.id, mass);
		}
		collideableComponent.addComponent(ent.id, CollideableComponent{});
	}

	MovementSystem movement = MovementSystem(positionComponent, velocityComponent, gravityComponent);
	RendringSystem renderer = RendringSystem(positionComponent, renderingComponent, sizeComponent);
	PhysicsSystem physics = PhysicsSystem(positionComponent, velocityComponent, physicsComponent, collideableComponent, renderingComponent, sizeComponent);

	while (!glfwWindowShouldClose(window))
	{
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		if (cursorEnabled) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		glClearColor(0.533, 0.996, 1, 0.78);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "cameraMatrix");
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
		processInput(window, camera);

		//for (int i = 0; i < 9; i++) {
		//	spheres[i]->Render(shaderProgram, camera, dotRender);

		//	if (start) {
		//		spheres[i]->velocity.z = 0.05 * (rand() * 0.00005);
		//		spheres[i]->velocity.x = 0.03 * (rand() * 0.00005);
		//		spheres[i]->velocity.y = 0.04;
		//	}

		//	if (spheres[i]->isColliding((Object)plane)) {
		//		spheres[i]->onCollisionWall((Object)plane);
		//	}
		//	else if (spheres[i]->isColliding((Object)wall1)) {
		//		spheres[i]->onCollisionWall((Object)wall1);
		//	}
		//	else if (spheres[i]->isColliding((Object)wall2)) {
		//		spheres[i]->onCollisionWall((Object)wall2);
		//	}
		//	else if (spheres[i]->isColliding((Object)wall3)) {
		//		spheres[i]->onCollisionWall((Object)wall3);
		//	}
		//	else if (spheres[i]->isColliding((Object)wall4)) {
		//		spheres[i]->onCollisionWall((Object)wall4);
		//	}
		//}

		//start = false;

		//for (QuadTre* quad : tre.quads) {
		//	if (quad->hasObjects()) {
		//		for (PhysicsObject* ball1 : quad->getObjects()) {
		//			for (PhysicsObject* ball2 : quad->getObjects()) {
		//				if (ball1->isCollidingBall(ball2)) {
		//					ball1->onBallCollision(ball2);
		//				}
		//			}
		//		}
		//	}
		//}

		//tre.clearObjects();

		//for (PhysicsObject* sphere : spheres) {
		//	QuadTre::Point2F point{ sphere->currPos.x, sphere->currPos.z };
		//	tre.insert(sphere);
		//}

		//plane.Render(shaderProgram, camera, dotRender);
		//wall1.Render(shaderProgram, camera, dotRender);
		//wall2.Render(shaderProgram, camera, dotRender);
		//wall3.Render(shaderProgram, camera, dotRender);
		//wall4.Render(shaderProgram, camera, dotRender);

		emitter.updateParticles();
		emitter.renderParticles(shaderProgram, camera);

		if (emitter.count < emitter.maxParticles) {
			emitter.spawnParticle(rainMesh, glm::vec3(4 - (0.05 * (rand() * 0.005)), 10, 4 -  (0.05 * (rand() * 0.005))), glm::vec3(0.05 - (0.05 * (rand() * 0.00005)), -0.1, 0.05 - (0.05 * (rand() * 0.00005))));
		}

		physics.update(entities);
		movement.update(entities);
		renderer.update(entities, shaderProgram, camera, dotRender);

		if (showLights) {
			light.Render(lightShader, camera, dotRender);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	lightShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
