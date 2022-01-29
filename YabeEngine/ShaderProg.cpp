#include "ShaderProg.h"
#include "Errors.h"

#include <fstream>	//fstream is a TEMPORARY solution. This is terrible for large shaders!
#include <vector>

namespace YabeEngine
{

	ShaderProg::ShaderProg() :_numAttributes(0), _programID(0), _vertID(0), _fragID(0)
	{
	}


	ShaderProg::~ShaderProg()
	{
	}


	/******************************************************************
	shadersCompile takes in the filepaths for the vertex and fragment
	shaders and handles shader initiation
	filepaths should never change, are left constant as a precaution
	******************************************************************/
	void ShaderProg::shadersCompile(const std::string& vertFP, const std::string& fragFP)
	{
		_programID = glCreateProgram();		//reserving our program

		//sticking any of these IDs are prone to failure - error handling is done here
		_vertID = glCreateShader(GL_VERTEX_SHADER);	//sticking vertex shader id
		if (_vertID == 0)
		{
			lethalError("Vertex Shader could not be created; check filepath.");
		}

		_fragID = glCreateShader(GL_FRAGMENT_SHADER);	//sticking fragment shader id
		if (_fragID == 0)
		{
			lethalError("Fragment Shader could not be created; check filepath.");
		}

		compile(vertFP, _vertID);
		compile(fragFP, _fragID);
	}

	/******************************************************************
	shaderLink links our program and attaches our shaders. The structure
	is directly lifted from the Khronos OpenGL documentation.
	******************************************************************/
	void ShaderProg::shaderLink()
	{
		// Attach our shaders to our program
		glAttachShader(_programID, _vertID);
		glAttachShader(_programID, _fragID);

		// Link our program
		glLinkProgram(_programID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(_programID);
			// Don't leak shaders either.
			glDeleteShader(_vertID);
			glDeleteShader(_fragID);

			// Use the infoLog as you see fit.
			std::printf("%s\n", &(errorLog[0]));
			lethalError("Shaders could not link!");
		}

		// Always detach shaders after a successful link.
		glDetachShader(_programID, _vertID);
		glDetachShader(_programID, _fragID);
		glDeleteShader(_vertID);
		glDeleteShader(_fragID);
	}

	/******************************************************************
	use uses the program and enables all bound attributes
	******************************************************************/
	void ShaderProg::use()
	{
		glUseProgram(_programID);
		//each bound attribute must be enabled
		for (int i = 0; i < _numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	/******************************************************************
	unuse unuses the program and disables all bound attributes
	******************************************************************/
	void ShaderProg::unuse()
	{
		glUseProgram(0);
		glUseProgram(_programID);
		//each bound attribute must be disabled
		for (int i = 0; i < _numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}


	/******************************************************************
	attribute adds things like position, color, texture, etc. Parameters
	here directly correspond to the implementation of the
	******************************************************************/
	void ShaderProg::bindAttribute(const std::string att)
	{
		glBindAttribLocation(_programID, _numAttributes++, att.c_str());
	}

	/******************************************************************
	getUniLoc gives the location of a uniform in the shader. this can be
	EXTREMELY useful, to the point that I included it despite some leaks
	in the class encapsulation. I could overhaul a lot of the design of
	this class so I wouldn't need it, but at this point its a bit counter-
	productive.
	******************************************************************/
	GLuint ShaderProg::getUniLoc(const std::string & uniName)
	{
		GLint locCheck = glGetUniformLocation(_programID, uniName.c_str());
		if (locCheck == GL_INVALID_INDEX)
		{
			lethalError("Uniform " + uniName + " not found in shader...!");
		}
		return locCheck;
	}

	/******************************************************************
	compile (ShaderProg private) goes through individual shader compilation
	to prevent redundant code. Accepts the shader filepath and its
	corresponding shader id.
	******************************************************************/
	void ShaderProg::compile(const std::string& fp, GLuint id)
	{
		//steams for reading
		//TODO: remove fstream, find a faster file reading method. Library?
		std::ifstream file(fp);
		if (file.fail())	//ifstream can fail easily, and screw everything up
		{
			perror(fp.c_str());
			lethalError("Could not establish file stream with " + fp);
		}

		std::string fGuts;		//file entirity
		std::string line;

		//reading the file in line by line
		while (std::getline(file, line))
		{
			fGuts += (line + "\n");
		}

		file.close();
		const char* gutsPointer = fGuts.c_str();	//opengl only takes c strings

		//step 2 of object compilation: getting the source
		glShaderSource(id, 1, &gutsPointer, nullptr);
		//step 3 of object compilation, compiling
		glCompileShader(id);

		//opengl has its own error handling methods for internal functions
		GLint isCompiled = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

		//error handling format pulled directly from khronos' opengl doc
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.

			//TODO: keep an eye on this, things go wrong here

			//I'm using my own error handling
			std::printf("%s\n", &(errorLog[0]));
			lethalError("Shader failed compilation.");
		}
	}

}