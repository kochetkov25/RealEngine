#version 440
#include "cameraUniform.glsl"

out vec4 outColor;

uniform vec2 uViewportSize;

// GRID SETTINGS
uniform float uGridStep      = 2.0;   // small grid step
uniform float uLineThickness = 0.05;  // grid thickness
uniform int   uMajorEvery    = 10;    // each N major line

uniform vec3 uColorX   = vec3(1.0, 0.0, 0.0); // axis X — red
uniform vec3 uColorZ   = vec3(0.0, 1.0, 0.0); // axis Z — green
uniform vec3 uColorGrid= vec3(0.6, 0.6, 0.6); // small grid
uniform vec3 uColorMaj = vec3(0.35,0.35,0.35);// magot grid

// edge smoothing
float lineMask(float coord, float thickness) {
    float d = abs(fract(coord) - 0.5);
    
    float dist = 0.5 - d;
    
    float aa = fwidth(coord);
    return smoothstep(thickness + aa, thickness - aa, dist);
}

void main() {
    // 1) ray from pixels
    vec2 ndc = (gl_FragCoord.xy / uViewportSize) * 2.0 - 1.0;
    vec4 clip = vec4(ndc, 1.0, 1.0);

    // transform to view
    vec4 viewPos = inverse(camera_u.projMat) * clip;
    viewPos /= viewPos.w;
    vec3 dirView = normalize(viewPos.xyz);

    // transform to world
    vec3 dirWorld = normalize((inverse(camera_u.viewMat) * vec4(dirView, 0.0)).xyz);
    vec3 orig     = camera_u.position;

    // 2) intersection of a ray with a plane Y=0: orig.y + t*dir.y = 0
    float denom = dirWorld.y;
    if (abs(denom) < 1e-6) discard; 
    float t = -orig.y / denom;
    if (t <= 0.0) discard;          

    vec3 worldPos = orig + t * dirWorld; // intersection poin on a plane

    // 3) Axis
    float axisMaskX = smoothstep(uLineThickness, uLineThickness*0.5, abs(worldPos.z)); // ось X: z≈0
    float axisMaskZ = smoothstep(uLineThickness, uLineThickness*0.5, abs(worldPos.x)); // ось Z: x≈0

    // 4) Grid
    float step = max(uGridStep, 1e-6);
    
    float gx = worldPos.x / step;
    float gz = worldPos.z / step;

    // small grid
    float smallX = lineMask(gx, 0.5 * uLineThickness / step);
    float smallZ = lineMask(gz, 0.5 * uLineThickness / step);
    float small  = max(smallX, smallZ);

    // major grid
    float majStep = float(uMajorEvery);
    float mx = lineMask(worldPos.x / (step*majStep), 0.5 * uLineThickness / (step*majStep));
    float mz = lineMask(worldPos.z / (step*majStep), 0.5 * uLineThickness / (step*majStep));
    float major = max(mx, mz);

    // 5) COLOR (AXSI -> MAJOR -> SMALL)
    vec3 col = vec3(0.0);
    float a = 0.0;

    // AXIS
    if (axisMaskX > 0.0 && axisMaskX >= axisMaskZ) { col = uColorX; a = axisMaskX; }
    else if (axisMaskZ > 0.0)                       { col = uColorZ; a = axisMaskZ; }

    // MAJOR LINE (Not Axis)
    if (a < 0.99 && major > 0.0) { col = mix(col, uColorMaj, major); a = max(a, major); }

    // SMALL GRID
    if (small > 0.0) { col = mix(col, uColorGrid, small); a = max(a, small); }

    if (a <= 0.0) discard;

    // FADING
    float dist = length(worldPos - orig);
    float fade = exp(-0.01 * dist);
    a *= fade;

    // CALC DEPTH
    vec4 clipPos = camera_u.projMat * camera_u.viewMat * vec4(worldPos, 1.0);
    clipPos /= clipPos.w; // NDC [-1..1]

    // OUT
    gl_FragDepth = clipPos.z * 0.5 + 0.5;

    outColor = vec4(col * fade, a);
}