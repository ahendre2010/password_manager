#pragma once
#include <map>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>

namespace password_database
{
	using CredentialDB = std::map<std::string, std::string>;
	std::string encrypt_string(const char* instr, const char* passPhrase)
	{
		std::string outstr(instr);

		//DefaultEncryptorWithMAC encryptor(passPhrase, new HexEncoder(new StringSink(outstr)));
		//encryptor.Put((byte*)instr, strlen(instr));
		//encryptor.MessageEnd();

		return outstr;
	}

	std::string decrypt_string(const char* instr, const char* passPhrase)
	{
		std::string outstr(instr);

		//HexDecoder decryptor(new DefaultDecryptorWithMAC(passPhrase, new StringSink(outstr)));
		//decryptor.Put((byte*)instr, strlen(instr));
		//decryptor.MessageEnd();

		return outstr;
	}

	std::string serialize_database(const CredentialDB& map)
	{
		std::stringstream sstream;
		for (auto it : map)
		{
			sstream << it.first;
			sstream << " ";
			sstream << it.second;
			sstream << std::endl;
		}
		std::string result = sstream.str();
		result.pop_back(); // Remove trailing white space

		return result;
	}

	CredentialDB deserialize_database(const std::string& str)
	{
		CredentialDB database;

		if (str.length() < 1) return database;

		std::stringstream sstream(str);
		while (!sstream.eof()) {
			std::string user_name;
			std::string encrypted_password;

			sstream >> user_name;
			sstream >> encrypted_password;

			database.emplace(std::pair(user_name, encrypted_password));
		}

		return database;
	}

	bool open_database_file(const std::filesystem::path& file_path, std::ios::openmode open_mode, std::fstream& file_stream)
	{
		// Check if file exists, create one otherwise
		if (!std::filesystem::exists(file_path))
		{
			std::filesystem::create_directories(file_path.parent_path());
			open_mode = std::ios::out;
		}

		file_stream.open(file_path, open_mode);
		if (!file_stream.good())
		{
			std::cerr << "Opening databse file failed." << std::endl;
			return false;
		}
		return true;
	}

	CredentialDB read_database_from_file(std::fstream& file_stream, const std::string& master_password)
	{
		std::string encrypted_content{ std::istreambuf_iterator<char>(file_stream), {} };
		std::string plain_text_content = decrypt_string(encrypted_content.c_str(), master_password.c_str());
		CredentialDB credentials = deserialize_database(plain_text_content);
		return credentials;
	}

	void write_database_to_file(std::fstream& file_stream, CredentialDB database, const std::string& master_password)
	{
		std::string plain_text_content = serialize_database(database);
		std::string encrypted_content = encrypt_string(plain_text_content.c_str(), master_password.c_str());
		file_stream.clear();
		file_stream.write(encrypted_content.c_str(), encrypted_content.size());
	}

#pragma region TestFunctions
	bool test_serialize_databse()
	{
		std::string expected_string = "David Woods\nEd Sheeran\nJohn Miller\nTaylor Swift";
		CredentialDB test_map;
		test_map.emplace(std::pair("John", "Miller"));
		test_map.emplace(std::pair("David", "Woods"));
		test_map.emplace(std::pair("Taylor", "Swift"));
		test_map.emplace(std::pair("Ed", "Sheeran"));
		auto str = serialize_database(test_map);

		return str == expected_string;
	}

	bool test_deserialize_databse()
	{
		CredentialDB test_map;
		test_map.emplace(std::pair("John", "Miller"));
		test_map.emplace(std::pair("David", "Woods"));
		test_map.emplace(std::pair("Taylor", "Swift"));
		test_map.emplace(std::pair("Ed", "Sheeran"));

		std::string test_string = "David Woods\nEd Sheeran\nJohn Miller\nTaylor Swift";
		auto db = deserialize_database(test_string);

		return db.size() == test_map.size();
	}
#pragma endregion
}