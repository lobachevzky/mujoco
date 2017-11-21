from mujoco.sim cimport BaseSim
from mujoco.sim import ObjType
from pxd.renderGlfw cimport GraphicsState, initOpenGL, renderOnscreen

cdef class Sim(BaseSim):
    """ 
    Sim that uses GLFW functionality, which supports both offscreen and onscreen rendering.
    """
    cdef GraphicsState graphics_state

    def init_opengl(self):
        """ Initialize GLFW. """
        initOpenGL(&self.graphics_state, &self.state)

    def close_opengl(self):
        """ Does nothing, because glfwTerminate has a bug. """
        pass

    def render(self, camera_name=None):
        """
        Display the view from camera corresponding to ``camera_name`` in an onscreen GLFW window. 
        """
        if camera_name is None:
            camid = -1
        else:
            camid = self.get_id(ObjType.CAMERA, camera_name)
        return renderOnscreen(camid, self.graphics_state, & self.state)

    def lastkey(self):
        if self.state.lastkey:
            key = chr(self.state.lastKeyPress)
            self.state.lastKeyPress = 0
            return key

    @property
    def dx(self):
        return self.state.mouseDx

    @property
    def dy(self):
        return self.state.mouseDx
