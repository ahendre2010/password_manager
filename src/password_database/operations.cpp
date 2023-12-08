#include <operations.h>
#include "helpers.hpp"

password_database::Operations::Operations(const std::string& db_file_path, const std::string& master_password)
	: p_db_file_path(db_file_path)
	, p_master_password(master_password)
{
	p_database = read_database_from_file(p_db_file_path, master_password);
}

password_database::Operations::~Operations()
{
	write_database_to_file(p_db_file_path, p_database, p_master_password);
}

void password_database::Operations::add_credential(const std::string& username, const std::string& password)
{
	std::string encrypted_password = encrypt_string(password.c_str(), p_master_password.c_str());

#define UPDATE_ALLOWED
#ifdef UPDATE_ALLOWED
	auto it = p_database.find(username);
	if (it != p_database.end())
		it->second = encrypted_password;
	else
#endif // UPDATE_ALLOWED
		p_database.emplace(std::pair(username, encrypted_password));
}

void password_database::Operations::view_credential(const std::string& username, std::ostream& stream) const
{
	auto it = p_database.find(username);
	if (it == p_database.end()) return; // do nothing in case of username not found

	std::string encypted_password = it->second;
	std::string password = decrypt_string(encypted_password.c_str(), p_master_password.c_str());
	stream << password << std::endl;
}

void password_database::Operations::remove_credential(const std::string& username)
{
	p_database.erase(username);
}
