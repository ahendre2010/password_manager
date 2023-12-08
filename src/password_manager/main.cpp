// password_manager.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>
#include <sstream>

using CredentialDB = std::map<std::string, std::string>;
namespace
{
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

	bool open_database_file(const std::string& file_path_name, std::ios::openmode open_mode, std::fstream& file_stream)
	{
		std::filesystem::path file_path = file_path_name;

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
		std::string encrypted_content{std::istreambuf_iterator<char>(file_stream), {}};
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

	void add_credential(CredentialDB& database, const std::string& username, const std::string& password, const std::string& master_password)
	{
		std::string encrypted_password = encrypt_string(password.c_str(), master_password.c_str());

#define UPDATE_ALLOWED
#ifdef UPDATE_ALLOWED
		auto it = database.find(username);
		if (it != database.end())
			it->second = encrypted_password;
		else
#endif // UPDATE_ALLOWED
			database.emplace(std::pair(username, encrypted_password));
	}

	void view_credential(CredentialDB& database, const std::string& username, const std::string& master_password)
	{
		auto it = database.find(username);
		if (it == database.end()) return; // do nothing in case of username not found

		std::string encypted_password = it->second;
		std::string password = decrypt_string(encypted_password.c_str(), master_password.c_str());
		std::cout << password << std::endl;
	}

	void remove_credential(CredentialDB& database, const std::string& username)
	{
		database.erase(username);
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

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		std::cerr << "Usage: " << argv[0] << " <operation> <credential> <file> <master password>" << std::endl;
		return EXIT_FAILURE;
	}

	std::string operation(argv[1]);
	std::string credential(argv[2]);
	std::string file_path(argv[3]);
	std::string master_password(argv[4]);

	// Try opening database file in read only mode.
	std::fstream db_file_stream;
	auto db_open_success = open_database_file(file_path, std::ios::in, db_file_stream);
	if (!db_open_success)
	{
		db_file_stream.close();
		return EXIT_FAILURE;
	}

	// Decrypt database file
	CredentialDB database = read_database_from_file(db_file_stream, master_password);
	db_file_stream.close(); // Close read-only stream of databse file.

	// Perform operation.
	if (operation == "add") {
		//separate username and password from credential input.
		auto delimiter_position = credential.find('=');
		std::string username = credential.substr(0, delimiter_position);
		std::string password = credential.substr(delimiter_position + 1);

		add_credential(database, username, password, master_password);
	}
	else if (operation == "view") {
		view_credential(database, credential, master_password);
	}
	else if (operation == "rem") {
		remove_credential(database, credential);
	}
	else {
		std::cerr << "Invalid operation. Supported operations: add, view, rem." << std::endl;
		return EXIT_FAILURE;
	}

	// Try opening database file in write mode with clearing existing content
	db_open_success = open_database_file(file_path, std::ios::out | std::ios::trunc, db_file_stream);
	if (!db_open_success)
	{
		db_file_stream.close();
		return EXIT_FAILURE;
	}

	// Save database on file system.
	write_database_to_file(db_file_stream, database, master_password);
	db_file_stream.close();

	return EXIT_SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
