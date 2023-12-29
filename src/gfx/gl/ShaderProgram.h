#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

#include "GLTypeDefs.h"

#include <glm/mat4x4.hpp>

struct Material;
struct Light;

class ShaderProgram
{
private:
	static inline constexpr GLuint NULL_HANDLE{ 0 };
	static inline constexpr bool PRINT_ERRORS{ true };

private:
	GLuint handle{ NULL_HANDLE };
	std::unordered_map<std::string_view, GLint> uniforms;

public:
	ShaderProgram() = default;
	ShaderProgram(
		const std::string_view& vertexShaderPath,
		const std::optional<std::string_view>& geometryShaderPath,
		const std::string_view& fragmentShaderPath,
		const std::vector<std::string_view>& uniformNames
	);

	~ShaderProgram();

	void init(
		const std::string_view& vertexShaderPath,
		const std::optional<std::string_view>& geometryShaderPath,
		const std::string_view& fragmentShaderPath,
		const std::vector<std::string_view>& uniformNames
	);

	void setUniformf(const std::string_view& name, float value) const;
	void setUniformi(const std::string_view& name, int value) const;
	void setUniformVec3f(const std::string_view& name, const glm::vec3& v) const;
	void setUniformMat4(const std::string_view& name, const glm::mat4& matrix) const;

	void setUniformMaterial(
		const std::string_view& shininess,
		const std::string_view& ambient,
		const std::string_view& diffuse,
		const std::string_view& specular,
		const Material& mat
	) const;
	void setUniformLight(
		const std::string_view& position,
		const std::string_view& ambient,
		const std::string_view& diffuse,
		const std::string_view& specular, 
		const Light& light
	) const;

	void use() const;
	void unuse() const;	// great name lol

	const GLuint get() const;

private:

	void createProgram(GLuint vertex, GLuint geometry, GLuint fragment);

	const GLuint createShader(
		GLuint shaderType, 
		const std::string_view& sourcePath
	) const;

	void checkStatus(
		GLuint statusType, 
		GLuint handle, 
		const std::string_view& name
	) const;

	void findUniforms(const std::vector<std::string_view>& uniformNames);

};

