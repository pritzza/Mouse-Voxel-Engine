#include "ShaderProgram.h"

#include <glad/glad.h>

#include <optional>

#include <glm/gtc/type_ptr.hpp>

#include "../Lighting.h"

#include "../../util/FileReader.h"

ShaderProgram::ShaderProgram(
    const std::string_view& vertexShaderPath, 
    const std::optional<std::string_view>& geometryShaderPath,
    const std::string_view& fragmentShaderPath, 
    const std::vector<std::string_view>& uniformNames
)
{
    init(
        vertexShaderPath, 
        geometryShaderPath, 
        fragmentShaderPath, 
        uniformNames
    );
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(handle);
}

void ShaderProgram::init(
    const std::string_view& vertexShaderPath,
    const std::optional<std::string_view>& geometryShaderPath,
    const std::string_view& fragmentShaderPath, 
    const std::vector<std::string_view>& uniformNames
)
{
    const GLuint vertexShader{ createShader(
        GL_VERTEX_SHADER,
        vertexShaderPath
    ) };

    GLuint geometryShader{ NULL_HANDLE };
    
    if (geometryShaderPath.has_value()) 
    {
        geometryShader = createShader(
            GL_GEOMETRY_SHADER,
            geometryShaderPath.value()
        );
    }

    const GLuint fragmentShader{ createShader(
        GL_FRAGMENT_SHADER,
        fragmentShaderPath
    ) };

    const bool geometryShaderValidOrOmitted{
        (geometryShaderPath.has_value() && geometryShader != NULL_HANDLE) ||
        !geometryShaderPath.has_value()
    };

    const bool shadersAreValid{
        vertexShader    != NULL_HANDLE &&
        fragmentShader  != NULL_HANDLE &&
        geometryShaderValidOrOmitted
    };

    if (shadersAreValid)
    {
        createProgram(vertexShader, geometryShader, fragmentShader);

        // free shader resources
        glDeleteShader(vertexShader);
        glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);

        use();

        findUniforms(uniformNames);
    }
    else
    {
        std::cerr << "Error! Something went wrong with creating shaders.\n";
    }
}

void ShaderProgram::setUniformf(const std::string_view& name, float value) const
{
    glUniform1f(uniforms.at(name), value);
}

void ShaderProgram::setUniformi(const std::string_view& name, int value) const
{
    glUniform1i(uniforms.at(name), value);
}

void ShaderProgram::setUniformVec3f(const std::string_view& name, const glm::vec3& v) const
{
    glUniform3fv(uniforms.at(name), 1, &v[0]);
}

void ShaderProgram::setUniformMat4(
    const std::string_view& name, 
    const glm::mat4& matrix
) const
{
    glUniformMatrix4fv(uniforms.at(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setUniformMaterial(
    const std::string_view& shininess,
    const std::string_view& ambient,
    const std::string_view& diffuse,
    const std::string_view& specular,
    const Material& material
) const
{
    // todo: there's no way i know of doing this, but need to somehow
    // statically assert that names of Material members line up with
    // atleast whats in the shader, if not both

    glUniform1f(uniforms.at(shininess), material.shininess);
    glUniform3fv(uniforms.at(ambient),  1, &material.ambientColor [0]);
    glUniform3fv(uniforms.at(diffuse),  1, &material.diffuseColor [0]);
    glUniform3fv(uniforms.at(specular), 1, &material.specularColor[0]);
}

void ShaderProgram::setUniformLight(
    const std::string_view& position,
    const std::string_view& ambient,
    const std::string_view& diffuse,
    const std::string_view& specular,
    const Light& light
) const
{
    // todo: there's no way i know of doing this, but need to somehow
    // statically assert that names of Material members line up with
    // atleast whats in the shader, if not both

    glUniform3fv(uniforms.at(position), 1,  &light.position     [0]);
    glUniform3fv(uniforms.at(ambient),  1,  &light.ambientColor [0]);
    glUniform3fv(uniforms.at(diffuse),  1,  &light.diffuseColor [0]);
    glUniform3fv(uniforms.at(specular), 1,  &light.specularColor[0]);
}

void ShaderProgram::use() const
{
    glUseProgram(handle);
}

const GLuint ShaderProgram::get() const
{
    if (handle == NULL_HANDLE)
    {
        if (PRINT_ERRORS)
            std::cerr << "Error! Accessing invalid Shader Program handle.\n";
    }

    return handle;
}

const GLuint ShaderProgram::createShader(
    const GLuint shaderType, 
    const std::string_view& sourcePath
) const
{
    // attempt to get contents of file at sourcePath
    std::optional<std::string> sourcePathContents{ 
        FileReader::getContents(sourcePath) 
    };
    
    // if contents were found at sourcePath
    if (sourcePathContents.has_value())
    {
        // get C string of source code
        const char* const source{ sourcePathContents.value().data() };

        // create shader
        const GLuint shader{ glCreateShader(shaderType) };

        // upload source code to shader
        glShaderSource(shader, 1, &source, NULL);

        // compile shader
        glCompileShader(shader);

        // check status
        checkStatus(GL_COMPILE_STATUS, shader, sourcePath);

        return shader;
    }
    else
        return -1;  // return -1 if something went wrong
}

void ShaderProgram::checkStatus(
    const GLuint statusType, 
    const GLuint id,
    const std::string_view& name
) const
{
    static int success;

    // check status of operation
    glGetShaderiv(id, statusType, &success);

    if (!success)
    {
        static constexpr int INFO_LOG_SIZE{ 512 };
        static char infoLog[INFO_LOG_SIZE];

        // get info log
        glGetShaderInfoLog(id, INFO_LOG_SIZE, NULL, infoLog);

        std::string what{ "do something" };

        switch (statusType)
        {
            case GL_COMPILE_STATUS: what = "compile";   break;
            case GL_LINK_STATUS:    what = "link";      break;
        }

        std::cerr << "Error! " << name << " failed to " << what << ":\n" 
            << infoLog << '\n';
    }
}

void ShaderProgram::findUniforms(const std::vector<std::string_view>& uniformNames)
{
    // find all uniforms
    for (const auto& uniformName : uniformNames)
    {
        // get uniform location
        const GLint uniformLocation{
            glGetUniformLocation(handle, uniformName.data())
        };

        // according to documentation, returns -1 when can't find uniform
        // https://docs.gl/gl3/glGetUniformLocation
        static constexpr GLint INVALID_LOCATION{ -1 };

        if (uniformLocation == INVALID_LOCATION)
        {
            std::cerr << "Error! Couldn't find uniform \""
                << uniformName << "\"\n";
        }
        else
        {
            // add value pair of uniform's name and location to map
            uniforms.insert({ uniformName, uniformLocation });
        }
    }
}

void ShaderProgram::createProgram(const GLuint vertex, const GLuint geometry, const GLuint fragment)
{
    // create program
    handle = glCreateProgram();

    // attach shaders to program
    glAttachShader(handle, vertex);
    glAttachShader(handle, fragment);

    if (geometry != NULL_HANDLE)
        glAttachShader(handle, geometry);

    // link program
    glLinkProgram(handle);
    checkStatus(GL_LINK_STATUS, handle, "Shader Program (" + handle + ')');
}
