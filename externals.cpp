#include "externals.h"
#include "deps/stb/stb_image.h"

//Shader
std::string VPEShader::ReadFile(const char* FilePath) const
{
	std::string FileContents;
	if (std::ifstream FileStream{ FilePath, std::ios::in })
	{
		FileContents.assign((std::istreambuf_iterator<char>(FileStream)), std::istreambuf_iterator<char>());
	}
	return FileContents;
}
void VPEShader::LoadShader(VPEFileDir VertexShaderDir, VPEFileDir FragmentShaderDir, const bool& ShowDebug)
{
	try
	{
		if (!std::filesystem::exists(VertexShaderDir))
		{
			const std::string _error = "Arquivo: \"" + VertexShaderDir + "\" nao encontrado.";
			throw std::runtime_error(_error);
		}
		else if (!std::filesystem::exists(FragmentShaderDir))
		{
			const std::string _error = "Arquivo: \"" + FragmentShaderDir + "\" nao encontrado.";
			throw std::runtime_error(_error);
		}
	}
	catch (std::runtime_error& out)
	{
		std::cerr << out.what() << std::endl;
		exit(-1);
	}

	VPEShaderID
		VertShaderId = glCreateShader(GL_VERTEX_SHADER),
		FragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	const std::string
		VertexShaderSource = this->ReadFile(VertexShaderDir.c_str()),
		FragmentShaderSource = this->ReadFile(FragmentShaderDir.c_str());

	if (ShowDebug)
	{
		std::cout << "Compilando " << VertexShaderDir << std::endl;
	}
	const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
	glShaderSource(VertShaderId, 1, &VertexShaderSourcePtr, nullptr);
	glCompileShader(VertShaderId);

	if (ShowDebug)
	{
		std::cout << "Compilando " << FragmentShaderDir << std::endl;
	}
	const char* FragmentShaderSourcePtr = FragmentShaderSource.c_str();
	glShaderSource(FragShaderId, 1, &FragmentShaderSourcePtr, nullptr);
	glCompileShader(FragShaderId);

	if (ShowDebug)
	{
		std::cout << "Linkando Programa" << std::endl;
	}

	VPEShaderID ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertShaderId);
	glAttachShader(ProgramId, FragShaderId);
	glLinkProgram(ProgramId);

	glDetachShader(ProgramId, VertShaderId);
	glDetachShader(ProgramId, FragShaderId);

	glDeleteShader(VertShaderId);
	glDeleteShader(FragShaderId);

	this->ShaderID = ProgramId;
	return;
}
void VPEShader::UseShader() const
{
	glUseProgram(this->ShaderID);

	return;
}
void VPEShader::DeleteShader()
{
	glDeleteShader(this->ShaderID);

	return;
}

//Texture
void VPETexture::LoadTexture(VPEFileDir TextureDir, const bool& FilpVertically, const bool& ShowDebug)
{
	try
	{
		if (!std::filesystem::exists(TextureDir))
		{
			const std::string _error = "Arquivo: \"" + TextureDir + "\" nao encontrado.";
			throw std::runtime_error(_error);
		}
	}
	catch (const std::runtime_error& out)
	{
		std::cerr << out.what() << std::endl;
		exit(-1);
	}

	if (ShowDebug)
	{
		std::cout << "Carregando Textura: " << TextureDir << std::endl;
	}
	stbi_set_flip_vertically_on_load(FilpVertically);

	int
		TextureWidth = 0,
		TextureHeight = 0,
		NumberOfComponents = 0;

	unsigned char* TextureData = stbi_load(TextureDir.c_str(), &TextureWidth, &TextureHeight, &NumberOfComponents, 4);

	VPETextureID TextureId = 0;
	glGenTextures(1, &TextureId);

	glBindTexture(GL_TEXTURE_2D, TextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureWidth, TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(TextureData);

	this->Texture = TextureId;
	return;
}
void VPETexture::UseTexture(const GLenum& Target)
{
	glBindTexture(Target, this->Texture);

	return;
}
void VPETexture::DeleteTexture()
{
	glDeleteTextures(1, &this->Texture);

	return;
}

//Draw SimpleObject
void __VPESimpleObject::Draw(const VPEShader& Shader, const glm::mat4& ViewProjection) const
{
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, Shader.MVPName.c_str()), 1, GL_FALSE, glm::value_ptr(ViewProjection * glm::translate(Id, this->Position)));

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->VertexAmount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(VPE_NULL);

	return;
}

//Draw SimpleInstancedObject
void __VPEInstancedObject::Draw(const VPEShader& Shader, const glm::mat4& ViewProjection) const
{
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, Shader.MVPName.c_str()), 1, GL_FALSE, glm::value_ptr(ViewProjection));

	glBindVertexArray(this->VAO);
	glDrawElementsInstanced(GL_TRIANGLES, this->VertexAmount, GL_UNSIGNED_INT, nullptr, this->Instances.size());
	glBindVertexArray(VPE_NULL);

	return;
}

//Draw AdvancedObject
void __VPEAdvancedObject::Draw(const VPEShader& Shader, const glm::mat4& ViewProjection)
{
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, Shader.MVPName.c_str()), 1, GL_FALSE, glm::value_ptr(ViewProjection));
	if (!this->TextureMap->Map.empty())
	{
		VPEuint counter = 0;
		for (const std::string i : this->Index)
		{
			counter++;
			glUniform1i(glGetUniformLocation(Shader.ShaderID, ("sampler" + std::to_string(counter)).c_str()), counter);

			(this->TextureMap->Map.find(i) != this->TextureMap->Map.end()) ?
				glBindTexture(GL_TEXTURE_2D, this->TextureMap->Map[i]) : glBindTexture(GL_TEXTURE_2D, 0);

			glBindVertexArray(this->VAOmap[i].VAO);
			glDrawElements(GL_TRIANGLES, this->VAOmap[i].VertexAmount, GL_UNSIGNED_INT, nullptr);
		}
	}
	else
	{
		glUniform1i(glGetUniformLocation(Shader.ShaderID, "sampler0"), 0);

		for (const std::string i : this->Index)
		{
			glBindVertexArray(this->VAOmap[i].VAO);
			glDrawElements(GL_TRIANGLES, this->VAOmap[i].VertexAmount, GL_UNSIGNED_INT, nullptr);
		}
	}
	return;
}
void __VPEAdvancedObject::DrawSingle(VPEStringIndex& Index, const VPEShader& Shader, const glm::mat4& ViewProjection)
{
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, Shader.MVPName.c_str()), 1, GL_FALSE, glm::value_ptr(ViewProjection));

	glBindVertexArray(this->VAOmap[Index].VAO);
	glDrawElements(GL_TRIANGLES, this->VAOmap[Index].VertexAmount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(VPE_NULL);

	return;
}

std::optional<std::string> VPEObjectEngine::GetFilteredLine(const std::string& InputLine) const
{
	std::string subLine = VPE_NULL_STRING;
	VPEcULongLong InputLineFind = InputLine.find_first_of("-0123456789");

	if (InputLineFind != std::string::npos)
	{
		subLine.insert(subLine.begin(), InputLine.begin() + InputLineFind, InputLine.end());
		return subLine;
	}
	return std::nullopt;
}
std::string VPEObjectEngine::GetFilteredIndex(const std::string& InputLine) const
{
	std::string subLine = VPE_NULL_STRING;
	subLine.insert(subLine.begin(), InputLine.begin() + InputLine.find(VPE_SPACE_CHAR) + 1, InputLine.end());

	return subLine;
}
VPEDefaultFun VPEObjectEngine::CheckStringValues(const VPEMode& Mode, const std::string& value, const std::string& filePath) const
{
	constexpr const char* invalidChars = "abcdefghijklmnopqrstuvwxyz ";
	if (value.find_first_of(invalidChars) != std::string::npos)
	{
		switch (Mode)
		{
		case VPE_CHECK_VERTEX_MODE:
			throw std::runtime_error("Valor invalido encontrado na leitura do vertice: " + value + ". Arquivo: " + filePath);
			break;
		case VPE_CHECK_NORMAL_MODE:
			throw std::runtime_error("Valor invalido encontrado na leitura da normal: " + value + ". Arquivo: " + filePath);
			break;
		case VPE_CHECK_UV_MODE:
			throw std::runtime_error("Valor invalido encontrado na leitura da uv: " + value + ". Arquivo: " + filePath);
			break;
		case VPE_CHECK_INDEX_MODE:
			throw std::runtime_error("Valor invalido encontrado na leitura do indice: " + value + ". Arquivo: " + filePath);
			break;
		}
	}
	return;
}
glm::ivec3 VPEObjectEngine::StringToIndex(const std::string& x, const std::string& y, const std::string& z, const VPEuint& Decrease) const
{
	return glm::ivec3{std::stoi(x) - Decrease, std::stoi(y) - Decrease, std::stoi(z) - Decrease};
}

VPEDefaultFun VPETextureMap::Insert(VPEStringIndex& VAOMapIndex, VPEcUint& Texture)
{
	this->Map.insert(std::pair<VPEStringIndex, VPEcUint>(VAOMapIndex, Texture));
	return;
}
VPEDefaultFun VPETextureMap::Remove(VPEStringIndex& Index)
{
	try
	{
		if (this->Map[Index] == VPE_NULL)
		{
			throw std::exception(("Nao foi possivel encontrar o indice: " + Index + " no mapa de texturas.").c_str());
		}
		this->Map.erase(Index);
	}
	catch (VPE_CATCH_EXCEPTION)
	{
		VPE_TRY_CATCH_EXIT(-1);
	}
	return;
}

OBJData VPEObjectEngine::LoadSimpleObjData(VPEFileDir filePath, const std::string& skip_chars, const std::string& mode_chars) const
{
	std::vector<glm::vec3> vData;
	std::vector<glm::vec3> normalData;
	std::vector<glm::vec2> uvData;
	std::vector<glm::ivec3> vIndex;
	std::vector<glm::ivec3> normalIndex;
	std::vector<glm::ivec3> uvIndex;

	std::ifstream file;
	std::istringstream iss, vIss, nIss, uvIss;
	std::string line = VPE_NULL_STRING;

	file.open(filePath);
	try
	{
		if (!file.is_open())
		{
			throw std::runtime_error("Erro ao abrir o mesh: " + filePath);
		}
		while (!file.eof())
		{
			std::getline(file, line);
			if (file.good())
			{
				if (line.find_first_of(skip_chars.c_str()) == std::string::npos)
				{
					if (line.find_first_of(mode_chars) != std::string::npos)
					{
						const std::string FilteredLine = this->GetFilteredLine(line).value_or(line);
						if (line[0] == 'v')
						{
							if (line[1] == VPE_SPACE_CHAR)
							{
								vIss.clear();
								vIss.str(FilteredLine);
								std::array<std::string, 3> strValue;
								std::array<float, 3> value;
								for (VPEushort i = 0; vIss.good() == true; i++)
								{
									vIss >> strValue[i];
									value[i] = std::stof(strValue[i]);
								}
								vData.push_back(glm::vec3{value[0], value[1], value[2]});
							}
							else if (line[1] == 'n')
							{
								nIss.clear();
								nIss.str(FilteredLine);
								std::array<std::string, 3> strValue;
								std::array<float, 3> value;
								for (VPEushort i = 0; nIss.good() == true; i++)
								{
									nIss >> strValue[i];
									value[i] = std::stof(strValue[i]);
								}
								normalData.push_back(glm::vec3{value[0], value[1], value[2]});
							}
							else if (line[1] == 't')
							{
								uvIss.clear();
								uvIss.str(FilteredLine);
								std::array<std::string, 2> strValue;
								std::array<float, 2> value;
								for (VPEushort i = 0; uvIss.good() == true; i++)
								{
									if (i >= strValue.size())
									{
										break;
									}
									uvIss >> strValue[i];
									value[i] = std::stof(strValue[i]);
								}
								uvData.push_back(glm::vec2{value[0], value[1]});
							}
						}
						else if (line[0] == 'f')
						{
							std::vector<std::string> indVStr, indNormalStr, indUvStr;
							std::string strValue = VPE_NULL_STRING;
							iss.clear();
							iss.str(FilteredLine);
							if (!vData.empty())
							{
								if (!normalData.empty() && !uvData.empty())
								{
									while (iss.good() == true)
									{
										iss >> strValue;
										indVStr.push_back(VPE_NULL_STRING);
										indUvStr.push_back(VPE_NULL_STRING);

										indVStr.back().insert(indVStr.back().begin(), strValue.begin(), strValue.begin() + strValue.find('/'));
										strValue.erase(strValue.begin(), strValue.begin() + strValue.find('/') + 1);

										indUvStr.back().insert(indUvStr.back().begin(), strValue.begin(), strValue.begin() + strValue.find('/'));
										strValue.erase(strValue.begin(), strValue.begin() + strValue.find('/') + 1);

										indNormalStr.push_back(strValue);
									}
									strValue.clear();
								}
								else if (normalData.empty() == true && !uvData.empty())
								{
									while (iss.good() == true)
									{
										iss >> strValue;
										indVStr.push_back(VPE_NULL_STRING);
										indUvStr.push_back(VPE_NULL_STRING);

										indVStr.back().insert(indVStr.back().begin(), strValue.begin(), strValue.begin() + strValue.find('/'));
										indUvStr.back().insert(indUvStr.back().begin(), strValue.begin() + strValue.find('/') + 1, strValue.end());
									}
									strValue.clear();
								}
								else if (!normalData.empty() && uvData.empty() == true)
								{
									while (iss.good() == true)
									{
										iss >> strValue;
										indVStr.push_back(VPE_NULL_STRING);
										indNormalStr.push_back(VPE_NULL_STRING);

										indVStr.back().insert(indVStr.back().begin(), strValue.begin(), strValue.begin() + strValue.find('/'));
										strValue.erase(strValue.begin(), strValue.begin() + strValue.find('/') + 1);
										strValue.erase(strValue.begin(), strValue.begin() + strValue.find('/') + 1);

										indNormalStr.push_back(strValue);
									}
									strValue.clear();
								}
								else if (normalData.empty() == true && uvData.empty() == true)
								{
									while (iss.good() == true)
									{
										iss >> strValue;
										indVStr.push_back(VPE_NULL_STRING);

										indVStr.back().insert(indVStr.back().begin(), strValue.begin(), strValue.begin() + strValue.find('/'));
									}
									strValue.clear();
								}
							}
							else
							{
								throw std::runtime_error("O vetor de indices de vertices esta vazio. Dir: " + filePath);
							}
							switch (indVStr.size())
							{
							case 3:
								if (!vData.empty())
								{
									vIndex.push_back(glm::ivec3{std::stoi(indVStr[0]) - 1, std::stoi(indVStr[1]) - 1, std::stoi(indVStr[2]) - 1});
								}
								if (!normalData.empty())
								{
									normalIndex.push_back(glm::ivec3{std::stoi(indNormalStr[0]) - 1, std::stoi(indNormalStr[1]) - 1, std::stoi(indNormalStr[2]) - 1});
								}
								if (!uvData.empty())
								{
									uvIndex.push_back(glm::ivec3{std::stoi(indUvStr[0]) - 1, std::stoi(indUvStr[1]) - 1, std::stoi(indUvStr[2]) - 1});
								}
								break;
							case 4:
								if (!vData.empty())
								{
									vIndex.push_back(glm::ivec3{std::stoi(indVStr[0]) - 1, std::stoi(indVStr[1]) - 1, std::stoi(indVStr[2]) - 1});
									vIndex.push_back(glm::ivec3{std::stoi(indVStr[2]) - 1, std::stoi(indVStr[3]) - 1, std::stoi(indVStr[0]) - 1});
								}
								if (!normalData.empty())
								{
									normalIndex.push_back(glm::ivec3{std::stoi(indNormalStr[0]) - 1, std::stoi(indNormalStr[1]) - 1, std::stoi(indNormalStr[2]) - 1});
									normalIndex.push_back(glm::ivec3{std::stoi(indNormalStr[2]) - 1, std::stoi(indNormalStr[3]) - 1, std::stoi(indNormalStr[0]) - 1});
								}
								if (!uvData.empty())
								{
									uvIndex.push_back(glm::ivec3{std::stoi(indUvStr[0]) - 1, std::stoi(indUvStr[1]) - 1, std::stoi(indUvStr[2]) - 1});
									uvIndex.push_back(glm::ivec3{std::stoi(indUvStr[2]) - 1, std::stoi(indUvStr[3]) - 1, std::stoi(indUvStr[0]) - 1});
								}
								break;
							default:
								break;
								//throw std::runtime_error("A enumeracao de indices formam padroes invalidos(!quad !triangle). Dir: " + filePath);
							}
							indVStr.clear();
							indNormalStr.clear();
							indUvStr.clear();
						}
					}
				}
			}
		}
	}
	catch (const std::runtime_error& out)
	{
		std::cerr << out.what() << std::endl;
		exit(-1);
	}
	file.close();
	
	return OBJData
	{ 
		.VData = VertexData{.Position = vData, .Index = vIndex}, 
		.NData = NormalData{.Normal = normalData, .Index = normalIndex}, 
		.UVData = UVData{.UV = uvData, .Index = uvIndex} 
	};
}
VPEObjectDataMap VPEObjectEngine::LoadAdvancedObjData(VPEFileDir filePath, VPEcChar& IndexIdentifier, std::vector<std::string>* IndexOut, VPEcBool& ModifyIndex, const std::string& skip_chars, const std::string& mode_chars) const
{
	VPEObjectDataMap map;
	std::vector<std::string> index;

	std::ifstream file;
	std::istringstream iss, vIss, nIss, uvIss;
	std::string line = VPE_NULL_STRING;
	
	file.open(filePath);
	try
	{
		if (!file.is_open())
		{
			throw std::runtime_error("Erro ao abrir o mesh: " + filePath);
		}
		while (!file.eof())
		{
			std::getline(file, line);
			if (file.good() == true)
			{
				if (line[0] == IndexIdentifier)
				{
					const std::string IndexString = this->GetFilteredIndex(line);
					map.insert(std::pair<const std::string, OBJData>(IndexString, OBJData{}));
					index.push_back(IndexString);
				}
				else if (line[0] != IndexIdentifier && line.find_first_of(skip_chars.c_str()) == std::string::npos)
				{
					if (line.find_first_of(mode_chars) != std::string::npos)
					{
						const std::string FilteredLine = this->GetFilteredLine(line).value_or(line);
						if (line[0] == 'v')
						{
							if (line[1] == VPE_SPACE_CHAR)
							{
								vIss.clear();
								vIss.str(FilteredLine);
								std::array<std::string, 3> strValue;
								std::array<float, 3> value;
								for (VPEushort i = 0; vIss.good() == true; i++)
								{
									vIss >> strValue[i];
									value[i] = std::stof(strValue[i]);
								}
								map[index.back()].VData.Position.push_back(glm::vec3{value[0], value[1], value[2]});
							}
							else if (line[1] == 'n')
							{
								nIss.clear();
								nIss.str(FilteredLine);
								std::array<std::string, 3> strValue;
								std::array<float, 3> value;
								for (VPEushort i = 0; nIss.good() == true; i++)
								{
									nIss >> strValue[i];
									value[i] = std::stof(strValue[i]);
								}
								map[index.back()].NData.Normal.push_back(glm::vec3{value[0], value[1], value[2]});
							}
							else if (line[1] == 't')
							{
								uvIss.clear();
								uvIss.str(FilteredLine);
								std::array<std::string, 2> strValue;
								std::array<float, 2> value;
								for (VPEushort i = 0; uvIss.good() == true; i++)
								{
									if (i >= strValue.size())
									{
										break;
									}
									uvIss >> strValue[i];
									value[i] = std::stof(strValue[i]);
								}
								map[index.back()].UVData.UV.push_back(glm::vec2{value[0], value[1]});
							}
						}
						else if (line[0] == 'f')
						{
							std::vector<std::string> indVStr, indNormalStr, indUvStr;
							std::string strValue = VPE_NULL_STRING;
							iss.clear();
							iss.str(FilteredLine);
							if (!map[index.back()].VData.Position.empty())
							{
								if (!map[index.back()].NData.Normal.empty() && !map[index.back()].UVData.UV.empty())
								{
									while (iss.good() == true)
									{
										iss >> strValue;
										indVStr.push_back(VPE_NULL_STRING);
										indUvStr.push_back(VPE_NULL_STRING);

										indVStr.back().insert(indVStr.back().begin(), strValue.begin(), strValue.begin() + strValue.find('/'));
										strValue.erase(strValue.begin(), strValue.begin() + strValue.find('/') + 1);

										indUvStr.back().insert(indUvStr.back().begin(), strValue.begin(), strValue.begin() + strValue.find('/'));
										strValue.erase(strValue.begin(), strValue.begin() + strValue.find('/') + 1);

										indNormalStr.push_back(strValue);
									}
									strValue.clear();
								}
								else if (map[index.back()].NData.Normal.empty() == true && !map[index.back()].UVData.UV.empty())
								{
									while (iss.good() == true)
									{
										iss >> strValue;
										indVStr.push_back(VPE_NULL_STRING);
										indUvStr.push_back(VPE_NULL_STRING);

										indVStr.back().insert(indVStr.back().begin(), strValue.begin(), strValue.begin() + strValue.find('/'));
										indUvStr.back().insert(indUvStr.back().begin(), strValue.begin() + strValue.find('/') + 1, strValue.end());
									}
									strValue.clear();
								}
								else if (!map[index.back()].NData.Normal.empty() && map[index.back()].UVData.UV.empty() == true)
								{
									while (iss.good() == true)
									{
										iss >> strValue;
										indVStr.push_back(VPE_NULL_STRING);
										indNormalStr.push_back(VPE_NULL_STRING);

										indVStr.back().insert(indVStr.back().begin(), strValue.begin(), strValue.begin() + strValue.find('/'));
										strValue.erase(strValue.begin(), strValue.begin() + strValue.find('/') + 1);
										strValue.erase(strValue.begin(), strValue.begin() + strValue.find('/') + 1);

										indNormalStr.push_back(strValue);
									}
									strValue.clear();
								}
								else
								{
									while (iss.good() == true)
									{
										iss >> strValue;
										indVStr.push_back(VPE_NULL_STRING);

										indVStr.back().insert(indVStr.back().begin(), strValue.begin(), strValue.begin() + strValue.find('/'));
									}
									strValue.clear();
								}
							}
							else
							{
								throw std::runtime_error("O vetor de indices de vertices esta vazio. Dir: " + filePath);
							}

							VPEuint vIDecreaseValue = 1, nIDecreaseValue = 1, uvIDecreaseValue = 1;
							if (ModifyIndex)
							{
								if (index.size() == 2)
								{
									for (VPEuint i = 1; i < index.size(); i++)
									{
										vIDecreaseValue = map[index[i - 1]].VData.Position.size() + 1;
										nIDecreaseValue = map[index[i - 1]].NData.Normal.size() + 1;
										uvIDecreaseValue = map[index[i - 1]].UVData.UV.size() + 1;
									}
								}
								else if (index.size() > 2)
								{
									for (VPEuint i = 1; i < index.size(); i++)
									{
										vIDecreaseValue += map[index[i - 1]].VData.Position.size();
										nIDecreaseValue += map[index[i - 1]].NData.Normal.size();
										uvIDecreaseValue += map[index[i - 1]].UVData.UV.size();
									}
								}
							}
							switch (indVStr.size())
							{
							case 3:
								map[index.back()].VData.Index.push_back(glm::ivec3{std::stoi(indVStr[0]) - vIDecreaseValue, std::stoi(indVStr[1]) - vIDecreaseValue, std::stoi(indVStr[2]) - vIDecreaseValue});
								if (!map[index.back()].NData.Normal.empty())
								{
									map[index.back()].NData.Index.push_back(glm::ivec3{std::stoi(indNormalStr[0]) - nIDecreaseValue, std::stoi(indNormalStr[1]) - nIDecreaseValue, std::stoi(indNormalStr[2]) - nIDecreaseValue});
								}
								if (!map[index.back()].UVData.UV.empty())
								{
									map[index.back()].UVData.Index.push_back(glm::ivec3{std::stoi(indUvStr[0]) - uvIDecreaseValue, std::stoi(indUvStr[1]) - uvIDecreaseValue, std::stoi(indUvStr[2]) - uvIDecreaseValue});
								}
								break;
							case 4:
								map[index.back()].VData.Index.push_back(this->StringToIndex(indVStr[0], indVStr[1], indVStr[2], vIDecreaseValue));
								map[index.back()].VData.Index.push_back(this->StringToIndex(indVStr[2], indVStr[3], indVStr[0], vIDecreaseValue));
								if (!map[index.back()].NData.Normal.empty())
								{
									map[index.back()].NData.Index.push_back(this->StringToIndex(indNormalStr[0], indNormalStr[1], indNormalStr[2], nIDecreaseValue));
									map[index.back()].NData.Index.push_back(this->StringToIndex(indNormalStr[2], indNormalStr[3], indNormalStr[0], nIDecreaseValue));
								}
								if (!map[index.back()].UVData.UV.empty())
								{
									map[index.back()].UVData.Index.push_back(this->StringToIndex(indUvStr[0], indUvStr[1], indUvStr[2], uvIDecreaseValue));
									map[index.back()].UVData.Index.push_back(this->StringToIndex(indUvStr[2], indUvStr[3], indUvStr[0], uvIDecreaseValue));
								}
								break;
							default:
								//throw std::runtime_error("A enumeracao de indices formam padroes invalidos(!quad !triangle). Dir: " + filePath);
								break;
							}
						}
					}
				}
			}
		}
	}
	catch (const std::runtime_error& out)
	{
		std::cerr << out.what() << std::endl;
		exit(-1);
	}
	file.close();
	*IndexOut = index;

	return map;
}

VPEVaoMap VPEObjectEngine::GetAdvancedVAO(VPEObjectDataMap& ObjDataMap, const std::span<std::string>& Index) const
{
	VPEVaoMap map;
	std::vector<Vertex> Geometry;
	std::vector<glm::ivec3> ind;

	for (std::string i : Index)
	{
		map.insert(std::pair<const std::string, VAOInfo>(i, VAOInfo{ .VAO = VPE_NULL, .VertexAmount = VPE_NULL }));
		Geometry.reserve(ObjDataMap[i].VData.Index.size() * 3);
		for (VPEuLongLong v = 0; v < ObjDataMap[i].VData.Index.size(); v++)
		{
			if (!ObjDataMap[i].NData.Normal.empty() && !ObjDataMap[i].UVData.UV.empty())
			{
				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].x],
					.Normal = ObjDataMap[i].NData.Normal[ObjDataMap[i].NData.Index[v].x],
					.UV = ObjDataMap[i].UVData.UV[ObjDataMap[i].UVData.Index[v].x] });

				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].y],
					.Normal = ObjDataMap[i].NData.Normal[ObjDataMap[i].NData.Index[v].y],
					.UV = ObjDataMap[i].UVData.UV[ObjDataMap[i].UVData.Index[v].y] });

				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].z],
					.Normal = ObjDataMap[i].NData.Normal[ObjDataMap[i].NData.Index[v].z],
					.UV = ObjDataMap[i].UVData.UV[ObjDataMap[i].UVData.Index[v].z] });
			}
			else if (ObjDataMap[i].NData.Normal.empty() == true && !ObjDataMap[i].UVData.UV.empty())
			{
				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].x],
					.Normal = VPEGetNull<glm::vec3>(),
					.UV = ObjDataMap[i].UVData.UV[ObjDataMap[i].UVData.Index[v].x] });

				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].y],
					.Normal = VPEGetNull<glm::vec3>(),
					.UV = ObjDataMap[i].UVData.UV[ObjDataMap[i].UVData.Index[v].y] });

				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].z],
					.Normal = VPEGetNull<glm::vec3>(),
					.UV = ObjDataMap[i].UVData.UV[ObjDataMap[i].UVData.Index[v].z] });
			}
			else if (!ObjDataMap[i].NData.Normal.empty() && ObjDataMap[i].UVData.UV.empty() == true)
			{
				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].x],
					.Normal = ObjDataMap[i].NData.Normal[ObjDataMap[i].NData.Index[v].x],
					.UV = VPEGetNull<glm::vec2>() });

				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].y],
					.Normal = ObjDataMap[i].NData.Normal[ObjDataMap[i].NData.Index[v].y],
					.UV = VPEGetNull<glm::vec2>() });

				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].z],
					.Normal = ObjDataMap[i].NData.Normal[ObjDataMap[i].NData.Index[v].z],
					.UV = VPEGetNull<glm::vec2>() });
			}
			else
			{
				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].x],
					.Normal = VPEGetNull<glm::vec3>(),
					.UV = VPEGetNull<glm::vec2>() });

				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].y],
					.Normal = VPEGetNull<glm::vec3>(),
					.UV = VPEGetNull<glm::vec2>() });

				Geometry.emplace_back(Vertex{
					.Position = ObjDataMap[i].VData.Position[ObjDataMap[i].VData.Index[v].z],
					.Normal = VPEGetNull<glm::vec3>(),
					.UV = VPEGetNull<glm::vec2>() });
			}
			ind.push_back(glm::ivec3{v * 3, (v * 3) + 1, (v * 3) + 2});
		}
		GLuint VertexBuffer = VPE_NULL, ElementBuffer = VPE_NULL;
		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Geometry.size(), Geometry.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &ElementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * ind.size(), ind.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &map[i].VAO);
		glBindVertexArray(map[i].VAO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));

		glBindVertexArray(VPE_NULL);
		map[i].VertexAmount = ind.size() * 3;

		Geometry.clear();
		ind.clear();
	}
	return map;
}
VPEuint& VPEObjectEngine::GetSimpleVAO(VPESimpleObject object, VPEcBool& ShowLog) const
{
	std::vector<Vertex> geometry;
	std::vector<glm::ivec3> index;

	for (size_t i = 0; i < object->ObjectData.VData.Index.size(); i++)
	{
		geometry.push_back(Vertex{ object->ObjectData.VData.Position[object->ObjectData.VData.Index[i].x], object->ObjectData.NData.Normal[object->ObjectData.NData.Index[i].x], object->ObjectData.UVData.UV[object->ObjectData.UVData.Index[i].x] });
		geometry.push_back(Vertex{ object->ObjectData.VData.Position[object->ObjectData.VData.Index[i].y], object->ObjectData.NData.Normal[object->ObjectData.NData.Index[i].y], object->ObjectData.UVData.UV[object->ObjectData.UVData.Index[i].y] });
		geometry.push_back(Vertex{ object->ObjectData.VData.Position[object->ObjectData.VData.Index[i].z], object->ObjectData.NData.Normal[object->ObjectData.NData.Index[i].z], object->ObjectData.UVData.UV[object->ObjectData.UVData.Index[i].z] });
		index.push_back(glm::ivec3{ i * 3, (i * 3) + 1, (i * 3) + 2 });
	}
	GLuint VertexBuffer = VPE_NULL, ElementBuffer = VPE_NULL, VAO = VPE_NULL;
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * geometry.size(), geometry.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * index.size(), index.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));

	glBindVertexArray(VPE_NULL);

	object->VertexAmount = index.size() * 3;
	if (ShowLog)
	{
		std::cout << "Vertex Loaded: " << object->VertexAmount << std::endl;
		std::cout << "VRAM using: " << sizeof(Vertex) * geometry.size() << std::endl;
	}
	return VAO;
}
VPEuint& VPEObjectEngine::GetSimpleInstancedVAO(VPEInstancedObject object, VPEcBool& ShowLog) const
{
	std::vector<Vertex> geometry;
	std::vector<glm::ivec3> index;

	VPEuint
		VertexBuffer = VPE_NULL,
		ElementBuffer = VPE_NULL,
		InstancesBuffer = VPE_NULL,
		InstanceVAO = VPE_NULL;

	for (size_t i = 0; i < object->InstancedObjectData.VData.Index.size(); i++)
	{
		geometry.push_back(Vertex{ object->InstancedObjectData.VData.Position[object->InstancedObjectData.VData.Index[i].x], object->InstancedObjectData.NData.Normal[object->InstancedObjectData.NData.Index[i].x], object->InstancedObjectData.UVData.UV[object->InstancedObjectData.UVData.Index[i].x] });
		geometry.push_back(Vertex{ object->InstancedObjectData.VData.Position[object->InstancedObjectData.VData.Index[i].y], object->InstancedObjectData.NData.Normal[object->InstancedObjectData.NData.Index[i].y], object->InstancedObjectData.UVData.UV[object->InstancedObjectData.UVData.Index[i].y] });
		geometry.push_back(Vertex{ object->InstancedObjectData.VData.Position[object->InstancedObjectData.VData.Index[i].z], object->InstancedObjectData.NData.Normal[object->InstancedObjectData.NData.Index[i].z], object->InstancedObjectData.UVData.UV[object->InstancedObjectData.UVData.Index[i].z] });
		index.push_back(glm::ivec3{ i * 3, (i * 3) + 1, (i * 3) + 2 });
	}
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, geometry.size() * sizeof(Vertex), geometry.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(glm::ivec3), index.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &InstancesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, InstancesBuffer);
	glBufferData(GL_ARRAY_BUFFER, object->Instances.size() * sizeof(glm::mat4), &object->Instances[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &InstanceVAO);
	glBindVertexArray(InstanceVAO);

	try
	{
		if (VertexBuffer == VPE_NULL || ElementBuffer == VPE_NULL || InstanceVAO == VPE_NULL)
		{
			throw std::runtime_error("Erro ao gerar os buffers da geometria.");
		}
	}
	catch (const std::runtime_error& output)
	{
		std::cerr << output.what() << std::endl;
		exit(-1);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));

	glBindBuffer(GL_ARRAY_BUFFER, InstancesBuffer);

	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), nullptr);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4) * 3));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(VPE_NULL);
	object->VertexAmount = index.size() * 3;

	return InstanceVAO;
}