#include "renderGlfw.h"
#include "lib.h"
#include "glfw3.h"
#include "stdio.h"
#define CAPTURE(keyname) if (key == GLFW_KEY_ ## keyname) { \
  state->lastKeyPress = (#keyname)[0]; \
}

// keyboard callback
void keyboard(GLFWwindow * window, int key, int scancode, int act, int mods)
{
	State *state = (State *) glfwGetWindowUserPointer(window);
	// backspace: reset simulation
	if (act == GLFW_PRESS) {
    if (key == GLFW_KEY_BACKSPACE) {
      mj_resetData(state->m, state->d);
      mj_forward(state->m, state->d);
    }
    if (key == GLFW_KEY_SPACE) { 
      state->lastKeyPress = ' ';
    }
    CAPTURE(0)
    CAPTURE(1)
    CAPTURE(2)
    CAPTURE(3)
    CAPTURE(4)
    CAPTURE(5)
    CAPTURE(6)
    CAPTURE(7)
    CAPTURE(8)
    CAPTURE(9)
    CAPTURE(Q)
    CAPTURE(W)
    CAPTURE(E)
    CAPTURE(R)
    CAPTURE(T)
    CAPTURE(Y)
    CAPTURE(U)
    CAPTURE(I)
    CAPTURE(O)
    CAPTURE(P)
    CAPTURE(A)
    CAPTURE(S)
    CAPTURE(D)
    CAPTURE(F)
    CAPTURE(G)
    CAPTURE(H)
    CAPTURE(J)
    CAPTURE(K)
    CAPTURE(L)
    CAPTURE(Z)
    CAPTURE(X)
    CAPTURE(Y)
    CAPTURE(C)
    CAPTURE(V)
    CAPTURE(B)
    CAPTURE(N)
    CAPTURE(M)
	}

}

// mouse button callback
void mouse_button(GLFWwindow * window, int button, int act, int mods)
{
	State *state = (State *) glfwGetWindowUserPointer(window);

	// update button state 
	state->buttonLeft =
	    (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	state->buttonMiddle =
	    (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) ==
	     GLFW_PRESS);
	state->buttonRight =
	    (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

	// update mouse position 
	glfwGetCursorPos(window, &(state->mouseLastX), &(state->mouseLastY));
}

// mouse move callback
void mouse_move(GLFWwindow * window, double xpos, double ypos)
{
	State *state = (State *) glfwGetWindowUserPointer(window);

	// compute mouse displacement, save 
	double dx = xpos - state->mouseLastX;
	double dy = ypos - state->mouseLastY;
	state->mouseDx = dx;
	state->mouseDy = dy;
	state->mouseLastX = xpos;
	state->mouseLastY = ypos;

	// no buttons down: nothing to do 
	if (!state->buttonLeft && !state->buttonMiddle
	    && !state->buttonRight)
		return;

	// get current window size 
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	// get shift key state 
	int mod_shift = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			 || glfwGetKey(window,
				       GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

	// determine action based on mouse button 
	mjtMouse action;
	if (state->buttonRight)
		action = mod_shift ? mjMOUSE_MOVE_H : mjMOUSE_MOVE_V;
	else if (state->buttonLeft)
		action = mod_shift ? mjMOUSE_ROTATE_H : mjMOUSE_ROTATE_V;
	else
		action = mjMOUSE_ZOOM;

	// move camera 
	mjv_moveCamera(state->m, action, dx / height, dy / height,
		       &(state->scn), &(state->cam));
}

// scroll callback
void scroll(GLFWwindow * window, double xoffset, double yoffset)
{
	State *state = (State *) glfwGetWindowUserPointer(window);

	// emulate vertical mouse motion = 5% of window height
	mjv_moveCamera(state->m, mjMOUSE_ZOOM, 0, -0.05 * yoffset,
		       &(state->scn), &(state->cam));
}

int initOpenGL(GraphicsState* graphicsState, State * state)
{
	if (!glfwInit())
		mju_error("Could not initialize GLFW");

	// create visible window, double-buffered glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	GLFWwindow* window =
	    glfwCreateWindow(800, 800, "Visible window", NULL, NULL);
	if (!window)
		mju_error("Could not create GLFW window");

	glfwMakeContextCurrent(window);

	state->buttonLeft = 0;
	state->buttonMiddle = 0;
	state->buttonRight = 0;
	state->mouseLastX = 0;
	state->mouseLastY = 0;
	state->mouseDx = 0;
	state->mouseDy = 0;
	state->lastKeyPress = 0;

	// install GLFW mouse and keyboard callbacks
	glfwSetWindowUserPointer(window, state);
  glfwSetKeyCallback(window, keyboard);
  glfwSetCursorPosCallback(window, mouse_move);
  glfwSetMouseButtonCallback(window, mouse_button);
  glfwSetScrollCallback(window, scroll);

  *graphicsState = window;
  return 0;
}

int renderOnscreen(int camid, GraphicsState window, State * state)
{
	setCamera(camid, state);

	mjvScene scn = state->scn;
	mjrContext con = state->con;
	mjrRect rect = { 0, 0, 0, 0 };
	glfwGetFramebufferSize(window, &rect.width, &rect.height);

	mjr_setBuffer(mjFB_WINDOW, &con);
	if (con.currentBuffer != mjFB_WINDOW)
		printf("Warning: window rendering not supported\n");
	mjr_render(rect, &scn, &con);
	glfwSwapBuffers(window);
	glfwPollEvents();
  return 0;
}
