#pragma once
#include <string>
#include <map>
#include <iostream>
#include <filesystem>

namespace password_database
{
	using CredentialDB = std::map<std::string, std::string>;

	class Operations {
	public:
		Operations(const std::string& db_file_path, const std::string& master_password);
		~Operations();

		void add_credential(const std::string& username, const std::string& password);
		void view_credential(const std::string& username, std::ostream& stream);
		void remove_credential(const std::string& username);

		Operations(const Operations&) = delete;
		Operations& operator=(const Operations&) = delete;
		Operations(Operations&&) = delete;
		Operations& operator=(Operations&&) = delete;

	private:
		CredentialDB p_database;
		std::filesystem::path p_db_file_path;
		const std::string p_master_password;
	};
}
