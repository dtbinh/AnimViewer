/**
 * @file application.cpp
 * @brief Application class and SDL windowing code.
 *
 * @author Eric Butler (edbutler)
 */
#include "lucPCH.h"
#include "application/application.hpp"
#include "application/opengl.hpp"
#include "scene/image/imageio.hpp"
#include "files/fileChangeNotification/FileChangeNotification.h"
#include <SDL/SDL.h>
#include <cassert>
#include <cstdlib>
#include <iostream>

namespace Luc {

struct LoopData
{
    Application* app;
    real_t fps;
    // so it will behave correctly if used for multi-threaded applications
    volatile bool* running;
};

// header function definitions

Application::Application()
    : mIsRunning( false ) { }

Application::~Application() { }

void Application::end_main_loop()
{
    mIsRunning = false;
}

void Application::get_dimension( int* width, int* height ) const
{
    assert( width && height );
    SDL_Surface* surface = SDL_GetVideoSurface();
    assert( surface );
    *width = surface->w;
    *height = surface->h;
}

void Application::take_screenshot()
{
    static const size_t MAX_LEN = 256;
    char filename[MAX_LEN];
    int width, height;

    get_dimension( &width, &height );
    imageio_gen_name( filename, MAX_LEN );

    if ( imageio_save_screenshot( filename, width, height ) ) {
        std::cout << "Saved image to '" << filename << "'.\n";
    } else {
        std::cout << "Error saving raytraced image to '" << filename << "'.\n";
    }
}

// sets up the main window
static bool initialize_window( int width, int height, const char* title )
{
    // set the caption
    SDL_WM_SetCaption( title, title );

    // used the preferred bpp
    unsigned int bits_per_pixel = SDL_GetVideoInfo()->vfmt->BitsPerPixel;
    unsigned int flags = SDL_OPENGL;

    // se up opengl stuff
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, bits_per_pixel );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    // create the window
    if ( SDL_SetVideoMode( width, height, bits_per_pixel, flags ) == 0 ) {
        std::cout << "Error initializing SDL surface: " << SDL_GetError() << ", aborting initialization." << std::endl;
        return false;
    }

#ifdef _APPLICATION_USING_GLEW
    GLenum error = glewInit();
    if ( error != GLEW_OK ) {
        std::cerr << "GLEW failed to initialize.";
        return false;
    }
#endif

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glPixelStorei( GL_PACK_ALIGNMENT, 1 );

    return true;
}

static void process_events( Application* app )
{
    assert( app );

    SDL_Event event;
    while ( SDL_PeepEvents( &event, 1, SDL_GETEVENT, SDL_ALLEVENTS ) ) {
        switch ( event.type )
        {
        case SDL_QUIT:
            app->end_main_loop();
            break;

        case SDL_KEYDOWN:
            if ( event.key.keysym.sym == SDLK_ESCAPE ) {
                app->end_main_loop();
            }
            break;

        default:
            break;
        }
        app->handle_event( event );
    }
}

/**
 * The single-threaded update function.
 */
static void loop_update_func( LoopData* data )
{
    // always pump events and update
    SDL_PumpEvents();
    process_events( data->app );

    //update data
    data->app->update( 1.0f / data->fps );

    //render
    data->app->render();
    glFlush();
    SDL_GL_SwapBuffers();
}

/**
 * Works with any update function.
 */
static void run_main_loop( void (*update_fn)( LoopData* ), LoopData* data, real_t ups )
{
    assert( data->running && ups > 0 );

    int frame_rate_counter = 0; // frame rate counter
    int total_frame_time = 0;

    while ( *data->running ) {
        int start_time = SDL_GetTicks();

        Luc::FileChangeNotification::UpdateFileChangeNotificationSystem();

        // update
        update_fn( data );
        frame_rate_counter++;

        // compute sleep time until end of period, may be negative
        int end_time = SDL_GetTicks();
        total_frame_time += end_time - start_time;

        if ( total_frame_time > 1000 ) {
            float real_fps = frame_rate_counter * 1000.0f / total_frame_time;
            printf("fps = %f.\n", real_fps);
            frame_rate_counter = 0;
            total_frame_time = 0;
        }

        // always yield at least once to keep system responsive
        SDL_Delay( 1 );
    }
}

int Application::start_application( Application* app, int width, int height, real_t fps, const char* title )
{
    LoopData ldata;

    assert( app );
    // only create an app the first time this function is called
    if ( SDL_WasInit( 0 ) )
        return -2;

    // init SDL
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) == -1 ) {
        std::cout << "Error initializing SDL" << std::endl;
        return -1;
    }

    if ( !initialize_window( width, height, title ) )
        goto FAIL;

    // initialize the application
    if ( !app->initialize() )
        goto FAIL;

    // run main loop
    app->mIsRunning = true;
    ldata.app = app;
    ldata.fps = fps;
    ldata.running = &app->mIsRunning;
    run_main_loop( loop_update_func, &ldata, fps );

    // clean up and exit
    app->destroy();
    return 0;


  FAIL:
    std::cerr << "Failed to start application, aborting.\n";
    return -1;
}

} 

