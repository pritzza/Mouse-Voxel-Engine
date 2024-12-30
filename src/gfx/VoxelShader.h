#pragma once

#include "gl/ShaderProgram.h"

#include "../voxel/Voxel.h"

typedef std::string_view FilePath;
typedef std::string_view UniformName;

class VoxelShader
{
public:
	void use() const
	{
		program.use();
	}

	void unuse() const
	{
		program.unuse();
	}

protected:
    ShaderProgram program;

	void init()
	{
		program.init(
			GET_VERTEX_SHADER_PATH(),
			GET_GEOMETRY_SHADER_PATH(),
			GET_FRAGMENT_SHADER_PATH(),
			GET_UNIFORM_NAMES()
		);
	}

protected:
    virtual constexpr FilePath GET_VERTEX_SHADER_PATH() const = 0;
    virtual constexpr FilePath GET_GEOMETRY_SHADER_PATH() const = 0;
    virtual constexpr FilePath GET_FRAGMENT_SHADER_PATH() const = 0;

    virtual const std::vector<UniformName> GET_UNIFORM_NAMES() const = 0;
};
