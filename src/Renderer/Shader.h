#include <string>
#include "../Common.h"

class Shader {
	public:
		int ID = -1;
		//void Load(const char* vertexPath, const char* fragmentPath);
		void Load(std::string vertexPath, std::string fragmentPath);
		void Use();
		void SetBool(const std::string& name, bool value);
		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetMat4(const std::string& name, glm::mat4 value);
		void SetVec3(const std::string& name, glm::vec3& value);
};