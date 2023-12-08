#include <iostream>
#include <operations.h>

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		std::cerr << "Usage: " << argv[0] << " <operation> <credential> <file> <master password>" << std::endl;
		return EXIT_FAILURE;
	}

	std::string operation(argv[1]);
	std::string credential(argv[2]);
	std::string db_file_path(argv[3]);
	std::string master_password(argv[4]);

	if (operation.empty() || credential.empty()
		|| db_file_path.empty() || master_password.empty())
	{
		std::cerr << "Usage: " << argv[0] << " <operation> <credential> <file> <master password>" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{

		password_database::Operations database(db_file_path, master_password);

		// Perform operation.
		if (operation == "add") {
			//separate username and password from credential input.
			auto delimiter_position = credential.find('=');
			std::string username = credential.substr(0, delimiter_position);
			std::string password = credential.substr(delimiter_position + 1);

			if (username.empty() || password.empty())
			{
				std::cerr << "Error: Credentials cannot be empty." << std::endl;
			}

			database.add_credential(username, password);
		}
		else if (operation == "view") {
			database.view_credential(credential, std::cout);
		}
		else if (operation == "rem") {
			database.remove_credential(credential);
		}
		else {
			std::cerr << "Error: Invalid operation. Supported operations: add, view, rem." << std::endl;
			return EXIT_FAILURE;
		}
	}
	catch (...) {
		std::cerr << "Error: something went wrong" << std::endl;
	}

	return EXIT_SUCCESS;
}
