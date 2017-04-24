#include "engine.hpp"
#include "camera.hpp"
#include "config.hpp"
#include "draw.hpp"
#include "mesh.hpp"
#include "shapes.hpp"
#include "uihelper.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class PencilPhysics: public Engine, UIMain {
public:

    SDL_Window *window;
    Camera2D camera;
    UIHelper uiHelper;
    Draw draw;

    Polyline walls;
    Circle redCircle;
    Box whiteBox;
    vector<Circle> circles;
    vector<Box> boxes;
    vector<Polyline> polylines;

    vec2 worldMin, worldMax;

    PencilPhysics() {
        worldMin = vec2(-8, 0);
        worldMax = vec2(8, 9);
        window = createWindow("4611", 1280, 720);
        camera = Camera2D(worldMin, worldMax);
        uiHelper = UIHelper(this, worldMin, worldMax, 1280, 720);
        draw = Draw(this);
        // Initialize world
        initWorld();
    }

    ~PencilPhysics() {
        SDL_DestroyWindow(window);
    }

    void initWorld() {

        // TODO: Create a Box2D world and make these shapes static
        // bodies in it.

        // Create walls
        vector<vec2> wallVerts;
        wallVerts.push_back(vec2(worldMin.x, worldMax.y));
        wallVerts.push_back(vec2(worldMin.x, worldMin.y));
        wallVerts.push_back(vec2(worldMax.x, worldMin.y));
        wallVerts.push_back(vec2(worldMax.x, worldMax.y));
        walls = Polyline(wallVerts);
        // Create two static bodies
        redCircle = Circle(vec2(-5,2), 0.5);
        whiteBox = Box(vec2(5,2), vec2(0.9,0.9));
    }

    void run() {
        float fps = 60, dt = 1/fps;
        while (!shouldQuit()) {
            handleInput();
            advanceState(dt);
            drawGraphics();
            waitForNextFrame(dt);
        }
    }

    vec2 randomVec2() {
        return vec2(2.*rand()/RAND_MAX-1, 2.*rand()/RAND_MAX-1);
    }

    void addCircle() {
        vec2 position = vec2(-5,7) + 0.5*randomVec2();
        circles.push_back(Circle(position, 0.5));
    }

    void addBox() {
        vec2 position = vec2(-5,7) + 0.5*randomVec2();
        boxes.push_back(Box(position, vec2(1.2,0.6)));
    }

    void addPolyline(vector<vec2> vertices) {
        polylines.push_back(Polyline(vertices));
    }

    void clear() {
        for (int i = 0; i < circles.size(); i++)
            circles[i].destroy();
        circles.clear();
        for (int i = 0; i < boxes.size(); i++)
            boxes[i].destroy();
        boxes.clear();
        for (int i = 0; i < polylines.size(); i++)
            polylines[i].destroy();
        polylines.clear();
    }

    void onKeyDown(SDL_KeyboardEvent &e) {
        uiHelper.onKeyDown(e);
    }
    void onKeyUp(SDL_KeyboardEvent &e) {
        uiHelper.onKeyUp(e);
    }
    void onMouseButtonDown(SDL_MouseButtonEvent &e) {
        uiHelper.onMouseButtonDown(e);
    }
    void onMouseButtonUp(SDL_MouseButtonEvent &e) {
        uiHelper.onMouseButtonUp(e);
    }
    void onMouseMotion(SDL_MouseMotionEvent &e) {
        uiHelper.onMouseMotion(e);
    }

    void advanceState(float dt) {

        // TODO: Step the Box2D world by dt.

    }

    void drawGraphics() {
        // Light gray background
        glClearColor(0.8,0.8,0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST); // don't use z-buffer because 2D
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1,1);
        // Apply camera transformation
        camera.apply(this);
        // Draw current polyline
        vector<vec2> currentPolyline = uiHelper.getPolyline();
        if (currentPolyline.size() >= 2)
            draw.polyline(mat4(), currentPolyline, vec3(0.6,0.6,0.6));

        // TODO: Modify these draw calls to draw the bodies with the
        // correct positions and angles.

        // Draw red circle and white box.
        draw.circle(mat4(), redCircle.center, redCircle.radius, vec3(1,0,0));
        draw.box(mat4(), whiteBox.center, whiteBox.size, vec3(1,1,1));
        // Draw all the other circles, boxes, and polylines
        for (int i = 0; i < circles.size(); i++)
            draw.circle(mat4(), circles[i].center, circles[i].radius, vec3(0,0,0));
        for (int i = 0; i < boxes.size(); i++)
            draw.box(mat4(), boxes[i].center, boxes[i].size, vec3(0,0,0));
        for (int i = 0; i < polylines.size(); i++)
            draw.polyline(mat4(), polylines[i].vertices, vec3(0,0,0));

        // Finish
        SDL_GL_SwapWindow(window);
    }

    void attachMouse(vec2 worldPoint) {

        // TODO: Check if there is a circle or box that contains the
        // world point. If so, set mouseJoint to be a new b2MouseJoint
        // attached to the body with the point as target. The most
        // important parameters of b2MouseJointDef are bodyA (a static
        // Box2D body, e.g. the walls), bodyB (the chosen body being
        // moved) and target (the world-space point to pull bodyB
        // towards). Recommended values for other parameters you will
        // need to set are:
        //   collideConnected: true
        //   maxForce: 100
        //   frequencyHz: 2
        //   dampingRatio: 0.5

    }

    void moveMouse(vec2 worldPoint) {

        // TODO: If mouseJoint is not null, use SetTarget() to update
        // its target to the given point.

    }

    void detachMouse() {

        // TODO: If mouseJoint is not null, destroy it and set it to
        // null.

    }

};

int main(int argc, char **argv) {
    PencilPhysics physics;
    physics.run();
    return EXIT_SUCCESS;
}
