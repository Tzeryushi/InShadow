#pragma once

#include <GL/glew.h>
#include <string>

/**************************************
This shader compilation structure uses the official OpenGL shader documentation
as an immediate reference. There are barely any abstractions at this point, so
the structure of this class is very much up for change.

object compilation<->error handling->program creation->attatchment->linking->cleanup

Consider: binary upload - cache relevant shaders to decrease fetch time
I doubt there will be enough shaders to warrant this, but its still decent practice
***************************************/
namespace YabeEngine
{
	class ShaderProg
	{
	public:
		ShaderProg();
		~ShaderProg();

		//main uses are linking and compiling
		void shadersCompile(const std::string& vertFP, const std::string& fragFP);
		void shaderLink();

		void use();
		void unuse();
		void bindAttribute(const std::string att);
		//void unbindAttribute(const std::string att);

		GLuint getUniLoc(const std::string& uniName);	//very useful as a public

	private:
		void compile(const std::string& fp, GLuint id);		//specific basis, revise?

		int _numAttributes;		//consider structure revision
		GLuint _programID;
		GLuint _vertID;
		GLuint _fragID;
	};
}