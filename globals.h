#ifndef GLOBALS_H
#define GLOBALS_H

#include "listener.h"
#include "scene_graph.h"
#include "control.h"
#include "screenshot.h"
#include "light.h"
#include "transform.h"
#include "texture_ctrl.h"

// THIS FILE IS ONLY INTENDED AS A LIST OF GLOBAL VARIABLES
// FOR THE FILES TO COMPILE AND LINK CORRECTLY, EVERY FILE
// USING GLOBAL VARIABLES FROM HERE SHOULD **COPY** THE DEFINITIONS
// TO THEIR OWN CODE, INSTEAD OF INCLUDING THIS FILE!!!

extern std::vector<std::shared_ptr<listener>> listeners;
extern std::shared_ptr<camera> cam;
extern std::shared_ptr<control> ctrl;
extern std::shared_ptr<screenshot> ss;
extern std::shared_ptr<trans> tf;
extern std::shared_ptr<texture_ctrl> texctrl;

#endif
