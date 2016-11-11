#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "Primitive.h"


// TODO 1: ...and the 3 libraries based on how we compile (Debug or Release)
// use the _DEBUG preprocessor define
#ifdef _DEBUG
#pragma comment(lib, "Bullet/libx86/BulletCollision_debug.lib")
#pragma comment(lib, "Bullet/libx86/BulletDynamics_debug.lib")
#pragma comment(lib, "Bullet/libx86/LinearMath_debug.lib")
#endif // _DEBUG

#ifndef _DEBUG
#pragma comment(lib, "Bullet/libx86/BulletCollision.lib")
#pragma comment(lib, "Bullet/libx86/BulletDynamics.lib")
#pragma comment(lib, "Bullet/libx86/LinearMath.lib")
#endif // !_DEBUG




ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug_draw = NULL;
	debug = true;

	// TODO 2: Create collision configuration, dispacher
	// broad _phase and solver
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	paircache = new btDbvtBroadphase();
	constraintSolver = new btSequentialImpulseConstraintSolver();


	// Uncomment this to enable debug drawer
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;

	// TODO 2: and destroy them!
	delete collisionConfiguration;
	delete dispatcher;
	delete paircache;
	delete constraintSolver;

}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	// TODO 3: Create the world and set default gravity
	// Have gravity defined in a macro!

	world = new btDiscreteDynamicsWorld(dispatcher, paircache, constraintSolver, collisionConfiguration);
	world->setGravity(GRAVITY);


	//Uncomment this line to have the world use our debug drawer
	world->setDebugDrawer(debug_draw);

	{
		// TODO 5: Create a big rectangle as ground
		// Big rectangle as ground
		btCollisionShape* colShape = new btBoxShape(btVector3(100.0f, 1.0f, 100.0f));

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbinfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbinfo);
		world->addRigidBody(body);
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	// TODO 4: step the world
	world->stepSimulation(dt, 10);

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		world->debugDrawWorld();
		
		if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			// TODO 6: Create a Solid Sphere when pressing 1 on camera position
			btCollisionShape* colShape = new btSphereShape(1.0f);

	
			btDefaultMotionState* myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z)));
			btRigidBody::btRigidBodyConstructionInfo rbinfo(1.0f, myMotionState, colShape);

			btRigidBody* body = new btRigidBody(rbinfo);
			world->addRigidBody(body);
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			// TODO 6: Create a Solid Sphere when pressing 1 on camera position
			btCollisionShape* colShape = new btSphereShape(1.0f);


			btDefaultMotionState* myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z)));
			btRigidBody::btRigidBodyConstructionInfo rbinfo(1.0f, myMotionState, colShape);

			btRigidBody* body = new btRigidBody(rbinfo);
			world->addRigidBody(body);


		}

	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// TODO 3: ... and destroy the world here!

	return true;
}

// =============================================

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
