/**
 * @file main_rayracer.cpp
 * @brief Raytracer entry
 *
 * @author Eric Butler (edbutler)
 */



#include "lucPCH.h"
#include "app/AnimViewerApplication.hpp"

#include <iostream>
#include <cstring>


int main( int argc, char* argv[] )
{
    Options opt;

    AnimationViewerApplication app( opt );

    // load the given scene
    if ( !app.load_scene(opt.input_filename.c_str()) ) {
        std::cout << "Error loading scene " << opt.input_filename << ". Aborting.\n";
        return 1;
    }

    return Luc::Application::start_application( &app, opt.width, opt.height, opt.mFps, opt.mWindowTitile.c_str() );
}

