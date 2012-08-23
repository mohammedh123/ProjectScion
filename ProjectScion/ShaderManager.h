#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H
 
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
 
class ShaderManager {
private:
	std::map<std::string, std::unique_ptr<sf::Shader>> shaderMap;
	
	ShaderManager(const ShaderManager&);
	ShaderManager& operator=(const ShaderManager&);
public:
	ShaderManager();
	~ShaderManager();

	sf::Shader* LoadFromFile(const std::string& filename, sf::Shader::Type type);
	sf::Shader* LoadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
};

#endif

