#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

#include "GLTypeDefs.h"

#include <glm/mat4x4.hpp>

typedef std::string_view FilePath;
typedef std::string_view UniformName;

class ShaderProgram
{
private:
	static inline constexpr GLuint NULL_HANDLE{ 0 };
	static inline constexpr bool PRINT_ERRORS{ true };

private:
	GLuint handle{ NULL_HANDLE };
	std::unordered_map<UniformName, GLint> uniforms;

public:
	ShaderProgram() = default;
	ShaderProgram(
		const FilePath& vertexShaderPath,
		const std::optional<FilePath>& geometryShaderPath,
		const FilePath& fragmentShaderPath,
		const std::vector<UniformName>& uniformNames
	);

	~ShaderProgram();

	void init(
		const FilePath& vertexShaderPath,
		const std::optional<FilePath>& geometryShaderPath,
		const FilePath& fragmentShaderPath,
		const std::vector<UniformName>& uniformNames
	);

	void setUniformf(const UniformName& name, float value) const;
	void setUniformi(const UniformName& name, int value) const;
	void setUniformVec3f(const UniformName& name, const glm::vec3& v) const;
	void setUniformMat4(const UniformName& name, const glm::mat4& matrix) const;

	void use() const;
	void unuse() const;	// great name lol

	const GLuint get() const;

private:

	void createProgram(GLuint vertex, GLuint geometry, GLuint fragment);

	const GLuint createShader(
		GLuint shaderType, 
		const FilePath& sourcePath
	) const;

	void checkStatus(
		GLuint statusType, 
		GLuint handle, 
		const FilePath& name
	) const;

	void findUniforms(const std::vector<UniformName>& uniformNames);

};
