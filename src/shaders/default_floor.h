#pragma once

#include "textureShadow.h"
#include "sphericalHarmonics.h"


// DEFAULT SHADERS
// -----------------------------------------------------
std::string default_floor_vert = "\n\
#ifdef GL_ES\n\
precision mediump float;\n\
#endif\n\
\n\
uniform mat4 u_modelViewProjectionMatrix;\n\
\n\
attribute vec4  a_position;\n\
varying vec4    v_position;\n\
\n\
#ifdef MODEL_HAS_COLORS\n\
attribute vec4  a_color;\n\
varying vec4    v_color;\n\
#endif\n\
\n\
#ifdef MODEL_HAS_NORMALS\n\
attribute vec3  a_normal;\n\
varying vec3    v_normal;\n\
#endif\n\
\n\
#ifdef MODEL_HAS_TEXCOORDS\n\
attribute vec2  a_texcoord;\n\
varying vec2    v_texcoord;\n\
#endif\n\
\n\
#ifdef MODEL_HAS_TANGENTS\n\
attribute vec4  a_tangent;\n\
varying vec4    v_tangent;\n\
varying mat3    v_tangentToWorld;\n\
#endif\n\
\n\
#ifdef SHADOW_MAP\n\
uniform mat4    u_lightMatrix;\n\
varying vec4    v_lightcoord;\n\
#endif\n\
\n\
void main(void) {\n\
    \n\
    v_position = a_position;\n\
    \n\
#ifdef MODEL_HAS_COLORS\n\
    v_color = a_color;\n\
#endif\n\
    \n\
#ifdef MODEL_HAS_NORMALS\n\
    v_normal = a_normal;\n\
#endif\n\
    \n\
#ifdef MODEL_HAS_TEXCOORDS\n\
    v_texcoord = a_texcoord;\n\
#endif\n\
    \n\
#ifdef MODEL_HAS_TANGENTS\n\
    v_tangent = a_tangent;\n\
    vec3 worldTangent = a_tangent.xyz;\n\
    vec3 worldBiTangent = cross(v_normal, worldTangent) * sign(a_tangent.w);\n\
    v_tangentToWorld = mat3(normalize(worldTangent), normalize(worldBiTangent), normalize(v_normal));\n\
#endif\n\
    \n\
#ifdef SHADOW_MAP\n\
    v_lightcoord = u_lightMatrix * v_position;\n\
#endif\n\
    \n\
    gl_Position = u_modelViewProjectionMatrix * v_position;\n\
}\n\
";

std::string default_floor_frag = "\n\
#ifdef GL_ES\n\
precision mediump float;\n\
#endif\n\
\n\
uniform vec3    u_light;\n\
uniform vec3    u_camera;\n\
\n\
varying vec4    v_position;\n\
varying vec4    v_color;\n\
varying vec3    v_normal;\n\
varying vec2    v_texcoord;\n\
\n\
\n" + textureShadow + "\n\
\n\
\n " + sphericalHarmonics + "\n\
\n\
void main(void) {\n\
    vec3 color = vec3(0.5);\n\
\n\
    vec3 ambient = vec3(0.0, 0.0, 0.3);\n\
    vec3 diffuse = v_color.rgb;\n\
\n\
    vec3 l = normalize(u_light);\n\
    vec3 e = normalize(u_camera);\n\
    vec3 n = normalize(v_normal);\n\
    float t = dot(n, l) * 0.5 + 0.5;\n\
\n\
#ifdef SH_ARRAY\n\
    vec3 sh = tonemap(sphericalHarmonics(n));\n\
    ambient += sh * 0.2;\n\
    diffuse += sh * 0.2;\n\
#endif\n\
\n\
#if defined(SHADOW_MAP) && defined(SHADOW_MAP_SIZE) && !defined(PLATFORM_RPI)\n\
    float bias = 0.005;\n\
    t *= clamp( 0.5 + textureShadowPCF(u_ligthShadowMap, vec2(SHADOW_MAP_SIZE), v_lightcoord.xy, v_lightcoord.z - bias), 0., 1.);\n\
#endif\n\
\n\
    color = mix(vec3(0.0, 0.0, .3) * ambient * 0.7, diffuse, t);\n\
\n\
    gl_FragColor = vec4(color, 1.0);\n\
}\n";
