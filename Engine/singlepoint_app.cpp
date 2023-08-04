
#include "singlepoint_app.h"

#include "MathEngine.h"
#include "MemEngine.h"

singlepoint_app::singlepoint_app() : program(0), vao(0), buffer(0), mv_location(0), proj_location(0), aspect(0), heapPtr(0), proj_matrix(0)
{
};

void singlepoint_app::init()
{
	static const char title[] = "OpenGL SuperBible - Spinny Cube";

	Engine::init();

	memcpy(info.title, title, sizeof(title));

	Mem::Initialize();

	Mem::HeapCreate(heapPtr, 2 * 1024 * 1024, "SpinCubeMats");

	this->proj_matrix = new(heapPtr, Mem::Align::Byte_16, __FILE__ , __LINE__) Matrix(IDENTITY);

}
void singlepoint_app::startup()
{
	static const char * vs_source[] =
	{
		"#version 410 core                                                  \n"
		"                                                                   \n"
		"in vec4 position;                                                  \n"
		"                                                                   \n"
		"out VS_OUT                                                         \n"
		"{                                                                  \n"
		"    vec4 color;                                                    \n"
		"} vs_out;                                                          \n"
		"                                                                   \n"
		"uniform mat4 mv_matrix;                                            \n"
		"uniform mat4 proj_matrix;                                          \n"
		"                                                                   \n"
		"void main(void)                                                    \n"
		"{                                                                  \n"
		"    gl_Position = proj_matrix * mv_matrix * position;              \n"
		"    vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);      \n"
		"}                                                                  \n"
	};

	static const char * fs_source[] =
	{
		"#version 410 core                                                  \n"
		"                                                                   \n"
		"out vec4 color;                                                    \n"
		"                                                                   \n"
		"in VS_OUT                                                          \n"
		"{                                                                  \n"
		"    vec4 color;                                                    \n"
		"} fs_in;                                                           \n"
		"                                                                   \n"
		"void main(void)                                                    \n"
		"{                                                                  \n"
		"    color = fs_in.color;                                           \n"
		"}                                                                  \n"
	};

	program = glCreateProgram();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	mv_location = glGetUniformLocation(program, "mv_matrix");
	proj_location = glGetUniformLocation(program, "proj_matrix");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	static const GLfloat vertex_positions[] =
	{
		-0.25f,  0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, -0.25f,  0.25f,
		0.25f,  0.25f, -0.25f,

		0.25f, -0.25f,  0.25f,
		0.25f,  0.25f,  0.25f,
		0.25f,  0.25f, -0.25f,

		0.25f, -0.25f,  0.25f,
		-0.25f, -0.25f,  0.25f,
		0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
		0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		0.25f, -0.25f,  0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f,  0.25f,

		-0.25f,  0.25f, -0.25f,
		0.25f,  0.25f, -0.25f,
		0.25f,  0.25f,  0.25f,

		0.25f,  0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
		-0.25f,  0.25f, -0.25f
	};

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertex_positions),
		vertex_positions,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void singlepoint_app::render(double currentTime)
{
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, green);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glUseProgram(program);

	glUniformMatrix4fv(proj_location, 1, GL_FALSE, (float*)proj_matrix);

#ifdef MANY_CUBES
	int i;
	for (i = 0; i < 24; i++)
	{
		float f = (float)i + (float)currentTime * 0.3f;
		vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -6.0f) *
			vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
			vmath::rotate((float)currentTime * 21.0f, 1.0f, 0.0f, 0.0f) *
			vmath::translate(sinf(2.1f * f) * 2.0f,
				cosf(1.7f * f) * 2.0f,
				sinf(1.3f * f) * cosf(1.5f * f) * 2.0f);
		glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv_matrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
}
#else
	float f = (float)currentTime * 0.3f;

	/* vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -4.0f) *
	vmath::translate(sinf(2.1f * f) * 0.5f,
	cosf(1.7f * f) * 0.5f,
	sinf(1.3f * f) * cosf(1.5f * f) * 2.0f) *
	vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
	vmath::rotate((float)currentTime * 81.0f, 1.0f, 0.0f, 0.0f);

	Matrix mv_matrixTest = (Matrix&)mv_matrix;*/

	//Matrix World;
	/*Matrix TransA(TRANS, 0.0f, 0.0f, -4.0f);
	Matrix TransB(TRANS, sinf(2.1f * f) * 0.5f, cosf(1.7f * f) * 0.5f, sinf(1.3f * f) * cosf(1.5f * f) * 2.0f);
	Matrix RotY(ROT_Y, ((float)currentTime * 45.0f) * MATH_PI_180);
	Matrix RotX(ROT_X, ((float)currentTime * 81.0f) * MATH_PI_180);*/

	//Matrix World = RotX * RotY * TransB * TransA;

	Matrix WorldT = Matrix(TRANS, 0.0f, 0.0f, -4.0f) *
		Matrix(TRANS, sinf(2.1f * f) * 0.5f, cosf(1.7f * f) * 0.5f, sinf(1.3f * f) * cosf(1.5f * f) * 2.0f) *
		Matrix(ROT_Y, ((float)currentTime * 45.0f) * MATH_PI_180) *  //this equals the below comment but skips the naming
		Matrix(ROT_X, ((float)currentTime * 81.0f) * MATH_PI_180); 	//Matrix WorldT = TransA * TransB * RotY * RotX;


																	//TODO:: google why GL_False is in for transposte column major in program should transpose to row major on gpu
	glUniformMatrix4fv(mv_location, 1, GL_FALSE, (float *)&WorldT);
	//glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv_matrix);
	glDrawArrays(GL_TRIANGLES, 0, 36);

#endif
	}

void singlepoint_app::shutdown()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glDeleteBuffers(1, &buffer);
	Mem::Destroy();
};

void singlepoint_app::onResize(int w, int h)
{
	Engine::onResize(w, h);

	aspect = (float)w / (float)h;

	//vmath::mat4 pm = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);

	//static inline mat4 perspective(float fovy, float aspect, float n, float f)

	float q = 1.0f / tanf(25.0f * MATH_PI_180);
	float A = q / aspect;
	float B = (0.1f + 1000.0f) / (0.1f - 1000.0f);
	float C = (2.0f * 0.1f * 1000.0f) / (0.1f - 1000.0f);

	//result[0] = vec4(A, 0.0f, 0.0f, 0.0f);
	//result[1] = vec4(0.0f, q, 0.0f, 0.0f);
	//result[2] = vec4(0.0f, 0.0f, B, -1.0f);
	//result[3] = vec4(0.0f, 0.0f, C, 0.0f);

	proj_matrix->m0() = A;
	proj_matrix->m5() = q;
	proj_matrix->m10() = B;
	proj_matrix->m11() = -1.0f;
	proj_matrix->m14() = C;
	proj_matrix->m15() = 0.0f;

	//return result;

	//*proj_matrix = (Matrix&)pm;
};