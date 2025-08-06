layout(std140, binding = 0) uniform CameraUniform {
   mat4 viewMat;
   mat4 projMat;
   vec3 position; float _pad0;
} camera_u;