#ifndef __WINDOW_CALLBACK_H__
#define __WINDOW_CALLBACK_H__

Window* g_CurrentInstance;

extern "C"
void drawCallback() {
  g_CurrentInstance->display();
}

extern "C"
void reshapeCallback(int width, int height) {
  g_CurrentInstance->reshape(width, height);
}

extern "C"
void keyboardCallback(unsigned char key, int x, int y) {
  g_CurrentInstance->keyboard(key, x, y);
}

extern "C"
void motionCallback(int x, int y) {
  g_CurrentInstance->motion(x, y);
}

extern "C"
void mouseCallback(int button, int state, int x, int y) {
  g_CurrentInstance->mouse(button, state, x, y);
}

extern "C"
void handleMenuEventsCallback(int option) {
  g_CurrentInstance->handleMenuEvents(option);
}


extern "C"
void processColorEventsCallback(int option) {
  g_CurrentInstance->handleColorEvents(option);
}

#endif // __WINDOW_CALLBACK_H__