OpenSSL
====

Compiling (Windows)
---------

- Get repos from OpenSSL fork or [https://github.com/openssl/openssl]
- Install Perl at [http://strawberryperl.com]
- Install Netwide Assembler (NASM) at [https://www.nasm.us]
- Make sure both Perl and NASM are in your %PATH%

- Use Visual Studio Developer Command Prompt and execute the following:
	perl Configure VC-WIN32
	nmake

- copy 'include' dir to 'external/openssl'
- copy 'libcrypto_static.lib' and 'libssl_static.lib' to 'external/lib' (and remove the '_static' postfix)