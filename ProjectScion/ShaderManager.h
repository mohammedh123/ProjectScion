#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H
 
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
 
class ShaderInfo
{
public:
	ShaderInfo(const std::string& vsf, const std::string& fsf)
	{
		vertexShaderFilename = vsf;
		fragmentShaderFilename = fsf;
	}
	~ShaderInfo()
	{

	}
	bool operator==(const ShaderInfo &other) const {
		return ((vertexShaderFilename == other.getVertexShader()) && (fragmentShaderFilename == other.getFragmentShader()));
	}
	std::string getVertexShader() const{return vertexShaderFilename;}
	std::string getFragmentShader() const {return fragmentShaderFilename;}

private:
	ShaderInfo(const ShaderInfo&);
	ShaderInfo& operator=(const ShaderInfo&);
protected:
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;

		
};

class ShaderManager {
private:
	std::map<std::pair<std::string, std::string>, std::unique_ptr<sf::Shader>> shaderMap;
	
	ShaderManager(const ShaderManager&);
	ShaderManager& operator=(const ShaderManager&);
public:
	ShaderManager();
	~ShaderManager();

	sf::Shader* LoadFromFile(const std::string& filename, sf::Shader::Type type);
	sf::Shader* LoadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
};

#endif

