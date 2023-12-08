# Password Manager
Basic password manager implementation in C++

---
- **Usage:** 
  - password_manager \<operation\> \<credential\> \<database file\> \<master password\>
- Supported operations *add*, *view*, *rem*.

- **Examples:**
  - password_manager add username=password c:\passwords.db supersecretpassphrase
  - password_manager view username c:\passwords.db supersecretpassphrase
  - password_manager rem username c:\passwords.db supersecretpassphrase
---
After cloning this repository perform following steps to build and run the password manager.

1. Run configure.ps1 script to generate Visual Studio 2022 solution.
2. Open generated solution from **_build/password_manager.sln**
3. Build the solution using Visual Studio 2022.
4. Output will be stored in exe directory at the root of repository.
---
Troubleshooting:
- In case you are facing to issues to configure cmake, please make sure installation directory of 7-zip is added into your system path variable and restart command prompt to get updated path variables.
---
Note:
- I am no security expert, so please help me understant the flaws you may find.
- I used CryptoPP library to encrypt and decrypt contents of database.
- I am happy to learn and work on any feedback in my free time.
- Do not hesitate to create pull requests in case you want to improve any part of it.
---
