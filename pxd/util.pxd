include "mujoco.pxd"
include "mjvisualize.pxd"
include "mjrender.pxd"

cdef extern from "util.h":
    ctypedef struct State:
        mjModel * m
        mjData * d
        mjvScene scn
        mjrContext con
        mjvCamera cam
        mjvOption opt
        int buttonLeft
        int buttonMiddle
        int buttonRight
        double mouseLastX
        double mouseLastY
        double mouseDx
        double mouseDy
        char lastKeyPress

    int initMujoco(const char *filepath, State * state)
    int setCamera(int camid, State * state)
    int addLabel(const char* label, float* pos, State* s)
    int renderOffscreen(unsigned char *rgb, int height, int width, State * state)
    int closeMujoco(State * state)
