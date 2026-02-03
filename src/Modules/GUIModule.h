#pragma once

struct GLFWwindow;

namespace Modules {

class GUIModule {
 public:
  static void onWindowCreate(GLFWwindow* pWindow);

  static void onWindowClose();

  static void GUIbegin();

  static void GUIend();
};

}  // namespace Modules