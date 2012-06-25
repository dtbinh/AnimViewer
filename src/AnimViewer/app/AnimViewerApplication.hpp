#ifndef ANIM_VIEWER_APPLICATION_H
#define ANIM_VIEWER_APPLICATION_H

#include "application/application.hpp"
#include "application/camera_roam.hpp"
#include "application/opengl.hpp"
#include "scene/scene.hpp"
#include "app/raytracer.hpp"
#include "app/options.hpp"
#include "files/fileChangeNotification/FileChangeNotification.h"

// pretty sure these are sequential, but use an array just in case
static const GLenum LightConstants[] = {
    GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3,
    GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7
};
static const size_t NUM_GL_LIGHTS = 8;

class AnimationViewerApplication : public Luc::Application, public Luc::FileChangeNotification::ICallback
{
public:

    AnimationViewerApplication( const Options& opt );
    virtual ~AnimationViewerApplication();

    virtual bool initialize();
    virtual void destroy();
    virtual void update( Luc::real_t );
    virtual void render();
    virtual void handle_event( const SDL_Event& event );

    virtual void OnFileChangeNotification();

    bool load_scene(const char* filename);

    // flips raytracing, does any necessary initialization
    void toggle_raytracing( int width, int height );
    // writes the current raytrace buffer to the output file
    void output_image();

    Luc::Raytracer raytracer;

    // the scene to render
    Luc::Scene scene;

    // options
    Options options;

    // the camera
    Luc::CameraRoamControl camera_control;

    // the image buffer for raytracing
    unsigned char* buffer;
    // width and height of the buffer
    int buf_width, buf_height;
// true if we are in raytrace mode.
    // if so, we raytrace and display the raytrace.
    // if false, we use normal gl rendering
    bool raytracing;
    // false if there is more raytracing to do
    bool raytrace_finished;
    Luc::FileChangeNotification::FCNHandle m_FCNHandle;

};

#endif 