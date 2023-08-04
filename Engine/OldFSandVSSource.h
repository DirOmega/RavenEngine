//#pragma once

//old gragment and vertex shader source just in case I need them for debugging

//static const char * vs_source[] =
//{
//	"#version 410 core                                                  \n"
//	"                                                                   \n"
//	"in vec4 position;                                                  \n"
//	"                                                                   \n"
//	"out VS_OUT                                                         \n"
//	"{                                                                  \n"
//	"    vec4 color;                                                    \n"
//	"} vs_out;                                                          \n"
//	"                                                                   \n"
//	"uniform mat4 mv_matrix;                                            \n"
//	"uniform mat4 proj_matrix;                                          \n"
//	"                                                                   \n"
//	"void main(void)                                                    \n"
//	"{                                                                  \n"
//	"    gl_Position = proj_matrix * mv_matrix * position;              \n"
//	"    vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);      \n"
//	"}                                                                  \n"
//};
//
//static const char * fs_source[] =
//{
//	"#version 410 core                                                  \n"
//	"                                                                   \n"
//	"out vec4 color;                                                    \n"
//	"                                                                   \n"
//	"in VS_OUT                                                          \n"
//	"{                                                                  \n"
//	"    vec4 color;                                                    \n"
//	"} fs_in;                                                           \n"
//	"                                                                   \n"
//	"void main(void)                                                    \n"
//	"{                                                                  \n"
//	"    color = fs_in.color;                                           \n"
//	"}                                                                  \n"
//};